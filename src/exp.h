//10:08 PM 11/10/2018
#ifndef N_CORE_H
#include "core.h"
#endif // !N_CORE_H
#ifdef __EXPORT
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#error Not Implemented for UNIX-style OS.
#else
extern "C" __declspec(dllexport) void ntemplate_test (char *);
extern "C" __declspec(dllexport) const char*  ntemplate_run_script (char *);
extern "C" __declspec(dllexport) const char* ntemplate_parse_template(const char*, const char*, const char*);
#endif
#endif//!__EXPORT