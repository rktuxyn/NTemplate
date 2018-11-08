/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#pragma once
#ifndef NTEMPLATE_COMP_
#define NTEMPLATE_COMP_
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
#define _NCO	Compiler:: 
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
	namespace Compiler {
		inline void RunScript(v8::Isolate* isolate, v8::Local<v8::String> source);
		void RunScript(const v8::FunctionCallbackInfo<v8::Value>& args);
		v8::Local<v8::Value> ParseScript(v8::Isolate* isolate,
			v8::Local<v8::String> source, v8::Local<v8::Value> innerFunc,
			v8::Local<v8::Value> innerArg, bool isAsync = false);
		/**
		* Parse & Run Javascript from string object
		* Params @ [Script Source@String, Inner Function@String, Callback@Function]
		*/
		void ParseRawScript(const v8::FunctionCallbackInfo<v8::Value>& args);
		v8::Local<v8::Value> RunTemplateScript(v8::Isolate* isolate, std::string& source, v8::Local<v8::Value> innerArg, bool isAsync = false);
#define RUN_SCRIPT NTemplate::Compiler::RunScript
	}
}
#endif //!NTEMPLATE_COMP_
