/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "async.h"
using namespace NTemplate;
using namespace NTemplate::Async;
inline void 
NTemplate::Async::ParseTemplate(ThreadContext *context, TemplateResult* rsinf) {
#if _TEST_RUNTIME
	std::cout << "NTemplate::Async::ParseTemplate..." << "\r\n";
#endif
	rsinf->is_strict = context->settings->isStrict;
	rsinf->remove_new_line = context->settings->removeNewLine;
	TemplateParser* tParser = new NTemplate::TemplateParser(rsinf, context->settings->pagePath.c_str(), context->settings->dir);
	malloc(sizeof(tParser));
	tParser->Start();
	if (rsinf->is_error == true) {
		return;
	}
#if _TEST_RUNTIME
	NTemplate::WriteFile("function ___NTEMPLATE__SCRIPT__RUNNER (data){ try{\r\n" + rsinf->t_source + "\r\n}catch(e){\r\nreturn e.message;\r\n}\r\n};");
#endif
	return;
};
void NTemplate::Async::FinalizationTask(ThreadContext * context, TemplateResult* rsinf, bool canRun) {
#if _TEST_RUNTIME
	std::cout << "NTemplate::Async::FinalizationTask..." << "\r\n";
#endif
	v8::Isolate *			isolate = v8::Isolate::GetCurrent();
	v8::HandleScope			handleScope(isolate);
	v8::Isolate::Scope		isolate_scope(isolate);
	v8::Local<v8::Context>	tcontext = isolate->GetCurrentContext();
	v8::Context::Scope		context_scope(tcontext);
	v8::Handle<v8::Value>	arg[1];
	v8::Handle<v8::Object>	rsp = v8::Handle<v8::Object>::New(isolate, v8::Object::New(isolate));

	if (rsinf->is_error == true) {
		rsinf->t_source.clear();
		rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, rsinf->is_error));
		rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, rsinf->err_msg.c_str()));
	}
	else {
		if (rsinf->is_script == true) {
			rsp->Set(v8::String::NewFromUtf8(isolate, "result"), NTemplate::Compiler::RunTemplateScript(isolate, rsinf->t_source, context->settings->data, canRun));
			rsinf->t_source.clear();
		}
		else {
			rsp->Set(v8::String::NewFromUtf8(isolate, "result"), v8::String::NewFromUtf8(isolate, rsinf->t_source.c_str()));
			rsinf->t_source.clear();
		}
		rsp->Set(v8::String::NewFromUtf8(isolate, "is_error"), v8::Boolean::New(isolate, false));
		rsp->Set(v8::String::NewFromUtf8(isolate, "err_msg"), v8::String::NewFromUtf8(isolate, ""));
	}


	arg[0] = { rsp };
	v8::Local<v8::Function>::New(isolate, context->settings->callback)->Call(tcontext->Global(), 1, arg);
	context->settings->Clear();
	rsp.Clear(); arg->Clear();
	v8::Unlocker l(isolate);
	isolate->Exit();
	return;
};
inline void 
NTemplate::Async::StartAsync(uv_async_t *handle) {
#if _TEST_RUNTIME
	std::cout << "NTemplate::Async::StartAsync..." << "\r\n";
#endif
	TemplateResult* rsinf = new TemplateResult();
	malloc(sizeof(rsinf));
	ThreadContext *context = static_cast<ThreadContext *>(handle->data);
	NTemplate::Async::ParseTemplate(context, rsinf);
	NTemplate::Async::FinalizationTask(context, rsinf);
	uv_close((uv_handle_t*)handle, [](uv_handle_t* handles) {
		free(handles);
	});
	delete rsinf;
	return;
};
int NTemplate::Async::DoAsyncTask(v8::Isolate * isolate, Settings * settings) {
#if _HAS_EXCEPTIONS
		try {
#endif	
#if _TEST_RUNTIME
			std::cout << "NTemplate::Async::DoAsyncTask..." << "\r\n";
#endif
			ThreadContext* context = new ThreadContext();
			malloc(sizeof(context));
			context->settings = settings;
			uv_async_t* async = (uv_async_t*)malloc(sizeof(uv_async_t));
			async->data = context;
			uv_async_init(uv_default_loop(), async, NTemplate::Async::StartAsync);
			v8::Locker l(isolate);
			isolate->Enter();
			uv_async_send(async);
#if _HAS_EXCEPTIONS
		} catch (std::exception& e) {
			isolate->ThrowException(v8::Exception::TypeError(
				v8::String::NewFromUtf8(isolate, e.what())));
		}
#endif
	return 0;
};
void NTemplate::Async::Run(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
			v8::String::NewFromUtf8(isolate, settings->errorMsg.c_str())));
		settings->Clear();
		free(settings);
		return;
	}
	NTemplate::Async::DoAsyncTask(isolate, settings);
	args.GetReturnValue().Set(v8::Undefined(isolate));
	isolate = NULL;
};