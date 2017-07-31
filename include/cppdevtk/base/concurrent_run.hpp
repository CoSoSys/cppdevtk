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
///
/// \brief Improved \c QtConcurrent::run()
/// \details
///	- \c QThreadPool has the main disadvantage that \c start() does not return a \c QFuture
/// (also it is not policy based (scheduling and size policies for ex) and does not offer more control
/// (active and pending tasks for ex)).
/// - \c QtConcurrent::run() has the main disadvantages that the returned future is not cancelable
/// and does not support priorities.
/// - Our \c CancelableTaskExecutor supports cancelable future and priorities.
/// \note If we'll have time we'll implement our own thread pool.
/// \sa \c QThreadPool and \c QtConcurrent::run()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CPPDEVTK_BASE_CONCURRENT_RUN_HPP_INCLUDED_
#define CPPDEVTK_BASE_CONCURRENT_RUN_HPP_INCLUDED_


#include "config.hpp"
#include "cancelable.hpp"
#include "non_copyable.hpp"
#include "mutex.hpp"

#include <QtCore/QFutureInterface>
#include <QtCore/QRunnable>
#include <QtCore/QFuture>
#include <QtCore/QThreadPool>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtCore/QException>
#include <QtCore/QUnhandledException>
#else
#include <QtCore/QtCore>
#endif

#include <new>
#include <memory>


namespace cppdevtk {
namespace base {
namespace concurrent {


class CPPDEVTK_BASE_API FutureInterfaceCancelable: public Cancelable {
public:
	explicit FutureInterfaceCancelable(QFutureInterfaceBase& futureInterfaceBase);
	
	virtual void Cancel();
	virtual bool IsCanceled() const;
private:
	QFutureInterfaceBase& futureInterfaceBase_;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract base class for cancelable tasks. Users should provide concrete implementations.
template <typename TResult>
class CancelableTask {
public:
	typedef FutureInterfaceCancelable CancelableType;
	
	
	virtual ~CancelableTask();
	
	virtual TResult Run(::std::auto_ptr<CancelableType> pCancelable) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Execute cancelable task in a separate thread.
class CPPDEVTK_BASE_API CancelableTaskExecutor {
public:
	template <typename TResult>
	static QFuture<TResult> Execute(::std::auto_ptr<CancelableTask<TResult> > pCancelableTask, int priority = 0);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	template <typename TResult>
	static QFuture<TResult> Execute(::std::auto_ptr<CancelableTask<TResult> > pCancelableTask, QThreadPool& threadPool,
			int priority = 0);
#	endif
};


namespace detail {


template <typename TResult>
class StartAndRunCancelableTaskBase: public QFutureInterface<TResult>, public QRunnable, private NonCopyable {
public:
	typedef CancelableTask<TResult> CancelableTaskType;
	
	
	QFuture<TResult> Start();
protected:
	StartAndRunCancelableTaskBase(::std::auto_ptr<CancelableTaskType> pCancelableTask, int priority);
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	StartAndRunCancelableTaskBase(::std::auto_ptr<CancelableTaskType> pCancelableTask, QThreadPool& threadPool, int priority);
#	endif
	virtual ~StartAndRunCancelableTaskBase();
	
	
	::std::auto_ptr<CancelableTaskType> pCancelableTask_;
private:
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QThreadPool& threadPool_;
#	endif
	const int kPriority_;
};


template <typename TResult>
class StartAndRunCancelableTask: public StartAndRunCancelableTaskBase<TResult> {
public:
	StartAndRunCancelableTask(
			::std::auto_ptr<typename StartAndRunCancelableTaskBase<TResult>::CancelableTaskType> pCancelableTask,
			int priority);
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	StartAndRunCancelableTask(
			::std::auto_ptr<typename StartAndRunCancelableTaskBase<TResult>::CancelableTaskType> pCancelableTask,
			QThreadPool& threadPool, int priority);
#	endif
	
	virtual void run();	// QRunnable::run()
};


template <>
class StartAndRunCancelableTask<void>: public StartAndRunCancelableTaskBase<void> {
public:
	StartAndRunCancelableTask(
			::std::auto_ptr<StartAndRunCancelableTaskBase<void>::CancelableTaskType> pCancelableTask, int priority);
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	StartAndRunCancelableTask(
			::std::auto_ptr<StartAndRunCancelableTaskBase<void>::CancelableTaskType> pCancelableTask,
			QThreadPool& threadPool, int priority);
#	endif
	
	virtual void run();
};


}	// namespace detail




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline FutureInterfaceCancelable::FutureInterfaceCancelable(QFutureInterfaceBase& futureInterfaceBase): Cancelable(),
		futureInterfaceBase_(futureInterfaceBase) {}

inline void FutureInterfaceCancelable::Cancel() {
	futureInterfaceBase_.cancel();
}

inline bool FutureInterfaceCancelable::IsCanceled() const {
	return futureInterfaceBase_.isCanceled();
}


template <typename TResult>
inline CancelableTask<TResult>::~CancelableTask() {}


template <typename TResult>
inline QFuture<TResult> CancelableTaskExecutor::Execute(::std::auto_ptr<CancelableTask<TResult> > pCancelableTask,
		int priority) {
	return (new ::cppdevtk::base::concurrent::detail::StartAndRunCancelableTask<TResult>(pCancelableTask, priority))->Start();
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

template <typename TResult>
inline QFuture<TResult> CancelableTaskExecutor::Execute(::std::auto_ptr<CancelableTask<TResult> > pCancelableTask,
		QThreadPool& threadPool, int priority) {
	return (new ::cppdevtk::base::concurrent::detail::StartAndRunCancelableTask<TResult>(pCancelableTask, threadPool,
			priority))->Start();
}

#endif


namespace detail {


template <typename TResult>
inline QFuture<TResult> StartAndRunCancelableTaskBase<TResult>::Start() {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	threadPool_.start(this, kPriority_);
#	else
	QThreadPool::globalInstance()->start(this, kPriority_);
#	endif
	QFutureInterface<TResult>::reportStarted();
	return QFutureInterface<TResult>::future();
}

template <typename TResult>
inline StartAndRunCancelableTaskBase<TResult>::StartAndRunCancelableTaskBase(
		::std::auto_ptr<CancelableTaskType> pCancelableTask, int priority): QFutureInterface<TResult>(), QRunnable(),
		NonCopyable(), pCancelableTask_(pCancelableTask),
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		threadPool_(*QThreadPool::globalInstance()),
#		endif
		kPriority_(priority) {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QFutureInterface<TResult>::setThreadPool(&threadPool_);
#	endif
	QFutureInterface<TResult>::setRunnable(this);
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

template <typename TResult>
inline StartAndRunCancelableTaskBase<TResult>::StartAndRunCancelableTaskBase(
		::std::auto_ptr<CancelableTaskType> pCancelableTask, QThreadPool& threadPool, int priority):
		QFutureInterface<TResult>(), QRunnable(), NonCopyable(), pCancelableTask_(pCancelableTask), threadPool_(threadPool),
		kPriority_(priority) {
	QFutureInterface<TResult>::setThreadPool(&threadPool_);
	QFutureInterface<TResult>::setRunnable(this);
}

#endif

template <typename TResult>
inline StartAndRunCancelableTaskBase<TResult>::~StartAndRunCancelableTaskBase() {}


template <typename TResult>
inline StartAndRunCancelableTask<TResult>::StartAndRunCancelableTask(
		::std::auto_ptr<typename StartAndRunCancelableTaskBase<TResult>::CancelableTaskType> pCancelableTask, int priority):
		StartAndRunCancelableTaskBase<TResult>(pCancelableTask, priority) {}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

template <typename TResult>
inline StartAndRunCancelableTask<TResult>::StartAndRunCancelableTask(
		::std::auto_ptr<typename StartAndRunCancelableTaskBase<TResult>::CancelableTaskType> pCancelableTask,
		QThreadPool& threadPool, int priority): StartAndRunCancelableTaskBase<TResult>(pCancelableTask, threadPool,
		priority) {}

#endif

template <typename TResult>
void StartAndRunCancelableTask<TResult>::run() {
	if (QFutureInterface<TResult>::isCanceled()) {
		QFutureInterface<TResult>::reportFinished();
		
		return;
	}
	
	::std::auto_ptr<typename StartAndRunCancelableTaskBase<TResult>::CancelableTaskType::CancelableType> pCancelable(
			new typename StartAndRunCancelableTaskBase<TResult>::CancelableTaskType::CancelableType(*this));
	TResult result;
	try {
		result = StartAndRunCancelableTaskBase<TResult>::pCancelableTask_->Run(pCancelable);
	}
	catch (const CPPDEVTK_QT_EXCEPTION& exc) {
		QFutureInterface<TResult>::reportException(exc);
	}
	catch (...) {
		QFutureInterface<TResult>::reportException(CPPDEVTK_QT_UNHANDLED_EXCEPTION());
	}
	
	QFutureInterface<TResult>::reportResult(result);
	QFutureInterface<TResult>::reportFinished();
}


inline StartAndRunCancelableTask<void>::StartAndRunCancelableTask(
		::std::auto_ptr<StartAndRunCancelableTaskBase<void>::CancelableTaskType> pCancelableTask, int priority):
		StartAndRunCancelableTaskBase<void>(pCancelableTask, priority) {}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

inline StartAndRunCancelableTask<void>::StartAndRunCancelableTask(
		::std::auto_ptr<StartAndRunCancelableTaskBase<void>::CancelableTaskType> pCancelableTask,
		QThreadPool& threadPool, int priority): StartAndRunCancelableTaskBase<void>(pCancelableTask, threadPool,
		priority) {}

#endif


}	// namespace detail


#if (CPPDEVTK_ENABLE_TMPL_EXPL_INST)

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API CancelableTask<void>;

namespace detail {

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API StartAndRunCancelableTaskBase<void>;
#if (!CPPDEVTK_COMPILER_MSVC || (_MSC_VER > 1500))	// msvc 2008: C2950: 'type' : cannot explicitly instantiate an explicit specialization
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API StartAndRunCancelableTask<void>;
#endif

}

#endif


}	// namespace concurrent
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_CONCURRENT_RUN_HPP_INCLUDED_