/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//3:15 AM 11/9/2018
#pragma once
#include "tp.h"
using namespace NTemplate;
using namespace NTemplate;
#pragma region TemplateParser
TemplateParser::TemplateParser(TemplateResult* tr, const char* v, std::string& dir) {
	this->view_path = v; this->root_dir = dir;
	this->regx = std::regex("<placeholder id=\"(.*)\">");
	this->key = "placeholder";
	this->endTag = "</placeholder>";
	this->endTagRegx = std::regex(this->endTag);
	this->_parent = v;
	this->result = tr;
	return;
};

TemplateParser::~TemplateParser() {
	this->root_dir.clear();
	this->key.clear();
	this->endTag.clear();
	return;
};
/***
* [TEMPLATE PARSER IMPLEMENT]
***/
void TemplateParser::getMatchList(std::list<std::string>&ml, std::string& str, std::regex rgx) {
#if _HAS_EXCEPTIONS
	try {
#endif
		REGEX_MATCH_LIST(ml, str, rgx);
#if _TEST_RUNTIME
		std::cout << "I'm passed from getMatchList." << "\r\n";
#endif
#if _HAS_EXCEPTIONS
	} catch (std::regex_error& e) {
		this->result->is_error = true;
		this->result->err_msg = e.what();
	}
#endif
	return;
};
void TemplateParser::getSetTemplate(std::string str, v8::Isolate* isolate) {
	if (str == "INVALID") {
		this->result->t_source = "NOT FOUND!!!";
		return;
	}
	this->result->t_source = str;
};
void TemplateParser::extendPlaceholder(std::map<std::string, std::string>&extendId, std::map<int, std::string>&match) {
	std::string key;
	for (auto itr = match.begin(); itr != match.end(); itr++) {
		key = REGEX_MATCH_STR(itr->second, this->regx);
		if (key == "INVALID" || key.empty()) {
			continue;
		}
		extendId[key] = key;
	}
	return;
};
void TemplateParser::overlapTemplate(std::list<std::string> ml, std::string& body) {
#if _HAS_EXCEPTIONS
	try {
#endif
#if _TEST_RUNTIME
		std::cout << "Start overlapTemplate" << "\r\n";
#endif
		std::map<std::string, std::string> extendId;
		extendId["__DEMO__"] = "__DEMO__";
		body = std::regex_replace(body, std::regex("(?:\\r\\n)"), this->repStr);//Make Single Line
		std::string sk;
		std::string brgx;
		std::string p;
		std::map<int, std::string>::iterator matchIt;
		std::string first;//!TODO
		std::map<int, std::string> match;
		for (auto s = ml.begin(); s != ml.end(); ++s) {
			sk = *s;
			if (sk.empty())continue;

			if (REGEX_IS_MATCH(body, std::regex(sk)) == 0) {
				this->result->t_source.clear(); body.clear();
				this->result->is_error = true;
				this->result->err_msg = ("Parser::overlapTemplate=>Invalid Placeholder defined " + sk + " :( ");
				return;
			}
			if (extendId.find(REGEX_MATCH_STR(sk, this->regx)) != extendId.end()) {
				continue;
			}
			brgx = sk + "(.*)<(\\/)?" + this->key + ">";
			brgx = STR_TRIM(brgx);
			p = REGEX_MATCH_STR(body, std::regex(brgx));
			if (p == "INVALID" || p == "ERROR") {
				this->result->is_error = true;
				this->result->err_msg = ("Parser::overlapTemplate=>Invalid Placeholder defined " + sk + " :( ");
				std::cout << this->result->err_msg << "\r\n";
				return;
			}
			if (p.empty())continue;
			p = STR_TRIM(p);
			STR_MAP_SPLIT(match, p, this->endTagRegx);
			first = "";
			if (!match.empty()) {

				matchIt = match.find(0);

				if (matchIt != match.end()) {
					first = matchIt->second;
				}
				if (first == "" || first.empty()) {
					this->result->t_source = std::regex_replace(this->result->t_source, std::regex(sk), p);
					match.clear();
					continue;
				}
				match.clear();
			}
			if (first == "") {
				p = STR_SPLIT(first, this->endTag);
				continue;
			}
			if (REGEX_MATCH_STR(first, std::regex(this->regx)) == "INVALID") {
				p = STR_SPLIT(first, this->endTag);
			}
			else {
				this->extendPlaceholder(extendId, match);
			}
			p = std::regex_replace(p, this->repStrRegex, "\r\n");//Back to Multiple Line
			this->result->t_source = std::regex_replace(this->result->t_source, std::regex(sk), p);
		}
		sk.clear(); brgx.clear(); p.clear(); first.clear();
		ml.clear(); body.clear(); extendId.clear();
#if _TEST_RUNTIME
		std::cout << "I'm passed from overlapTemplate." << "\r\n";
#endif
		return;
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		this->result->err_msg = e.what();
		this->result->t_source.clear();
		this->result->is_error = true;
		return;
	}
#endif//!_HAS_EXCEPTIONS
}
void TemplateParser::margeTemplate() {
#if _HAS_EXCEPTIONS
	try {
#endif

		int count = 0;
		for (auto iter = this->_templdct.rbegin(); iter != this->_templdct.rend(); ++iter) {
			if (count == 0) {
				this->result->t_source = iter->second;
				count++;
				continue;
			}
			std::list<std::string> ml;
			this->getMatchList(ml, iter->second, this->regx);
			if (ml.empty())continue;
			if (ml.size() <= 0) {
				continue;
			}
			this->overlapTemplate(ml, STR_TRIM(iter->second));
			if (this->result->is_error) {
				this->result->t_source.clear();
				break;
			}
		}
		this->result->t_source = std::regex_replace(this->result->t_source, this->endTagRegx, "");
#if _TEST_RUNTIME
		std::cout << "I'm passed from margeTemplate." << "\r\n";
#endif
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		this->result->err_msg = e.what();
		this->result->is_error = true;
		this->result->t_source.clear();
	}
#endif//!_HAS_EXCEPTIONS
	return;
}
void TemplateParser::implimantAttachment() {
	std::list<std::string> rml;
	this->getMatchList(rml, this->result->t_source, this->attachRegx);
	if (rml.size() > 0) {
#if _TEST_RUNTIME
		std::cout << "Start implimantAttachment." << "\r\n";
#endif
		for (auto s = rml.begin(); s != rml.end(); ++s) {
			std::string itr = *s;
#if _TEST_RUNTIME
			std::cout << itr << "\r\n";
#endif
			std::string path = std::regex_replace(itr, std::regex("#attach "), "");
			std::string relativePath = this->root_dir + "\\" + path;
			const char* dir = relativePath.c_str();
			char* cresult = this->ReadFile(dir);
			if (cresult == "INVALID") {
				this->result->is_error = true;
				this->result->err_msg = "Unable to found this attachment=> " + path + " & realative full-path =>" + dir + ".";
				return;
			}
			this->result->t_source = std::regex_replace(this->result->t_source, std::regex("#attach \\" + path), std::string(cresult));
			delete cresult;
		}
	}
#if _TEST_RUNTIME
	std::cout << "I'm passed from implimantAttachment." << "\r\n";
#endif
	return;
};
void TemplateParser::nonImplementPlaceholder() {
#if _TEST_RUNTIME
	std::cout << "I'm passed from nonImplementPlaceholder." << "\r\n";
#else
	std::list<std::string> rml;
	this->getMatchList(rml, this->result->t_source, this->regx);
	std::string requiredPlaceholder("__NOP__");
	if (rml.size() > 0) {
		requiredPlaceholder = "This following Placeholder were missing to implimant in child:\r\n";
		for (std::list<std::string>::iterator s = rml.begin(); s != rml.end(); ++s) {
			requiredPlaceholder.append(*s + "\r\n");
		}
		this->result->is_error = true;
		this->result->err_msg = requiredPlaceholder;
	};
	std::list<std::string>().swap(rml);
#endif
	return;
};
void TemplateParser::Start() {
#if _TEST_RUNTIME
	std::cout << "TemplateParser::Start..." << "\r\n";
#endif
	this->result->is_error = false;
	this->result->t_source = "Working!!!";
	char* rresult = this->ReadFile(this->view_path);
	if (rresult == "INVALID") {
		this->result->err_msg = "NOT FOUND!!!";
		this->result->is_error = true;
		return;
	}
	this->result->t_source = rresult;
	delete rresult;
	try {
		this->ReadTemplate(this->root_dir);
		if (this->result->is_error) return;
		this->margeTemplate();
		if (this->result->is_error) return;
		if (this->result->is_strict) {
			this->nonImplementPlaceholder();
			if (this->result->is_error) return;
		}
		this->implimantAttachment();
		if (this->result->is_error) return;
		if (REGEX_IS_MATCH(this->result->t_source, this->regxSts) == 0) {
			this->result->is_script = false;
			return;
		}
		this->result->is_script = true;
		this->result->is_script_template = true;
		this->ParseScript();
		if (this->result->is_error) return;
	} catch (std::exception&e) {
		this->result->err_msg = e.what();
	}
};

/***
* [TEMPLATE PARSER IMPLEMENT]
***/
#pragma endregion