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

        std::cout << "��������� �������: " << functions.size() << std::endl;

        if (functions.empty()) {
            std::cerr << "�� ������� ��������� ������� �� DLL" << std::endl;
            return 1;
        }

        std::string expression;
        while (true) {
            std::cout << "������� ��������� (��� ������� 'exit' ��� ������): ";
            std::getline(std::cin, expression);

            if (expression == "exit" || expression.empty()) {
                std::cout << "����� �� ���������." << std::endl;
                break;
            }

            try {
                double result = calculate(expression, functions);
                std::cout << "���������: " << result << std::endl;
            }
            catch (const std::exception& ex) {
                std::cerr << "������ ��� ����������: " << ex.what() << std::endl;
            }
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "������: " << ex.what() << std::endl;
    }

    return 0;
}
