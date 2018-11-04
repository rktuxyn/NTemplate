#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif // !NTEMPLATE_DEP_
_NT_BEGIN
class Result {
private:
	stringx					_source;
	stringx					_err_msg;
	bool					_is_error;
	bool					_is_script;
	bool					_remove_new_line;
	bool					_is_strict;
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
	void error_msg(_STD string sr);
	/*Set Error Message*/
	void error_msg(char* sr);
	/*Get Error Message*/
	const char* error_msg();
	/*Set Source*/
	void source(char* sr);
	/*Set Source*/
	void source(_STD string sr);
	/*Set Source*/
	void source(_STD string* sr);
	/*Get Source*/
	const char* source();
	/*Get Source*/
	_STD string& gsource();
	~Result();
};
_NT_END