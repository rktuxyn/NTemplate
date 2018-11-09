/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "core.h"
inline void 
NTemplate::ProcessCallback(v8::Isolate* isolate, TemplateResult * rsinf, Settings* settings, v8::Local<v8::Value> value) {
	v8::Isolate::Scope					isolate_scope(isolate);
	v8::Local<v8::Context> tcontext = isolate->GetCurrentContext();
	v8::Context::Scope					context_scope(tcontext);
	v8::Handle<v8::Value>				arg[1];

	v8::Handle<v8::Object> rsp = v8::Handle<v8::Object>::New(isolate, v8::Object::New(isolate));
	if (rsinf->is_error == true) {
		rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, rsinf->is_error));
		rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, rsinf->err_msg.c_str()));
	}
	else {
		rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, false));
		rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, ""));
		if (value->IsNullOrUndefined()) {
			rsp->Set(v8::String::NewFromUtf8(isolate, "result"), v8::String::NewFromUtf8(isolate, rsinf->t_source.c_str()));
		}
		else {
			rsp->Set(v8::String::NewFromUtf8(isolate, "result"), value);
		}

	}
	rsinf->t_source.clear();
	arg[0] = { rsp };
	v8::Local<v8::Function>::New(isolate, settings->callback)->Call(tcontext->Global(), 1, arg);
	rsp.Clear(); arg->Clear();
	return;
};

void NTemplate::ParseInternal(const v8::FunctionCallbackInfo<v8::Value>& args) {

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope					scope(isolate);
	Settings* settings = new Settings();
	NTemplate::GetSettings(isolate, args, settings);
	if (settings->isError) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, settings->errorMsg.c_str())));
		settings->Clear();
		isolate = NULL;
		return;
	}

	if (settings->isAsync) {
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
		NTemplate::Async::DoAsyncTask(isolate, settings);
		args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "START ASYNC"));
		//Will be Dispose While Thread Exited!!!
		//isolate = NULL;
		return;
#endif //!_M_CEE

	}
	TemplateResult * rsinf = new TemplateResult();
	TemplateParser* tParser = new NTemplate::TemplateParser(rsinf, settings->pagePath.c_str(), settings->dir);
	tParser->Start();
	if (rsinf->is_error == true) {
		if (!settings->callback.IsEmpty()) {
			ProcessCallback(isolate, rsinf,
				settings, v8::Local<v8::Value>::New(isolate, v8::Null(isolate))
			);
			settings->Clear();
			args.GetReturnValue().Set(v8::Undefined(isolate));
			return;
		}
		settings->Clear();
		args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, rsinf->err_msg.c_str()));
		return;
	}
	if (rsinf->is_script == false) {
		if (!settings->callback.IsEmpty()) {
			ProcessCallback(isolate, rsinf,
				settings, v8::Local<v8::Value>::New(isolate, v8::Null(isolate))
			);
			settings->Clear();
			args.GetReturnValue().Set(v8::Undefined(isolate));
			return;
		}
		settings->Clear();
		args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, rsinf->t_source.c_str()));
		return;
	}
#if _TEST_RUNTIME
	NTemplate::WriteFile("function ___NTEMPLATE__SCRIPT__RUNNER (data){ try{\r\n" + rsinf->t_source + "\r\n}catch(e){\r\nreturn e.message;\r\n}\r\n};");
#endif
	if (settings->isObject) {
		if (!settings->callback.IsEmpty()) {
			ProcessCallback(isolate, rsinf, settings,
				NTemplate::Compiler::RunTemplateScript(isolate, rsinf->t_source, settings->data)
			);
			settings->Clear();
			args.GetReturnValue().Set(v8::Undefined(isolate));
			return;
		}
		args.GetReturnValue().Set(NTemplate::Compiler::RunTemplateScript(isolate, rsinf->t_source, settings->data));
	}
	else {
		if (!settings->callback.IsEmpty()) {
			ProcessCallback(isolate, rsinf, settings,
				NTemplate::Compiler::RunTemplateScript(isolate, rsinf->t_source, args[2])
			);
			settings->Clear();
			args.GetReturnValue().Set(v8::Undefined(isolate));
			return;
		}
		args.GetReturnValue().Set(NTemplate::Compiler::RunTemplateScript(isolate, rsinf->t_source, args[2]));
	}
	settings->Clear();
	return;

};
void
NTemplate::Parse(const v8::FunctionCallbackInfo<v8::Value>& args) {

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope					scope(isolate);
	if (!args[0]->IsString()) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, "String Required!!! (APP DIRECTORY PATH) :(")));
		isolate = NULL;
		return;
	}
	if (!args[1]->IsString()) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, "String Required!!! (PAGE_PATH) :(")));
		isolate = NULL;
		return;
	}
	v8::String::Utf8Value			utfDirStr(isolate, args[0]);
	v8::String::Utf8Value			utfPagePathStr(isolate, args[1]);
	const char* pagePath = T_CHAR(utfPagePathStr);
	std::string							dir(T_CHAR(utfDirStr));
	try {
		TemplateResult* rsinf = new TemplateResult();
		rsinf->remove_new_line = true;
		TemplateParser* tParser = new NTemplate::TemplateParser(rsinf, pagePath, dir);
		tParser->Start();
		if (rsinf->is_error == true) {
			args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, rsinf->err_msg.c_str()));
			//delete[]rsinf; delete[]tParser;
			return;
		}
		if (rsinf->is_script == false) {
			args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, rsinf->t_source.c_str()));
			return;
		}
#if _WRITE_OUTPUT
		WriteFile("function ___NTEMPLATE__SCRIPT__RUNNER (data){ try{\r\n" + rsinf->t_source + "\r\n}catch(e){\r\nreturn e.message;\r\n}\r\n};");
#endif
		args.GetReturnValue().Set(NTemplate::Compiler::RunTemplateScript(isolate, rsinf->t_source, args[2]));
		return;
	} catch (std::exception&e) {
		args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, e.what()));
		return;
	}

};
