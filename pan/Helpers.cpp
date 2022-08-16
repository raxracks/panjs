#include <ChakraCore.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>

#include "Helpers.h"
#include "NativeFunctions.h"
#include "Task.h"

void Helpers::SetProperty(JsValueRef object, const wchar_t* propertyName, JsValueRef property)
{
    JsPropertyIdRef propertyId;
    JsGetPropertyIdFromName(propertyName, &propertyId);
    JsSetProperty(object, propertyId, property, true);
}

void Helpers::SetCallback(JsValueRef object, const wchar_t* propertyName, JsNativeFunction callback, void* callbackState)
{
    JsPropertyIdRef propertyId;
    JsGetPropertyIdFromName(propertyName, &propertyId);
    JsValueRef function;
    JsCreateFunction(callback, callbackState, &function);
    JsSetProperty(object, propertyId, function, true);
}

void Helpers::BindNativeFunctions() {
    JsValueRef globalObject;
    JsGetGlobalObject(&globalObject);
    JsValueRef console;
    JsCreateObject(&console);

    Helpers::SetCallback(globalObject, L"NATIVE_REQUIRE", NativeFunctions::Require, nullptr);

    Helpers::SetCallback(globalObject, L"setTimeout", NativeFunctions::SetTimeout, nullptr);
    Helpers::SetCallback(globalObject, L"setInterval", NativeFunctions::SetInterval, nullptr);

    Helpers::SetProperty(globalObject, L"console", console);
    Helpers::SetCallback(console, L"log", NativeFunctions::Console::Log, nullptr);
    Helpers::SetCallback(console, L"clear", NativeFunctions::Console::Clear, nullptr);

    NativeFunctions::Bootstrap();
}

JsRuntimeHandle Helpers::CreateRuntimeAndContext() {
    JsRuntimeHandle runtime;
    JsContextRef context;

    // Create a runtime. 
    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);

    // Create an execution context. 
    JsCreateContext(runtime, &context);

    // Now set the current execution context.
    JsSetCurrentContext(context);

    return runtime;
}

std::string Helpers::ReadFile(const std::string& fileName)
{
    std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return std::string(bytes.data(), fileSize);
}

std::wstring Helpers::StringToWString(const std::string& s)
{
    std::wstring ws(s.begin(), s.end());

    return ws;
}

std::string Helpers::WStringToString(std::wstring wide)
{
    std::string str;

    std::transform(wide.begin(), wide.end(), std::back_inserter(str), [](wchar_t c) {
        return (char)c;
    });

    return str;
}

bool Helpers::StringEndsWith(std::string value, std::string ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool Helpers::StringStartsWith(std::string value, std::string starting) {
    return value.rfind(starting, 0) == 0;
}

std::string& Helpers::LeftTrim(std::string& str, std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}