#include <ChakraCore.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

#include "Helpers.h"
#include "pan.h"

#define VERSION "0.0.1"

unsigned CurrentSourceContext = 0;

int main(int argc, char* argv[])
{
	JsValueRef result;

	JsRuntimeHandle runtime = Helpers::CreateRuntimeAndContext();
	Helpers::BindNativeFunctions();

	std::wstring script;

	bool Looping = true;

	if (argc <= 1) {
		std::cout << "pan v" << VERSION << std::endl;
		std::cout << "This REPL is currently incredibly buggy, it is far better to just execute files." << std::endl << std::endl;
	}

	while (Looping) {
		if (argc > 1)
			script = Helpers::StringToWString(Helpers::ReadFile(argv[1]));
		else {
			std::cout << "> ";
			std::wcin >> script;
		}

		script = L"function require(p) {return NATIVE_REQUIRE(p, \"\")};\n" + script;

		// run script and increment source context
		JsRunScript(script.c_str(), CurrentSourceContext, L"", &result);

		// convert the result to a js string
		/*JsValueRef resultJSString;
		JsConvertValueToString(result, &resultJSString);*/

		// convert the resultant js string to a c++ wide char string
		//const wchar_t* resultWC;
		//size_t stringLength;
		//JsStringToPointer(resultJSString, &resultWC, &stringLength);

		//std::wstring resultW(resultWC);

		//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		//SetConsoleTextAttribute(hConsole, resultW == L"undefined" ? 8 : 7);
		////std::cout << std::string(resultW.begin(), resultW.end()) << std::endl;
		//SetConsoleTextAttribute(hConsole, 7);

		if (argc > 1) Looping = false;
	}

	JsSetCurrentContext(JS_INVALID_REFERENCE);
	JsDisposeRuntime(runtime);

	return 0;
}