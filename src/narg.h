/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#pragma once
#ifndef NTEMPLATE_ARG_
#define NTEMPLATE_ARG_
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
#ifndef _Try
#define _Try try {
#endif // !_Try
#ifndef _Catch
#define _Catch } catch (std::exception& e) {
#endif // !_Catch
#ifndef _TrE
#define _TrE }
#endif // !_TrE
#ifndef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 1 // Predefine as 0 to disable exceptions
#endif//!_HAS_EXCEPTIONS
namespace NTemplate {
	struct Settings {
		v8::Local<v8::Value> data;
		std::string pagePath;
		std::string dir;
		bool isAsync;
		bool isObject;
		bool removeNewLine;
		bool isStrict;
		v8::Persistent<v8::Function> callback;
		bool isError;
		std::string errorMsg;
		void Clear();

	};
	void GetSettings(v8::Isolate* isolate, const v8::FunctionCallbackInfo<v8::Value>& args, Settings* settingsObj, bool isAsyncReq = false);
};
#endif//!NTEMPLATE_AG_