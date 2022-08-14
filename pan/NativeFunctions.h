#pragma once
#include <ChakraCore.h>
#include <vector>
#include <string>

static class NativeFunctions {
public:
	static class Console {
	public:
		static JsValueRef CALLBACK Log(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
		static JsValueRef CALLBACK Clear(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
	};
	static JsValueRef CALLBACK Require(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
};