/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "narg.h"
using namespace NTemplate;
void NTemplate::GetSettings(v8::Isolate * isolate, const v8::FunctionCallbackInfo<v8::Value>& args, Settings * settingsObj, bool isAsyncReq) {
#if _HAS_EXCEPTIONS
	try {
#endif	
		settingsObj->isAsync = false;
		v8::Local<v8::Value> cb;
		if (!args[0]->IsObject()) {
			settingsObj->isObject = false;
			if (!args[0]->IsString()) {
				settingsObj->isError = true;
				settingsObj->errorMsg = "String Required!!! (APP DIRECTORY PATH) :(";
				return;
			}
			if (!args[1]->IsString()) {
				settingsObj->isError = true;
				settingsObj->errorMsg = "String Required!!! (APP Path PATH) :(";
				return; //return settingsObj;
			}
			v8::String::Utf8Value pp(isolate, args[1]);
			v8::String::Utf8Value dr(isolate, args[0]);
			settingsObj->pagePath = T_CHAR(pp);
			settingsObj->dir = T_CHAR(dr);
			if (!args[2]->IsUndefined() && args[3]->IsNull()) {
				settingsObj->data = args[2];
			}
			else {
				settingsObj->data = v8::Local<v8::Value>::New(isolate, v8::Null(isolate));
			}

			if (!args[3]->IsUndefined() && args[3]->IsBoolean()) {
				settingsObj->isAsync = v8::Local<v8::Boolean>::Cast(args[3])->IsTrue();
			}
			else {
				settingsObj->isAsync = false;
			}
			if (!args[4]->IsUndefined() && args[4]->IsBoolean()) {
				settingsObj->removeNewLine = v8::Local<v8::Boolean>::Cast(args[4])->IsTrue();
			}
			else {
				settingsObj->isAsync = false;
			}
			if (!args[5]->IsUndefined() && args[5]->IsBoolean()) {
				settingsObj->isStrict = v8::Local<v8::Boolean>::Cast(args[5])->IsTrue();
			}
			else {
				settingsObj->isAsync = false;
			}
			if (!args[5]->IsUndefined() && args[5]->IsFunction()) {
				cb = args[5];
			}
			else {
				cb = v8::Local<v8::Value>::New(isolate, v8::Null(isolate));
			}
		}
		else {
			if (!args[0]->IsObject()) {
				settingsObj->isError = true;
				settingsObj->errorMsg = "Setting Object Required!!! :(";
				return;
			}
			settingsObj->isObject = true;
			v8::Local<v8::Value> val;
			v8::Handle<v8::Object> settings = v8::Handle<v8::Object>::Cast(args[0]);
			val = settings->Get(v8::String::NewFromUtf8(isolate, "page_path"));
			if (val->IsNullOrUndefined() || !val->IsString()) {
				settingsObj->isError = true;
				settingsObj->errorMsg = "String Required!!! (PAGE_PATH) NO POPERTY :(";
				return;
			}
			settingsObj->pagePath = T_CHAR(v8::String::Utf8Value(isolate, val));
			val.Clear();
			if (settingsObj->pagePath.empty()) {
				settingsObj->isError = true;
				settingsObj->errorMsg = "String Required!!! (PAGE_PATH):(";
				return;
			}
			val = settings->Get(v8::String::NewFromUtf8(isolate, "root_dir"));
			if (val->IsNullOrUndefined() || !val->IsString()) {
				settingsObj->isError = true;
				settingsObj->errorMsg = "String Required!!! (APP DIRECTORY PATH) NO POPERTY :(";
				return;
			}
			settingsObj->dir = T_CHAR(v8::String::Utf8Value(isolate, val));
			val.Clear();
			if (settingsObj->dir.empty()) {
				settingsObj->isError = true;
				settingsObj->errorMsg = "String Required!!! (APP DIRECTORY PATH) :(";
				return; //return settingsObj;
			}
			{
				val = settings->Get(v8::String::NewFromUtf8(isolate, "data"));
				if (val->IsNullOrUndefined() || !val->IsObject()) {
					settingsObj->data = v8::Local<v8::Value>::New(isolate, v8::Null(isolate));
				}
				else {
					settingsObj->data = val;
				}
				val.Clear();
			}
			{
				val = settings->Get(v8::String::NewFromUtf8(isolate, "isAsync"));
				if (val->IsNullOrUndefined() || !val->IsBoolean()) {
					settingsObj->isAsync = false;
				}
				else {
					v8::Local<v8::Boolean> isAsync = v8::Local<v8::Boolean>::Cast(val);
					settingsObj->isAsync = isAsync->IsTrue();
				}
				val.Clear();
			}
			{
				val = settings->Get(v8::String::NewFromUtf8(isolate, "removeNewLine"));
				if (val->IsNullOrUndefined() || !val->IsBoolean()) {
					settingsObj->removeNewLine = true;
				}
				else {
					v8::Local<v8::Boolean> removeNewLine = v8::Local<v8::Boolean>::Cast(val);
					settingsObj->removeNewLine = removeNewLine->IsTrue();
				}
				val.Clear();
			}
			{
				val = settings->Get(v8::String::NewFromUtf8(isolate, "isStrict"));
				if (val->IsNullOrUndefined() || !val->IsBoolean()) {
					settingsObj->isStrict = false;
				}
				else {
					v8::Local<v8::Boolean> isStrict = v8::Local<v8::Boolean>::Cast(val);
					settingsObj->isStrict = isStrict->IsTrue();
				}
				val.Clear();
			}
			val = settings->Get(v8::String::NewFromUtf8(isolate, "callback"));
			if (val->IsNullOrUndefined() || !val->IsFunction()) {
				cb = v8::Local<v8::Value>::New(isolate, v8::Null(isolate));
			}
			else {
				cb = val;
			}
			val.Clear();
		}
		if (cb->IsFunction()) {
			settingsObj->callback.Reset(isolate, v8::Local<v8::Function>::Cast(cb));
		}
		if (isAsyncReq || settingsObj->isAsync) {
			if (cb->IsNullOrUndefined()) {
				//settingsObj->Clear();
				settingsObj->isError = true;
				settingsObj->errorMsg = "Callback Required!!! For Completing async request :(";
				return; //return settingsObj;
			}
			if (!cb->IsFunction()) {
				//settingsObj->Clear();
				settingsObj->isError = true;
				settingsObj->errorMsg = "Invalid argument defined instead of function (Callback) !!! :(";
				return;
			}
		}
		cb.Clear();
#if _HAS_EXCEPTIONS
	} catch (std::exception& e) {
		//std:: cout << e.what() << "\n";
		settingsObj->isError = true;
		settingsObj->errorMsg = e.what();
	}
#endif
	return;
}
;
void NTemplate::Settings::Clear() {
	if (!data->IsNull()) data.Clear();
	if (!callback.IsEmpty()) callback.Reset();
	errorMsg.clear();
	dir.clear();
};
