#include "./main.hpp"


#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>


using namespace std;
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
  replace_uno_minus(expression);
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



int handler_data:: get_priority(const string& op) {
    switch (op[0]) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '&':
            if (op.size() == 2 && op[1] == '&') {
                return 9;
            }
            return 3;
        case '|':
            if (op.size() == 2 && op[1] == '|') {
                return 10;
            }
            return 4;
        case '^':
            return 5;
        case '<':
        case '>':
            if (op.size() == 2 && (op[1] == '=')) {
                return 7;
            }
            return 6;
        case '=':
            if (op.size() == 2 && op[1] == '=') {
                return 7;
            }
            return -1;
        case '!':
            if (op.size() == 1) {
                return 8;
            } else if (op.size() == 2 && op[1] == '=') {
                return 7;
            }
            return -1;
        case '~':
            return 12;
        default:
            return -1;
    }
}




bool handler_data:: check_postfix(const string& postfix) {
    stack<string> st;

    std::istringstream iss(postfix);
    std::string tk;

    try {
    while (iss >> tk) {
        if (is_type(tk[0])){
            st.push(tk);
        } else if (is_bin_operator(tk)){
            string first = st.top();
            st.pop();
            string second = st.top();
            st.pop();

            if (tk == "^" || tk == "&" || tk == "|" || tk == "%"){
                if (first == "d" || second == "d" || first == "p" || second == "p"){
                    return false;
                } else {
                    st.emplace("i");
                }
            } else {

                if (first == "d" || second == "d"){
                    st.emplace("d");
                } else if (first == "i" || second == "i"){
                    st.emplace("i");
                }else {
                    return false;
                }

            }
        } else if(tk[0] == '!' ){
            st.pop();
            st.emplace("i");
        }else if (tk[0] == '~') {
            if (tk == "p") {
                return false;
            }
        }else {
            st.pop();
            st.pop();
            st.emplace("i");
        }

    }
    if (st.size() > 1){
        return false;
    }
    }
    catch (...){
        return false;
    }


    return true;
}

bool isDouble(const std::string& str) {
    try {
        size_t pos;
        std::stod(str, &pos);
        return pos == str.size() && str.find('.') != string::npos;  // Возвращает true, если все символы были успешно прочитаны
    } catch (std::invalid_argument const& ex) {
        return false;  // В случае недопустимого аргумента (не является числом) возвращает false
    } catch (std::out_of_range const& ex) {
        return false;  // В случае выхода за пределы диапазона возвращает false
    }
}

bool handler_data::validate_type(const string &line, variable_types &vt) {
  string expression = line;
  add_space(expression);
  stack<string> st;
    //add_space(expression);
  /*Источник istringstream представляет собой поток ввода, который считывает
   * данные из строки string, в отличие от стандартного потока ввода cin.
   * istringstream позволяет
   * считывать данные из строки, предоставляя возможность инициализировать
   * его содержимым нужной строки для разделения на токены*/
   std::istringstream iss(expression);
   std::string tk;

   //TODO: написать проверку на унарный и бинарный оператор и доделать польскую нотацию для проверки типов
   // 2. Проверка на унарный оператор
   // 1. Проверка на бинарный оператор
   // 3. Проверка на тип переменной
   // 4. проверка на правильность операций
    string postfix;
   try {

       while (iss >> tk) {
           if (is_type(tk[0])) {
               if (isDouble(tk)) {
                   postfix += "d";
               } else if (isdigit(tk[0]) || tk[0] == 'i') {
                   postfix += "i";
               } else if(tk[0] == 'd'){
                     postfix += "d";
               }else {
                   postfix += "p";
               }
               postfix += ' ';
           } else if (tk == "(") {
               st.push(tk);
           } else if (tk == ")") {
               while (!st.empty() && st.top() != "(") {
                   postfix += ' ';
                   postfix += st.top();
                   postfix += ' ';
                   st.pop();
               }
               st.pop();  // Pop the '('
           } else {  // Operator
               while (!st.empty() && get_priority(tk) <= get_priority(st.top())) {
                   postfix += ' ';
                   postfix += st.top();
                   postfix += ' ';
                   st.pop();
               }
               st.push(tk);
           }
       }
       while (!st.empty()) {
           postfix += " ";
           postfix += st.top();
           st.pop();
       }
   }    catch(...) {
       return false;
   }
    delete_double_space(postfix);

    return check_postfix(postfix);
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
      while (expression.find(*it) != string::npos) {
        size_t pos = expression.find(*it);
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
        expression.replace(pos, replace.size(), replace);
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
      "wchar_t",      "size_t"};
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

  std::vector<std::string> operators_bin = {  "<<", ">>", "&&", "||",
                                         "==", "!=",  "<=", ">=","+",  "-",  "*",  "/",  "%",  "&",
                                           "|",  "^", "<",  ">"};

  std::vector<std::string> operators_uno = {"+", "-", "!", "~"};
  try {

    for (size_t i = 0; i < line.length(); ++i) {
      string temp = "";
      if (is_type(line[i])) {
        continue;
      }
        if (i != line.length() - 1 &&
            (is_string_in_set(operators_bin,
                              (temp += line[i], temp += line[i + 1]))) &&
            (is_type(line[i + 2]) || line[i + 2] == '(')) {
            i++;
            continue;
        } else if (i != line.length() - 1 &&
          (is_string_in_set(operators_uno, line[i]) &&
           (line[i + 1] == '(' || is_type(line[i + 1])))) {
        continue;
      } else if (i != line.length() - 1 &&
                 (is_string_in_set(operators_bin, line[i])) &&
                 (is_type(line[i + 1]) || line[i + 1] == '(')) {
        continue;
      } else  if (i != line.length() - 1 && line[i] == '(' &&
                 !is_string_in_set(operators_bin, line[i + 1])) {
        continue;
      } else if (i != 0 && line[i] == ')' &&
                 !is_string_in_set(operators_bin, line[i - 1])) {
        continue;
      } else if (i == line.length() - 1 &&
                 (is_string_in_set(operators_bin, line[i]) ||
                  is_string_in_set(operators_uno, line[i]))) {
        return false;
      } else {

        return false;
      }
    }
  } catch (...) {
    return false;
  }
  return true;
}

bool handler_data::is_type(const char &c) {
  if (c == 'i' || c == 'd' || c == 'p' || isdigit(c) || c == '.')
    return true;
  return false;
}

bool handler_data::is_bin_operator(const string &st) {
    std::set<std::string> operators = {
            "+",  "-",  "*",  "/", "%", "&",  "|",  "^", "<<", ">>", "&&",
            "||", "==", "!=", "<", ">", "<=", ">="};

    if (operators.find(st) != operators.end())
        return true;

    return false;
}

void handler_data::replace_uno_minus(string &line) {
  std::set<std::string> operators = {
      "+",  "-",  "*",  "/", "%", "&",  "|",  "^", "<<", ">>", "&&",
      "||", "==", "!=", "<", ">", "<=", ">=", "(", ")",  "!",  "~"};
  for (int i = 0; i < line.size(); i++) {
    if (line[i] == '-' && i == 0) {
      line[i] = ' ';
    } else if (i != 0 && i != line.size()  && line[i] == '-' &&
               is_string_in_set(operators, line[i - 1])) {
      line[i] = ' ';
    } else if (line[i] == '+' && i == 0) {
      line[i] = ' ';
    } else if (i != 0 && line[i] == ' ' &&
               is_string_in_set(operators, line[i - 1])&& i != line.size()) {
      line[i] = ' ';
    }
  }
}

void handler_data::add_space(string &line) {
    std::vector<std::string> vec = {
            "==", "!=", "<=", ">=", "<<", ">>", "&&", "||", "+", "-", "*", "/", "%",
            "&",  "|",  "^",  "<",  ">",  "(",  ")",  "!",  "~", "i", "d", "p",
    };
    std::string result;
    size_t pos = 0;
    while (pos < line.length()) {
        bool found = false;
        for (const std::string &t : vec) {
            if (line.substr(pos, t.length()) == t) {
                result += " " + line.substr(pos, t.length()) + " ";  // Add space before and after the operator
                pos += t.length();
                found = true;
                break;
            }
        }
        if (!found) {
            result += line[pos];
            pos++;
        }
    }

    line = trim(result, " ");
}

