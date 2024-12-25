#include <iostream>
#include <map>
#include <string>
#include "dll_manager.h"
#include "expression_parser.h"

int main() {
    std::map<std::string, Func> functions;
    setlocale(LC_CTYPE, "Russian");

    try {
        loadAllPlugins("./plugins", functions);

        std::cout << "Загружено функций: " << functions.size() << std::endl;

        if (functions.empty()) {
            std::cerr << "Не удалось загрузить функции из DLL" << std::endl;
            return 1;
        }

        std::string expression;
        while (true) {
            std::cout << "Введите выражение (или введите 'exit' для выхода): ";
            std::getline(std::cin, expression);

            if (expression == "exit" || expression.empty()) {
                std::cout << "Выход из программы." << std::endl;
                break;
            }

            try {
                double result = calculate(expression, functions);
                std::cout << "Результат: " << result << std::endl;
            }
            catch (const std::exception& ex) {
                std::cerr << "Ошибка при вычислении: " << ex.what() << std::endl;
            }
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
    }

    return 0;
}
