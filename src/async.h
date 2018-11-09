/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#pragma once
#ifndef NTEMPLATE_ASYNC_
#define NTEMPLATE_ASYNC_
#ifndef NTEMPLATE_ASYNC_DEP_
#include "asyncdep.h"
#endif //!NTEMPLATE_ASYNC_DEP_
#ifndef TR_H_
#include "tr.h"
#endif //!TR_H_
#ifndef JSP_H_
#include "jsp.h"
#endif //!JSP_H_
#ifndef TP_H_
#include "tp.h"
#endif //!TP_H_
#undef _M_CEE
//SHOULD REMOVE!!!

namespace NTemplate {
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
	namespace Async {
		struct ThreadContext {
			Settings*							settings;
			uv_async_t*							asyncRequest;
			uv_work_t*							work_t;
		};
		inline void ParseTemplate(ThreadContext *context, TemplateResult* rsinf);
		inline void StartAsync(uv_async_t *handle);
		inline void FinalizationTask(ThreadContext *context, TemplateResult* rsinf, bool canRun = false);
		int DoAsyncTask(v8::Isolate* isolate, Settings* settings);
		void Run(const v8::FunctionCallbackInfo<v8::Value>& args);
#define RUN_ASYNC NTemplate::Async::Run
	}
#endif //!_M_CEE
}//!NTemplate
#endif//!NTEMPLATE_ASYNC_
