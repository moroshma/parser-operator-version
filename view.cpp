#include "./main.hpp"

void handler_data::print_error(const string &line, Error err) {
  switch (err) {
  case INVALID_BRACETS_VISION:
    cout << "In line: " << this->cur_line + 1
         << "  incorrect placement of parentheses: *ฅ^•ﻌ•^ฅ*" << endl;
    break;
  case INVALID_VARIBLE_INIT:
    cout << "INVALID VARIABLE INIT (☞ ͡° ͜ʖ ͡°)☞ line: " << this->cur_line + 1
         << line << endl;
    break;
  case MALLOC_ERROR:
    cout << "Memory error ＼(￣▽￣)／" << endl;
    break;
  case UNDEFIND_ERROR:
    cout << "Undefined  error ¯\\_(ツ)_/¯ LINE: " << this->cur_line + 1
         << " string: " << line << endl;
    break;
  case MANY_CONST:
    cout << "more than 1 constructors in 1 line (╯°益°)╯彡┻━┻ LINE: "
         << this->cur_line + 1 << " string: " << line << endl;
    break;
  case TYPE_OPERATION_ERROR:
    cout << "TYPE OPERATION ERROR (•_•) LINE: " << this->cur_line + 1
         << " string: " << line << endl;
    break;
  case OPEN_FILE_ERROR:
    cout << "can't open file ʕ•͡-•ʔ " << endl;
    break;
  case OPERATOR_ERROR:
    cout << "OPERATOR NON VALID (◣ _ ◢) LINE: " << this->cur_line + 1
         << " string: " << line << endl;
    break;

  default:
    cout << "Undefined  error (╯°益°)╯彡┻━┻ LINE: " << this->cur_line + 1
         << " string: " << line << endl;
    break;
  }
}