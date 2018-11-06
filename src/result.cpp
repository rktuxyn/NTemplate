/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "result.h"
using namespace NTemplate;
NTemplate::Result::Result() {
	this->_err_msg = stringx("");
	this->_source = stringx("");
	this->_is_error = false;
	this->_is_script = false;
	this->_remove_new_line = false;
	this->_is_strict = false;
};
bool NTemplate::Result::is_strict() {
	return this->_is_strict;
};
/*Set is strict ?*/
void NTemplate::Result::is_strict(bool strict) {
	this->_is_strict = strict;
	return;
};
/*Remove new line ?*/
bool NTemplate::Result::remove_new_line() {
	return this->_remove_new_line;
};
/*Set new line ?*/
void NTemplate::Result::remove_new_line(bool rnl) {
	this->_remove_new_line = rnl;
};
/*Check is script ?*/
bool NTemplate::Result::is_script() {
	return this->_is_script;
};
/*Set is script ?*/
void NTemplate::Result::is_script(bool _ke) {
	this->_is_script = _ke;
};
/*Check is error ?*/
bool NTemplate::Result::is_error() {
	return this->_is_error;
};
/*Set Error*/
void NTemplate::Result::is_error(bool _ke) {
	if (_ke == true) {
		this->_source = "";
	}
	this->_is_error = _ke;
};
/*Set Error Message*/
void NTemplate::Result::error_msg(std::string sr) {
	this->_err_msg = "";
};
/*Set Error Message*/
void NTemplate::Result::error_msg(char* sr) {
	this->_err_msg = sr;
};
/*Get Error Message*/
const char* NTemplate::Result::error_msg() {
	return this->_err_msg->c_str();
};
/*Set Source*/
void NTemplate::Result::source(char* sr) {
	this->_source = sr;
};
/*Set Source*/
void NTemplate::Result::source(std::string sr) {
	this->_source = sr;
};
/*Set Source*/
void NTemplate::Result::source(std::string* sr) {
	this->_source = sr;
};
/*Get Source*/
const char* NTemplate::Result::source() {
	return this->_source->c_str();
};
/*Get Source*/
std::string& NTemplate::Result::gsource() {
	return _source.get_Data();
};
stringx& NTemplate::Result::sourcex() {
	return _source;
};
NTemplate::Result::~Result() {
	_source.dispose();
	_err_msg.dispose();
};