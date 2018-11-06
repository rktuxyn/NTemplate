//1:11 AM 11/4/2018
#pragma once
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
		char* ReadFile(const char*& name);
		void Read(stringx path);
		void DeepRead(stringx &path, stringx &data);
	public:
		TemplateReader();
		~TemplateReader();
		TemplateResult* Read(stringx rootTemplate, const char* rootPath, stringx rootDir);
		void Add(std::string _data);
		friend class TemplateParser;
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
		~JavaScriptParser();
		stringx ParseScript(stringx templateStr);

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
		void startTag(std::regex regx, Result* info);
		void endTag(std::regex regx, Result* info);
	public:
		JavaScriptParser();
		friend class TemplateParser;
	};
#pragma endregion
#pragma region TemplateParser
	class TemplateParser : public JavaScriptParser,
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
		TemplateParser(const char* v, stringx& dir);
		 ~TemplateParser();
		void Start(TemplateResult* tr);
		void getSetTemplate(stringx str, v8::Isolate* isolate);
		void margeTemplate();
		void extendPlaceholder(std::map<std::string, stringx>&extendId, std::map<int, stringx>&match);
		stringx overlapTemplate(stringx& mainBody, std::list<stringx> ml, stringx& body);
		stringx nonImplementPlaceholder();
		void implimantAttachment();
		std::list<stringx> getMatchList(stringx str, std::regex rgx);
		stringx _overlapTemplate(stringx& mainBody, std::list<stringx> ml, stringx& body);
		//friend class JavaScriptParser;
		//friend class TemplateReader;
	};
#pragma endregion
}
#endif  // NTEMPLATE_H_