#pragma once
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
#ifndef NTEMPLATE_H_
#include "ntemplate.h"
#endif //!NTEMPLATE_H_
using namespace NTemplate;
namespace NTemplate {
	void ProcessCallback(v8::Isolate* isolate, TemplateResult * rsinf, Settings* settings, v8::Local<v8::Value> value);
	void ParseInternal(const v8::FunctionCallbackInfo<v8::Value>& args);
#define PARSE_INTERNAL NTemplate::ParseInternal
	void Parse(const v8::FunctionCallbackInfo<v8::Value>& args);
#define PARSE_TEMPLATE NTemplate::Parse
}//!NTemplate