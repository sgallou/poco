//
// LOB.h
//
// $Id: //poco/Main/Data/include/Poco/Data/LOB.h#12 $
//
// Library: Data
// Package: DataCore
// Module:  LOB
//
// Definition of the LOB class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Data_LOB_INCLUDED
#define Data_LOB_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/SharedPtr.h"
#include "Poco/Dynamic/VarHolder.h"
#include "Poco/Exception.h"
#include <vector>


namespace Poco {
namespace Data {


template <typename T>
class LOB
	/// Representation of a Large OBject.
	///
	/// A LOB can hold arbitrary data.
	/// The maximum size depends on the underlying database.
	///
	/// The LOBInputStream and LOBOutputStream classes provide
	/// a convenient way to access the data in a LOB.
{
public:
	typedef typename std::vector<T>::const_iterator Iterator;
	typedef T ValueType;
	typedef typename std::vector<T> Container;
	typedef Poco::SharedPtr<Container> ContentPtr;

	LOB(): _pContent(new std::vector<T>())
		/// Creates an empty LOB.
	{
	}

	LOB(const std::vector<T>& content):
		_pContent(new std::vector<T>(content))
		/// Creates the LOB, content is deep-copied.
	{
	}

	LOB(const T* const pContent, std::size_t size):
		_pContent(new std::vector<T>(pContent, pContent + size))
		/// Creates the LOB by deep-copying pContent.
	{
	}

	LOB(const std::basic_string<T>& content):
		_pContent(new std::vector<T>(content.begin(), content.end()))
		/// Creates a LOB from a string.
	{
	}

	LOB(const LOB& other): _pContent(other._pContent)
		/// Creates a LOB by copying another one.
	{
	}

	~LOB()
		/// Destroys the LOB.
	{
	}

	LOB& operator = (const LOB& other)
		/// Assignment operator.
	{
		LOB tmp(other);
		swap(tmp);
		return *this;
	}

	bool operator == (const LOB& other) const
		/// Compares for equality LOB by value.
	{
		return *_pContent == *other._pContent;
	}

	bool operator != (const LOB& other) const
		/// Compares for inequality LOB by value.
	{
		return *_pContent != *other._pContent;
	}

	void swap(LOB& other)
		/// Swaps the LOB with another one.
	{
		using std::swap;
		swap(_pContent, other._pContent);
	}

	const std::vector<T>& content() const
		/// Returns the content.
	{
		return *_pContent;
	}

	const T* rawContent() const
		/// Returns the raw content.
		///
		/// If the LOB is empty, returns NULL.
	{
		if (_pContent->empty())
			return 0;
		else
			return &(*_pContent)[0];
	}

	void assignVal(std::size_t count, const T& val)
		/// Assigns raw content to internal storage.
	{
		ContentPtr tmp = new Container(count, val);
		_pContent.swap(tmp);
	}

	void assignRaw(const T* ptr, std::size_t count)
		/// Assigns raw content to internal storage.
	{
		poco_assert_dbg (ptr);
		LOB tmp(ptr, count);
		swap(tmp);
	}

	void appendRaw(const T* pChar, std::size_t count)
		/// Assigns raw content to internal storage.
	{
		poco_assert_dbg (pChar);
		_pContent->insert(_pContent->end(), pChar, pChar+count);
	}

	void clear(bool doCompact = false)
		/// Clears the content of the blob.
		/// If doCompact is true, trims the excess capacity.
	{
		_pContent->clear();
		if (doCompact) compact();
	}

	void compact()
		/// Trims the internal storage excess capacity.
	{
		std::vector<T>(*_pContent).swap(*_pContent);
	}

	Iterator begin() const
	{
		return _pContent->begin();
	}

	Iterator end() const
	{
		return _pContent->end();
	}

	std::size_t size() const
		/// Returns the size of the LOB in bytes.
	{
		return _pContent->size();
	}

private:
	ContentPtr _pContent;
};


typedef LOB<unsigned char> BLOB;
typedef LOB<char> CLOB;


//
// inlines
//

template <typename T>
inline void swap(LOB<T>& b1, LOB<T>& b2)
{
	b1.swap(b2);
}


} } // namespace Poco::Data


namespace std
{
	template<>
	inline void swap<Poco::Data::BLOB>(Poco::Data::BLOB& b1, 
		Poco::Data::BLOB& b2)
		/// Full template specalization of std:::swap for BLOB
	{
		b1.swap(b2);
	}

	template<>
	inline void swap<Poco::Data::CLOB>(Poco::Data::CLOB& c1, 
		Poco::Data::CLOB& c2)
		/// Full template specalization of std:::swap for CLOB
	{
		c1.swap(c2);
	}
}


//
// VarHolderImpl<LOB>
//


namespace Poco {
namespace Dynamic {


template <>
class VarHolderImpl<Poco::Data::BLOB>: public VarHolder
{
public:
	VarHolderImpl(const Poco::Data::BLOB& val): _val(val)
	{
	}

	~VarHolderImpl()
	{
	}
	
	const std::type_info& type() const
	{
		return typeid(Poco::Data::BLOB);
	}

	void convert(std::string& val) const
	{
		val.assign(_val.begin(), _val.end());
	}

	VarHolder* clone() const
	{
		return new VarHolderImpl(_val);
	}
	
	const Poco::Data::BLOB& value() const
	{
		return _val;
	}

private:
	VarHolderImpl();
	Poco::Data::BLOB _val;
};


template <>
class VarHolderImpl<Poco::Data::CLOB>: public VarHolder
{
public:
	VarHolderImpl(const Poco::Data::CLOB& val): _val(val)
	{
	}

	~VarHolderImpl()
	{
	}
	
	const std::type_info& type() const
	{
		return typeid(Poco::Data::CLOB);
	}

	void convert(std::string& val) const
	{
		val.assign(_val.begin(), _val.end());
	}

	VarHolder* clone() const
	{
		return new VarHolderImpl(_val);
	}
	
	const Poco::Data::CLOB& value() const
	{
		return _val;
	}

private:
	VarHolderImpl();
	Poco::Data::CLOB _val;
};


} } // namespace Poco::Dynamic


#endif // Data_LOB_INCLUDED