/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//3:04 AM 11/9/2018
#pragma once
#ifndef TP_H_
#define TP_H_
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
#ifndef JSP_H_
#include "jsp.h"
#endif
namespace NTemplate {
#pragma region TemplateParser
	class TemplateParser : public JavaScriptParser {
	private:
		const char* view_path;
		std::string root_dir;
		std::regex regx;
		std::regex endTagRegx;
		std::string key;
		std::string endTag;
	public:
		TemplateParser(TemplateResult* tr, const char* v, std::string& dir);
		~TemplateParser();
		void Start();
		void getSetTemplate(std::string str, v8::Isolate* isolate);
		void margeTemplate();
		void extendPlaceholder(std::map<std::string, std::string>&extendId, std::map<int, std::string>&match);
		void overlapTemplate(std::list<std::string> ml, std::string& body);
		void nonImplementPlaceholder();
		void implimantAttachment();
		void getMatchList(std::list<std::string>&ml, std::string& str, std::regex rgx);
	};
#pragma endregion
}
#endif  // TP_H_
