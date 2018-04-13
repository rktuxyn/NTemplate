#pragma once
#ifndef NTEMPLATE_ASYNC_
#define NTEMPLATE_ASYNC_
#ifndef NTEMPLATE_ASYNC_DEP_
#include "NAsyncDep.h"
#endif //!NTEMPLATE_ASYNC_DEP_
#ifndef NTEMPLATE_TP_
#include "TemplatParser.h"
#endif //!NTEMPLATE_TP_
using namespace NTemplate;
namespace NTemplate {
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
	#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
	struct ThreadContext {
		Settings* settings;
		Result* result;
		uv_async_t* asyncRequest;
	};
	NTEMPLATE_INLINE NTEMPLATE_EXPORT void NTEMPLATE_PARSE_TEMPLATE_ASYNC_N(const v8::FunctionCallbackInfo<v8::Value>& args);
#define NTEMPLATE_PARSE_TEMPLATE_ASYNC NTEMPLATE_PARSE_TEMPLATE_ASYNC_N
	template<class _Context, class _Result>
	static void FinalizationTask(_Context *context, _Result* rsinf, bool canRun = false) {
		{
			v8::Isolate * isolate = v8::Isolate::GetCurrent();
			v8::HandleScope handleScope(isolate);
			v8::Isolate::Scope isolate_scope(isolate);
			v8::Local<v8::Context> tcontext = isolate->GetCurrentContext();
			v8::Context::Scope context_scope(tcontext);
			v8::Handle<v8::Value> arg[1];
			v8::Handle<v8::Object> rsp = v8::Handle<v8::Object>::New(isolate, v8::Object::New(isolate));
			
			if (rsinf->is_error() == true) {
				rsinf->source("");
				rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, rsinf->is_error()));
				rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, rsinf->error_msg()->c_str()));
			}
			else {
				if (rsinf->is_script() == true) {
					rsp->Set(v8::String::NewFromUtf8(isolate, "result"), NTemplate::RunTemplateScript(isolate, std::string(rsinf->source()->c_str()), context->settings->data, canRun));
					rsinf->source("");
				}
				else {
					rsp->Set(v8::String::NewFromUtf8(isolate, "result"), v8::String::NewFromUtf8(isolate, rsinf->source()->c_str()));
					rsinf->source("");

				}
				rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, false));
				rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, ""));
			}
			arg[0] = { rsp };
			v8::Local<v8::Function>::New(isolate, context->settings->callback)->Call(tcontext->Global(), 1, arg);
			context->settings->callback.Reset(); delete context->settings;
			rsp.Clear(); arg->Clear();
			delete context;
			v8::Unlocker l(isolate);
			isolate->Exit();
		}
		return;
	}
	template<class _Context, class _Result>
	static void ParseTemplate(_Context *context, _Result* rsinf) {
		{
			TemplateParser::TemplateInfo* tinf = new TemplateParser::TemplateInfo(context->settings->pagePath.c_str(), context->settings->dir);
			malloc(sizeof(tinf));
			TemplateParser::Start(tinf, rsinf);
			free(tinf);
			if (rsinf->is_error() == true) {
				return;
			}
			Parser::ParseScript(rsinf);
		}
		return;
	}
	template<class _async>
	static void StartAsync(_async *handle) {
		{
			ThreadContext *context = static_cast<ThreadContext *>(handle->data);
			Result* rsinf = new Result();
			malloc(sizeof(rsinf));
			ParseTemplate(context, rsinf);
			FinalizationTask(context, rsinf);
			free(rsinf);
			uv_close((uv_handle_t*)handle, [](uv_handle_t* _handle) {
				free(_handle);
			});
			delete context;
		}
		return;
	}
	template<class _Settings>
	int DoAsyncTask(v8::Isolate* isolate, _Settings* settings) {
		{
#if _HAS_EXCEPTIONS
			try {
#endif	
				uv_loop_t *loop = uv_default_loop();
				ThreadContext * context = new ThreadContext();
				context->settings = settings;
				context->asyncRequest = new uv_async_t();//(uv_async_t*)malloc(sizeof(uv_async_t));
				malloc(sizeof(uv_async_t));
				context->asyncRequest->data = context;
				//context->trequest.data = context;
				//uv_async_t*                   handle;
				//handle = (uv_async_t*)malloc(sizeof(uv_async_t));
				/***http://nikhilm.github.io/uvbook/threads.html***/
				uv_async_init(loop, context->asyncRequest, StartAsync);
				//uv_queue_work(loop, &context->trequest, TWork, TComplete);
				//uv_run(loop, UV_RUN_DEFAULT);
				v8::Locker l(isolate);
				isolate->Enter();
				uv_async_send(context->asyncRequest);
				isolate = NULL; loop = NULL; context = NULL;
#if _HAS_EXCEPTIONS
			}
			catch (std::exception& e) {
				isolate->ThrowException(v8::Exception::TypeError(
					v8::String::NewFromUtf8(isolate, e.what())));
			}
#endif
		}
		return 0;
	}
	NTEMPLATE_INLINE NTEMPLATE_EXPORT void NTEMPLATE_PARSE_TEMPLATE_ASYNC_N(const v8::FunctionCallbackInfo<v8::Value>& args) {
		{
			v8::Isolate* isolate = v8::Isolate::GetCurrent();
			v8::HandleScope scope(isolate);
			if (!args[0]->IsObject()) {
				isolate->ThrowException(v8::Exception::TypeError(
					v8::String::NewFromUtf8(isolate, "Setting Object Required!!! :(")));
				return;
			}
			Settings* settings = NTemplate::GetSettings(isolate, args, true);
			if (settings->isError) {
				isolate->ThrowException(v8::Exception::TypeError(
					v8::String::NewFromUtf8(isolate, settings->errorMsg.c_str())));
				delete settings;
				return;
			}

			NTemplate::DoAsyncTask(isolate, settings);
			args.GetReturnValue().Set(v8::Undefined(isolate));
			isolate = NULL;
		}
	}
#endif //!_M_CEE
}//!NTemplate
#endif//!NTEMPLATE_ASYNC_