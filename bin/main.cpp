#include <iostream>
#include <parsing_args.h>
#include <sandpile_model.h>

int main(int argc, char** argv){
    Args args;
    if (!ReadAllArgs(&args, argv, argc)){
        std::cerr<<"ERORR WHILE READING ARGUMENTS"<<std::endl;
        return 1;
    }
    SandpileModel(&args);
    return 0;
}
