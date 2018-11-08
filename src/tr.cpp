/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//3:15 AM 11/9/2018
#pragma once
#include "tr.h"
using namespace NTemplate;
#pragma region TEMPLATEREADER
/***
* [TEMPLATE READER IMPLEMENT]
***/
TemplateReader::TemplateReader() {
	this->_count = 0;
	this->pattern = "#include (.*)";
	this->attachPattern = "#attach (.*)";
	this->attachRegx = std::regex(this->attachPattern.c_str());

	this->patternRegx = std::regex(this->pattern.c_str());
	//this->result = new TemplateResult();
	//this->result->is_error = false;

};
TemplateReader::~TemplateReader() {
	this->_templdct.clear();
	delete this->result;
	return;
};
char* TemplateReader::ReadFile(const char*& name) {
	char* chars = "INVALID";
#if _HAS_EXCEPTIONS
	try {
#endif
		FILE*stream;
		errno_t err;
		err = fopen_s(&stream, name, "rb");
		if (stream == NULL) return "INVALID";
		if (err != 0)return "INVALID";
		fseek(stream, 0, SEEK_END);
		size_t size = ftell(stream);
		rewind(stream);
		chars = new char[size + 1];
		chars[size] = '\0';
		for (size_t i = 0; i < size;) {
			i += fread(&chars[i], 1, size - i, stream);
			if (ferror(stream)) {
				fclose(stream);
				return "INVALID";
			}
		}
		fclose(stream);
		stream = NULL;
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		//std::cout << e.what() << "\n";
		chars = "INVALID";
		this->result->is_error = true;
		this->result->err_msg = e.what();
	}
#endif
	return chars;
};
void TemplateReader::Add(std::string& _data) {
	this->_templdct[this->_count] = _data; _data.clear();
	this->_count++;
	return;
};
void TemplateReader::DeepRead(std::string &_path, std::string &data) {
	if (data.empty()) {
		std::string relativePath = std::string(this->_rootDir + "\\" + _path);
		const char* dir = relativePath.c_str();
		char* result;
		result = this->ReadFile(dir);
		if (result == "INVALID") {
			this->result->is_error = true;
			this->result->err_msg = ("Parent Template =>") + (_path)+(" not found. Chield Template =>") + (this->_parent);
			return;
		}
		data = static_cast<std::string>(result);
		delete result; delete dir;
	}
	std::list<std::string> rml;
	REGEX_MATCH_LIST(rml, data, this->patternRegx);
	auto x = rml.begin();
	size_t increment = 0;
	std::string nextTemplate;
	for (auto s = rml.begin(); s != rml.end(); ++s) {
		std::string path = std::regex_replace((*s), std::regex("#include "), "");
		if (increment == 0) {
			nextTemplate = path;
			continue;
		}
		std::string relativePath = this->_rootDir + "\\" + path;
		const char* dir = relativePath.c_str();
		char* rsp = this->ReadFile(dir);
		if (rsp == "INVALID") {
			this->result->is_error = true;
			this->result->err_msg = ("Unable to found this =>% & realative full-path =>%") + (path)+(dir);
			return;
		}
		data = std::regex_replace(data, std::regex("#include \\" + path), std::string(rsp));
		delete rsp;
	}
	if (nextTemplate.empty()) {
		return;
	}
	this->_parent = _path;
	this->Add(std::regex_replace(data, this->patternRegx, ""));
	this->DeepRead(nextTemplate, std::string());

};
void TemplateReader::Read(std::string& path) {
	std::string& relativePath = this->_rootDir + "\\" + path;
	std::cout << "Reading==>" << relativePath << "\r\n";
	const char* absolutepath = relativePath.c_str();
	char* result = this->ReadFile(absolutepath);
	if (result == "INVALID") {
		this->result->is_error = true;
		this->result->err_msg = this->result->err_msg + "Parent Template =>" + path + " not found. Chield Template =>" + this->_parent + ".";
		return;
	}
	//relativePath.clear();
	std::string str = std::string(result); //reinterpret_cast<std::string&>(result);//static_cast<std::string>(result);
	this->Add(std::regex_replace(str, this->patternRegx, ""));
	delete result; //delete absolutepath;
	std::string nextTemplate = REGEX_MATCH_STR(str, this->patternRegx);
	str.clear();
	if (nextTemplate.empty() || nextTemplate == "INVALID") {
		std::cout << "Reading==>" << "Go Back" << "\r\n";
		return;
	}
	this->_parent = path;
	this->Read(nextTemplate);
	return;
};
void TemplateReader::ReadTemplate(std::string& rootDir) {
	std::string nextTemplate = REGEX_MATCH_STR(this->result->t_source, this->patternRegx);
	if (nextTemplate == "INVALID" || nextTemplate.empty()) {
		this->Add(this->result->t_source);
		return;
	}
	this->_rootDir = rootDir;
	this->Add(std::regex_replace(this->result->t_source, this->patternRegx, ""));
	this->Read(nextTemplate);
	return;
}
;
/***
* [TEMPLATE READER IMPLEMENT]
***/
#pragma endregion