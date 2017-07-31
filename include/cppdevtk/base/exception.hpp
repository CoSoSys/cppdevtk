/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>\n
/// Licensed under the Apache License, Version 2.0 (the "License");\n
/// you may not use this file except in compliance with the License.\n
/// You may obtain a copy of the License at\n
/// http://www.apache.org/licenses/LICENSE-2.0\n
/// Unless required by applicable law or agreed to in writing, software\n
/// distributed under the License is distributed on an "AS IS" BASIS,\n
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
/// See the License for the specific language governing permissions and\n
/// limitations under the License.\n
/// Please see the file COPYING.
///
/// \authors Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CPPDEVTK_BASE_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "throwable.hpp"
#include "cloneable.hpp"
#include "stringizable.hpp"
#include "source_code_info.hpp"
#include "stack_trace.hpp"
#include "name_mangling.hpp"
#include "unused.hpp"
#include "tstring.hpp"
#include "typeinfo.hpp"

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtCore/QException>
#else
#include <QtCore/QtCore>
#endif

#include <exception>
#include <algorithm>	// swap(), C++98
#include <utility>	// swap(), C++11
#include <new>
#include <cstddef>
#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup exceptions Exceptions
/// Exception classes
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup std_exceptions Standard Exceptions
/// Exceptions similar to the standard ones and providing:
/// - multiple virtual inheritance
/// - throw point
/// - stack trace
/// - cause
/// - polymorphically throw
/// \attention In derived classes declare/define dtor (with throw()) even if empty.
/// Compiler generated dtor will correctly trigger with gcc: "looser throw specifier for"
/// Clever is that gcc does trigger this only if derived class adds data members
/// (ex for LogicException that adds whatArg_ and not for InvalidArgumentException that does not add any data member).
/// MSVC silently ignore this (bug!).
/// \attention C++11 std, 18.8.1 Class exception: Each standard library class T that derives from class exception
/// shall have a publicly accessible copy constructor and a publicly accessible copy assignment operator
/// that do not exit with an exception.
/// So if derived class adds data members please define/implement nothrow copy ctor/assignment
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exception

#define CPPDEVTK_EXCEPTION()	\
	::cppdevtk::base::Exception(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_EXCEPTION_W_CAUSE(cause)	\
	::cppdevtk::base::Exception(CPPDEVTK_SOURCE_CODE_INFO(), (cause))

#define CPPDEVTK_MAKE_EXCEPTION(excName)	\
	::cppdevtk::base::Exception excName(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_EXCEPTION_W_CAUSE(excName, cause)	\
	::cppdevtk::base::Exception excName(CPPDEVTK_SOURCE_CODE_INFO(), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 18.6.1 Class exception
class CPPDEVTK_BASE_API Exception: public Throwable, public Cloneable,
		public QStringizable, public CPPDEVTK_QT_EXCEPTION {
public:
	typedef CPPDEVTK_TR1_NS::shared_ptr<const StackTrace> StackTracePtrType;
	typedef CPPDEVTK_TR1_NS::shared_ptr<const Exception> CausePtrType;
	
	
	explicit Exception(const SourceCodeInfo& throwPoint) /* throw() */;
	Exception(const SourceCodeInfo& throwPoint, const Exception& cause) /* throw() */;
	
	Exception(const Exception& other) throw();
	
	virtual ~Exception() throw();
	
	Exception& operator=(const Exception& other) throw();
	
	::std::auto_ptr<Exception> Clone() const;
	
	/// \remark May include throw point and stack trace.
	/// \attention Do not override in derived classes; override DoOwnWhat() if needed.
	virtual QString ToString() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual Exception* clone() const;
#	endif
	
	virtual void raise() const; ///< \attention Do not override in derived classes (is inherited virtual, but our implementation is OK)
	
	virtual const char* what() const throw();	///< \attention Do not override in derived classes; override DoOwnWhat() if needed.

	QString What() const;	///< \attention Non-virtual; in derived classes override DoOwnWhat() if needed.
	
	SourceCodeInfo GetThrowPoint() const;
	StackTracePtrType GetStackTrace() const;
	CausePtrType GetCause() const;
	CausePtrType GetInitialCause() const;
	
	/// \remark Exception safety: nothrow guarantee.
	/// \attention Must be overloaded in all derived classes (also nothrow guarantee).
	void Swap(Exception& other);
	
	/// If exc is derived from Exception returns ToString(), runtime type plus what() otherwise.
	static QString GetDetailedInfo(const ::std::exception& exc);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual Exception* DoClone() const;
#	endif
	
	QString DoWhat(bool includeCause) const;
	virtual QString DoOwnWhat() const;	///< \attention Override in derived classes if needed.
	
	/// \remark Exception safety: nothrow guarantee.
	/// \attention Must be overloaded in all derived classes (also nothrow guarantee).
	void SwapOwnData(Exception& other);
private:
	mutable CPPDEVTK_TR1_NS::shared_ptr< ::std::string> pStdWhatMsg_;	// shared_ptr because std::string copy ctor may throw
	StackTracePtrType pStackTrace_;	// shared_ptr because StackTrace copy ctor may throw
	SourceCodeInfo throwPoint_;
	CausePtrType pCause_;
};


/// \remark Exception safety: nothrow guarantee.
/// \attention Must be overloaded for all Exception derived classes (also nothrow guarantee).
CPPDEVTK_BASE_API void swap(Exception& x, Exception& y);


/// @}	// std_exceptions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @}	// exceptions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline Exception::Exception(const Exception& other) throw(): Throwable(other), Cloneable(other), QStringizable(other),
		CPPDEVTK_QT_EXCEPTION(other), pStdWhatMsg_(other.pStdWhatMsg_), pStackTrace_(other.pStackTrace_),
		throwPoint_(other.throwPoint_), pCause_(other.pCause_) {}

inline Exception::~Exception() throw() {}

inline Exception& Exception::operator=(const Exception& other) throw() {
	Exception tmp(other);
	Swap(tmp);
	return *this;
}

inline ::std::auto_ptr<Exception> Exception::Clone() const {
	return ::std::auto_ptr<Exception>(static_cast<Exception*>(Cloneable::Clone().release()));
}


#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* Exception::clone() const {
#else
inline Exception* Exception::clone() const {
#endif
	return Clone().release();
}

inline void Exception::raise() const {
	Throw();
}

inline QString Exception::What() const {
	return DoWhat(true);
}

inline SourceCodeInfo Exception::GetThrowPoint() const {
	return throwPoint_;
}

inline Exception::StackTracePtrType Exception::GetStackTrace() const {
	return pStackTrace_;
}

inline Exception::CausePtrType Exception::GetCause() const {
	return pCause_;
}

inline void Exception::Swap(Exception& other) {
	using ::std::swap;
	
	
	if (this != &other) {
		swap(static_cast<CPPDEVTK_QT_EXCEPTION&>(*this), static_cast<CPPDEVTK_QT_EXCEPTION&>(other));
		SwapOwnData(other);
	}
}

inline void Exception::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* Exception::DoClone() const {
#else
inline Exception* Exception::DoClone() const {
#endif
	return new Exception(*this);
}

inline QString Exception::DoWhat(bool includeCause) const {
	QString doWhatMsg = DoOwnWhat();
	
	if (includeCause && pCause_) {
		doWhatMsg += "\nCaused by: " + pCause_->DoWhat(includeCause);
	}
	
	return doWhatMsg;
}

inline QString Exception::DoOwnWhat() const {
	QString demangledName;
	
	const QString kTypeInfoName = typeid(*this).name();
	if (!kTypeInfoName.isEmpty()) {
		if (IsMangled(kTypeInfoName)) {
			demangledName = Demangle(kTypeInfoName);
		}
		if (demangledName.isEmpty()) {
			demangledName = kTypeInfoName;
		}
	}
	
	return demangledName;
}

inline void Exception::SwapOwnData(Exception& other) {
	using ::std::swap;
	
	
	pStdWhatMsg_.swap(other.pStdWhatMsg_);
	pStackTrace_.swap(other.pStackTrace_);
	swap(throwPoint_, other.throwPoint_);
	pCause_.swap(other.pCause_);
}


inline CPPDEVTK_BASE_API void swap(Exception& x, Exception& y) {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_EXCEPTION_HPP_INCLUDED_