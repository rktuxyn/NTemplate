#pragma once
#ifndef NTEMPLATE_ASYNC_
#define NTEMPLATE_ASYNC_
#ifndef NTEMPLATE_ASYNC_DEP_
#include "asyncdep.h"
#endif //!NTEMPLATE_ASYNC_DEP_
#ifndef NTEMPLATE_TP_
#include "ntemplate.h"
#endif //!NTEMPLATE_TP_
//SHOULD REMOVE!!!
#define _NT_BEGIN	namespace NTemplate {
#define _NT_END		}
# define NTEMPLATE_INLINE inline
#undef _M_CEE
//SHOULD REMOVE!!!
#ifndef N_RESULT
#include "result.h"
#endif // !N_RESULT

namespace NTemplate {
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
	namespace Async {
		struct ThreadContext {
			Settings*							settings;
			Result*								result;
			uv_async_t*							asyncRequest;
			uv_work_t*							work_t;
		};
		struct ThreadSContext {
			Settings*							settings;
			Result*								rsinf;
			uv_work_t*							work_t;
		};
		inline void FinalizationTask(ThreadContext *context, Result* rsinf, bool canRun = false);
		inline void ParseTemplate(ThreadContext *context, Result* rsinf);
		inline void ParseTemplate(ThreadSContext *context);
		inline void StartAsync(uv_async_t *handle);
		inline void FinalizationTask(ThreadSContext *context, bool canRun = false);
		inline void Runner(uv_work_t *req);
		inline void RunDone(uv_work_t *req, int status);
		inline int __DoAsyncTask(v8::Isolate* isolate, Settings* settings);
		int DoAsyncTask(v8::Isolate* isolate, Settings* settings);
		inline void Runx(const v8::FunctionCallbackInfo<v8::Value>& args);
	}
#endif //!_M_CEE
}//!NTemplate
#endif//!NTEMPLATE_ASYNC_
