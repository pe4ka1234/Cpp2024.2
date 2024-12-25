#pragma once
#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <map>
#include <string>

typedef double(*Func)(double);


double calculate(const std::string& expression, const std::map<std::string, Func>& functions);

#endif 
