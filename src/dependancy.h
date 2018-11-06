/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#pragma once
#ifndef NTEMPLATE_DEP_
#define NTEMPLATE_DEP_
#ifndef INCLUDE_V8_H_
#include <v8.h>
#endif // !INCLUDE_V8_H_
#ifndef _IOSTREAM_
#include <iostream>
#endif // !_IOSTREAM_
#ifndef _INC_STDLIB
#include <stdlib.h>
#endif // !_INC_STDLIB
#ifndef _XSTRING_
#include <string>
#endif // !_XSTRING_
#ifndef _REGEX_
#include <regex>
#endif// !_REGEX_
#ifndef _LIST_
#include <list>
#endif // !_LIST_
#ifndef _MAP_
#include <map>
#endif // !_MAP_
#ifndef _INLINE
# define _INLINE inline
#endif // !_INLINE
#ifndef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 1 // Predefine as 0 to disable exceptions
#endif//!_HAS_EXCEPTIONS
#ifndef _REGEX_MAX_STACK_COUNT
#define _REGEX_MAX_STACK_COUNT 0;
#endif//!_REGEX_MAX_STACK_COUNT
#define TEST_PERFORMANCE 0
#define READ_PERFORMANCE 0
#if TEST_PERFORMANCE
#ifndef _CHRONO_
#include <chrono>
#endif //!_CHRONO_
#endif //!TEST_PERFORMANCE
#if READ_PERFORMANCE
//Read File
#ifndef _CHRONO_
#include <chrono>
#endif //!_CHRONO_
#ifndef _FSTREAM_
#include <fstream>
#endif //!_FSTREAM_
#ifndef _STREAMBUF_
#include <streambuf>
#endif //!_STREAMBUF_
//Read File
#endif //!READ_PERFORMANCE
#if defined(__linux__)//_M_CEE ///clr or /clr:pure
#ifndef _FUTURE_
#include <future> //_STD async, _STD future
#endif // !_FUTURE_
#endif // !__linux__
#ifndef  _Stringx
#include "stringx.h"
#endif // ! _Stringx
#ifndef STD_EXTEND_REGEX
#include "stder.h"
#endif // !STD_EXTEND_REGEX
#ifdef _WIN32
#define NTEMPLATE_EXPORT __declspec(dllexport)
#else
# define NTEMPLATE_EXPORT __attribute__((visibility("default")))
#endif//!NTEMPLATE_EXPORT
#undef _M_CEE //REMOVE_IT
#ifndef _STD_BEGIN
#define _STD_BEGIN	namespace std {
#endif //!_STD_BEGIN
#ifndef _STD_END
#define _STD_END		}
#endif //!_STD_END
#ifndef _STD
#define _STD	::std::
#endif //!_STD
#define _NT_BEGIN	namespace NTemplate {
#define _NT_END		}
#define _NT			NTemplate::
#ifndef _V8
#define _V8 ::v8::
#endif
#ifndef _Try
#define _Try try {
#endif // !_Try
#ifndef _Catch
#define _Catch } catch (std::exception& e) {
#endif // !_Catch
#ifndef _TrE
#define _TrE }
#endif // !_TrE

#define NEW_REGEX(rgx)(\
new _STD regex(rgx)\
)
#define SWAP_REGEX(rgx)(\
_STD regex().swap(rgx)\
)
#define NEW_LIST_STR()(\
new _STD list<_STD string>()\
)
#define SWAP_LIST_STR(ls)(\
_STD list<_STD string>().swap(ls)\
)
#define NEW_MAP_STR()(\
new _STD map<int, _STD string>()\
)
#define SWAP_MAP_STR(ms)(\
_STD map<int, _STD string>().swap(ms)\
)
#define DELETE_VER(var)(\
delete (void*)(&var)\
)
//#include "result.h"
namespace NTemplate {
	//Blueprint
#ifndef  T_CHAR
	static const char* _toCharStr(const v8::String::Utf8Value& value);
#define T_CHAR _toCharStr
	const char* _toCharStr(const v8::String::Utf8Value& value) {
		if (value.length() <= 0)return "";
		return *value ? *value : "<string conversion failed>";
	};
#endif // ! T_CHAR
	template<class _S>
	void _Destroy(_S* v) {
		std::allocator<_S> stringAlloc;
		stringAlloc.destroy(v);
	};
	/**#include<stdarg.h>
	stringx xformat(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		stringx str("");
		while (*fmt != '\0') {
			if (*fmt != '%') {
				str += *fmt;
				++fmt;
				continue;
			}
			char * s = va_arg(args, char*);
			str += s;
			++fmt;
		}
		va_end(args);
		return str;
	};*/
	struct TemplateResult {
		stringx t_source;
		stringx err_msg;
		bool is_error;
		bool is_script;
	};
	class TemplateReader;
	class TemplateParser;
	class JavaScriptParser;
}
#endif //!NTEMPLATE_DEP_
