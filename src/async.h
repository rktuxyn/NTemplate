#if defined(_MSC_VER)
#  pragma once
#endif
#ifndef NTEMPLATE_ASYNC_
#define NTEMPLATE_ASYNC_
#ifndef NTEMPLATE_ASYNC_DEP_
#include "NAsyncDep.h"
#endif //!NTEMPLATE_ASYNC_DEP_
#ifndef NTEMPLATE_TP_
#include "TemplatParser.h"
#endif //!NTEMPLATE_TP_
//SHOULD REMOVE!!!
#define _NT_BEGIN	namespace NTemplate {
#define _NT_END		}
# define NTEMPLATE_INLINE inline
#undef _M_CEE
//SHOULD REMOVE!!!
#define _NSC	NAsync::
_NT_BEGIN
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
namespace NAsync {
	struct ThreadContext {
		Settings*							settings;
		_NT Result<_STD string>*			result;
		uv_async_t*							asyncRequest;
		uv_work_t*							work_t;
	};
	struct ThreadSContext {
		Settings*							settings;
		_NT Result<_STD string>*			rsinf;
		uv_work_t*							work_t;
	};
	NTEMPLATE_INLINE NTEMPLATE_EXPORT void NTEMPLATE_PARSE_TEMPLATE_ASYNC_N(const _V8 FunctionCallbackInfo<_V8 Value>& args);
#define NTEMPLATE_PARSE_TEMPLATE_ASYNC NTEMPLATE_PARSE_TEMPLATE_ASYNC_N
	template<class _Context, class _Result>
	static void FinalizationTask(_Context *context, _Result* rsinf, bool canRun = false) {
		{
			_V8 Isolate *			isolate = _V8 Isolate::GetCurrent();
			_V8 HandleScope			handleScope(isolate);
			_V8 Isolate::Scope		isolate_scope(isolate);
			_V8 Local<_V8 Context>	tcontext = isolate->GetCurrentContext();
			_V8 Context::Scope		context_scope(tcontext);
			_V8 Handle<_V8 Value>	arg[1];
			_V8 Handle<_V8 Object>	rsp = _V8 Handle<_V8 Object>::New(isolate, _V8 Object::New(isolate));

			if (rsinf->is_error() == true) {
				rsinf->source("");
				rsp->Set(_V8 String::NewFromUtf8(isolate, "is_error"), _V8 Boolean::New(isolate, rsinf->is_error()));
				rsp->Set(_V8 String::NewFromUtf8(isolate, "err_msg"), _V8 String::NewFromUtf8(isolate, rsinf->error_msg()->c_str()));
			}
			else {
				if (rsinf->is_script() == true) {
					rsp->Set(_V8 String::NewFromUtf8(isolate, "result"), _NCO RunTemplateScript(isolate, _STD string(rsinf->source()->c_str()), context->settings->data, canRun));
					rsinf->source("");
				}
				else {
					rsp->Set(_V8 String::NewFromUtf8(isolate, "result"), _V8 String::NewFromUtf8(isolate, rsinf->source()->c_str()));
					rsinf->source("");
				}
				rsp->Set(_V8 String::NewFromUtf8(isolate, "is_error"), _V8 Boolean::New(isolate, false));
				rsp->Set(_V8 String::NewFromUtf8(isolate, "err_msg"), _V8 String::NewFromUtf8(isolate, ""));
			}


			arg[0] = { rsp };
			_V8 Local<_V8 Function>::New(isolate, context->settings->callback)->Call(tcontext->Global(), 1, arg);
			context->settings->Clear();
			rsp.Clear(); arg->Clear();
			delete context;
			_V8 Unlocker l(isolate);
			isolate->Exit();
		}
		return;
	}
	template<class _Context, class _Result>
	static void ParseTemplate(_Context *context, _Result* rsinf) {
		{
			_NT_ TemplateInfo<_STD string, char, _STD regex> * tinf = new TemplateInfo<_STD string, char, _STD regex>(context->settings->pagePath.c_str(), context->settings->dir);
			malloc(sizeof(tinf));
			rsinf->is_strict(context->settings->isStrict);
			_NT_ TemplateParser<_STD string, char, _STD regex> tParser;
			tParser.Start(tinf, rsinf);
			free(tinf);
			if (rsinf->is_error() == true) {
				return;
			}
			rsinf->remove_new_line(context->settings->removeNewLine);
			_NTSP ParseScript(rsinf);
		}
		return;
	}
	template<class _async>
	static void StartAsync(_async *handle) {
		{
			_NSC ThreadContext *context = static_cast<_NSC ThreadContext *>(handle->data);
			_NT Result<_STD string>* rsinf = new _NT Result<_STD string>();
			malloc(sizeof(rsinf));
			_NSC ParseTemplate(context, rsinf);
			_NSC FinalizationTask(context, rsinf);
			free(rsinf);
			uv_close((uv_handle_t *)handle, NULL);
			delete context;
		}
		return;
	}
	template<class _Context>
	static void ParseTemplate(_Context *context) {
		{
			_NT_ TemplateInfo<_STD string, char, _STD regex> * tinf = new TemplateInfo<_STD string, char, _STD regex>(context->settings->pagePath.c_str(), context->settings->dir);
			malloc(sizeof(tinf));
			context->rsinf->is_strict(context->settings->isStrict);
			_NT_ TemplateParser<_STD string, char, _STD regex> tParser;
			tParser.Start(tinf, context->rsinf);
			free(tinf);
			if (context->rsinf->is_error() == true) {
				return;
			}
			context->rsinf->remove_new_line(context->settings->removeNewLine);
			_NTSP ParseScript(context->rsinf);
		}
		return;
	};
	template<class _Context>
	static void FinalizationTask(_Context *context, bool canRun = false) {
		{
			_V8 Isolate *			isolate = _V8 Isolate::GetCurrent();
			_V8 HandleScope			handleScope(isolate);
			_V8 Isolate::Scope		isolate_scope(isolate);
			_V8 Local<_V8 Context>	tcontext = isolate->GetCurrentContext();
			_V8 Context::Scope		context_scope(tcontext);
			_V8 Handle<_V8 Value>	arg[1];
			_V8 Handle<_V8 Object>	rsp = _V8 Handle<_V8 Object>::New(isolate, _V8 Object::New(isolate));

			if (context->rsinf->is_error() == true) {
				context->rsinf->source("");
				rsp->Set(_V8 String::NewFromUtf8(isolate, "is_error"), _V8 Boolean::New(isolate, context->rsinf->is_error()));
				rsp->Set(_V8 String::NewFromUtf8(isolate, "err_msg"), _V8 String::NewFromUtf8(isolate, context->rsinf->error_msg()->c_str()));
			}
			else {
				if (context->rsinf->is_script() == true) {
					rsp->Set(_V8 String::NewFromUtf8(isolate, "result"), _NCO RunTemplateScript(isolate, _STD string(context->rsinf->source()->c_str()), context->settings->data, canRun));
					context->rsinf->source("");
				}
				else {
					rsp->Set(_V8 String::NewFromUtf8(isolate, "result"), _V8 String::NewFromUtf8(isolate, context->rsinf->source()->c_str()));
					context->rsinf->source("");
				}
				rsp->Set(_V8 String::NewFromUtf8(isolate, "is_error"), _V8 Boolean::New(isolate, false));
				rsp->Set(_V8 String::NewFromUtf8(isolate, "err_msg"), _V8 String::NewFromUtf8(isolate, ""));
			}


			arg[0] = { rsp };
			_V8 Local<_V8 Function>::New(isolate, context->settings->callback)->Call(tcontext->Global(), 1, arg);
			context->settings->Clear();
			rsp.Clear(); arg->Clear();
			//_V8 Unlocker l(isolate);
			//isolate->Exit();
		}
		return;
	}
	static void Runner(uv_work_t *req){
		printf("Async work running!\n");
		_NSC ParseTemplate(static_cast<_NSC ThreadSContext *>(req->data));
		printf("Async work Completed!\n");
	};

	static void RunDone(uv_work_t *req, int status){
		
		_NSC FinalizationTask(static_cast<_NSC ThreadSContext *>(req->data));
		delete req;
		printf("Async work done!\n");
		
	};
	template<class _Settings>
	int __DoAsyncTask(_V8 Isolate* isolate, _Settings* settings) {
		{
#if _HAS_EXCEPTIONS
			_Try
#endif	
				_NSC ThreadSContext * context = new _NSC ThreadSContext();
				malloc(sizeof(context));
				context->settings = settings;
				context->rsinf = (_NT Result<_STD string>*)malloc(sizeof(_NT Result<_STD string>));
				context->work_t = (uv_work_t*)malloc(sizeof(uv_work_t));
				context->work_t->data = context;
				printf("Queue work\n");
				//_V8 Locker l(isolate);
				//isolate->Enter();
				uv_queue_work(uv_default_loop(), context->work_t, &Runner, &RunDone);
				isolate = NULL; context = NULL;
#if _HAS_EXCEPTIONS
			_Catch
				isolate->ThrowException(_V8 Exception::TypeError(
					_V8 String::NewFromUtf8(isolate, e.what())));
			_TrE
#endif
		}
		return 0;
	};

	template<class _Settings>
	int DoAsyncTask(_V8 Isolate* isolate, _Settings* settings) {
		{
#if _HAS_EXCEPTIONS
			_Try
#endif	
				_NSC ThreadContext * context = new _NSC ThreadContext();
				malloc(sizeof(context));
				context->settings = settings;
				//context->asyncRequest = (uv_async_t*)malloc(sizeof(uv_async_t));
				//context->asyncRequest = new uv_async_t();//(uv_async_t*)malloc(sizeof(uv_async_t));
				//malloc(sizeof(context->asyncRequest));
				//context->asyncRequest->data = context;
				uv_async_t* async = (uv_async_t*)malloc(sizeof(uv_async_t));
				async->data = context;
				//context->trequest.data = context;
				//uv_async_t*                   handle;
				//handle = (uv_async_t*)malloc(sizeof(uv_async_t));
				/***http://nikhilm.github.io/uvbook/threads.html***/
				uv_async_init(uv_default_loop(), async, _NSC StartAsync);
				//uv_async_init(uv_default_loop(), context->asyncRequest, _NSC StartAsync);
				//uv_queue_work(loop, &context->trequest, TWork, TComplete);
				//uv_run(loop, UV_RUN_DEFAULT);
				_V8 Locker l(isolate);
				isolate->Enter();
				//uv_async_send(context->asyncRequest);
				uv_async_send(async);
				isolate = NULL; context = NULL;
#if _HAS_EXCEPTIONS
			_Catch
				isolate->ThrowException(_V8 Exception::TypeError(
					_V8 String::NewFromUtf8(isolate, e.what())));
			_TrE
#endif
		}
		return 0;
	}
	
	NTEMPLATE_INLINE NTEMPLATE_EXPORT void NTEMPLATE_PARSE_TEMPLATE_ASYNC_N(const _V8 FunctionCallbackInfo<_V8 Value>& args) {
		{
			_V8 Isolate* isolate = _V8 Isolate::GetCurrent();
			_V8 HandleScope scope(isolate);
			if (!args[0]->IsObject()) {
				isolate->ThrowException(_V8 Exception::TypeError(
					_V8 String::NewFromUtf8(isolate, "Setting Object Required!!! :(")));
				return;
			}
			_NT_ Settings* settings = new Settings();
			malloc(sizeof(_NT_ Settings));
			_NT GetSettings(isolate, args, settings, true);
			if (settings->isError) {
				isolate->ThrowException(_V8 Exception::TypeError(
					_V8 String::NewFromUtf8(isolate, settings->errorMsg.c_str())));
				settings->Clear();
				free(settings);
				return;
			}

			_NSC DoAsyncTask(isolate, settings);
			args.GetReturnValue().Set(_V8 Undefined(isolate));
			isolate = NULL;
		}
	}
}
#endif //!_M_CEE
_NT_END//!NTemplate
#endif//!NTEMPLATE_ASYNC_
