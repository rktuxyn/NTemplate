/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//3:04 AM 11/9/2018
#pragma once
#ifndef TR_H_
#define TR_H_
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
namespace NTemplate {
#pragma region READ_MASTER
	class TemplateReader {
	private:
		int _count;
		std::string _rootDir;
		std::string _parent;
		std::map<int, std::string> _templdct;
		std::string pattern;
		std::regex patternRegx;
		std::string attachPattern;
		std::regex attachRegx;
		TemplateResult* result;
		char* ReadFile(const char*& name);
		void Read(std::string& path);
		void DeepRead(std::string &path, std::string &data);
	public:
		TemplateReader();
		~TemplateReader();
		void ReadTemplate(std::string& rootDir);
		void Add(std::string& _data);
		friend class TemplateParser;
		friend class JavaScriptParser;
	};
#pragma endregion
}
#endif  // JSP_H_