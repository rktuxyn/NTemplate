#pragma once
#ifndef _Stringx
#include "stringx.h"
#endif // !NTEMPLATE_DEP_
#define N_RESULT
namespace NTemplate {
	class Result {
	private:
		stringx					_source = stringx("");
		stringx					_err_msg = stringx("");
		bool					_is_error = false;
		bool					_is_script = false;
		bool					_remove_new_line = false;
		bool					_is_strict = false;
	public:
		Result();
		/*Check is strict ?*/
		bool is_strict();
		/*Set is strict ?*/
		void is_strict(bool strict);
		/*Remove new line ?*/
		bool remove_new_line();
		/*Set new line ?*/
		void remove_new_line(bool rnl);
		/*Check is script ?*/
		bool is_script();
		/*Set is script ?*/
		void is_script(bool _ke);
		/*Check is error ?*/
		bool is_error();
		/*Set Error*/
		void is_error(bool _ke);
		/*Set Error Message*/
		void error_msg(std::string sr);
		/*Set Error Message*/
		void error_msg(char* sr);
		/*Get Error Message*/
		const char* error_msg();
		/*Set Source*/
		void source(char* sr);
		/*Set Source*/
		void source(std::string sr);
		/*Set Source*/
		void source(std::string* sr);
		/*Get Source*/
		const char* source();
		/*Get Source*/
		std::string& gsource();
		stringx& sourcex();
		~Result();
	};
};