/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//3:15 AM 11/9/2018
#pragma once
#include "jsp.h"
using namespace NTemplate;
#pragma region JAVASCRIPTPARSER
/***
* [JAVASCRIPT PARSER IMPLEMENT]
***/
JavaScriptParser::~JavaScriptParser() {};

JavaScriptParser::JavaScriptParser() {
	Tag *tags = new Tag();
	this->regxSts = std::regex("\\" + tags->sts);
	this->regxSte = std::regex("\\" + tags->ste);
	this->regxRts = std::regex("\\" + tags->rts);
	this->regxRte = std::regex("\\" + tags->rte);
	this->repStr = "88__NTEMPLATE___TEMPLATE___REP__STR__88";
	this->repStrRegex = std::regex(this->repStr);
	this->resultKey = "_nopr_";
};
void JavaScriptParser::startTag(std::regex regx, JavaScriptParser::Result* info) {
	if (REGEX_IS_MATCH(info->line, regx) == 0) {
		if (info->isTagEnd) {
			info->line = info->line += " " + this->repStr + "; " + this->resultKey + " += " + this->repStr;
		}
		return;
	}
	bool _default = true;
	info->isTagStart = true;

	std::string _repstr(this->repStr);
	switch (info->etag) {
	case ETag::sts/**/:
		if (REGEX_IS_MATCH(info->line, this->regxSte) == 0) {
			info->isTagEnd = false;
			info->line = std::regex_replace(info->line, regx, " " + this->repStr + ";\r\n");
			info->line = std::regex_replace(info->line, std::regex("'"), " " + this->repStr);
			break;
		}
		info->isTagEnd = true;
		info->isTagStart = false;
		info->line = std::regex_replace(info->line, regx, " " + this->repStr + ";");
		info->line = std::regex_replace(info->line, this->regxSte, " " + this->resultKey + " += " + this->repStr);
		break;
	case ETag::rts/**/:
		if (REGEX_IS_MATCH(info->line, this->regxRte) == 0) {
			info->isTagEnd = false;
			info->line = std::regex_replace(info->line, std::regex("'"), " " + this->repStr);
			info->line = std::regex_replace(info->line, regx, "");
			info->line.append(this->repStr + ";\n" + this->resultKey + " +=");
			break;
		}
		info->isTagEnd = true;
		info->isTagStart = false;

		info->line = REGEX_REPLACE_MATCH(info->line, std::regex("/<js=(.+?)=js>/g"),
			[&_repstr](const std::smatch& m) {
			std::string matchstr(m.str(0));
			if (matchstr.empty()) {
				return std::string("");
			}
			return std::regex_replace(matchstr, std::regex("'"), " " + _repstr);
		});
		info->line = std::regex_replace(info->line, regx, " " + this->repStr + "; " + this->resultKey + " +=");
		info->line = std::regex_replace(info->line, this->regxRte, "; " + this->resultKey + " += " + this->repStr);
		break;
	default: _default = false; break;
	}
	if (_default == false) {
		//std::cout << info.line << "\r\n";
		throw new std::exception("We got error, Invalid script tag found... :(");
		//return info;
	}
	return;
};
void JavaScriptParser::endTag(std::regex regx, JavaScriptParser::Result* info) {
	if (info->isTagStart == false && info->isTagEnd == true) {
		return;
	}
	if (info->isTagStart != false && info->isTagEnd != true) {
		/** Check End Tag**/
		info->isTagStart = true;
		if (REGEX_IS_MATCH(info->line, regx) == 0) {
			info->isTagEnd = false;
			return;
		}
		switch (info->etag) {
		case ETag::ste/*%}*/:
			info->isTagEnd = true;
			info->isTagStart = false;
			info->line = std::regex_replace(info->line, regx, " " + this->resultKey + " += " + this->repStr);
			break;
		case ETag::rte/*=}*/:
			info->isTagEnd = true;
			info->isTagStart = false;
			info->line = std::regex_replace(info->line, regx, "; " + this->resultKey + " += " + this->repStr);
			break;
		default:break;
		}
		return;
	}
	return;
};
void JavaScriptParser::ParseScript() {
	std::cout << "I'm here" << "\r\n";
	JavaScriptParser::Result* info = new JavaScriptParser::Result();
	std::string out = "/** ** [START CLIENT SCRIPT]** **/\r\n/** ** [START STRIPE-1]** **/ \r\n";
	out.append("var " + this->resultKey + "='';\r\n");
	int i = 0;
	info->isTagStart = false;
	info->isTagEnd = true;
	const std::regex rgx("(?:\\r\\n|\\r|\\n)");
	std::sregex_token_iterator iter(this->result->t_source.begin(), this->result->t_source.end(), rgx, -1);
	for (std::sregex_token_iterator end; iter != end; ++iter) {
		out.append("\r\n");
		info->line = iter->str();
		if (info->line == "") {
			out.append(this->resultKey + " += '';");
			continue;
		}
		i++;
		info->line = std::regex_replace(info->line, std::regex("/^\\s*|\\s*$"), " ");
		if (info->isTagEnd) {
			info->line = std::string(this->resultKey + " += " + this->repStr + info->line);
		}
		info->etag = ETag::sts;
		this->startTag(this->regxSts, info);/**TAG-1 <js::**/
		info->etag = ETag::ste;
		this->endTag(this->regxSte, info);/**TAG-2 ::js>**/
		info->etag = ETag::rts;
		this->startTag(this->regxRts, info);/**TAG-3 <js=**/
		info->etag = ETag::rte;
		this->endTag(this->regxRte, info);/**TAG-4 =js>**/
		if (info->isTagEnd) {
			info->line = std::regex_replace(info->line, std::regex("'"), "\\x27");
			info->line = std::regex_replace(info->line, this->repStrRegex, "'");
			out.append(info->line.append("';"));
			info->line = "";
			continue;
		}
		info->line = std::regex_replace(info->line, this->repStrRegex, "'");
		out.append(info->line); info->line = "";
	}
	delete info;
	out.append("\r\n/** ** [/END STRIPE-" + std::to_string(i + 1 - 1) + "] ** **/\r\n/** ** [END CLIENT SCRIPT]** **/");
	out = std::regex_replace(out, std::regex("/" + this->resultKey + " \\+\\= '';"), "");
	out.append("\r\n return " + this->resultKey + ";");
	if (this->result->remove_new_line) {
		out = REGEX_REPLACE_ALL(out, std::regex("(?:\\r\\n)"), "");
	}
	this->result->t_source.clear();
	this->result->t_source = std::string(out);
	out.clear();
	return;
};
/***
* [/JAVASCRIPT PARSER IMPLEMENT]
***/
#pragma endregion