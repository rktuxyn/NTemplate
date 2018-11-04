//11:15 PM 11/4/2018
#if defined(_MSC_VER)
#  pragma once
#endif
#include "ntemplate.h"
using namespace NTemplate;
#pragma region TEMPLATEREADER
/***
* [TEMPLATE READER IMPLEMENT]
***/
TemplateReader::TemplateReader() {
	this->_count = 0;
	this->pattern = stringx("#include (.*)");
	this->attachPattern = stringx("#attach (.*)");
	this->attachRegx = std::regex(this->attachPattern->c_str());

	this->patternRegx = std::regex(this->pattern->c_str());
	this->reader_result = new TemplateResult();
	this->reader_result->is_error = false;
};
TemplateReader::~TemplateReader() {
	this->_templdct.clear();
	delete this->reader_result;
	return;
};
inline char*
TemplateReader::ReadFile(const char*& name) {
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
		this->reader_result->is_error = true;
		this->reader_result->err_msg = e.what();
	}
#endif
	return chars;
};
inline void
TemplateReader::Add(std::string _data) {
	this->_templdct[this->_count] = _data; _data.clear();
	this->_count++;
	return;
};
inline void
TemplateReader::DeepRead(stringx &_path, stringx &data) {
	if (data->empty()) {
		stringx relativePath = stringx(this->_rootDir + "\\" + _path);
		const char* dir = relativePath->c_str();
		const char* result;
		result = this->ReadFile(dir);
		if (result == "INVALID") {
			this->reader_result->is_error = true;
			this->reader_result->err_msg.concat("Parent Template =>").concat(_path).concat(" not found. Chield Template =>").concat(this->_parent);
			return;
		}
		data = static_cast<std::string>(result);
		delete result; delete dir;
	}
	std::list<stringx> rml = REGEX_MATCH_LIST(data.get_Data(), this->patternRegx);
	auto x = rml.begin();
	size_t increment = 0;
	stringx nextTemplate;
	for (auto s = rml.begin(); s != rml.end(); ++s) {
		stringx path = REGEX_REPLACE_ALL((*s).get_Data(), std::regex("#include "), "");
		if (increment == 0) {
			nextTemplate = path;
			continue;
		}
		stringx relativePath = this->_rootDir + "\\" + path;
		const char* dir = relativePath->c_str();
		const char* rsp = this->ReadFile(dir);
		if (rsp == "INVALID") {
			this->reader_result->is_error = true;
			this->reader_result->err_msg.concat("Unable to found this =>% & realative full-path =>%").concat(path).concat(dir);
			return;
		}
		data = REGEX_REPLACE_ALL(data.get_Data(), std::regex("#include \\" + path.get_Data()), std::string(rsp));
		delete dir; delete rsp;
	}
	if (nextTemplate->empty()) {
		return;
	}
	this->_parent = _path;
	this->Add(REGEX_REPLACE_ALL(data.get_Data(), this->patternRegx, ""));
	this->DeepRead(nextTemplate, stringx());

};
inline void
TemplateReader::Read(stringx path) {
	stringx relativePath = this->_rootDir + "\\" + path;
	const char* dir = relativePath->c_str();
	const char* result;
	result = this->ReadFile(dir);
	if (result == "INVALID") {
		this->reader_result->is_error = true;
		this->reader_result->err_msg = this->reader_result->err_msg + "Parent Template =>" + path + " not found. Chield Template =>" + this->_parent + ".";
		return;
	}
	std::string& str = static_cast<std::string>(result);
	delete result; delete dir;
	this->Add(REGEX_REPLACE_ALL(str, this->patternRegx, ""));
	std::string nextTemplate = REGEX_MATCH_STR(str, this->patternRegx);
	if (nextTemplate == "INVALID") {
		return;
	}
	this->_parent = path;
	this->Read(nextTemplate);
	return;
};
inline TemplateResult*
TemplateReader::Read(stringx rootTemplate, const char* rootPath, stringx rootDir) {
	//this->DeepRead(std::string(), rootTemplate);
	std::string nextTemplate = REGEX_MATCH_STR(rootTemplate.get_Data(), this->patternRegx);

	if (nextTemplate == "INVALID") {
		this->Add(rootTemplate.get_Data());
		return this->reader_result;
	}
	this->_rootDir = rootDir;
	this->Add(REGEX_REPLACE_ALL(rootTemplate.get_Data(), this->patternRegx, ""));
	this->Read(nextTemplate);

	return this->reader_result;
};
/***
* [TEMPLATE READER IMPLEMENT]
***/
#pragma endregion
#pragma region JAVASCRIPTPARSER
/***
* [JAVASCRIPT PARSER IMPLEMENT]
***/
JavaScriptParser::~JavaScriptParser() {};
inline 
JavaScriptParser::JavaScriptParser() {
	Tag *tags = new Tag();
	this->regxSts = std::regex("\\" + tags->sts);
	this->regxSte = std::regex("\\" + tags->ste);
	this->regxRts = std::regex("\\" + tags->rts);
	this->regxRte = std::regex("\\" + tags->rte);
	this->repStr = "88__NTEMPLATE___TEMPLATE___REP__STR__88";
	this->repStrRegex = std::regex(this->repStr.get_Data());
	this->resultKey = "__N_OUT_PUT_RESULT__";
};
inline void
JavaScriptParser::startTag(std::regex regx, JavaScriptParser::Result* info) {
	if (REGEX_IS_MATCH(info->line.get_Data(), regx) == 0) {
		if (info->isTagEnd) {
			info->line = info->line += " " + this->repStr.get_Data() + "; " + this->resultKey.get_Data() + " += " + this->repStr.get_Data();
		}
		return;
	}
	bool _default = true;
	info->isTagStart = true;
	std::string _repstr(this->repStr.get_Data());
	switch (info->etag) {
	case ETag::sts/**/:
		if (REGEX_IS_MATCH(info->line.get_Data(), this->regxSte) == 0) {
			info->isTagEnd = false;
			info->line = REGEX_REPLACE_ALL(info->line.get_Data(), regx, " " + this->repStr.get_Data() + ";\r\n");
			info->line = REGEX_REPLACE_ALL(info->line.get_Data(), std::regex("'"), " " + this->repStr.get_Data());
			break;
		}
		info->isTagEnd = true;
		info->isTagStart = false;
		info->line = REGEX_REPLACE_ALL(info->line.get_Data(), regx, " " + this->repStr.get_Data() + ";");
		info->line = REGEX_REPLACE_ALL(info->line.get_Data(), this->regxSte, " " + this->resultKey.get_Data() + " += " + this->repStr.get_Data());
		break;
	case ETag::rts/**/:
		if (REGEX_IS_MATCH(info->line.get_Data(), this->regxRte) == 0) {
			info->isTagEnd = false;
			info->line = REGEX_REPLACE_ALL(info->line.get_Data(), std::regex("'"), " " + this->repStr.get_Data());
			info->line = REGEX_REPLACE_ALL(info->line.get_Data(), regx, "");
			info->line->append(this->repStr.get_Data() + ";\n" + this->resultKey.get_Data() + " +=");
			break;
		}
		info->isTagEnd = true;
		info->isTagStart = false;

		info->line = REGEX_REPLACE_MATCH(info->line.get_Data(), std::regex("/<js=(.+?)=js>/g"),
			[&_repstr](const std::smatch& m) {
			std::string matchstr(m.str(0));
			if (matchstr.empty()) {
				return std::string("");
			}
			return REGEX_REPLACE_ALL(matchstr, std::regex("'"), " " + _repstr);
		});
		info->line = REGEX_REPLACE_ALL(info->line.get_Data(), regx, " " + this->repStr.get_Data() + "; " + this->resultKey.get_Data() + " +=");
		info->line = REGEX_REPLACE_ALL(info->line.get_Data(), this->regxRte, "; " + this->resultKey.get_Data() + " += " + this->repStr.get_Data());
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
inline void
JavaScriptParser::endTag(std::regex regx, JavaScriptParser::Result* info) {
	//stringx line = info->line;
	//stringx tag = info->tag;
	if (info->isTagStart == false && info->isTagEnd == true) {
		return;
	}
	if (info->isTagStart != false && info->isTagEnd != true) {
		/** Check End Tag**/
		info->isTagStart = true;
		if (REGEX_IS_MATCH(info->line.get_Data(), regx) == 0) {
			info->isTagEnd = false;
			return;
		}
		switch (info->etag) {
		case ETag::ste/*%}*/:
			info->isTagEnd = true;
			info->isTagStart = false;
			info->line = REGEX_REPLACE_ALL(info->line.get_Data(), regx, " " + this->resultKey.get_Data() + " += " + this->repStr.get_Data());
			break;
		case ETag::rte/*=}*/:
			info->isTagEnd = true;
			info->isTagStart = false;
			info->line = REGEX_REPLACE_ALL(info->line.get_Data(), regx, "; " + this->resultKey.get_Data() + " += " + this->repStr.get_Data());
			break;
		default:break;
		}
		return;
	}
	return;
};
inline stringx
JavaScriptParser::ParseScript(stringx templateStr) {
	//!TODO
	JavaScriptParser::Result* info = new JavaScriptParser::Result();
	stringx out = "/** ** [START CLIENT SCRIPT]** **/\r\n/** ** [START STRIPE-1]** **/ \r\n";
	out->append("var " + this->resultKey.get_Data() + "='';\r\n");
	int i = 0;
	info->isTagStart = false;
	info->isTagEnd = true;

	const std::regex rgx("(?:\\r\\n|\\r|\\n)");
	std::sregex_token_iterator iter(templateStr->begin(), templateStr->end(), rgx, -1);
	for (std::sregex_token_iterator end; iter != end; ++iter) {
		out.append("\r\n");
		info->line = iter->str();
		if (info->line == "") {
			out.append(this->resultKey + " += '';");
			continue;
		}
		i++;
		info->line = REGEX_REPLACE_ALL(info->line.get_Data(), std::regex("/^\\s*|\\s*$"), " ");
		if (info->isTagEnd) {
			info->line = std::string(this->resultKey.get_Data() + " += " + this->repStr.get_Data() + info->line.get_Data());
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
			info->line = REGEX_REPLACE_ALL(info->line.get_Data(), std::regex("'"), "\\x27");
			info->line = REGEX_REPLACE_ALL(info->line.get_Data(), this->repStrRegex, "'");
			out.append(info->line->append("';"));
			info->line = "";
			continue;
		}
		info->line = REGEX_REPLACE_ALL(info->line.get_Data(), this->repStrRegex, "'");
		out.append(info->line); info->line = "";
	}
	delete info;
	out.append("\r\n/** ** [/END STRIPE-" + std::to_string(i + 1 - 1) + "] ** **/\r\n/** ** [END CLIENT SCRIPT]** **/");
	out = REGEX_REPLACE_ALL(out.get_Data(), std::regex("/" + this->resultKey.get_Data() + " \\+\\= '';"), "");
	out.append("\r\n return " + this->resultKey.get_Data() + ";");
	return out;
};
/***
* [/JAVASCRIPT PARSER IMPLEMENT]
***/
#pragma endregion
#pragma region TemplatParser
inline 
TemplatParser::TemplatParser(const char* v, std::string& dir) {
	this->view_path = v; this->root_dir = dir;
	this->hasError = false;
	this->isScriptTemplate = false;
	this->regx = std::regex("<placeholder id=\"(.*)\">");
	this->key = "placeholder";
	this->endTag = "</placeholder>";
	this->endTagRegx = std::regex(this->endTag.get_Data());
	this->_parent = std::string(v);
	return;
};
inline 
TemplatParser::~TemplatParser() {
	this->outStr.dispose();
	this->root_dir.dispose();
	this->outResult.dispose();
	this->key.dispose();
	this->endTag.dispose();
	return;
};
/***
* [TEMPLATE PARSER IMPLEMENT]
***/
inline std::list<stringx>
TemplatParser::getMatchList(stringx str, std::regex rgx) {
	std::list<stringx> result;
#if _HAS_EXCEPTIONS
	try {
#endif
		return REGEX_MATCH_LIST(str.get_Data(), rgx);
#if _HAS_EXCEPTIONS
	} catch (std::regex_error& e) {
		this->hasError = true;
		this->errorMsg = e.what();
		//std::cout << e.what() << "\n";
	}
#endif
	return result;
}
inline void
TemplatParser::getSetTemplate(stringx str, v8::Isolate* isolate) {
	if (str == "INVALID") {
		this->outResult = "NOT FOUND!!!";
		return;
	}
	this->outResult = str;
}
inline std::map<stringx, stringx>
TemplatParser::extendPlaceholder(std::map<stringx, stringx> extendId, std::map<int, stringx> match) {
	std::string key;
	//std::string* key = NEW_STAR();
	std::regex re = std::regex(this->regx);
	for (auto itr = match.begin(); itr != match.end(); itr++) {
		key = REGEX_MATCH_STR(itr->second.get_Data(), re);
		if (key == "INVALID" || key.empty()) {
			continue;
		}
		extendId[key] = key;
	}
	return extendId;
}
inline stringx
TemplatParser::_overlapTemplate(stringx& mainBody, std::list<stringx> ml, stringx& body) {
	if (mainBody->empty())return "";
	std::map<stringx, stringx> extendId;
	extendId[stringx("__DEMO__")] = "__DEMO__";
	body = REGEX_REPLACE_ALL(body.get_Data(), std::regex("(?:\\r\\n)"), this->repStr.get_Data());
	stringx sk;
	stringx brgx;
	stringx p;
	std::map<int, stringx>::iterator matchIt;
	stringx first;//!TODO
	std::map<int, stringx> match;
	for (std::list<stringx>::iterator s = ml.begin(); s != ml.end(); ++s) {
		sk = *s;
		if (sk->empty())continue;

		if (REGEX_IS_MATCH(body.get_Data(), std::regex(sk.get_Data())) == 0) {
			mainBody.dispose(); body.dispose();
			this->hasError = true;
			this->errorMsg.append("Parser::overlapTemplate=>Invalid Placeholder defined ").append(sk + " :( ");
			return stringx("INVALID");
		}
		if (extendId.find(REGEX_MATCH_STR(sk.get_Data(), this->regx)) != extendId.end()) {
			continue;
		}
		brgx = sk + "(.*)<(\\/)?" + this->key + ">";
		brgx = STR_TRIM(brgx.get_Data());
		p = REGEX_MATCH_STR(body.get_Data(), std::regex(brgx.get_Data()));
		if (p == "INVALID" || p == "ERROR") {
			this->hasError = true;
			this->errorMsg.append("Parser::overlapTemplate=>Invalid Placeholder defined ").append(sk + " :( ");

			return stringx("INVALID");
		}
		if (p->empty())continue;
		p = STR_TRIM(p.get_Data());

		match = std::_map_split(p.get_Data(), this->endTagRegx);
		first = "";
		if (!match.empty()) {

			matchIt = match.find(0);

			if (matchIt != match.end()) {
				first = matchIt->second;
			}
			if (first == "" || first->empty()) {
				mainBody = REGEX_REPLACE_ALL(mainBody.get_Data(), std::regex(sk.get_Data()), p.get_Data());

				continue;
			}
		}
		if (first == "") {
			p = STR_SPLIT(first.get_Data(), this->endTag.get_Data());
			continue;
		}
		if (REGEX_MATCH_STR(first.get_Data(), std::regex(this->regx)) == "INVALID") {
			p = STR_SPLIT(first.get_Data(), this->endTag.get_Data());
		}
		else {
			extendId = this->extendPlaceholder(extendId, match);
		}
		p = REGEX_REPLACE_ALL(p.get_Data(), this->repStrRegex, "\r\n");//Back to Multiple Line
		mainBody = REGEX_REPLACE_ALL(mainBody.get_Data(), std::regex(sk.get_Data()), p.get_Data());

	}
	sk.dispose(); brgx.dispose(); p.dispose(); first.dispose();
	std::map<stringx, stringx>().swap(extendId);
	return mainBody;
}
inline stringx
TemplatParser::overlapTemplate(stringx& mainBody, std::list<stringx> ml, stringx& body) {
#if _HAS_EXCEPTIONS
	try {
#endif
		std::map<stringx, stringx> extendId;
		extendId[stringx("__DEMO__")] = "__DEMO__";
		body = REGEX_REPLACE_ALL(body.get_Data(), std::regex("(?:\\r\\n)"), this->repStr.get_Data());//Make Single Line
		stringx sk;
		stringx brgx;
		stringx p;
		std::map<int, stringx>::iterator matchIt;
		stringx first;//!TODO
		std::map<int, stringx> match;
		for (std::list<stringx>::iterator s = ml.begin(); s != ml.end(); ++s) {
			sk = *s;
			if (sk->empty())continue;

			if (REGEX_IS_MATCH(body.get_Data(), std::regex(sk.get_Data())) == 0) {
				mainBody.dispose(); body.dispose();
				this->hasError = true;
				this->errorMsg = ("Parser::overlapTemplate=>Invalid Placeholder defined " + sk.get_Data() + " :( ");
				return stringx("INVALID");
			}
			if (extendId.find(REGEX_MATCH_STR(sk.get_Data(), this->regx)) != extendId.end()) {
				continue;
			}
			brgx = sk + "(.*)<(\\/)?" + this->key + ">";
			brgx = STR_TRIM(brgx.get_Data());
			p = REGEX_MATCH_STR(body.get_Data(), std::regex(brgx.get_Data()));
			if (p == "INVALID" || p == "ERROR") {
				this->hasError = true;
				this->errorMsg = ("Parser::overlapTemplate=>Invalid Placeholder defined " + sk.get_Data() + " :( ");
				std::cout << this->errorMsg.get_Data() << "\r\n";
				return std::string("INVALID");
			}
			if (p->empty())continue;
			p = STR_TRIM(p.get_Data());

			match = STR_MAP_SPLIT(p.get_Data(), this->endTagRegx);
			first = "";
			if (!match.empty()) {

				matchIt = match.find(0);

				if (matchIt != match.end()) {
					first = matchIt->second;
				}
				if (first == "" || first->empty()) {
					mainBody = REGEX_REPLACE_ALL(mainBody.get_Data(), std::regex(sk.get_Data()), p.get_Data());
					continue;
				}
			}
			if (first == "") {
				p = STR_SPLIT(first.get_Data(), this->endTag.get_Data());
				continue;
			}
			if (REGEX_MATCH_STR(first.get_Data(), std::regex(this->regx)) == "INVALID") {
				p = STR_SPLIT(first.get_Data(), this->endTag.get_Data());
			}
			else {
				extendId = this->extendPlaceholder(extendId, match);
			}
			p = REGEX_REPLACE_ALL(p.get_Data(), this->repStrRegex, "\r\n");//Back to Multiple Line
			mainBody = REGEX_REPLACE_ALL(mainBody.get_Data(), std::regex(sk.get_Data()), p.get_Data());
		}
		sk.dispose(); brgx.dispose(); p.dispose(); first.dispose();
		ml.clear(); body.dispose(); extendId.clear();
		return mainBody;
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		this->errorMsg = e.what();
		mainBody.dispose();
		this->hasError = true;
		return "";
	}
#endif//!_HAS_EXCEPTIONS
}
inline void
TemplatParser::margeTemplate() {
#if _HAS_EXCEPTIONS
	try {
#endif
		int count = 0;
		for (auto iter = this->_templdct.rbegin(); iter != this->_templdct.rend(); ++iter) {
			if (count == 0) {
				this->outStr = iter->second;
				count++;
				continue;
			}
			std::list<stringx> ml = this->getMatchList(iter->second, this->regx);
			if (ml.empty())continue;
			if (ml.size() <= 0) {
				continue;
			}
			this->outStr = this->overlapTemplate(this->outStr, ml, STR_TRIM(iter->second.get_Data()));
			if (this->hasError) {
				this->outStr.dispose();
				break;
			}
		}
		this->_templdct.clear();
		std::map<int, stringx>().swap(this->_templdct);
		this->outStr = REGEX_REPLACE_ALL(this->outStr.get_Data(), this->endTagRegx, "");
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		this->errorMsg = e.what();
		this->outStr.dispose();
		this->hasError = true;
	}
#endif//!_HAS_EXCEPTIONS
	return;
}
inline void
TemplatParser::implimantAttachment() {
	std::list<stringx> rml = this->getMatchList(this->outStr, this->attachRegx);
	if (rml.size() > 0) {
		const char* result;
		for (std::list<stringx>::iterator s = rml.begin(); s != rml.end(); ++s) {
			stringx path = REGEX_REPLACE_ALL((*s).get_Data(), std::regex("#attach "), "");
			stringx relativePath = this->root_dir + "\\" + path;
			const char* dir = relativePath->c_str();
			result = this->ReadFile(dir);
			if (result == "INVALID") {
				this->hasError = true;
				this->errorMsg = "Unable to found this attachment=> " + path.get_Data() + " & realative full-path =>" + dir + ".";
				return;
			}
			this->outStr = REGEX_REPLACE_ALL(this->outStr.get_Data(), std::regex("#attach \\" + path.get_Data()), std::string(result));
		}
		delete result;
	}
	return;
}
inline stringx
TemplatParser::nonImplementPlaceholder() {
	std::list<stringx> rml = this->getMatchList(this->outStr, this->regx);
	stringx requiredPlaceholder("__NOP__");
	if (rml.size() > 0) {
		requiredPlaceholder = "This following Placeholder were missing to implimant in child:\r\n";
		for (std::list<stringx>::iterator s = rml.begin(); s != rml.end(); ++s) {
			requiredPlaceholder.append(*s + "\r\n");
		}
		std::cout << requiredPlaceholder.get_Data() << "\r\n";
	};
	std::list<stringx>().swap(rml);
	//return requiredPlaceholder;
	return "__NOP__";
}
inline TemplateResult*
TemplatParser::Start() {
	TemplateResult* tr = new TemplateResult();
	tr->is_error = false;
#if _HAS_EXCEPTIONS
	try {
#endif
#if TEST_PERFORMANCE //Check PERFORMANCE
		auto startApps = std::chrono::system_clock::now();
#endif //!TEST_PERPORMANCE
#if READ_PERFORMANCE
		auto bstart = std::chrono::system_clock::now();
		std::string results = this->ReadS(this->view_path);
		auto bend = std::chrono::system_clock::now();
		std::chrono::duration<double> belapsed_seconds = bend - bstart;
		std::cout << "this->ReadS=>" << belapsed_seconds.count() << "\r\n";
		auto astart = std::chrono::system_clock::now();
#endif /* defined(READ_PERFORMANCE) */
		char* result;
		result = this->ReadFile(this->view_path);
#if READ_PERFORMANCE
		auto aend = std::chrono::system_clock::now();
		std::chrono::duration<double> aelapsed_seconds = aend - astart;
		std::cout << "this->ReadFile=>" << aelapsed_seconds.count() << "\r\n";
#endif /* defined(READ_PERFORMANCE) */
		if (result == "INVALID") {
			tr->err_msg = "NOT FOUND!!!";
			tr->is_error = true;
			return tr;
		}
		stringx& str = static_cast<stringx>(result);
		delete result;
		TemplateResult* rr = this->Read(str, this->view_path, this->root_dir);
		if (rr->is_error) {
			tr->is_error = true;
			tr->err_msg = rr->err_msg;
			return tr;
		}
		delete tr;
		this->margeTemplate();
		if (this->hasError) {
			tr->is_error = true;
			tr->err_msg = this->errorMsg;
			return tr;
		}
		stringx hasError = this->nonImplementPlaceholder();
		if (hasError != "__NOP__") {
			this->outStr.dispose();
			tr->is_error = true;
			tr->err_msg = hasError;
			return tr;
		}
		this->implimantAttachment();
		if (this->hasError) {
			tr->is_error = true;
			tr->err_msg = this->errorMsg;
			return tr;
		}
		if (REGEX_IS_MATCH(this->outStr.get_Data(), this->regxSts) == 0) {
			if (REGEX_IS_MATCH(this->outStr.get_Data(), this->regxRts) == 0) {
				tr->t_source = this->outStr;
				tr->is_script = false;
				return tr;
			}
		}
		tr->is_script = true;
		this->isScriptTemplate = true;
#if TEST_PERFORMANCE
		std::string res = this->ParseScript(this->outStr);
		this->outStr.clear();
		auto endApps = std::chrono::system_clock::now();
		std::chrono::duration<double> appselapsed_seconds = endApps - startApps;
		std::cout << "Taking Time=>" << appselapsed_seconds.count() << "\r\n";
		return res;
#endif //!TEST_PERFORMANCE
		tr->t_source = this->ParseScript(this->outStr);
		this->outStr.dispose();
		return tr;
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		tr->is_error = true;
		tr->err_msg = std::string(e.what());
		return tr;
	}
#endif
}
/***
* [TEMPLATE PARSER IMPLEMENT]
***/
#pragma endregion