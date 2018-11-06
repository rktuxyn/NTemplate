#include "async.h"
using namespace NTemplate;
using namespace NTemplate::Async;
void NTemplate::Async::FinalizationTask(ThreadContext * context, Result * rsinf, bool canRun) {
	v8::Isolate *			isolate = v8::Isolate::GetCurrent();
	v8::HandleScope			handleScope(isolate);
	v8::Isolate::Scope		isolate_scope(isolate);
	v8::Local<v8::Context>	tcontext = isolate->GetCurrentContext();
	v8::Context::Scope		context_scope(tcontext);
	v8::Handle<v8::Value>	arg[1];
	v8::Handle<v8::Object>	rsp = v8::Handle<v8::Object>::New(isolate, v8::Object::New(isolate));

	if (rsinf->is_error() == true) {
		rsinf->source("");
		rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, rsinf->is_error()));
		rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, rsinf->error_msg()));
	}
	else {
		if (rsinf->is_script() == true) {
			rsp->Set(v8::String::NewFromUtf8(isolate, "result"), NTemplate::Compiler::RunTemplateScript(isolate, rsinf->sourcex(), context->settings->data, canRun));
			rsinf->source("");
		}
		else {
			rsp->Set(v8::String::NewFromUtf8(isolate, "result"), v8::String::NewFromUtf8(isolate, rsinf->sourcex()->c_str()));
			rsinf->source("");
		}
		rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, false));
		rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, ""));
	}


	arg[0] = { rsp };
	v8::Local<v8::Function>::New(isolate, context->settings->callback)->Call(tcontext->Global(), 1, arg);
	context->settings->Clear();
	rsp.Clear(); arg->Clear();
	delete context;
	v8::Unlocker l(isolate);
	isolate->Exit();
	return;
};
inline void
NTemplate::Async::ParseTemplate(ThreadContext *context, Result* rsinf) {
	TemplateParser* tParser = new NTemplate::TemplateParser(context->settings->pagePath->c_str(), context->settings->dir);
	malloc(sizeof(tParser));
	rsinf->is_strict(context->settings->isStrict);
	rsinf->remove_new_line(context->settings->removeNewLine);
	TemplateResult * tinf = new TemplateResult();
	tParser->Start(tinf);
	malloc(sizeof(tinf));
	if (rsinf->is_error() == true) {
		return;
	}
	return;
};
inline void 
NTemplate::Async::ParseTemplate(ThreadSContext *context) {
	TemplateParser* tParser = new NTemplate::TemplateParser(context->settings->pagePath->c_str(), context->settings->dir);
	malloc(sizeof(tParser));
	context->rsinf->is_strict(context->settings->isStrict);
	context->rsinf->remove_new_line(context->settings->removeNewLine);
	TemplateResult * tinf = new TemplateResult();
	tParser->Start(tinf);
	malloc(sizeof(tinf));
	if (context->rsinf->is_error() == true) {
		return;
	}
	return;
};
inline void 
NTemplate::Async::StartAsync(uv_async_t *handle) {
	ThreadContext *context = static_cast<ThreadContext *>(handle->data);
	Result* rsinf = new Result();
	malloc(sizeof(rsinf));
	NTemplate::Async::ParseTemplate(context, rsinf);
	NTemplate::Async::FinalizationTask(context, rsinf);
	free(rsinf);
	uv_close((uv_handle_t *)handle, NULL);
	delete context;
	return;
}
void NTemplate::Async::FinalizationTask(ThreadSContext * context, bool canRun) {
	v8::Isolate *			isolate = v8::Isolate::GetCurrent();
	v8::HandleScope			handleScope(isolate);
	v8::Isolate::Scope		isolate_scope(isolate);
	v8::Local<v8::Context>	tcontext = isolate->GetCurrentContext();
	v8::Context::Scope		context_scope(tcontext);
	v8::Handle<v8::Value>	arg[1];
	v8::Handle<v8::Object>	rsp = v8::Handle<v8::Object>::New(isolate, v8::Object::New(isolate));

	if (context->rsinf->is_error() == true) {
		context->rsinf->source("");
		rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, context->rsinf->is_error()));
		rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, context->rsinf->error_msg()));
	}
	else {
		if (context->rsinf->is_script() == true) {
			rsp->Set(v8::String::NewFromUtf8(isolate, "result"), NTemplate::Compiler::RunTemplateScript(isolate, context->rsinf->sourcex(), context->settings->data, canRun));
			context->rsinf->source("");
		}
		else {
			rsp->Set(v8::String::NewFromUtf8(isolate, "result"), v8::String::NewFromUtf8(isolate, context->rsinf->source()));
			context->rsinf->source("");
		}
		rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, false));
		rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, ""));
	}


	arg[0] = { rsp };
	v8::Local<v8::Function>::New(isolate, context->settings->callback)->Call(tcontext->Global(), 1, arg);
	context->settings->Clear();
	rsp.Clear(); arg->Clear();
	return;
};
inline void 
NTemplate::Async::Runner(uv_work_t *req) {
	printf("Async work running!\n");
	NTemplate::Async::ParseTemplate(static_cast<ThreadSContext *>(req->data));
	printf("Async work Completed!\n");
};
inline void 
NTemplate::Async::RunDone(uv_work_t *req, int status) {
	NTemplate::Async::FinalizationTask(static_cast<ThreadSContext *>(req->data));
	delete req;
	printf("Async work done!\n");
};
inline int 
NTemplate::Async::__DoAsyncTask(v8::Isolate* isolate, Settings* settings) {
	{
#if _HAS_EXCEPTIONS
		try {
#endif	
			ThreadSContext * context = new ThreadSContext();
			malloc(sizeof(context));
			context->settings = settings;
			context->rsinf = (Result*)malloc(sizeof(Result));
			context->work_t = (uv_work_t*)malloc(sizeof(uv_work_t));
			context->work_t->data = context;
			printf("Queue work\n");
			//v8:: Locker l(isolate);
			//isolate->Enter();
			uv_queue_work(uv_default_loop(), context->work_t, &NTemplate::Async::Runner, &NTemplate::Async::RunDone);
			isolate = NULL; context = NULL;
#if _HAS_EXCEPTIONS
		} catch (std::exception& e) {
			isolate->ThrowException(v8::Exception::TypeError(
				v8::String::NewFromUtf8(isolate, e.what())));
		}
#endif
	}
	return 0;
};
int NTemplate::Async::DoAsyncTask(v8::Isolate * isolate, Settings * settings) {
#if _HAS_EXCEPTIONS
		try {
#endif	
			ThreadContext * context = new ThreadContext();
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
			uv_async_init(uv_default_loop(), async, NTemplate::Async::StartAsync);
			//uv_async_init(uv_default_loop(), context->asyncRequest, _NSC StartAsync);
			//uv_queue_work(loop, &context->trequest, TWork, TComplete);
			//uv_run(loop, UV_RUN_DEFAULT);
			v8::Locker l(isolate);
			isolate->Enter();
			//uv_async_send(context->asyncRequest);
			uv_async_send(async);
			isolate = NULL; context = NULL;
#if _HAS_EXCEPTIONS
		} catch (std::exception& e) {
			isolate->ThrowException(v8::Exception::TypeError(
				v8::String::NewFromUtf8(isolate, e.what())));
		}
#endif
	
	return 0;
};
inline void 
NTemplate::Async::Runx(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	if (!args[0]->IsObject()) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, "Setting Object Required!!! :(")));
		return;
	}
	Settings* settings = new Settings();
	malloc(sizeof(settings));
	NTemplate::GetSettings(isolate, args, settings, true);
	if (settings->isError) {
		isolate->ThrowException(v8::Exception::TypeError(
			v8::String::NewFromUtf8(isolate, settings->errorMsg->c_str())));
		settings->Clear();
		free(settings);
		return;
	}

	NTemplate::Async::DoAsyncTask(isolate, settings);
	args.GetReturnValue().Set(v8::Undefined(isolate));
	isolate = NULL;
};