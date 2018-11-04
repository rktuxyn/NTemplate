/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "result.h"
_NT Result::Result() {
	_err_msg = stringx("");
	_source = stringx("");
	_is_error = false;
	_is_script = false;
	_remove_new_line = false;
	_is_strict = false;
};
bool _NT Result::is_strict() {
	return _is_strict;
};
/*Set is strict ?*/
void _NT Result::is_strict(bool strict) {
	_is_strict = strict;
	return;
};
/*Remove new line ?*/
bool _NT Result::remove_new_line() {
	return _remove_new_line;
};
/*Set new line ?*/
void _NT Result::remove_new_line(bool rnl) {
	_remove_new_line = rnl;
};
/*Check is script ?*/
bool _NT Result::is_script() {
	return _is_script;
};
/*Set is script ?*/
void _NT Result::is_script(bool _ke) {
	_is_script = _ke;
};
/*Check is error ?*/
bool _NT Result::is_error() {
	return _is_error;
};
/*Set Error*/
void _NT Result::is_error(bool _ke) {
	if (_ke == true) {
		_source = "";
	}
	_is_error = _ke;
};
/*Set Error Message*/
void _NT Result::error_msg(_STD string sr) {
	_err_msg = "";
};
/*Set Error Message*/
void _NT Result::error_msg(char* sr) {
	_err_msg = sr;
};
/*Get Error Message*/
const char* _NT Result::error_msg() {
	return _err_msg->c_str();
};
/*Set Source*/
void _NT Result::source(char* sr) {
	_source = sr;
};
/*Set Source*/
void _NT Result::source(_STD string sr) {
	_source = sr;
};
/*Set Source*/
void _NT Result::source(_STD string* sr) {
	_source = sr;
};
/*Get Source*/
const char* _NT Result::source() {
	return _source->c_str();
};
/*Get Source*/
_STD string& _NT Result::gsource() {
	return _source.get_Data();
};
_NT Result::~Result() {
	_source.dispose();
	_err_msg.dispose();
};