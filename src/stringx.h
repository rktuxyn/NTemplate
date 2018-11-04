/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//11:17 PM 10/30/2018
#if defined(_MSC_VER)
#  pragma once
#endif
#ifndef _XSTRING_
#include <string>
#endif // !_XSTRING_
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#define _Stringx

template <class Type> class Stringx;
template <class Type> class Stringx {
private:
	bool _isDisposed;
	Type *pData;
public:
	Stringx(const Type &t) {
		pData = new Type(t);
		malloc(sizeof(pData));
		_isDisposed = false;
	};
	Stringx(Type *p) {
		if (p) { pData = new Type(p); malloc(sizeof(pData)); }
		else { pData = NULL; }
	};
	Stringx() { pData = new Type(); malloc(sizeof(pData)); };
	Type& Get() {
		return *pData;
	};
	~Stringx() {
		this->dispose();
	}
	void dispose() {
		if (_isDisposed)return;
		_isDisposed = true;
		delete pData;
	};
	Stringx &append(const char& _Off) {
		this->pData->append(_Off);
		return (*this);
	};
	Stringx &append(const Type& _Off) {
		this->pData->append(_Off);
		return (*this);
	};
	Stringx &append(const Stringx& _Off) {
		this->pData->append(_Off.pData->c_str());
		return (*this);
	};
	Stringx &operator=(Type *p) {
		if (p == NULL) {
			if (!_isDisposed)
				dispose();
			return (*this);
		}
		if (!_isDisposed) {
			dispose();
		}
		pData = p;
		_isDisposed = false;
		return (*this);
	};
	Stringx& concat(const char& _Off) {
		this->pData->append(_Off);
		return (*this);
	};
	Stringx& concat(const Type& _Off) {
		this->pData->append(_Off);
		return (*this);
	};
	Stringx& concat(const Stringx& _Off) {
		this->pData->append(_Off.pData->c_str());
		return (*this);
	};
	Type& operator=(const char& _Off) {
		if (!_isDisposed)
			dispose();
		pData = new Type(_Off);
		malloc(sizeof(pData));
		_isDisposed = false;
		return *pData;
	};
	Type& operator=(const Type& _Off) {
		if (!_isDisposed)
			dispose();
		pData = new Type(_Off);
		malloc(sizeof(pData));
		_isDisposed = false;
		return *pData;
	};
	Stringx& operator+=(const Stringx& _Off) {
		if (_isDisposed) {
			pData = new Type();
			_isDisposed = false;
		}
		pData->append(_Off.pData->c_str());
		return (*this);
	};
	Stringx& operator+(const Stringx& _Off) {
		if (_isDisposed) {
			pData = new Type();
			_isDisposed = false;
		}
		pData->append(_Off.pData->c_str());
		return (*this);
	};
	Stringx& operator+(const Type& _Off) {
		if (_isDisposed) {
			pData = new Type();
			_isDisposed = false;
		}
		pData->append(_Off);
		return (*this);
	};
	Stringx& operator+(const char& _Off) {
		if (_isDisposed) {
			pData = new Type();
			_isDisposed = false;
		}
		pData->append(_Off);
		return (*this);
	};
	bool IsNull() { return pData == NULL; };

	Type& get_Data() {
		return *pData;
	};

	Type* operator->()const {
		if (_isDisposed) throw "Object Disposed...";
		return pData;
	};

	Type& operator+=(Type &_Off) {
		if (_isDisposed) {
			pData = new Type();
			_isDisposed = false;
		}
		pData->operator+=(_Off.pData->c_str());
		return (*this);
	};

	Type& operator+(const char _Off)const {
		if (_isDisposed) {
			pData = new Type();
			_isDisposed = false;
		}
		return pData->operator+(_Off);
	};

	Type& operator+=(const char _Off) {
		if (_isDisposed) {
			pData = new Type();
			_isDisposed = false;
		}
		return pData->operator+=(_Off);
	};

	Type& operator+=(Type _Off) {
		if (_isDisposed) {
			pData = new Type();
			_isDisposed = false;
		}
		return pData->operator+=(_Off);
	};

	inline bool operator==(const Type &_Right)const {
		// test for iterator equality
		if (_isDisposed) throw "Object Disposed...";
		return pData->_Equal(_Right);
	};

	inline bool operator!=(const Type &_Right)const {
		if (_isDisposed) throw "Object Disposed...";
		return (!(pData->compare(_Right)));
	};

	inline bool operator<(const Type& _Right) const {	// test if this < _Right
		if (_isDisposed) throw "Object Disposed...";
		return pData->compare(_Right) < 0;
	};

	inline bool operator>(const Type& _Right) const {	// test if this > _Right
		if (_isDisposed) throw "Object Disposed...";
		return pData->operator>(_Right) == true;
	};

	inline bool operator<=(const Type& _Right) const {	// test if this <= _Right
		if (_isDisposed) throw "Object Disposed...";
		return pData->compare(_Right) <= 0;
	};

	inline bool operator>=(const Type& _Right) const {
		if (_isDisposed) throw "Object Disposed...";
		return pData->compare(_Right) >= 0;
	};

protected:
	//
};
//Custom std::string*
typedef Stringx<std::string> stringx;