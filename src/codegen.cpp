#include <codegen.hpp>

/*
section .data
hello db "Hello, World!", 10
hello_len equ $ - hello

section .text
global _start
_start:
    mov rax, 1          ; sys_write
    mov rdi, 1          ; stdout
    mov rsi, hello      ; message address
    mov rdx, hello_len  ; message length
    syscall

    mov rax, 60         ; sys_exit
    xor rdi, rdi        ; exit code 0
    syscall
*/
namespace CodeGenerator{
    void generate(Program& AST, std::ostream& out){
        CodeGeneratorContext ctx{
            &out,
        };
        emitProgram(AST, ctx);

    }
    void emitProgram(Program& AST, CodeGeneratorContext& ctx){
        //loop through declarations
        for(std::unique_ptr<Declaration>& decl : AST.TopLevel){
            if(GlobalVariableDeclaration* globalVar = dynamic_cast<GlobalVariableDeclaration*>(decl.get())){
                emitGlobalVariableDeclaration(globalVar, ctx);
            }
            else if(Function* func = dynamic_cast<Function*>(decl.get())){
                emitFunction(func, ctx);
            }
            else if(Assembly* assembly = dynamic_cast<Assembly*>(decl.get())){
                emitAssembly(assembly, ctx);
            }
            else if(Comment* cmt = dynamic_cast<Comment*>(decl.get())){
                emitComment(cmt, ctx);
            }
        }
        *ctx.out << ";VELC CODEGEN START\n";

        *ctx.out << "section .data\n";
        *ctx.out << ctx.section_data << "\n";
        *ctx.out << "section .bss\n";
        *ctx.out << ctx.section_bss << "\n";
        *ctx.out << "section .text\n";
        *ctx.out << "    global _start\n";
        *ctx.out << "_start:\n";
        *ctx.out << ctx.section_text << "\n";

        *ctx.out << ";VELC CODEGEN END\n";
    }
    void emitGlobalVariableDeclaration(GlobalVariableDeclaration* varDecl, CodeGeneratorContext& ctx){
        if(!varDecl->Expr){
            std::cout << "NOEXPR\n";
            ctx.section_bss += varDecl->Id->Text;
            ctx.section_bss += " ";
            //get bss reserve for a datatype
            std::string bss_res_size = "res";
            int bss_res_num = 1;
            if(!varDecl->Typename.isBuiltin){
                std::cout << "velc: Code generator: non builtin type cannot be handled in emitGlobalVariableDeclaration\n";
                exit(1);
            }
            switch(varDecl->Typename.builtinType){
                case BuiltinType::BOOL:
                case BuiltinType::CHAR:
                case BuiltinType::INT8:
                case BuiltinType::UINT8:
                    bss_res_size += "b";
                    break;
                case BuiltinType::INT16:
                case BuiltinType::UINT16:
                    bss_res_size += "w";
                    break;
                case BuiltinType::INT32:
                case BuiltinType::UINT32:
                case BuiltinType::FLOAT32:
                    bss_res_size += "d";
                    break;
                case BuiltinType::INT64:
                case BuiltinType::UINT64:
                case BuiltinType::FLOAT64:
                    bss_res_size += "q";
                    break;
                case BuiltinType::STRING:
                case BuiltinType::VOID:
                    std::cout << "velc: Code generator: string or void types can't be reserved\n";
                    exit(1);
                default:
                    std::cout << "Bug #2 in emitGlobalVariableDeclaration\n";
                    exit(1);
            }
            ctx.section_bss += bss_res_size;
            ctx.section_bss += " ";
            ctx.section_bss += std::to_string(bss_res_num);
            ctx.section_bss += "\n";
        }
        else{
            if(Literal* vel = dynamic_cast<Literal*>(varDecl->Expr.get())){
                std::cout << "EXPR!\n";
                ctx.section_data += varDecl->Id->Text;
                ctx.section_data += " ";

                if(!varDecl->Typename.isBuiltin){
                    std::cout << "velc: Code generator: non builtin type cannot be handled in emitGlobalVariableDeclaration\n";
                    exit(1);
                }
                switch(varDecl->Typename.builtinType){
                    case BuiltinType::BOOL:
                    case BuiltinType::CHAR:
                    case BuiltinType::INT8:
                    case BuiltinType::UINT8:
                        ctx.section_data += "db ";
                        ctx.section_data += std::to_string(vel->IntValue);
                        break;
                    case BuiltinType::STRING: {
                        ctx.section_data += "db ";
                        bool stringMode = false;
                        bool beginning = true;
                        if(vel->StringValue.empty()){
                            ctx.section_data += "\"\"";
                            break;
                        }
                        if(vel->StringValue.at(0) >= 32){
                            stringMode = true;
                            ctx.section_data += "\"";
                            ctx.section_data += vel->StringValue.at(0);
                        }
                        for(int i=1;i<vel->StringValue.length();i++){
                            char s = vel->StringValue.at(i);

                            if(s >= 32){
                                if(stringMode == false){
                                    stringMode = true;
                                    if(!beginning){
                                        ctx.section_data += ", ";
                                    }
                                    else{
                                        beginning = false;
                                    }
                                    ctx.section_data += "\"";
                                }
                                ctx.section_data += s;
                            }
                            else{
                                if(stringMode == true){
                                    stringMode = false;
                                    ctx.section_data += "\"";
                                }
                                if(!beginning){
                                    ctx.section_data += ", ";
                                }
                                else{
                                    beginning = false;
                                }
                                ctx.section_data += std::to_string((uint8_t)s);
                            }
                        }
                        if(stringMode){
                            ctx.section_data += "\"";
                        }
                        ctx.section_data += ", 0";
                        break;
                    }
                    case BuiltinType::INT16:
                    case BuiltinType::UINT16:
                        ctx.section_data += "dw ";
                        ctx.section_data += std::to_string(vel->IntValue);
                        break;
                    case BuiltinType::INT32:
                    case BuiltinType::UINT32:
                    case BuiltinType::FLOAT32:
                        ctx.section_data += "dd ";
                        ctx.section_data += std::to_string(vel->IntValue);
                        break;
                    case BuiltinType::INT64:
                    case BuiltinType::UINT64:
                    case BuiltinType::FLOAT64:
                        ctx.section_data += "dq ";
                        ctx.section_data += std::to_string(vel->IntValue);
                        break;
                    case BuiltinType::VOID:
                        std::cout << "velc: Code generator: void type can't be stored\n";
                        exit(1);
                    default:
                        std::cout << "Bug #3 in emitGlobalVariableDeclaration\n";
                        exit(1);
                }
                ctx.section_data += "\n";
            }
            else{
                std::cout << "Bug #1 in emitGlobalVariableDeclaration\n";
                exit(1);
            }
        }
    }
    void emitFunction(Function* func, CodeGeneratorContext& ctx){

    }
    void emitStatement(Statement* statement, CodeGeneratorContext& ctx){

    }
    void emitBlock(Block* block, CodeGeneratorContext& ctx){

    }
    void emitVariableDeclaration(VariableDeclaration* varDecl, CodeGeneratorContext& ctx){

    }
    void emitReturn(Return* ret, CodeGeneratorContext& ctx){

    }
    void emitIf(If* ifStatement, CodeGeneratorContext& ctx){

    }
    void emitWhile(While* whileStatement, CodeGeneratorContext& ctx){

    }
    void emitFor(For* forStatement, CodeGeneratorContext& ctx){

    }
    void emitAssembly(Assembly* inlineAsm, CodeGeneratorContext& ctx){

        //clear whitespace
        for(int i=0;i<inlineAsm->Text.length();i++){
            if(inlineAsm->Text[i] != ' ' && inlineAsm->Text[i] == '\t' && inlineAsm->Text[i] == '\r' && inlineAsm->Text[i] == '\n'){
                inlineAsm->Text.erase(0, i);
            }
        }

        std::string* target;
        if(startsWith(inlineAsm->Text, "data")){
            target = &ctx.section_data;
            inlineAsm->Text = inlineAsm->Text.substr(4);
        }
        else if(startsWith(inlineAsm->Text, "bss")){
            target = &ctx.section_bss;
            inlineAsm->Text = inlineAsm->Text.substr(3);
        }
        else if(startsWith(inlineAsm->Text, "text")){
            target = &ctx.section_text;
            inlineAsm->Text = inlineAsm->Text.substr(4);
        }
        else{
            target = &ctx.section_text;
        }

        //clear newline
        for(int i=0;i<inlineAsm->Text.length();i++){
            if(inlineAsm->Text[i] == '\r' && inlineAsm->Text[i] == '\n'){
                inlineAsm->Text.erase(0, i);
            }
        }

        std::istringstream stream(inlineAsm->Text);
        std::string line;

        *target += ";Assembly start\n";
        while(std::getline(stream, line)){
            *target += line;
            *target += "\n";
        }
        *target += ";Assembly end\n";
    }
    void emitComment(Comment* vel, CodeGeneratorContext& ctx){
        std::istringstream stream(vel->Text);
        std::string line;
        while(std::getline(stream, line)){
            if(!line.empty()){
                ctx.section_text += ";";
                ctx.section_text += line;
                ctx.section_text += "\n";
            }
        }
    }
    void emitExpression(Expression* expr, CodeGeneratorContext& ctx){

    }
    void emitTypeCast(TypeCast* cast, CodeGeneratorContext& ctx){

    }


    inline bool startsWith(const std::string& string, const std::string& substr){
        if(substr.size() > string.size()){
            return false;
        }
        return string.compare(0, substr.size(), substr) == 0;
    }

}
















