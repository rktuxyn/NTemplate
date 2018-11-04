#if defined(_MSC_VER)
#  pragma once
#endif
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
#ifndef NTEMPLATE_ARG_
#include "narg.h"
#endif //!NTEMPLATE_ARG_
#ifndef NTEMPLATE_COMP_
#include "compiler.h"
#endif //!NTEMPLATE_COMP_
#ifndef NTEMPLATE_ASYNC_
#include "async.h"
#endif //!NTEMPLATE_ASYNC_
#ifndef _NT_
#define _NT_ NTemplate::
#endif
namespace NTemplate {
	template<class _Isolate, class _Settings, class _Result, class _Value>
	void ProcessCallback(_Isolate* isolate,
		_Result * rsinf,
		_Settings* settings,
		_Value value
	) {
		{
			_V8 Isolate::Scope					isolate_scope(isolate);
			_V8 Local<_V8 Context> tcontext = isolate->GetCurrentContext();
			_V8 Context::Scope					context_scope(tcontext);
			_V8 Handle<_V8 Value>				arg[1];
			_V8 Handle<_V8 Object> rsp = _V8 Handle<_V8 Object>::New(isolate, _V8 Object::New(isolate));
			if (rsinf->is_error() == true) {
				rsp->Set(_V8 String::NewFromUtf8(isolate, "is_error"), _V8 Boolean::New(isolate, rsinf->is_error()));
				rsp->Set(_V8 String::NewFromUtf8(isolate, "err_msg"), _V8 String::NewFromUtf8(isolate, rsinf->error_msg()->c_str()));
			}
			else {
				rsp->Set(_V8 String::NewFromUtf8(isolate, "is_error"), _V8 Boolean::New(isolate, false));
				rsp->Set(_V8 String::NewFromUtf8(isolate, "err_msg"), _V8 String::NewFromUtf8(isolate, ""));
				if (value->IsNullOrUndefined()) {
					rsp->Set(_V8 String::NewFromUtf8(isolate, "result"), _V8 String::NewFromUtf8(isolate, rsinf->source()->c_str()));
				}
				else {
					rsp->Set(_V8 String::NewFromUtf8(isolate, "result"), value);
				}

			}
			rsinf->source("");
			arg[0] = { rsp };
			_V8 Local<_V8 Function>::New(isolate, settings->callback)->Call(tcontext->Global(), 1, arg);
			rsp.Clear(); arg->Clear();
		}
		return;
	};
	NTEMPLATE_INLINE NTEMPLATE_EXPORT void NTEMPLATE_PARSE_TEMPLATE_GLOBAL(const _V8 FunctionCallbackInfo<_V8 Value>& args) {
		{
			_V8 Isolate* isolate = _V8 Isolate::GetCurrent();
			_V8 HandleScope					scope(isolate);
			Settings* settings = new Settings();
			malloc(sizeof(settings));
			GetSettings(isolate, args, settings);
			if (settings->isError) {
				isolate->ThrowException(_V8 Exception::TypeError(
					_V8 String::NewFromUtf8(isolate, settings->errorMsg.c_str())));
				settings->Clear();
				free(settings);
				isolate = NULL;
				return;
			}

			if (settings->isAsync) {
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
				_NSC DoAsyncTask(isolate, settings);
				args.GetReturnValue().Set(_V8 String::NewFromUtf8(isolate, "START ASYNC"));
				//Will be Dispose While Thread Exited!!!
				isolate = NULL;
				return;
#endif //!_M_CEE

			}
			TemplateResult * rsinf = new TemplateResult();
			malloc(sizeof(rsinf));
			_NT_ TemplateInfo<_STD string, char, _STD regex> * tinf = new _NT_ TemplateInfo<_STD string, char, _STD regex>(settings->pagePath.c_str(), settings->dir);
			malloc(sizeof(tinf));
			_NT_ TemplateParser<_STD string, char, _STD regex> tParser;
			tParser.Start(tinf, rsinf);
			free(tinf);
			if (rsinf->is_error() == true) {
				if (!settings->callback.IsEmpty()) {
					ProcessCallback(isolate, rsinf,
						settings, _V8 Local<_V8 Value>::New(isolate, _V8 Null(isolate))
					);
					settings->Clear();
					free(settings);
					free(rsinf);
					args.GetReturnValue().Set(_V8 Undefined(isolate));
					return;
				}
				settings->Clear();
				free(settings);
				args.GetReturnValue().Set(_V8 String::NewFromUtf8(isolate, rsinf->error_msg()));
				free(rsinf);
				return;
			}
			rsinf->remove_new_line(settings->removeNewLine);
			_NTSP ParseScript(rsinf);
			if (rsinf->is_script() == false) {
				if (!settings->callback.IsEmpty()) {
					ProcessCallback(isolate, rsinf,
						settings, _V8 Local<_V8 Value>::New(isolate, _V8 Null(isolate))
					);
					settings->Clear();
					free(settings);
					free(rsinf);
					args.GetReturnValue().Set(_V8 Undefined(isolate));
					return;
				}
				settings->Clear();
				free(settings);
				args.GetReturnValue().Set(_V8 String::NewFromUtf8(isolate, rsinf->source()));
				free(rsinf);
				return;
			}
			if (settings->isObject) {
				if (!settings->callback.IsEmpty()) {
					ProcessCallback(isolate, rsinf, settings,
						_NCO RunTemplateScript(isolate, _STD string(rsinf->source()), settings->data)
					);
					settings->Clear();
					free(settings);
					free(rsinf);
					args.GetReturnValue().Set(_V8 Undefined(isolate));
					return;
				}
				args.GetReturnValue().Set(_NCO RunTemplateScript(isolate, _STD string(rsinf->source()), settings->data));
			}
			else {
				if (!settings->callback.IsEmpty()) {
					ProcessCallback(isolate, rsinf, settings,
						_NCO RunTemplateScript(isolate, _STD string(rsinf->source()), args[2])
					);
					settings->Clear();
					free(settings);
					free(rsinf);
					args.GetReturnValue().Set(_V8 Undefined(isolate));
					return;
				}
				args.GetReturnValue().Set(_NCO RunTemplateScript(isolate, _STD string(rsinf->source()), args[2]));
			}
			settings->Clear();
			free(settings);
			free(rsinf);
		}
		return;
	}
#define NTEMPLATE_PARSE_TEMPLATE_GLOBAL NTEMPLATE_PARSE_TEMPLATE_GLOBAL


	NTEMPLATE_INLINE NTEMPLATE_EXPORT void NTEMPLATE_PARSE_TEMPLATE_N(const _V8 FunctionCallbackInfo<_V8 Value>& args) {
		{
			_V8 Isolate* isolate = _V8 Isolate::GetCurrent();
			_V8 HandleScope					scope(isolate);
			if (!args[0]->IsString()) {
				isolate->ThrowException(_V8 Exception::TypeError(
					_V8 String::NewFromUtf8(isolate, "String Required!!! (APP DIRECTORY PATH) :(")));
				isolate = NULL;
				return;
			}
			if (!args[1]->IsString()) {
				isolate->ThrowException(_V8 Exception::TypeError(
					_V8 String::NewFromUtf8(isolate, "String Required!!! (PAGE_PATH) :(")));
				isolate = NULL;
				return;
			}
			//_V8 Isolate::Scope isolate_scope(isolate);///!TODO
			_V8 String::Utf8Value			utfDirStr(args[0]);
			_V8 String::Utf8Value			utfPagePathStr(args[1]);
			const char* pagePath = T_CHAR(utfPagePathStr);
			_STD string						dir(T_CHAR(utfDirStr));
			//Result
			_NT_ Result<_STD string> * rsinf = new _NT_ Result<_STD string>();
			malloc(sizeof(rsinf));
			_NT_ TemplateInfo<_STD string, char, _STD regex> * tinf = new _NT_ TemplateInfo<_STD string, char, _STD regex>(pagePath, dir);
			malloc(sizeof(tinf));
			_NT_ TemplateParser<_STD string, char, _STD regex>tParser;
			tParser.Start(tinf, rsinf);
			free(tinf);
			if (rsinf->is_error() == true) {
				args.GetReturnValue().Set(_V8 String::NewFromUtf8(isolate, rsinf->error_msg()));
				free(rsinf);
				return;
			}
			Parser::ParseScript(rsinf);
			if (rsinf->is_script() == false) {
				args.GetReturnValue().Set(_V8 String::NewFromUtf8(isolate, rsinf->source()));
				free(rsinf);
				return;
			}
			args.GetReturnValue().Set(_NCO RunTemplateScript(isolate, _STD string(rsinf->source()), args[2]));
			free(rsinf);
			return;
		}
	}
#define NTEMPLATE_PARSE_TEMPLATE NTEMPLATE_PARSE_TEMPLATE_N
}//!NTemplate