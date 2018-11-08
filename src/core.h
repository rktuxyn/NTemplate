/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
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
#ifndef TR_H_
#include "tr.h"
#endif //!TR_H_
#ifndef JSP_H_
#include "jsp.h"
#endif //!JSP_H_
#ifndef TP_H_
#include "tp.h"
#endif //!TP_H_
using namespace NTemplate;
namespace NTemplate {
	void ProcessCallback(v8::Isolate* isolate, TemplateResult * rsinf, Settings* settings, v8::Local<v8::Value> value);
	void ParseInternal(const v8::FunctionCallbackInfo<v8::Value>& args);
#define PARSE_INTERNAL NTemplate::ParseInternal
	void Parse(const v8::FunctionCallbackInfo<v8::Value>& args);
#define PARSE_TEMPLATE NTemplate::Parse
}//!NTemplate