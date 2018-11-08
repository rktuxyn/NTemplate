/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#ifndef SRC_NODE_H_
#include <node.h>
#endif //!SRC_NODE_H_
#ifndef INCLUDE_V8_H_
#include <v8.h>
#endif // !INCLUDE_V8_H_
#include "src/core.h"
#ifdef __cplusplus
extern "C" {
#endif//!__cplusplus
	void RegisterModule(v8::Handle<v8::Object> target) {
		NODE_SET_METHOD(target, "runScript", RUN_SCRIPT);
		NODE_SET_METHOD(target, "parse", PARSE_TEMPLATE);
#if defined(_M_CEE) ///clr or /clr:pure //_IOSFWD_
#error <thread> is not supported when compiling with /clr or /clr:pure.
#else
		NODE_SET_METHOD(target, "parseo", PARSE_INTERNAL);
#endif //!_M_CEE
	}
	NODE_MODULE(NTemplate, RegisterModule)
#ifdef __cplusplus
}
#endif//!__cplusplus
