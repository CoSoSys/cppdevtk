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


#ifndef CPPDEVTK_BASE_TASK_CANCELED_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_TASK_CANCELED_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "stdexcept.hpp"


namespace cppdevtk {
namespace base {
namespace concurrent {


#define CPPDEVTK_TASK_CANCELED_EXCEPTION(whatArg)	\
	::cppdevtk::base::concurrent::TaskCanceledException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_TASK_CANCELED_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::concurrent::TaskCanceledException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


class CPPDEVTK_BASE_API TaskCanceledException: public RuntimeException {
public:
	TaskCanceledException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	
	virtual ~TaskCanceledException() throw();
	
	::std::auto_ptr<TaskCanceledException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual TaskCanceledException* clone() const;
#	endif
	
	void Swap(TaskCanceledException& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual TaskCanceledException* DoClone() const;
#	endif
	
	void SwapOwnData(TaskCanceledException& other);
};


CPPDEVTK_BASE_API void swap(TaskCanceledException& x, TaskCanceledException& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline TaskCanceledException::TaskCanceledException(const SourceCodeInfo& throwPoint,
		const QString& whatArg): Exception(throwPoint), RuntimeException(throwPoint, whatArg) {}

inline TaskCanceledException::~TaskCanceledException() throw() {}

inline ::std::auto_ptr<TaskCanceledException> TaskCanceledException::Clone() const {
	return ::std::auto_ptr<TaskCanceledException>(dynamic_cast<TaskCanceledException*>(
			Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* TaskCanceledException::clone() const {
#else
inline TaskCanceledException* TaskCanceledException::clone() const {
#endif
	return Clone().release();
}

inline void TaskCanceledException::Swap(TaskCanceledException& other) {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline void TaskCanceledException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* TaskCanceledException::DoClone() const {
#else
inline TaskCanceledException* TaskCanceledException::DoClone() const {
#endif
	return new TaskCanceledException(*this);
}

inline void TaskCanceledException::SwapOwnData(TaskCanceledException& other) {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(TaskCanceledException& x, TaskCanceledException& y) {
	x.Swap(y);
}


}	// namespace concurrent
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_TASK_CANCELED_EXCEPTION_HPP_INCLUDED_