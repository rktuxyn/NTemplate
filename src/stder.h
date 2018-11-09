/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#pragma once
#ifndef STD_EXTEND_REGEX
#define STD_EXTEND_REGEX
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
//December 01, 2017, 01:48 AM
namespace std {
	template<class BidirIt,
		class Traits,
		class CharT,
		class UnaryFunction>
		std::basic_string<CharT>
		__regex_replacematch(BidirIt first, BidirIt last,
			const std::basic_regex<CharT, Traits>& re,
			UnaryFunction f, size_t count = 0
		) {
		std::basic_string<CharT> str;
		{
			typename std::match_results<BidirIt>::difference_type
				positionOfLastMatch = 0;
			auto endOfLastMatch = first;
			size_t increment = 0;
			std::sregex_iterator begin(first, last, re), end;
			std::all_of(begin, end, [&](const _STD match_results<BidirIt>& match) {
				auto positionOfThisMatch = match.position(0);
				auto diff = positionOfThisMatch - positionOfLastMatch;
				auto startOfThisMatch = endOfLastMatch;
				std::advance(startOfThisMatch, diff);
				str.append(endOfLastMatch, startOfThisMatch);
				str.append(f(match));
				auto lengthOfMatch = match.length(0);
				positionOfLastMatch = positionOfThisMatch + lengthOfMatch;
				endOfLastMatch = startOfThisMatch;
				if (count != 0) {
					increment++;
					if (increment >= count) {
						return false;
					}
				}
				std::advance(endOfLastMatch, lengthOfMatch);
				return true;
			});
			str.append(endOfLastMatch, last);
		}
		return str;
	}
	template<class BidirIt,
		class Traits,
		class CharT,
		class UnaryFunction>
		std::basic_string<CharT>
		__regex_core(BidirIt first, BidirIt last,
			const std::basic_regex<CharT, Traits>& re,
			UnaryFunction f, size_t count = 0
		) {
		std::basic_string<CharT> str;
		{
			std::sregex_iterator next(first, last, re);
			std::sregex_iterator end;
			size_t increment = 0;
			while (next != end) {
				std::smatch match = *next;
				str.append(f(match));
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
		std::basic_string<CharT>
		regex__replacematch(const std::basic_string<CharT>& s,
			const std::basic_regex<CharT, Traits>& re,
			UnaryFunction f, size_t count = 0
		) {
		return __regex_replacematch(s.cbegin(), s.cend(), re, f, count);
	}
#define REGEX_REPLACE_MATCH regex__replacematch
	template<class Traits,
		class CharT>
		std::basic_string<CharT>
		regex__matchstr(const std::basic_string<CharT>& str,
			const std::basic_regex<CharT, Traits>& re
		) {
		//auto& tmp = std::basic_string<CharT>("INVALID");
		std::basic_string<CharT> tmp;
		__regex_replacematch(str.cbegin(), str.cend(), re, [&tmp](const std::smatch& m) {
			tmp = m.str(1);
			return std::basic_string<CharT>("___NOP___");
		}, 1);
		return tmp;
	}
#define REGEX_MATCH_STR regex__matchstr
	template<class Traits,
		class CharT>
		void
		regex__matchlist(std::list<std::string>&ml, const std::basic_string<CharT>& str,
			const std::basic_regex<CharT, Traits>& re
		) {
			{
				if (str.empty())return;
				std::sregex_iterator next(str.begin(), str.end(), re);
				std::sregex_iterator end;
				while (next != end) {
					std::smatch match = *next;
					ml.push_back(match.str());
					next++;
				}
			}
			return;
	};
#define REGEX_MATCH_LIST regex__matchlist
	/**============================================================================*/
	template<class Traits,
		class CharT, class BidirIt>
		void
		regex__match_list(std::list<std::string>& result, BidirIt irFirst, BidirIt irLast,
			const std::basic_regex<CharT, Traits>& re
		) {
			{
				std::sregex_iterator next(irFirst, irLast, re);
				std::sregex_iterator end;
				while (next != end) {
					std::smatch match = *next;
					result.push_back(match.str());
					next++;
				}
			}
	};
#define REGEX_MATCH_LIST_N regex__match_list
	/*=============================================================================*/
	template<class Traits,
		class CharT>
		std::basic_string<CharT>
		regex__matchstrreplaceall(const std::basic_string<CharT>& str,
			const std::basic_regex<CharT, Traits>& re, std::string repstr
		) {
		return std::regex_replace(str, re, repstr);
	}
#define REGEX_REPLACE_ALL regex__matchstrreplaceall
	/*================================================*/
	template<class Traits,
		class CharT, class DataRefStr>
		std::basic_string<CharT>
		regex__match_str_replace_all(DataRefStr& str,
			const std::basic_regex<CharT, Traits>& re, _STD string repstr
		) {
		return std::regex_replace(str, re, repstr);
	}
#define REGEX_REPLACE_ALL_N regex__match_str_replace_all
	/*=================================================*/
	template<class BidirIt,
		class Traits, class CharT>
		size_t
		__regex_ismatch(BidirIt first, BidirIt last,
			const std::basic_regex<CharT, Traits>& re
		) {
		size_t increment = 0;
		std::sregex_iterator begin(first, last, re), end;
		std::all_of(begin, end, [&](const std::match_results<BidirIt>& match) {
			increment++;
			return false;
		});
		return increment;
	}
	template<class Traits,
		class CharT>
		size_t
		regex__ismatch(const std::basic_string<CharT>& str,
			const std::basic_regex<CharT, Traits>& re
		) {
		size_t count = 0;
		{
			//if (str == NULL)return 0;
			if (str.empty())return count;
			//return __regex_ismatch(str.cbegin(), str.cend(), re);
			std::sregex_iterator next(str.begin(), str.end(), re);
			//_STD sregex_iterator end;
			std::sregex_iterator end;

			while (next != end) {
				count++;
				break;
			}
			//if (next->length() > 0)return 1;
		}
		return count;

	}
#define REGEX_IS_MATCH regex__ismatch
	/*=================================================*/
	template<class Traits,
		class CharT, class BidirIt>
		size_t
		regex__is_match(BidirIt irFirst, BidirIt irLast,
			const std::basic_regex<CharT, Traits>& re
		) {
		size_t count = 0;
		{
			std::sregex_iterator next(irFirst, irLast, re);
			std::sregex_iterator end;
			while (next != end) {
				count++;
				break;
			}
		}
		return count;

	}
#define REGEX_IS_MATCH_N regex__is_match
	/*=================================================*/
	template<class CharT>
	std::string
		_trim(std::basic_string<CharT>& str) {
		if (str.empty())return str;
		size_t first = str.find_first_not_of(' ');
		size_t last = str.find_last_not_of(' ');
		return std::string(str.substr(first, (last - first + 1)));
	}
#define STR_TRIM _trim
	/**=================================================*/
	template<class Traits,
		class CharT>
		void
		_map_split(std::map<int, std::string>&result, std::basic_string<CharT>& str,
			const std::basic_regex<CharT, Traits>& rgx
		) {
		{
			if (str.empty())return;
			int count = 0;
			std::sregex_token_iterator iter(str.begin(), str.end(), rgx, -1);
			for (std::sregex_token_iterator end; iter != end; ++iter) {
				result[count] = iter->str();
				count++;
			}
		}
		return;
	};
#define STR_MAP_SPLIT _map_split
	/**=====================================================*/
	template<class Traits,
		class CharT, class BidirIt>
		std::map<int, std::string>
		_map_split_n(BidirIt irFirst, BidirIt irLast,
			const std::basic_regex<CharT, Traits>& rgx
		) {
		std::map<int, std::string> result;
		{
			int count = 0;
			std::sregex_token_iterator iter(irFirst, irLast, rgx, -1);
			for (std::sregex_token_iterator end; iter != end; ++iter) {
				result[count] = iter->str();
				count++;
			}
		}
		return result;
	}
#define STR_MAP_SPLIT_N _map_split_n
	/**=====================================================*/
	template<class CharT>
	std::basic_string<CharT>
		_split(std::basic_string<CharT> str,
			const std::basic_string<CharT>& delimiter
		) {
		if (str.empty())return str;

		return str.substr(0, str.find(delimiter));
	}
#define STR_SPLIT _split
	/**======================================================*/
	template<class DataRefStr>
	std::string
		_split_n(DataRefStr str,
			const std::string& delimiter
		) {
		return str->substr(0, str->find(delimiter));
	}
#define STR_SPLIT_N _split_n
	/**======================================================*/
} // namespace std
#endif// !STD_EXTEND_REGX
