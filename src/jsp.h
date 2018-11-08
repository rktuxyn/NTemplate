/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//3:04 AM 11/9/2018
#pragma once
#ifndef JSP_H_
#define JSP_H_
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
#ifndef TR_H_
#include "tr.h"
#endif
namespace NTemplate {
#pragma region JAVASCRIPTPARSER
	class JavaScriptParser : public TemplateReader {
	public:
		struct Tag {
			std::string
				sts/*sts*/ = "<js::", //= "<%",// = "<%",
				ste/*ste*/ = "::js>",//= "%>",// = "%>",
				rts/*rts*/ = "<js=",//= "{=",// = "<=",
				rte/*rte*/ = "=js>"//= "=>"// = "=>"
				;
		};
		~JavaScriptParser();
		void ParseScript();

	private:
		std::regex regxSts;
		std::regex regxSte;
		std::regex regxRts;
		std::regex regxRte;
		std::string repStr;
		std::regex repStrRegex;
		std::string resultKey;
		enum ETag {
			sts,
			ste,
			rts,
			rte
		};
		struct Result {
			std::string line, tag, startTageName;
			bool isTagStart, isTagEnd, isLastTag;
			ETag etag;
		};
		void startTag(std::regex regx, Result* info);
		void endTag(std::regex regx, Result* info);
	public:
		JavaScriptParser();
		friend class TemplateParser;
	};
#pragma endregion
}
#endif  // JSP_H_