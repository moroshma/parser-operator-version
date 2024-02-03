#include "main.hpp"



int main(int argc, char *argv[]) {
    variable_types vt("./types.txt");
    handler_data hd;


    if (hd.read_file("./test1.txt", vt) == Error::OPEN_FILE_ERROR){
        hd.print_error("", 0, Error::OPEN_FILE_ERROR);
        return Error::OPEN_FILE_ERROR;
    }

    return 0;
}