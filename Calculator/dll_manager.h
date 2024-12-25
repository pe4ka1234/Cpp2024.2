#pragma once
#ifndef DLL_MANAGER_H
#define DLL_MANAGER_H

#include <map>
#include <string>

typedef double(*Func)(double);

void loadAllPlugins(const std::string& pluginDirectory, std::map<std::string, Func>& functions);

#endif 
