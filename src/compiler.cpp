/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "compiler.h"
using namespace NTemplate;
inline void NTemplate::Compiler::RunScript(v8::Isolate* isolate, v8::Local<v8::String> source) {
#if _HAS_EXCEPTIONS
	try {
#endif	
		if (source->IsUndefined() || source->IsNull()) {
			throw new std::exception("String Required!!! (Script) :(");
		}
		v8::HandleScope handle_scope(isolate);
		v8::Local<v8::Context> context = v8::Context::New(isolate);
		//v8::Script::Compile(context, source).ToLocalChecked()->Run(context);
		v8::Script::Compile(source)->Run(context);
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, e.what())));
	}

#endif
};
void NTemplate::Compiler::RunScript(const v8::FunctionCallbackInfo<v8::Value>& args) {

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
#if _HAS_EXCEPTIONS
	try {
#endif	
		v8::HandleScope scope(isolate);
		if (!args[0]->IsString()) {
			isolate->ThrowException(v8::Exception::TypeError(
				v8::String::NewFromUtf8(isolate, "String Required!!! (Script) :(")));
			return;
		}
		NTemplate::Compiler::RunScript(isolate, v8::Local<v8::String>::Cast(args[0]));
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, e.what())));
	}

#endif
};

v8::Local<v8::Value> NTemplate::Compiler::ParseScript(v8::Isolate * isolate, v8::Local<v8::String> source, v8::Local<v8::Value> innerFunc, v8::Local<v8::Value> innerArg, bool isAsync) {
	
	if (isolate->IsDead()) {
#if _HAS_EXCEPTIONS
		throw new std::exception("ISOLATE IS DEAD :(");
#else
		return v8::String::NewFromUtf8(isolate, "ISOLATE IS DEAD :(");
#endif
	}
	if (source->IsUndefined() || source->IsNull()) {
#if _HAS_EXCEPTIONS
		throw new std::exception("String Required!!! (Script) :(");
#else
		return v8::String::NewFromUtf8(isolate, "String Required!!! (Script) :(");
#endif
	}
	if (innerFunc->IsUndefined() || innerFunc->IsNull() || !innerFunc->IsString()) {
#if _HAS_EXCEPTIONS
		throw new std::exception("Inner Call Required!!! (String) :(");
#else
		return v8::String::NewFromUtf8(isolate, "Inner Call Required!!! (String) :(");
#endif
	}
#if _HAS_EXCEPTIONS
	try {
#endif	
#if _TEST_RUNTIME
		std::cout << "NTemplate::Compiler::ParseScript..." << "\r\n";
#endif
		v8::Isolate::Scope isolate_scope(isolate);
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		v8::Context::Scope context_scope(context);
		/**[Create a string containing the JavaScript source code]*/
		/**[Run Script With Context]*/
		//v8::Script::Compile(context, source).ToLocalChecked()->Run(context);
		v8:: Script::Compile(source)->Run(context);
#if _TEST_RUNTIME
		std::cout << "NTemplate::Compiler::ParseScript-->Script Compile Completed..." << "\r\n";
#endif
		v8::Local<v8::Object> jsGlobal = context->Global();
		v8::Local<v8::Function> jsUpdateFunc = v8::Handle<v8::Function>::Cast(jsGlobal->Get(innerFunc));
		if (jsUpdateFunc->IsUndefined() || jsUpdateFunc->IsNull()) {
			return v8::String::NewFromUtf8(isolate, "INVALID FUNCTION OBJECT DEFINED!!!");
		}
		if (isAsync) {
			return jsUpdateFunc;
		}
		v8::Handle<v8::Value> arg[1];
		if (!innerArg->IsUndefined() || innerArg->IsNull()) {
			arg[0] = { innerArg };
		}
		else {
			arg[0] = { v8::Undefined(isolate) };
		}
		return jsUpdateFunc->Call(
			jsGlobal, 1, arg
		);
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, e.what())));
	}
#endif
	return v8::Undefined(isolate);

};
/**
* Parse & Run Javascript from string object
* Params @ [Script Source@String, Inner Function@String, Callback@Function]
*/
void Compiler::ParseRawScript(const v8::FunctionCallbackInfo<v8::Value>& args) {

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
#if _HAS_EXCEPTIONS
	try {
#endif
		/**
		* Build Nov 10, 2017, 02:37 AM GMT +6
		*/
		v8::HandleScope handle_scope(isolate);
		if (args[1]->IsUndefined() || args[1]->IsNull() || !args[0]->IsString()) {
			isolate->ThrowException(v8::Exception::TypeError(
				v8::String::NewFromUtf8(isolate, "String Required!!! (Script) :(")));
			return;
		}
		if (args[1]->IsUndefined() || args[1]->IsNull() || !args[2]->IsString()) {
			isolate->ThrowException(v8::Exception::TypeError(
				v8::String::NewFromUtf8(isolate, "Inner Call Required!!! (String) :(")));
			return;
		}
		v8::Local<v8::Value> resp = Compiler::ParseScript(isolate, v8::Local<v8::String>::Cast(args[0]), v8::Local<v8::Value>::Cast(args[2]), v8::Local<v8::Value>::Cast(args[3]));
		if (!args[1]->IsFunction()) {
			args.GetReturnValue().Set(resp);
			return;
		}
		v8::Handle<v8::Value> argv[2];
		argv[0] = { resp };
		if (args[2]->IsString()) {
			argv[0] = { resp };
		}
		else {
			argv[0] = v8::String::NewFromUtf8(isolate, "SUCCESS");
		}
		argv[1] = v8::String::NewFromUtf8(isolate, "SUCCESS");
		v8::Local<v8::Function>::/**[Cast Call back from Arguments]*/Cast(args[1])->Call(/**[Current Object Inharitence]*/args.Holder(), 2, argv);
		args.GetReturnValue().Set(resp);
		return;
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, e.what())));
	}

#endif
};
v8::Local<v8::Value> NTemplate::Compiler::RunTemplateScript(v8::Isolate * isolate, std::string& source, v8::Local<v8::Value> innerArg, bool isAsync) {
	source = "function ___NTEMPLATE__SCRIPT__RUNNER (data){\r\ntry{\r\n" + source + "\r\n}catch(e){\r\nreturn e.message;\r\n}\r\n};";
	v8::Local<v8::String> _source = v8::String::NewFromUtf8(isolate, source.c_str());
	source.clear();
	v8::Local<v8::String> innerFunc = v8::String::NewFromUtf8(isolate, "___NTEMPLATE__SCRIPT__RUNNER");
	return Compiler::ParseScript(isolate, _source, innerFunc, innerArg, isAsync);
}
;