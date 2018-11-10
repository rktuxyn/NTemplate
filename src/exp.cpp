//10:08 PM 11/10/2018
#include "exp.h"
#ifdef __EXPORT
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
/* UNIX-style OS. ------------------------------------------- */
#error Not Implemented yet.
#else
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
};
extern "C" __declspec(dllexport) void ntemplate_test (char* source) {
	source = "SUCCESS";
	return;
};

extern "C" __declspec(dllexport) const char * ntemplate_run_script(char* source) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	return "SUCCESS";
};
extern "C" __declspec(dllexport) const char * ntemplate_parse_template(const char* pagePath, const char* dir, const char* data) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope					scope(isolate);
	TemplateResult* rsinf = new TemplateResult();
	rsinf->remove_new_line = true;
	TemplateParser* tParser = new NTemplate::TemplateParser(rsinf, pagePath, std::string(dir));
	tParser->Start();
	if (rsinf->is_error == true) {
		return rsinf->err_msg.c_str();
	}
	if (rsinf->is_script == false) {
		return rsinf->t_source.c_str();
	}
#if _WRITE_OUTPUT
	WriteFile("function ___NTEMPLATE__SCRIPT__RUNNER (data){ try{\r\n" + rsinf->t_source + "\r\n}catch(e){\r\nreturn e.message;\r\n}\r\n};");
#endif
	v8::Local<v8::Value> val = v8::String::NewFromUtf8(isolate, data);
	v8::String::Utf8Value strx(isolate, NTemplate::Compiler::RunTemplateScript(isolate, rsinf->t_source, val));
	val.Clear();
	return T_CHAR(strx);
};
#endif

#endif//!__EXPORT

