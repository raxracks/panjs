#include <ChakraCore.h>
#include <string>
#include <iostream>
#include <vector>
#include "NativeFunctions.h"
#include "Helpers.h"
#include "pan.h"

JsValueRef CALLBACK NativeFunctions::Console::Log(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
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

JsValueRef CALLBACK NativeFunctions::Console::Clear(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
#ifndef WIN32
	system("clear");
#else
	system("cls");
#endif

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
	std::wstring Directory = L"";

	JsValueRef stringValue;
	JsConvertValueToString(arguments[2], &stringValue);
	const wchar_t* string;
	size_t length;
	JsStringToPointer(stringValue, &string, &length);

	Directory = string;

	std::cout << "Require file: " << FileName << " Dir: " << Helpers::WStringToString(Directory) << std::endl;

	if(Helpers::StringStartsWith(FileName, ".")) {
		FileName = Helpers::WStringToString(Directory) + FileName;
	}

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

		std::size_t DirectoryPosition = FileName.find_last_of("/");
		Directory = Helpers::StringToWString(FileName.substr(0, DirectoryPosition));
	}

	if (Helpers::StringStartsWith(FileName, ".") && !Helpers::StringEndsWith(FileName, ".js")) FileName += ".js";

	// check if module and its exports already exist in cache
	for (std::pair<std::string, JsValueRef> CacheItem : RequireCache) {
		if (CacheItem.first == FileName) {
			// return module exports if in cache
			return CacheItem.second;
		}
	}

	JsValueRef ExportsResult;

	std::cout << "Read File: " << FileName << std::endl;

	std::wstring ModuleScript = Helpers::StringToWString(Helpers::ReadFile(FileName));

	// monstrosity
	ModuleScript =
		L"(function(module,exports,path){exports=module.exports;\n"
		L"function require(p){console.log(\"rqr\", p, path);return NATIVE_REQUIRE(p,path)};\n" +
		ModuleScript +
		L"\nreturn typeof module.exports!==\"object\"||typeof exports!==\"object\"?typeof module.exports!==\"object\"?module.exports:exports:module.exports.length>0&&exports.length>0?module.exports.length>0?module.exports:exports:this;\n})({id:'.',exports:{}},undefined,\"" + Directory + L"\"); ";

	// run module script and increment source context
	JsRunScript(ModuleScript.c_str(), CurrentSourceContext, L"", &ExportsResult);

	// convert the exports to a js object
	JsValueRef ModuleExportsObject;
	JsConvertValueToObject(ExportsResult, &ModuleExportsObject);

	RequireCache.push_back(std::make_pair(FileName, ModuleExportsObject));

	return ModuleExportsObject;
}