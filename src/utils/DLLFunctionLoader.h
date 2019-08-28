#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <windows.h>

template <typename T>
struct TypeParser {};

template <typename Ret, typename... Args>
struct TypeParser<Ret(Args...)> {
	static std::function<Ret(Args...)> createFunction(const FARPROC lpfnGetProcessID) {
		return std::function<Ret(Args...)>(reinterpret_cast<Ret(__stdcall*)(Args...)>(lpfnGetProcessID));
	}
};

template <typename T>
std::function<T> loadDllFunc(const std::string& dllName, const std::string& funcName) {
	// Load DLL.
	HINSTANCE hGetProcIDDLL = LoadLibraryA(dllName.c_str());

	// Check if DLL is loaded.
	if (hGetProcIDDLL == NULL) {
		std::cerr << "Could not load DLL \"" << dllName << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Locate function in DLL.
	FARPROC lpfnGetProcessID = GetProcAddress(hGetProcIDDLL, funcName.c_str());

	// Check if function was located.
	if (!lpfnGetProcessID) {
		std::cerr << "Could not locate the function \"" << funcName << "\" in DLL\"" << dllName << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Create function object from function pointer.
	return TypeParser<T>::createFunction(lpfnGetProcessID);
}
