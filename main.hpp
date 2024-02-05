#ifndef PARSER_OPERATOR_VERSION_MAIN_H
#define PARSER_OPERATOR_VERSION_MAIN_H

#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

enum Error {
  SUCCESS,
  MALLOC_ERROR,
  INVALID_BRACETS_VISION,
  UNDEFIND_ERROR,
  INVALID_VARIBLE_INIT,
  MANY_CONST,
  OPEN_FILE_ERROR,
  ARGUMENT_ERROR,
  OPERATOR_ERROR,
  TYPE_OPERATION_ERROR
};

using namespace std;
class base_type {
protected:
  // множество типов, которые могут быть в файле
  set<string> type_set;

public:
  // получение типов из файла и запись их в множество type_set
  int get_type(const string &name_file);
  // получение множества типов из класса base_type
  set<string> get_set() { return type_set; }
};

class handler_data;

class variable_types : public base_type {
private:
  // мапа, где ключ - тип переменной, а значение - вектор имен переменных
  map<string, vector<string>> variable{};

public:
  variable_types(const string &name_file) { get_type(name_file); }

  map<string, vector<string>> &get_variable() { return this->variable; }
  // добавление переменной в мапу
  void add_variable(const string &type, const string &name);
};

class handler_data {
  size_t cur_line = 0;

public:
  // чтение файла и запись переменных в мапу variable_types
  int read_file(const string &name_file, variable_types &vt);

  // поиск переменной в строке и запись ее в мапу variable_types
  void find_variable(const string &line, variable_types &vt);

  // удаление пробелов в начале и конце строки
  static string trim(const string &str, const string &chars);

  // поиск оператора в строке и обработка корректности выполнения
  void get_oprator(const string &line, variable_types &vt);

  // функция для вывода ошибок
  void print_error(const string &line, Error err);

  // помогает определить содержится ли функция в строке или нет
  bool is_function(const string &line);

  // функция удаления двойных пробелов
  void delete_double_space(string &line);

  // функция замены переменной на ее тип
  string replase_var_to_type(const string &line, variable_types &vt);

  // проверка выражений в строке на скобки и правильное расположение операторов
  void validate_expressions(const string &line, variable_types &vt,
                            const string &type = "");

  // проверка типа переменной и корректность операций с ними
  bool validate_type(const string &line, variable_types &vt);

  // проверка является ли этот тип double
  bool is_double(const string &type);

  // проверка на целочисленный
  bool is_int(const string &type);

  // Проверка на указатель
  bool is_pointer(const string &type);

  // проверка на корректность скобок
  bool validate_brackets(const string &line);

  // проверка на валидность операторов
  bool validate_operator(const string &line);

  // Увеличение текущей строки на 1
  void iter_line() { this->cur_line++; }

  static bool is_string_in_container(const std::set<std::string> &set,
                                     const std::string &str) {
    return set.find(str) != set.end();
  }
  static bool is_string_in_container(const std::vector<std::string> &vec,
                                     const std::string &str) {
    return std::find(vec.begin(), vec.end(), str) != vec.end();
  }
  static bool is_string_in_container(const std::vector<std::string> &vec,
                                     const char &ch) {
    return std::find(vec.begin(), vec.end(), string(1, ch)) != vec.end();
  }

  static bool is_string_in_container(const std::set<std::string> &set,
                                     const char &ch) {
    return set.find(string(1, ch)) != set.end();
  }

  bool is_type(const char &c);

  void replace_uno_minus(string &line);

  void add_space(string &line);

  bool check_postfix(const string &postfix);

  bool is_bin_operator(const string &st);
  int get_priority(const string &op);
};
// [ - integer
// ] - double
// @ pinter
#endif // PARSER_OPERATOR_VERSION_MAIN_H
