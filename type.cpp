#include "./main.hpp"

//TODO: add parser var with = handler
//TODO: add parser polish notation
//TODO: add validation of the variable
//TODO: add comments skip handler

int base_type::get_type(const string &name_file){
    int ret = Error::SUCCESS;
    std::ifstream file(name_file);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\n') {
                line.pop_back();
            }
            type_set.insert(line);
        }
        file.close();
    } else {
        ret = Error::OPEN_FILE_ERROR;
    }
    return ret;
}

int handler_data::read_file(const string &name_file, variable_types &vt) {
    std::ifstream file(name_file);
    if (!file.is_open()) {
        return Error::OPEN_FILE_ERROR;
    }
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\n') {
                line.pop_back();
            }
            find_variable(line, vt);
        }
        file.close();
    return Error::SUCCESS;
}

void handler_data::find_variable(const string &line, variable_types &vt) {
    string type;
    string copy_line = line;
    copy_line = trim(copy_line, " ");
    delete_double_space(copy_line);
    size_t pos_first_space = copy_line.find(' ');

    if (pos_first_space != string::npos) {
        size_t pos_second_space = copy_line.find(' ', pos_first_space + 1);

        string uno_type = copy_line.substr(0, pos_first_space);
        string bin_type = copy_line.substr(0, pos_second_space);

        if (vt.get_set().find(uno_type) != vt.get_set().end() && !is_function(copy_line)) {
            type = uno_type;
        } else if (vt.get_set().find(bin_type) != vt.get_set().end() && !is_function(copy_line)) {
            type = bin_type;
        }

        if (!is_function(copy_line)) {
            string name = copy_line.substr(type.size(), copy_line.size());
            name = trim(name, " ;{}");
            vt.add_variable(type, name);
        }
    }
}

string handler_data::trim(const string &str, const string &chars) {
    size_t first = str.find_first_not_of(chars);
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(chars);
    return str.substr(first, (last - first + 1));
}

void handler_data::get_oprator(const string &line, variable_types &vt) {

}

void variable_types::add_variable(const string &type, const string &name) {
    variable[type].push_back(name);
}

void handler_data::print_error(const string &line, size_t current_line,
                            Error err) {
    switch (err) {
        case INVALID_BRACETS_VISION:
            cout << "In line: " << current_line + 1
                 << " error in quotes or scope operator line: *ฅ^•ﻌ•^ฅ*" << endl;
            break;
        case INVALID_VARIBLE_INIT:
            cout << "INVALID VARIABLE INIT (☞ ͡° ͜ʖ ͡°)☞ line: " << line << endl;
            break;
        case MALLOC_ERROR:
            cout << "Memory error ＼(￣▽￣)／" << endl;
            break;
        case UNDEFIND_ERROR:
            cout << "Undefined  error ¯\\_(ツ)_/¯ LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;
        case MANY_CONST:
            cout << "more than 1 constructors in 1 line (╯°益°)╯彡┻━┻ LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;
        case INITIALIZATION_LIST_ERROR:
            cout << "INITIALIZATION LIST ERROR (•_•) LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;
        case OPEN_FILE_ERROR:
            cout << "can't open file ʕ•͡-•ʔ " << endl;
            break;
        case INVALID_DEFULT_INITIAL:
            cout << "INVALID DEFAULT INITIAL (◣ _ ◢) LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;

        default:
            cout << "Undefined  error (╯°益°)╯彡┻━┻ LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;
    }
}

bool handler_data::is_function(const string &line) {
        // Создаем регулярное выражение для проверки функции
        std::regex functionRegex(R"(^[a-zA-Z_][a-zA-Z0-9_]*\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(.*\)\s*\{.*)");
        // Проверяем входную строку на соответствие регулярному выражению
        return  std::regex_match(trim(line," "), functionRegex);
}

void handler_data::delete_double_space(string &line) {
    size_t pos = line.find("  ");
    while (pos != string::npos) {
        line.erase(pos, 1);
        pos = line.find("  ");
    }
}
