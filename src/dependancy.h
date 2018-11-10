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
#ifndef STD_EXTEND_REGEX
#include "stder.h"
#endif // !STD_EXTEND_REGEX
#ifndef _TEST_RUNTIME
#define _TEST_RUNTIME 0L
#endif // !_WRITE_OUTPUT
#undef _M_CEE //REMOVE_IT
#if _TEST_RUNTIME
#ifndef _FSTREAM_
#include <fstream>
#endif
#endif // _TEST_RUNTIME
#ifdef _STR_FRMT
#ifndef _INC_STDARG
#include<stdarg.h>
#endif // !_INC_STDARG
#endif // _STR_FRMT
#define __EXPORT_REQ 0L
#if __EXPORT_REQ
#define __EXPORT
#endif

namespace NTemplate {
#ifndef  T_CHAR
	static const char* _toCharStr(const v8::String::Utf8Value& value);
#define T_CHAR _toCharStr
	const char* _toCharStr(const v8::String::Utf8Value& value) {
		if (value.length() <= 0)return "";
		return *value ? *value : "<string conversion failed>";
	};
#endif // ! T_CHAR
	static void WriteFile(std::string str) {
#if _TEST_RUNTIME
		std::ofstream outputFile;
		outputFile.open(".\\temp\\out.js");
		outputFile << str;
		str.clear(); std::string().swap(str);
		outputFile.close();
		outputFile.clear();
		std::ofstream().swap(outputFile);
#else
		throw new std::exception("Not supported");
#endif
	};
	
	static std::string xformat(const char *fmt, ...) {
#if _STR_FRMT
		va_list args;
		va_start(args, fmt);
		std::string str("");
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
#else
		throw new std::exception("Not supported");
#endif
	};
	struct TemplateResult {
		std::string t_source;
		std::string err_msg;
		bool is_error;
		bool is_script;
		bool is_script_template;
		bool remove_new_line;
		bool is_strict;
	};
	class TemplateReader;
	class TemplateParser;
	class JavaScriptParser;
}
#endif //!NTEMPLATE_DEP_
