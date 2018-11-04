#if defined(_MSC_VER)
#  pragma once
#endif
#ifndef NTEMPLATE_R_
#define NTEMPLATE_R_
#ifndef NTEMPLATE_DEP_
#include "dependancy.h"
#endif //!NTEMPLATE_DEP_
/**[Native Template Reader]*/

_NT_BEGIN
	template<class _STR/*string*/, class _RE/*regx*/>
	class Info {
	private:
		int					_count;
	public:
		/**[Total template dectionary!!!]*/
		_NDK DataRef<_STD map<int, _STR>, int, _STR> _templdct;
		_STR*				_root_dir;
		_STR*				_pattern;
		_STR*				_parent;
		_RE					_patternRegx;
		_STR				_attachPattern;
		_RE					_attachRegx;
		Info(_STR* rootDir, _STR* parent){
			_templdct = _NDK DataRef<_STD map<int, _STR>, int, _STR>();
			//_templdct				= _STD map<int, _STD string>();
			_pattern = new _STR("#include (.*)");
			_patternRegx = _RE(_pattern->c_str());
			_count = 0;
			_root_dir = rootDir;
			_parent = parent;
			_attachPattern = _STR("#attach (.*)");
			_attachRegx = _STR(_attachPattern);
		};
		void dct_add(_STR _data) {
			_templdct[_count] = _data;
			_count++;
			return;
		};
		/**_STD map<int, _STD string> get_dct() {
		//6704
		return _templdct;
		}*/
		/*_NDK DataRef<_STD map<int, _STD string>, int, _STD string> get_dct() {
		//6704
		return _templdct;
		}*/
		void clear_dct() {
			_count = 0;
			_templdct.dispose();
			return;
		};
		~Info() {
			delete _root_dir;
			delete _pattern;
			delete _parent;
			if (!_templdct.IsNull()) {
				_templdct.dispose();
			}
		};
	};
	template<class _STR/*string*/, class _Char/*char*/, class _RE/*regx*/> class Reader {
	private:
	public:
		Reader() {};
		~Reader() {};
		template<class _Result/*Result*/>
		void ReadFile(
			const _Char*& path, _Result* resp) {
				{
#if _HAS_EXCEPTIONS
					_Try
#endif //!_HAS_EXCEPTIONS
						_Char* chars = "INVALID";
						FILE*stream;
						errno_t err;
						err = fopen_s(&stream, path, "rb");
						if (stream == NULL) {
							resp->error_msg("File Not found!!!" + _STD string(path));
							resp->is_error(true);
							return;
						};
						if (err != 0) {
							resp->error_msg("File Not found!!!" + _STD string(path));
							resp->is_error(true);
							return;
						};
						fseek(stream, 0, SEEK_END);
						size_t size = ftell(stream);
						rewind(stream);
						chars = new char[size + 1];
						chars[size] = '\0';
						for (size_t i = 0; i < size;) {
							i += fread(&chars[i], 1, size - i, stream);
							if (ferror(stream)) {
								fclose(stream);
								resp->error_msg("Error occured while reading file !!!" + _STD string(path));
								resp->is_error(true);
								return;
							}
						}
						fclose(stream);
						stream = NULL;
						resp->source(chars);
						delete chars;
	#if _HAS_EXCEPTIONS
						_Catch
							resp->error_msg(e.what());
						resp->is_error(true);
						return;
					_TrE
#endif //!_HAS_EXCEPTIONS

				}
				return;
		}
		template<class _Result/*Result*/, class _RinF>
		void Read(
			_STR* path, _RinF* inf, _Result* rsinf) {
				{
#if _HAS_EXCEPTIONS
					_Try
#endif //!_HAS_EXCEPTIONS
						_STR* relativePath = new _STR(inf->_root_dir->c_str());
						malloc(sizeof(relativePath));
						relativePath->append("\\");
						relativePath->append(path->c_str());
						const _Char* new_template_path = relativePath->c_str();
						free(relativePath);
						ReadFile(new_template_path, rsinf);
						delete new_template_path;
						if (rsinf->is_error() == true) {
							return;
						}
	
						_STR str(rsinf->source()->c_str());
						rsinf->source("");
						inf->dct_add(_STR(REGEX_REPLACE_ALL(str, inf->_patternRegx, "")));
						_STR nextTemplate = REGEX_MATCH_STR(str, inf->_patternRegx);
						_NT _Destroy(&str);
						if (nextTemplate == "INVALID") {
							return;
						}
						inf->_parent = path;
						Read(new _STR(nextTemplate.c_str()), inf, rsinf);
#if _HAS_EXCEPTIONS
					_Catch
						rsinf->is_error(true);
						rsinf->source("");
						rsinf->error_msg(e.what());
					_TrE
#endif //!_HAS_EXCEPTIONS
				}
				return;
		};
		template<class _Result/*Result*/, class _RinF>
		void Read(
			_RinF* inf, _STR rootTemplate, _Result* rsinf) {
				{
#if _HAS_EXCEPTIONS
					_Try
#endif //!_HAS_EXCEPTIONS
						_STR nextTemplate = REGEX_MATCH_STR(rootTemplate, inf->_patternRegx);
						if (nextTemplate == "INVALID") {
							inf->dct_add(rootTemplate);
							_NT _Destroy(&rootTemplate);
							return;
						}
						inf->dct_add(REGEX_REPLACE_ALL(rootTemplate, inf->_patternRegx, ""));
						_NT _Destroy(&rootTemplate);
						Read(new _STR(nextTemplate.c_str()), inf, rsinf);
						_NT _Destroy(&nextTemplate);
#if _HAS_EXCEPTIONS
					_Catch
						rsinf->is_error(true);
						rsinf->source("");
						rsinf->error_msg(e.what());
					_TrE
						return;
#endif //!_HAS_EXCEPTIONS
				}
		};
		template<class _Result/*Result*/, class _RinF>
		void ImplimantAttachment(
			_RinF* rinf, _Result* rsinf) {
				{
					_NDK DataRef<_STD list<_STR>, _STR, _STR> rml = REGEX_MATCH_LIST(_STR(rsinf->source()->c_str()), rinf->_attachRegx);
					//_STD list<_STR> rml = REGEX_MATCH_LIST(_STR(rsinf->source()->c_str()), rinf->_attachRegx);
					if (rml->size() <= 0) {
						rml.dispose();
						return;
					}
					_STR outStr = _STR(rsinf->source()->c_str());
					rsinf->source("");
					_STR rootDir = _STR(rinf->_root_dir->c_str());
					_RE re("#attach ");
					for (_STD list<_STR>::iterator s = rml->begin(); s != rml->end(); ++s) {
						_STR path = REGEX_REPLACE_ALL(*s, re, "");
						_STR relativePath = rootDir + "\\" + path;
						const char* dir = relativePath.c_str();
						_NT _Destroy(&relativePath);
						ReadFile(dir, rsinf); delete dir;
						if (rsinf->is_error() == true) {
							rsinf->error_msg("Unable to found this attachment=> " + path + " & realative full-path =>" + dir + ".");
							_NT _Destroy(&outStr);
							return;
						}
						outStr = REGEX_REPLACE_ALL(outStr, _RE("#attach \\" + path), _STR(rsinf->source()->c_str()));
						rsinf->source("");
					}
					rml.dispose();
					rsinf->source(REGEX_REPLACE_ALL(outStr, rinf->_attachRegx, ""));
					_STD allocator<_STR> alloc;
					alloc.destroy(&outStr);
					alloc.destroy(&rootDir);
				}
				return;
		}
	};
_NT_END//!NTemplate
#endif //!NTEMPLATE_S_
