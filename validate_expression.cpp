#include "./main.hpp"
#include <stack>

void handler_data::validate_expressions(const string &line, variable_types &vt,
                                        const string &type) {
  string copy_line = line;
  delete_double_space(copy_line);
  size_t pos = copy_line.find('=');
  if (pos == string::npos) {
    return;
  }
  string expression = copy_line.substr(pos + 1, copy_line.size());

  expression = trim(expression, " =;{}");
  expression = replase_var_to_type(expression, vt);

  expression.erase(remove(expression.begin(), expression.end(), ' '),
                   expression.end());

  if (!validate_brackets(expression)) {
    print_error(line, Error::INVALID_BRACETS_VISION);
    return;
  }
  if (!validate_operator(expression)) {
        print_error(line, Error::OPERATOR_ERROR);
        return;
    }
  if (!validate_type(expression, vt)) {
    print_error(line, Error::TYPE_OPERATION_ERROR);
    return;
  }
}

bool handler_data::validate_type(const string &line, variable_types &vt) {

    return true;
}

string handler_data::replase_var_to_type(const string &line,
                                         variable_types &vt) {
  string copy_line = line;
  delete_double_space(copy_line);
  string expression = copy_line;
  expression = trim(expression, " =;{}");

  for (auto first = vt.get_variable().begin(); first != vt.get_variable().end();
       ++first) {
    for (auto it = first->second.begin(); it != first->second.end(); ++it) {
      size_t pos = expression.find(*it);
      if (pos != string::npos) {
        string replace;
        if (is_double(first->first)) {
          replace = "d";
        } else if (is_int(first->first)) {
          replace = "i";
        } else if (is_pointer(first->first)) {
          replace = "p";
        } else {
          replace = first->first;
        }
        expression.replace(pos, it->size(), first->first);
      }
    }
  }
  return expression;
}

bool handler_data::is_double(const string &type) {
  vector<string> double_type = {"double", "float", "long double", "long float"};
  for (auto &it : double_type) {
    if (type == it) {
      return true;
    }
  }
  return false;
}

bool handler_data::is_int(const string &type) {
  vector<string> double_type = {
      "int",          "short",          "long",          "long long",
      "unsigned int", "unsigned short", "unsigned long", "unsigned long long",
      "char",         "unsigned char",  "signed char",   "bool",
      "wchar_t"};
  for (auto &it : double_type) {
    if (type == it) {
      return true;
    }
  }
  return false;
}

bool handler_data::is_pointer(const string &type) {
  size_t pos = type.find('*');
  if (pos != string::npos) {
    return true;
  }
  return false;
}

bool handler_data::validate_brackets(const string &line) {

  stack<char> stack;
  for (size_t i = 0; i < line.size(); i++) {
    if (line[i] == '(') {
      stack.push(line[i]);
    } else if (line[i] == ')') {
      if (stack.empty()) {

        return false;
      }
      stack.pop();
    }
  }

  return true;
}

bool handler_data::validate_operator(const string &line) {

  std::set<std::string> operators_bin = {"+",  "-",  "*",  "/",  "%",  "&",
                                         "|",  "^",  "<<", ">>", "&&", "||",
                                         "==", "!=", "<",  ">",  "<=", ">="};
  std::set<string> operators_uno = {"+", "-", "!", "~"};
  try {

    for (size_t i = 0; i < line.length(); ++i) {
        string temp = "";
      if (is_type(line[i])) {
        continue;
      }
      if (line[i] != line.length() - 1 &&
          (is_string_in_set(operators_uno, line[i]) &&
           (line[i + 1] == '(' || is_type(line[i + 1])))) {
        continue;
      } else if (line[i] != line.length() - 1 &&
                 (is_string_in_set(operators_bin,  line[i])) &&
                  (is_type(line[i + 1]) || line[i + 1] == '(')) {
        continue;
      } else if (line[i] != line.length() - 1 &&
                 (is_string_in_set(operators_bin,  (temp += line[i] , temp += line[i+1]))) &&
                 (is_type(line[i + 2]) || line[i + 2] == '(')) {
        continue;
      } else if(i == line.length() - 1  &&
      (is_string_in_set(operators_bin, line[i])|| is_string_in_set(operators_uno, line[i]))){
          return false;
      }else {
        return false;
      }
    }
  } catch (...) {
    return false;
  }
  return true;
}

bool handler_data::is_type(const char &c) {
  if (c == 'u' || c == 'd' || c == 'p' || isdigit(c) || c == '.')
    return true;
  return false;
}
