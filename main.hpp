#ifndef PARSER_OPERATOR_VERSION_MAIN_H
#define PARSER_OPERATOR_VERSION_MAIN_H

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <regex>

enum  Error {
    SUCCESS,
    MALLOC_ERROR,
    INVALID_BRACETS_VISION,
    UNDEFIND_ERROR,
    INVALID_DEFULT_INITIAL,
    INVALID_VARIBLE_INIT,
    MANY_CONST,
    INITIALIZATION_LIST_ERROR,
    OPEN_FILE_ERROR
};



using namespace std;
class base_type {
protected:
    //множество типов, которые могут быть в файле
     set<string> type_set;
public:
    //получение типов из файла и запись их в множество type_set
    int get_type(const string &name_file);
    //получение множества типов из класса base_type
    set<string> get_set(){
        return type_set;
    }
};

class handler_data;
class variable_types : public base_type{
private:
    //мапа, где ключ - тип переменной, а значение - вектор имен переменных
    map<string, vector<string>> variable {};
public:
    variable_types(const string& name_file){
        get_type(name_file);
    }
    //добавление переменной в мапу
    void add_variable(const string& type, const string& name);
};

class handler_data {
public:
    //чтение файла и запись переменных в мапу variable_types
    int read_file(const string &name_file, variable_types &vt);
    //поиск переменной в строке и запись ее в мапу variable_types
    void find_variable(const string & line, variable_types &vt);
    //удаление пробелов в начале и конце строки
    static string trim(const string &str, const string &chars);
    //поиск оператора в строке и обработка корректности выполнения
    void get_oprator(const string &line, variable_types &vt);

    void print_error(const string &line, size_t current_line, Error err);
    bool is_function(const string &line);
    void delete_double_space(string &line);

};


#endif //PARSER_OPERATOR_VERSION_MAIN_H
