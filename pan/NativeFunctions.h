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
	static void CALLBACK PromiseContinuationCallback(JsValueRef task, void* callbackState);
	static JsValueRef CALLBACK Require(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
	static JsValueRef CALLBACK SetTimeout(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
	static JsValueRef CALLBACK SetInterval(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
	static void Bootstrap();
	static void WhileQueueNotEmpty();
};