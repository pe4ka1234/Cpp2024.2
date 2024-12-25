#include "dll_manager.h"
#include <windows.h>
#include <filesystem>
#include <iostream>


std::string getExportedFunctionName(HMODULE hDll) {
    BYTE* baseAddress = (BYTE*)hDll;
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)baseAddress;
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        throw std::runtime_error("Неверный DOS-заголовок.");
    }

    IMAGE_NT_HEADERS* ntHeaders = (IMAGE_NT_HEADERS*)(baseAddress + dosHeader->e_lfanew);
    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
        throw std::runtime_error("Неверный PE-заголовок.");
    }

    IMAGE_EXPORT_DIRECTORY* exportDir = (IMAGE_EXPORT_DIRECTORY*)(baseAddress +
        ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

    DWORD* nameArray = (DWORD*)(baseAddress + exportDir->AddressOfNames);
    const char* funcName = (const char*)(baseAddress + nameArray[0]);
    
    if (IsBadReadPtr(funcName, sizeof(char))) {
        throw std::runtime_error("Не удалось загрузить функцию - некорректный dll.");
    }

    return std::string(funcName);
}

Func getFirstFunction(HMODULE hDll) {
    std::string funcName = getExportedFunctionName(hDll);
    Func func = reinterpret_cast<Func>(GetProcAddress(hDll, funcName.c_str()));
    if (!func) {
        throw std::runtime_error("Не удалось найти функцию в DLL");
    }

    std::cout << "Функция с именем '" << funcName << "' загружена." << std::endl;
    return func;
}

void loadPlugin(const std::string& path, std::map<std::string, Func>& functions) {
    HMODULE hDll = LoadLibraryA(path.c_str());
    if (!hDll) {
        throw std::runtime_error("Не удалось загрузить DLL: " + path);
    }
    try {
        std::string funcName = getExportedFunctionName(hDll);
        Func func = getFirstFunction(hDll);
        std::cout << "Загружена функция из: " << path << std::endl;
        functions[funcName] = func;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        FreeLibrary(hDll);
    }
}

void loadAllPlugins(const std::string& pluginDirectory, std::map<std::string, Func>& functions) {
    if (!std::filesystem::exists(pluginDirectory) || std::filesystem::is_empty(pluginDirectory)) {
        throw std::runtime_error("Не удалось найти плагины в директории: " + pluginDirectory);
    }

    for (const auto& entry : std::filesystem::directory_iterator(pluginDirectory)) {
        if (entry.path().extension() == ".dll") {
            std::cout << "DLL: " << entry.path().filename().string() << std::endl;
            loadPlugin(entry.path().string(), functions);
        }
    }

    if (functions.empty()) {
        throw std::runtime_error("Не удалось загрузить ни одной функции из DLL.");
    }
}
