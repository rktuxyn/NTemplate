#if defined(_MSC_VER)
#  pragma once
#endif
#ifndef NTEMPLATE_ASYNC_DEP_
#define NTEMPLATE_ASYNC_DEP_
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
#ifndef NTEMPLATE_ARG_
#include "NArguments.h"
#endif //!NTEMPLATE_ARG_
#ifndef NTEMPLATE_COMP_
#include "NCompiler.h"
#endif //!NTEMPLATE_COMP_
#ifndef _CHRONO_
#include <chrono>
#endif //!_CHRONO_
#ifndef _M_CEE ///clr or /clr:pure //_IOSFWD_
#ifndef _THREAD_
#include <thread>
#endif //!_THREAD_
#ifndef UV_H
#include <uv.h>
#endif //!UV_H
#endif //!_M_CEE
#endif//!NTEMPLATE_ASYNC_DEP_