/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
////https://github.com/nodejs/node-addon-examples
#ifndef SRC_NODE_H_
#include <node.h>
#endif //!SRC_NODE_H_
#ifndef INCLUDE_V8_H_
#include <v8.h>
#endif // !INCLUDE_V8_H_
//#include "src/compiler.h"
#include "src/core.h"
//npm install node-gyp -g
//npm install --global --production windows-build-tools
//npm install --global node-gyp
//npm i -g node-gyp
//npm install -g node-gyp
//node-gyp configure
//node-gyp build
//node-gyp rebuild
//echo %python% should be C:\Python27\python.exe
/*void test(const v8::FunctionCallbackInfo<v8::Value>& args) {
	PARSE_TEMPLATE(args);
	// assert(status == 0);
	return;
};*/
/*
$ npm install -g npm-check-updates
$ npm-check-updates -u
$ npm install
*/
//Final Build on 8:45 PM 11/6/2018
#ifdef __cplusplus
extern "C" {
#endif//!__cplusplus
	void RegisterModule(v8::Handle<v8::Object> target) {
		//NODE_SET_METHOD(target, "parseScript", test);
		NODE_SET_METHOD(target, "runScript", RUN_SCRIPT);
		NODE_SET_METHOD(target, "parse", PARSE_TEMPLATE);
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
		NODE_SET_METHOD(target, "parseo", PARSE_INTERNAL);
		//NODE_SET_METHOD(target, "async", _NT _NSC NTEMPLATE_PARSE_TEMPLATE_ASYNC);
#endif //!_M_CEE
	}
	NODE_MODULE(NTemplate, RegisterModule)
#ifdef __cplusplus
}
#endif//!__cplusplus
