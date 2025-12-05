#include <velc.hpp>


//#define ARGS_DEBUG
//#define LEXER_DEBUG
//#define PARSER_DEBUG

const std::string HELPTEXT =
"Usage: velc <file>.vel [options]\n"
"Options:\n"
"  -h, --help               Display this information.\n"
"  -v, --version            Display compiler version information.\n"
"  -t, --time               Time the execution of each subprocess.\n"
"  -I <path>                Adds <path> to the list of include file search paths.\n"
"  -K, --save               Keep the intermediate files generated.\n"
"  -E                       Preprocess only.\n"
"  -S                       Preprocess and compile only.\n"
"  -c                       Preprocess, compile, and assemble only.\n"
"  -o <file>                Place the output into <file>.\n"
"  -r, --run                Execute the compiled executable after compilation.\n"
;

const std::string VERSIONTEXT =
"velc (VEL Compiler) 1.0.0 INDEV\n"
;

int main(int argc, char* argv[]){

    CompilerOptions context{};
    { //Parse argv
        if(argc == 1){ //if no args provided
            std::cout << HELPTEXT;
            exit(1);
        }

        #ifdef ARGS_DEBUG
            std::cout << "ARGC: " << argc << "\n";
        #endif

        for(int i=1;i<argc;i++){
            std::string arg = argv[i];

            #ifdef ARGS_DEBUG
                std::cout << "ARGV " << i+1 << ": " << argv[i] << "\n";
            #endif


            //check if expecting anything before
            if(context.expectOutputFile){
                context.output_filename = arg;
                context.expectOutputFile = false;
            }
            else if(context.expectIncludePath){
                context.includePaths.push_back(arg);
                context.expectIncludePath = false;
            }

            //if argument is --help or -h
            else if(arg == "-h" || arg == "--help"){
                std::cout << HELPTEXT;
                exit(0);
            }
            else if(arg == "-v" || arg == "--version"){
                std::cout << VERSIONTEXT;
                exit(0);
            }
            //time
            else if(arg == "-t" || arg == "--time"){
                context.timeExecution = true;
            }
            //add include path
            else if(arg == "-I"){
                context.expectIncludePath = true;
            }
            //save intermediate files
            else if(arg == "-K" || arg == "--save"){
                context.deleteIntermediateFiles = false;
            }
            //preprocess only
            else if(arg == "-E"){
                context.doPreprocess = true;
                context.doCompile = false;
                context.doAssemble = false;
                context.doLink = false;
            }
            //preprocess and compile only
            else if(arg == "-S"){
                context.doPreprocess = true;
                context.doCompile = true;
                context.doAssemble = false;
                context.doLink = false;
            }
            //preprocess, compile, and assemble only
            else if(arg == "-c"){
                context.doPreprocess = true;
                context.doCompile = true;
                context.doAssemble = true;
                context.doLink = false;
            }
            //output
            else if(arg == "-o"){
                context.expectOutputFile = true;
            }
            //run
            else if(arg == "-r" || arg == "--run"){
                context.doExecute = true;
            }
            else{
                //get input filename from argv[1]
                context.input_filename = arg;
                //do checks on input filename to make sure it is valid
                if(!endsWith(context.input_filename, ".vel")){
                    std::cout << "velc: invalid input filename extension, expected \".vel\".\n";
                }
            }
        }
        //raise error if unhandled expects
        if(context.expectOutputFile){
            std::cout << "velc: Expected input file after -o\n";
            exit(1);
        }
        if(context.expectIncludePath){
            std::cout << "velc: Expected include file path after -I\n";
            exit(1);
        }

        //raise error if no input filename
        if(context.input_filename.empty()){
            std::cout << "velc: no input files\n";
            exit(1);
        }
        //cannot execute if the processes are not finished
        if(!(context.doPreprocess && context.doCompile && context.doAssemble && context.doLink)){
            context.doExecute = false;
        }
        //default output filename if not specified
        if(context.output_filename.empty()){
            context.output_filename = "a.out";
        }
        int vel = sizeof(".vel")-1;
        context.input_filename_base = context.input_filename.substr(0, context.input_filename.length()-vel);
    }

    auto start = std::chrono::high_resolution_clock::now();
    //--PREPROCESS--
    if(!context.doPreprocess){
        exit(0);
    }
    context.source = Preprocessor::Process(context.input_filename, context.includePaths);

    auto stop = std::chrono::high_resolution_clock::now();
    if(context.timeExecution){
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
        std::cout << "Preprocessor: " << (duration.count())/1000.0 << "μs\n";
    }

    //--COMPILE--
    if(!context.doCompile){
        {
            //write preprocessed source to file if not compiling anymore
            std::ofstream file;
            file.open(context.input_filename_base + ".vee");
            if(!file.is_open()){
                std::cout << "velc: Failed to open file: " << context.input_filename_base << ".vee" << "\n";
                exit(1);
            }
            file << context.source;
            file.close();
        }
        exit(0);
    }

    start = std::chrono::high_resolution_clock::now();
    //Lexer
    context.tokens = Lexer::tokenize(context.source);

    stop = std::chrono::high_resolution_clock::now();
    if(context.timeExecution){
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
        std::cout << "Lexer: " << (duration.count())/1000.0 << "μs\n";
    }

    #ifdef LEXER_DEBUG
        std::cout << "Tokens: \n";
        for(Token t : context.tokens){
            std::cout << Lexer::KeywordToString(t.type) << '|' << t.text << '|' << t.value << ' ' << t.valuef << "       " << t.line << ' ' << t.col << '\n';
        }
    #endif

    start = std::chrono::high_resolution_clock::now();
    //Parser
    context.AST = Parser::constructAST(context.tokens);

    stop = std::chrono::high_resolution_clock::now();
    if(context.timeExecution){
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
        std::cout << "Parser: " << (duration.count())/1000.0 << "μs\n";
    }

    #ifdef PARSER_DEBUG
        context.AST.print();
    #endif

    start = std::chrono::high_resolution_clock::now();  
    //Semantic Analyzer
    Analyzer::analyze(context.AST);

    stop = std::chrono::high_resolution_clock::now();
    if(context.timeExecution){
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
        std::cout << "Semantic Analyzer: " << (duration.count())/1000.0 << "μs\n";
    }

    /*
    asm{
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
        }
    */

    //--ASSEMBLE--
    //write assembly source to file
    {
        std::ofstream file;
        file.open(context.input_filename_base + ".asm");
        if(!file.is_open()){
            std::cout << "velc: Failed to open file: " << context.input_filename_base << ".asm" << "\n";
            exit(1);
        }
        file << context.sourceAssembly;
        file.close();
    }

    if(!context.doAssemble){
        exit(0);
    }

    std::string nasm_cmd = "nasm -f elf64 \"" + context.input_filename_base + ".asm\" -o \"" + context.input_filename_base + ".o\"";
    std::cout << "\t" << nasm_cmd << "\n";
    if(std::system(nasm_cmd.c_str()) != 0){
        std::cout << "velc: NASM Failed!\n";
        exit(1);
    }

    //--LINK--
    if(!context.doLink){
        exit(0);
    }
    
    std::string ld_cmd = "ld \"" + context.input_filename_base + ".o\" -o \"" + context.output_filename + "\"";
    std::cout << "\t" << ld_cmd << "\n";
    if(std::system(ld_cmd.c_str()) != 0){
        std::cout << "velc: LD Failed!\n";
        exit(1);
    }

    //delete if no need to keep intermediate files
    if(context.deleteIntermediateFiles){
        std::string filename = context.input_filename_base + ".asm";
        if(std::remove(filename.c_str()) != 0){
            std::cout << "velc: Failed to delete intermediate file: " << filename << "\n";
            exit(1);
        }

        filename = context.input_filename_base + ".o";
        if(std::remove(filename.c_str()) != 0){
            std::cout << "velc: Failed to delete intermediate file: " << filename << "\n";
            exit(1);
        }
    }

    //--EXECUTE--
    if(!context.doExecute){
        exit(0);
    }

    std::string run_cmd = "./" + context.output_filename;
    std::cout << "Executing executable: \n";
    std::cout << "\t" << run_cmd << "\n";
    std::cout << "\t-----------------------------\n";
    if(std::system(run_cmd.c_str()) != 0){
        std::cout << "velc: Execution Failed!\n";
        exit(1);
    }


    return 0;
}

std::string read_file(const std::string& path){
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path);
    }

    // Seek to end to determine file size
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::cout << "filesize: " << size << "\n";
    std::string buffer(size, '\0');

    // Read entire file into the string
    file.seekg(0);
    file.read(&buffer[0], size);

    return buffer;
}

inline bool endsWith(const std::string& string, const std::string& extension){
    if(extension.size() > string.size()){
        return false;
    }
    return string.compare(string.size()-extension.size(), extension.size(), extension) == 0;
}


