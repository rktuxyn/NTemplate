//1:11 AM 11/4/2018
#if defined(_MSC_VER)
#  pragma once
#endif
#ifndef NTEMPLATE_H_
#define NTEMPLATE_H_
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
namespace NTemplate {
#pragma region READ_MASTER
	class TemplateReader {
	private:
		int _count;
		stringx _rootDir;
		stringx _parent;
		std::map<int, stringx> _templdct;
		stringx pattern;
		std::regex patternRegx;
		stringx attachPattern;
		std::regex attachRegx;
		TemplateResult* reader_result;
		inline char* ReadFile(const char*& name);
		inline void Read(stringx path);
		inline void DeepRead(stringx &path, stringx &data);
	public:
		inline TemplateReader();
		inline ~TemplateReader();
		inline TemplateResult* Read(stringx rootTemplate, const char* rootPath, stringx rootDir);
		inline void Add(std::string _data);
		friend class TemplatParser;
	};
#pragma endregion
#pragma region SCRIPTPARSER
	class JavaScriptParser {
	public:
		struct Tag {
			std::string
				sts/*sts*/ = "<js::", //= "<%",// = "<%",
				ste/*ste*/ = "::js>",//= "%>",// = "%>",
				rts/*rts*/ = "<js=",//= "{=",// = "<=",
				rte/*rte*/ = "=js>"//= "=>"// = "=>"
				;
		};
		inline ~JavaScriptParser();
		inline stringx ParseScript(stringx templateStr);

	private:
		std::regex regxSts;
		std::regex regxSte;
		std::regex regxRts;
		std::regex regxRte;
		stringx repStr;
		std::regex repStrRegex;
		stringx resultKey;
		enum ETag {
			sts,
			ste,
			rts,
			rte
		};
		struct Result {
			stringx line, tag, startTageName;
			bool isTagStart, isTagEnd, isLastTag;
			ETag etag;
		};
		inline void startTag(std::regex regx, Result* info);
		inline void endTag(std::regex regx, Result* info);
	public:
		inline JavaScriptParser();
		friend class TemplatParser;
	};
#pragma endregion
#pragma region TemplatParser
	class TemplatParser : public JavaScriptParser,
		public TemplateReader {
	private:
		const char* view_path;
		stringx root_dir;
		stringx outResult;
		std::regex regx;
		std::regex endTagRegx;
		stringx key;
		stringx endTag;
		stringx outStr;
	public:
		bool hasError;
		stringx errorMsg;
		bool isScriptTemplate;
		inline TemplatParser(const char* v, std::string& dir);
		inline ~TemplatParser();
		inline TemplateResult* Start();
		inline void getSetTemplate(stringx str, v8::Isolate* isolate);
		inline void margeTemplate();
		inline std::map<stringx, stringx> extendPlaceholder(std::map<stringx, stringx> extendId, std::map<int, stringx> match);
		inline stringx overlapTemplate(stringx& mainBody, std::list<stringx> ml, stringx& body);
		inline stringx nonImplementPlaceholder();
		inline void implimantAttachment();
		inline std::list<stringx> getMatchList(stringx str, std::regex rgx);
		stringx _overlapTemplate(stringx& mainBody, std::list<stringx> ml, stringx& body);
		//friend class JavaScriptParser;
		//friend class TemplateReader;
	};
#pragma endregion
}
#endif  // NTEMPLATE_H_