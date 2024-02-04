#include "main.hpp"

int main(int argc, char *argv[]) {
  variable_types vt("./types.txt");
  handler_data hd;



  if (argc < 2) {
    hd.print_error("", Error::ARGUMENT_ERROR);
  } else {
    if (hd.read_file(argv[1], vt) == Error::OPEN_FILE_ERROR) {
      hd.print_error("", Error::OPEN_FILE_ERROR);
      return Error::OPEN_FILE_ERROR;
    }
  }

  return 0;
}