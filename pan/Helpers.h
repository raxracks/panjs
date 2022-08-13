#pragma once
#include <ChakraCore.h>
#include <string>

static class Helpers {
public:
	static void SetProperty(JsValueRef object, const wchar_t* propertyName, JsValueRef property);
	static void SetCallback(JsValueRef object, const wchar_t* propertyName, JsNativeFunction callback, void* callbackState);
	static void BindNativeFunctions();
	static JsRuntimeHandle CreateRuntimeAndContext();
	static std::string ReadFile(const std::string& fileName);
	static std::wstring StringToWString(const std::string& s);
	static std::string WStringToString(std::wstring wide);
	static bool StringEndsWith(std::string value, std::string ending);
	static bool StringStartsWith(std::string value, std::string starting);
	static std::string& LeftTrim(std::string& str, std::string& chars);
};