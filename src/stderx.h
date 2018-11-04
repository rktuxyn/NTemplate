#pragma once
#ifndef STD_EXTEND_REGEX_S
#define STD_EXTEND_REGEX_S
#ifndef _IOSTREAM_
#include <iostream>
#endif // !_IOSTREAM_
#ifndef _REGEX_
#include <regex>
#endif// !_REGEX_
#ifndef _CSTDLIB_
#include <cstdlib>
#endif// !_CSTDLIB_
#ifndef _XSTRING_
#include <string>
#endif // !_XSTRING_
#ifndef _LIST_
#include <list>
#endif // !_LIST_
#ifndef _MAP_
#include <map>
#endif // !_MAP_
#ifndef _STD_BEGIN
#define _STD_BEGIN	namespace std {
#endif //!_STD_BEGIN
#ifndef _STD_END
#define _STD_END		}
#endif //!_STD_END
#ifndef _STD
#define _STD	::_STD 
#endif //!_STD
//December 01, 2017, 01:48 AM
_STD_BEGIN
	template<class BidirIt,
		class Traits,
		class CharT,
		class UnaryFunction>
		_STD basic_string<CharT>* 
		__regex_replacematch_s(BidirIt first, BidirIt last,
			const _STD basic_regex<CharT, Traits>& re,
			UnaryFunction f, size_t count = 0
		) {
	_STD basic_string<CharT>* str = new _STD basic_string<CharT>();
		{
			typename _STD match_results<BidirIt>::difference_type
				positionOfLastMatch = 0;
			auto endOfLastMatch = first;
			size_t increment = 0;
			_STD sregex_iterator begin(first, last, re), end;
			_STD all_of(begin, end, [&](const _STD match_results<BidirIt>& match) {
				auto positionOfThisMatch = match.position(0);
				auto diff = positionOfThisMatch - positionOfLastMatch;
				auto startOfThisMatch = endOfLastMatch;
				_STD advance(startOfThisMatch, diff);
				str->append(endOfLastMatch, startOfThisMatch);
				str->append(f(match));
				auto lengthOfMatch = match.length(0);
				positionOfLastMatch = positionOfThisMatch + lengthOfMatch;
				endOfLastMatch = startOfThisMatch;
				if (count != 0) {
					increment++;
					if (increment >= count) {
						return false;
					}
				}
				_STD advance(endOfLastMatch, lengthOfMatch);
				return true;
			});
			str->append(endOfLastMatch, last);
		}
		return str;
	}
	template<class BidirIt,
		class Traits,
		class CharT,
		class UnaryFunction>
		_STD basic_string<CharT>*
		__regex_core(BidirIt first, BidirIt last,
			const _STD basic_regex<CharT, Traits>& re,
			UnaryFunction f, size_t count = 0
		) {
		_STD basic_string<CharT>* str = new _STD basic_string<CharT>();
		{
			_STD sregex_iterator next(first, last, re);
			_STD sregex_iterator end;
			size_t increment = 0;
			while (next != end) {
				_STD smatch match = *next;
				str->append(f(match));
				if (count != 0) {
					increment++;
					if (increment >= count) {
						break;
					}
				}
				next++;
			}
		}
		return str;
	}
	template<class Traits,
		class CharT,
		class UnaryFunction>
		_STD basic_string<CharT>*
		regex__replacematch_s(const _STD basic_string<CharT>* s,
			const _STD basic_regex<CharT, Traits>& re, 
			UnaryFunction f, size_t count = 0
		) {
		return __regex_replacematch_s(s->cbegin(), s->cend(), re, f, count);
	}
#define REGEX_REPLACE_MATCH_S regex__replacematch_s
	template<class Traits,
		class CharT>
		_STD basic_string<CharT>* 
		regex__matchstr_s(const _STD basic_string<CharT>* str,
			const _STD basic_regex<CharT, Traits>& re
		) {
		_STD string tmp = new _STD string("INVALID");
		__regex_replacematch(str->cbegin(), str->cend(), re, [&tmp](const _STD smatch& m) {
			tmp = new _STD string(m.str(1));
			return _STD string("___NOP___");
		}, 1);
		return tmp;
		/*if (str.empty())return "INVALID";
		_STD basic_string<CharT> result;
		_STD smatch match;
		if (_STD regex_search(str, match, re) && match.size() > 0) {
			result = match.str(1);
		}
		return result;*/
	}
#define REGEX_MATCH_STR_S regex__matchstr_s
	template<class Traits,
		class CharT>
		_STD list<_STD basic_string<CharT>*>* 
		regex__matchlist_s(const _STD basic_string<CharT>* str,
			const _STD basic_regex<CharT, Traits>& re
		) {
		_STD list<_STD string*>* result= new _STD list<_STD string*>();
		{
			if (str->empty())return result;
			_STD sregex_iterator next(str->begin(), str->end(), re);
			_STD sregex_iterator end;
			while (next != end) {
				_STD smatch match = *next;
				result.push_back(new _STD string(match.str()));
				next++;
			}
		}
		return result;
	}
#define REGEX_MATCH_LIST_S regex__matchlist_s
	template<class Traits,
		class CharT>
		_STD basic_string<CharT>*
		regex__matchstrreplaceall_s(const _STD basic_string<CharT>* str,
			const _STD basic_regex<CharT, Traits>& re, _STD basic_string<CharT>* repstr
		) {
		return _STD regex_replace(str, re, repstr);
	}
#define REGEX_REPLACE_ALL_S regex__matchstrreplaceall_s
	template<class BidirIt,
		class Traits,
		class CharT>
		size_t 
		__regex_ismatch(BidirIt first, BidirIt last,
			const _STD basic_regex<CharT, Traits>& re
		) {
		size_t increment = 0;
		_STD sregex_iterator begin(first, last, re), end;
		_STD all_of(begin, end, [&](const _STD match_results<BidirIt>& match) {
			increment++;
			return false;
		});
		return increment;
	}
	template<class Traits, class CharT>
	size_t 
		regex__ismatch_s(const _STD basic_string<CharT>* str,
		const _STD basic_regex<CharT, Traits>& re
		) {
		size_t count = 0;
		{
			//if (str == NULL)return 0;
			if (str->empty())return count;
			//return __regex_ismatch(str.cbegin(), str.cend(), re);
			_STD sregex_iterator next(str->begin(), str->end(), re);
			//_STD sregex_iterator end;
			_STD sregex_iterator end;

			while (next != end) {
				count++;
				break;
			}
			//if (next->length() > 0)return 1;
		}
		return count;
		
	}
#define REGEX_IS_MATCH_S regex__ismatch_s
	template<class CharT>
	_STD basic_string<CharT>* 
		_trim_s(_STD basic_string<CharT>* str) {
		if (str->empty())return str;
		size_t first = str->find_first_not_of(' ');
		size_t last = str->find_last_not_of(' ');
		return str->dsubstr(first, (last - first + 1));
	}
#define STR_TRIM_S _trim_s
	template<class Traits,
		class CharT>
	_STD map<int, _STD basic_string<CharT>*> 
		_map_split_s(_STD basic_string<CharT>* str,
			const _STD basic_regex<CharT, Traits>& rgx
		) {
		_STD map<int, _STD string*> result;
		{
			if (str->empty())return result;
			int count = 0;
			_STD sregex_token_iterator iter(str->begin(), str->end(), rgx, -1);
			for (_STD sregex_token_iterator end; iter != end; ++iter) {
				result[count] = iter->str();
				count++;
			}
		}
		return result;
	}
#define STR_MAP_SPLIT_S _map_split_s
	template<class CharT>
	_STD basic_string<CharT>* 
		_split_s(_STD basic_string<CharT>*str,
		const _STD basic_string<CharT>* delimiter
	) {
		if (str->empty())return str;

		return str->substr(0, str->find(delimiter));
	}
#define STR_SPLIT_S _split_s
_STD_END // namespace std
#endif// !STD_EXTEND_REGX_S