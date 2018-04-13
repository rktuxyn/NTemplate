////https://github.com/nodejs/node-addon-examples
#ifndef SRC_NODE_H_
#include <node.h>
#endif //!SRC_NODE_H_
#ifndef INCLUDE_V8_H_
#include <v8.h>
#endif // !INCLUDE_V8_H_
/*#include <iostream>
#include <string>
#include "testcls.h"*/
#include "NTemplate/NTemplateImp.h"
#include "NTemplate/NAsync.h"
//#include "NTemplate/NAsyncStd.h"
//using namespace v8;
//using namespace _T;
// npm install node-gyp -g
// npm install --global --production windows-build-tools
//npm install --global node-gyp
// node-gyp configure
//node-gyp build
//node-gyp rebuild
void RegisterModule(v8::Handle<v8::Object> target) {
	NODE_SET_METHOD(target, "parseScript", NTEMPLATE_PARSE_SCRIPT);
	NODE_SET_METHOD(target, "runScript", NTEMPLATE_RUN_SCRIPT);
	NODE_SET_METHOD(target, "processTemplate", NTEMPLATE_PARSE_TEMPLATE);
	NODE_SET_METHOD(target, "global", NTEMPLATE_PARSE_TEMPLATE_GLOBAL);
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
	#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
	NODE_SET_METHOD(target, "async", NTEMPLATE_PARSE_TEMPLATE_ASYNC);
#endif //!_M_CEE
	/*v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::Handle<v8::Object> NTobj = v8::Object::New(isolate);
	NODE_SET_METHOD(NTobj, "processTemplate", NTEMPLATE_PARSE_TEMPLATE);
	NODE_SET_METHOD(NTobj, "parseScript", NTEMPLATE_PARSE_SCRIPT);
	NODE_SET_METHOD(NTobj, "runScript", NTEMPLATE_RUN_SCRIPT);
	NODE_SET_METHOD(NTobj, "processTemplate", NTEMPLATE_PARSE_TEMPLATE);
	v8::Local<v8::String> poperty = v8::String::NewFromUtf8(isolate, "NTemplate");
	target->Set(poperty, NTobj);*/
}
NODE_MODULE(addon, RegisterModule)