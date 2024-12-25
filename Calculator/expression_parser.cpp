#include "expression_parser.h"
#include <iostream>
#include <functional>
#include <cctype>
#include <stdexcept>


std::map<char, std::function<double(double, double)>> operations = {
    {'+', [](double a, double b) { return a + b; }},
    {'-', [](double a, double b) { return a - b; }},
    {'*', [](double a, double b) { return a * b; }},
    {'/', [](double a, double b) { return a / b; }}
};


class ExpressionParser {
public:
    ExpressionParser(const std::string& expression, const std::map<std::string, Func>& functions)
        : expression_(expression), functions_(functions), pos_(0) {}

    double parseExpression() {
        return parseAddSub();  
    }

private:
    std::string expression_;
    const std::map<std::string, Func>& functions_;
    size_t pos_;

    void skipWhitespace() {
        while (pos_ < expression_.length() && std::isspace(expression_[pos_])) {
            ++pos_;
        }
    }

    double parsePrimary() {
        skipWhitespace();


        if (std::isdigit(expression_[pos_]) || expression_[pos_] == '.') {
            double value = parseNumber();


            skipWhitespace();
            if (pos_ < expression_.length()) {
                char nextChar = expression_[pos_];

                if (nextChar != '+' && nextChar != '-' && nextChar != '*' && nextChar != '/' && nextChar != ')' && nextChar != '%') {
                    throw std::runtime_error("������������ ���������: ����� ����� ������ ���� ��������, ����������� ������ ��� ����� ���������.");
                }
            }

            return value;
        }


        else if (std::isalpha(expression_[pos_])) {
            double value = parseFunction();

            skipWhitespace();
            if (pos_ < expression_.length()) {
                char nextChar = expression_[pos_];
  
                if (nextChar != '+' && nextChar != '-' && nextChar != '*' && nextChar != '/' && nextChar != ')' && nextChar != '%') {
                    throw std::runtime_error("������������ ���������: ����� ������� ������ ���� ��������, ����������� ������ ��� ����� ���������.");
                }
            }

            return value;
        }


        else if (expression_[pos_] == '(') {
            double value = parseParentheses();


            skipWhitespace();
            if (pos_ < expression_.length()) {
                char nextChar = expression_[pos_];

                if (nextChar != '+' && nextChar != '-' && nextChar != '*' && nextChar != '/' && nextChar != ')' && nextChar != '%') {
                    throw std::runtime_error("������������ ���������: ����� ����������� ������ ������ ���� ��������, ����������� ������ ��� ����� ���������.");
                }
            }

            return value;
        }

        else {
            throw std::runtime_error("������������ ���������.");
        }
    }





    double parseNumber() {
        size_t startPos = pos_;
        while (pos_ < expression_.length() && (std::isdigit(expression_[pos_]) || expression_[pos_] == '.')) {
            ++pos_;
        }
        return std::stod(expression_.substr(startPos, pos_ - startPos));
    }

    double parseParentheses() {
        ++pos_;  
        double result = parseAddSub();
        if (pos_ >= expression_.length() || expression_[pos_] != ')') {
            throw std::runtime_error("��������� ����������� ������.");
        }
        ++pos_; 
        return result;
    }

    double parseFunction() {
        skipWhitespace();
        size_t startPos = pos_;


        while (pos_ < expression_.length() && std::isalpha(expression_[pos_])) {
            ++pos_;
        }
        std::string funcName = expression_.substr(startPos, pos_ - startPos);


        skipWhitespace();
        if (pos_ >= expression_.length() || expression_[pos_] != '(') {
            throw std::runtime_error("��������� ����������� ������ ��� ������� '" + funcName + "'.");
        }
        ++pos_;  

  
        skipWhitespace();
        try {
            double arg = parseAddSub();  

           
            skipWhitespace();
            if (pos_ >= expression_.length() || expression_[pos_] != ')') {
                throw std::runtime_error("��������� ����������� ������ ��� ������� '" + funcName + "'.");
            }
            ++pos_;

            auto it = functions_.find(funcName);
            if (it == functions_.end()) {
                throw std::runtime_error("������� '" + funcName + "' �� �������.");
            }
            return it->second(arg); 
        }
        catch (const std::invalid_argument&) {
            throw std::runtime_error("������������ �������� ��� ������� '" + funcName + "'. ��������� ���������.");
        }
        catch (const std::out_of_range&) {
            throw std::runtime_error("�������� ��� ������� '" + funcName + "' ������� �� ���������� �������.");
        }
    }




    double parseMulDiv() {
        double lhs = parsePrimary();
        while (true) {
            skipWhitespace();
            if (pos_ >= expression_.length()) return lhs;
            char op = expression_[pos_];
            if (op != '*' && op != '/') return lhs;
            ++pos_;
            double rhs = parsePrimary();
            lhs = operations[op](lhs, rhs);  
        }
    }

    double parseAddSub() {
        double lhs = parseMulDiv();
        while (true) {
            skipWhitespace();
            if (pos_ >= expression_.length()) return lhs;
            char op = expression_[pos_];
            if (op != '+' && op != '-') return lhs;
            ++pos_;
            double rhs = parseMulDiv();
            lhs = operations[op](lhs, rhs);  
        }
    }
};


double calculate(const std::string& expression, const std::map<std::string, Func>& functions) {
    ExpressionParser parser(expression, functions);
    return parser.parseExpression();
}
