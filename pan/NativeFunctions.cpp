#include <ChakraCore.h>
#include <string>
#include <iostream>
#include <vector>
#include "NativeFunctions.h"
#include "Helpers.h"
#include "pan.h"

JsValueRef CALLBACK NativeFunctions::Log(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	for (unsigned int index = 1; index < argumentCount; index++)
	{
		if (index > 1)
		{
			wprintf(L" ");
		}

		JsValueRef stringValue;
		JsConvertValueToString(arguments[index], &stringValue);
		const wchar_t* string;
		size_t length;
		JsStringToPointer(stringValue, &string, &length);
		wprintf(L"%s", string);
	}

	wprintf(L"\n");

	return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK NativeFunctions::Clear(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	system("cls");

	return JS_INVALID_REFERENCE;
}

std::vector<std::pair<std::string, JsValueRef>> RequireCache;

JsValueRef CALLBACK NativeFunctions::Require(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	JsValueRef FileStringValue;
	JsConvertValueToString(arguments[1], &FileStringValue);
	const wchar_t* FileNameW;
	size_t Length;
	JsStringToPointer(FileStringValue, &FileNameW, &Length);
	std::string FileName = Helpers::WStringToString(FileNameW);

	if (Helpers::StringEndsWith(FileName, ".json")) {
		std::wstring JSONContent = Helpers::StringToWString(Helpers::ReadFile(FileName));

		JsValueRef JSONResult;

		JsRunScript(((std::wstring)L"(function() {\nreturn " + JSONContent + L"\n})();").c_str(), CurrentSourceContext++, L"", &JSONResult);

		JsValueRef JSONObject;
		JsConvertValueToObject(JSONResult, &JSONObject);

		return JSONObject;
	}

	if (!Helpers::StringStartsWith(FileName, ".")) {
		std::string PackageJSON = Helpers::ReadFile("node_modules/" + FileName + "/package.json");
		PackageJSON = PackageJSON.substr(PackageJSON.find("\"main\":") + 7);

		std::string chars_to_trim = " \t";

		Helpers::LeftTrim(PackageJSON, chars_to_trim);

		PackageJSON = PackageJSON.substr(1);

		PackageJSON = PackageJSON.erase(PackageJSON.find_first_of("\""));

		FileName = "./node_modules/" + FileName + "/" + PackageJSON;
	}

	if (Helpers::StringStartsWith(FileName, ".") && !Helpers::StringEndsWith(FileName, ".js")) FileName += ".js";

	std::cout << "Require file: " << FileName << std::endl;

	// check if module and its exports already exist in cache
	for (std::pair<std::string, JsValueRef> CacheItem : RequireCache) {
		if (CacheItem.first == FileName) {
			// return module exports if in cache
			return CacheItem.second;
		}
	}

	JsValueRef ExportsResult;
	
	std::wstring ModuleScript = Helpers::StringToWString(Helpers::ReadFile(FileName));

	ModuleScript = L"(function(module) {\n" + ModuleScript + L"\nreturn module.exports;\n})({id:'.', exports:{}});";

	// run module script and increment source context
	JsRunScript(ModuleScript.c_str(), CurrentSourceContext++, L"", &ExportsResult);

	// convert the exports to a js object
	JsValueRef ModuleExportsObject;
	JsConvertValueToObject(ExportsResult, &ModuleExportsObject);

	RequireCache.push_back(std::make_pair(FileName, ModuleExportsObject));

	return ModuleExportsObject;
}