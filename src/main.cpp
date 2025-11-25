#include <main.hpp>


int main(int argc, char* argv[]){

    std::string input_filename;
    std::string output_filename;
    { //Parse argv
        if(argc < 3){ //requires 2 args
            std::cout << "Usage: " << argv[0] << " <input_file.vel> <output_file>\n";
            return 1;
        }

        input_filename = argv[1];
        output_filename = argv[2];
    }

    //Files for reading
    std::ifstream input_file;
    std::ofstream output_file;
    input_file.open(input_filename);
    if(!input_file.is_open()){
        std::cout << "Failed to open input file: " << input_filename << "\n";
        return 1;
    }
    output_file.open(output_filename + ".asm");
    if(!output_file.is_open()){
        std::cout << "Failed to open output file: " << output_filename << "\n";
        return 1;
    }
    //Read entire input file into a string
    std::string source = read_file(input_filename);
    input_file.close();

    std::cout << "Source Code: \n" << "\t-----------------------------\n";
    std::cout << source;
    std::cout << "\t-----------------------------\n";
    //Lexer
    std::vector<Token> tokens = Lexer::tokenize(source);
    std::cout << "Tokens: \n";
    for(Token t : tokens){
        std::cout << Lexer::KeywordToString(t.type) << '|' << t.text << '|' << t.value << ' ' << t.valuef << "       " << t.line << ' ' << t.col << '\n';
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


    output_file.close();

    std::string nasm_cmd = "nasm -f elf64 \"" + output_filename + ".asm\" -o \"" + output_filename + ".o\"";
    std::cout << "\t" << nasm_cmd << "\n";
    if(std::system(nasm_cmd.c_str()) != 0){
        std::cout << "NASM Failed!\n";
        return 1;
    }

    std::string ld_cmd = "ld \"" + output_filename + ".o\" -o \"" + output_filename + "\"";
    std::cout << "\t" << ld_cmd << "\n";
    if(std::system(ld_cmd.c_str()) != 0){
        std::cout << "LD Failed!\n";
        return 1;
    }

    std::string run_cmd = "./" + output_filename;
    std::cout << "Executing executable: \n";
    std::cout << "\t" << run_cmd << "\n";
    std::cout << "\t-----------------------------\n";
    if(std::system(run_cmd.c_str()) != 0){
        std::cout << "Run Failed!\n";
        return 1;
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
    std::cout << size << "\n";
    std::string buffer(size, '\0');

    // Read entire file into the string
    file.seekg(0);
    file.read(&buffer[0], size);

    return buffer;
}




























