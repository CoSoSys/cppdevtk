/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#include <cppdevtk/jni/config.hpp>
#if (CPPDEVTK_FORCE_DBC_IN_JNI_API)
#undef CPPDEVTK_ENABLE_DBC
#define CPPDEVTK_ENABLE_DBC 1
#undef CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION
#define CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION (!CPPDEVTK_ENABLE_DBC || 0)
#endif

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif

#include <cppdevtk/jni/exceptions_support.hpp>
#include <cppdevtk/jni/exceptions.hpp>
#include <cppdevtk/jni/string_conv.hpp>

#include <cppdevtk/util/no_such_file_or_directory_exception.hpp>

#include <cppdevtk/base/task_canceled_exception.hpp>
#include <cppdevtk/base/ios.hpp>
#include <cppdevtk/base/dbc_exceptions.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/string_conv.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <cstddef>
#include <cstdlib>
#include <new>
#include <typeinfo>


using ::cppdevtk::base::Q2Utf8;
using ::cppdevtk::base::Exception;
using ::std::exception;


namespace cppdevtk {
namespace jni {


CPPDEVTK_JNI_API void ThrowJavaExceptionFromCatchedCppException(JNIEnv* pJniEnv) try {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pJniEnv);
	
	if (pJniEnv->ExceptionCheck() == JNI_TRUE) {
		return;
	}
	
	// TODO: keep updated
	// NOTE: we must support Java >= 1.6 (this is why we can not use for ex java.nio.file.FileSystemException (since 1.7))
	try {
		throw;
	}
	
	// CppDevTk JNI exceptions
	catch (const NoSuchFieldError& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java NoSuchFieldError; caught C++ NoSuchFieldError: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/NoSuchFieldError", exc.what());
	}
	catch (const NoSuchMethodError& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java NoSuchMethodError; caught C++ NoSuchMethodError: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/NoSuchMethodError", exc.what());
	}
	catch (const NoClassDefFoundError& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java NoClassDefFoundError; caught C++ NoClassDefFoundError: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/NoClassDefFoundError", exc.what());
	}
	catch (const OutOfMemoryError& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java OutOfMemoryError; caught C++ OutOfMemoryError: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/OutOfMemoryError", exc.what());
	}
	catch (const JniException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java Exception; caught C++ JniException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/Exception", exc.what());
	}
	
	// CppDevTk specific exceptions
	catch (const ::cppdevtk::base::concurrent::TaskCanceledException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java CancellationException; caught C++ TaskCanceledException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/util/concurrent/CancellationException", exc.what());
	}
	catch (const ::cppdevtk::util::NoSuchFileOrDirectoryException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java FileNotFoundException; caught C++ NoSuchFileOrDirectoryException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/io/FileNotFoundException", exc.what());
	}
	catch (const ::cppdevtk::base::NullArgumentException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java IllegalArgumentException; caught C++ NullArgumentException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/IllegalArgumentException", exc.what());
	}
	catch (const ::cppdevtk::base::NullPointerException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java NullPointerException; caught C++ NullPointerException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/NullPointerException", exc.what());
	}
	catch (const ::cppdevtk::base::InvalidStateException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java IllegalStateException; caught C++ InvalidStateException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/IllegalStateException", exc.what());
	}
	
	// CppDevTk std exceptions
	catch (const ::cppdevtk::base::IosFailureException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java IOException; caught C++ IosFailureException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/io/IOException", exc.what());
	}
	catch (const ::cppdevtk::base::RuntimeException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java Exception; caught C++ RuntimeException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/Exception", exc.what());
	}
	catch (const ::cppdevtk::base::InvalidArgumentException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java IllegalArgumentException; caught C++ InvalidArgumentException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/IllegalArgumentException", exc.what());
	}
	catch (const ::cppdevtk::base::LogicException& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java RuntimeException; caught C++ LogicException: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/RuntimeException", exc.what());
	}
	catch (const ::cppdevtk::base::Exception& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java Exception; caught C++ Exception: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/Exception", exc.what());
	}
	
	// C++ std exceptions
	catch (const ::std::ios_base::failure& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java IOException; caught C++ ios_base::failure: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/io/IOException", exc.what());
	}
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java Exception; caught C++ runtime_error: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/Exception", exc.what());
	}
	catch (const ::std::invalid_argument& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java IllegalArgumentException; caught C++ invalid_argument: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/IllegalArgumentException", exc.what());
	}
	catch (const ::std::logic_error& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java RuntimeException; caught C++ logic_error: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/RuntimeException", exc.what());
	}
	catch (const ::std::bad_cast& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java ClassCastException; caught C++ bad_cast: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/ClassCastException", exc.what());
	}
	catch (const ::std::bad_alloc& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java OutOfMemoryError; caught C++ bad_alloc: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/OutOfMemoryError", exc.what());
	}
	catch (const exception& exc) {
		CPPDEVTK_LOG_ERROR("throwing Java Exception; caught C++ exception: "
				<< Exception::GetDetailedInfo(exc));
		detail::ThrowJavaException(pJniEnv, "java/lang/Exception", exc.what());
	}
	
	// unknown exception
	catch (...) {
		CPPDEVTK_LOG_ERROR("throwing Java Exception; caught unknown C++ exception");
		detail::ThrowJavaException(pJniEnv, "java/lang/Exception", "unknown C++ exception");
	}
}
catch (const exception& exc) {
	if (pJniEnv->ExceptionCheck() == JNI_FALSE) {
		CPPDEVTK_LOG_FATAL("could not throw java exc from cpp exc; caught: " << Exception::GetDetailedInfo(exc));
		pJniEnv->FatalError(exc.what());
	}
	else {
		CPPDEVTK_LOG_WARN("could not throw java exc from cpp exc; caught: " << Exception::GetDetailedInfo(exc));
	}
}
catch (...) {
	if (pJniEnv->ExceptionCheck() == JNI_FALSE) {
		CPPDEVTK_LOG_FATAL("could not throw java exc from cpp exc (there is a java exc pending); caught unknown exception");
		pJniEnv->FatalError("could not throw java exc from cpp exc (there is a java exc pending); caught unknown exception");
	}
	else {
		CPPDEVTK_LOG_WARN("could not throw java exc from cpp exc (there is a java exc pending); caught unknown exception");
	}
}

CPPDEVTK_JNI_API void ThrowCppExceptionFromJavaPendingException(JNIEnv* pJniEnv, bool clearPendingException) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pJniEnv);
	
	if (pJniEnv->ExceptionCheck() == JNI_FALSE) {
		return;
	}
	
	CPPDEVTK_JNI_EXCEPTION_DESCRIBE(pJinEnv);
	
	jthrowable jThrowable = pJniEnv->ExceptionOccurred();
	CPPDEVTK_ASSERT(jThrowable != NULL);
	pJniEnv->ExceptionClear();
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&clearPendingException)(&pJniEnv)(&jThrowable)) {
		if (!clearPendingException) {
			if (pJniEnv->ExceptionCheck() == JNI_FALSE) {
				if (pJniEnv->Throw(jThrowable) != 0) {
					if (pJniEnv->ExceptionCheck() == JNI_FALSE) {
						pJniEnv->DeleteLocalRef(jThrowable);
						CPPDEVTK_LOG_FATAL("failed to rethrow Java exception");
						pJniEnv->FatalError("failed to rethrow Java exception");
					}
				}
			}
		}
		else {
			if (pJniEnv->ExceptionCheck() == JNI_TRUE) {
				CPPDEVTK_JNI_EXCEPTION_DESCRIBE(pJinEnv);
				pJniEnv->ExceptionClear();
			}
		}
		pJniEnv->DeleteLocalRef(jThrowable);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	jclass jThrowableClass = pJniEnv->GetObjectClass(jThrowable);
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pJniEnv)(&jThrowableClass)) {
		pJniEnv->DeleteLocalRef(jThrowableClass);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	CPPDEVTK_THROW_JNI_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, jThrowableClass,
			"failed to get Java pending exception class", false);
	
	jmethodID jThrowableGetMessageMid = pJniEnv->GetMethodID(jThrowableClass, "getMessage", "()Ljava/lang/String;");
	CPPDEVTK_THROW_JNI_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, jThrowableGetMessageMid,
			"failed to get 'String Throwable::getMessage()' methodID", false);
	
	jstring jThrowableMsg = (jstring)pJniEnv->CallObjectMethod(jThrowable, jThrowableGetMessageMid);
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pJniEnv)(&jThrowableMsg)) {
		pJniEnv->DeleteLocalRef(jThrowableMsg);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	CPPDEVTK_THROW_JNI_EXC_IF_JAVA_PENDING_EXC(pJniEnv, "'String Throwable::getMessage()' failed", false);
	
	QString qThrowableMsg = "no message";
	if (jThrowableMsg != NULL) {
		qThrowableMsg = J2Q(pJniEnv, jThrowableMsg);
	}
	
	// Derived from java/lang/RuntimeException
	if (detail::IsBaseOf(pJniEnv, "java/lang/IllegalArgumentException", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ InvalidArgumentException; caught Java IllegalArgumentException: " << qThrowableMsg);
		throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION(qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/IllegalStateException", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ InvalidStateException; caught Java IllegalStateException: " << qThrowableMsg);
		throw CPPDEVTK_INVALID_STATE_EXCEPTION("", "", qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/NullPointerException", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ NullPointerException; caught Java NullPointerException: " << qThrowableMsg);
		throw CPPDEVTK_NULL_POINTER_EXCEPTION();
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/ClassCastException", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ bad_cast; caught Java ClassCastException: " << qThrowableMsg);
		throw ::std::bad_cast();
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/RuntimeException", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ LogicException; caught Java RuntimeException: " << qThrowableMsg);
		throw CPPDEVTK_LOGIC_EXCEPTION(qThrowableMsg);
	}
	
	// Derived from java/lang/Exception
	if (detail::IsBaseOf(pJniEnv, "java/util/concurrent/CancellationException", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ TaskCanceledException; caught Java CancellationException: " << qThrowableMsg);
		throw CPPDEVTK_TASK_CANCELED_EXCEPTION_W_WA(qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/io/FileNotFoundException", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ NoSuchFileOrDirectoryException; caught Java FileNotFoundException: " << qThrowableMsg);
		throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/io/IOException", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ IosFailureException; caught Java IOException: " << qThrowableMsg);
		throw CPPDEVTK_IOS_FAILURE_EXCEPTION_W_WA(qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/Exception", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ JniException; caught Java Exception: " << qThrowableMsg);
		throw CPPDEVTK_JNI_EXCEPTION(qThrowableMsg);
	}
	
	// Derived from java/lang/Error
	if (detail::IsBaseOf(pJniEnv, "java/lang/OutOfMemoryError", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ OutOfMemoryError; caught Java OutOfMemoryError: " << qThrowableMsg);
		throw CPPDEVTK_OUT_OF_MEMORY_ERROR(qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/NoClassDefFoundError", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ NoClassDefFoundError; caught Java NoClassDefFoundError: " << qThrowableMsg);
		throw CPPDEVTK_NO_CLASS_DEF_FOUND_ERROR(qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/NoSuchMethodError", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ NoSuchMethodError; caught Java NoSuchMethodError: " << qThrowableMsg);
		throw CPPDEVTK_NO_SUCH_METHOD_ERROR(qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/NoSuchFieldError", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ NoSuchFieldError; caught Java NoSuchFieldError: " << qThrowableMsg);
		throw CPPDEVTK_NO_SUCH_FIELD_ERROR(qThrowableMsg);
	}
	if (detail::IsBaseOf(pJniEnv, "java/lang/Error", jThrowableClass)) {
		CPPDEVTK_LOG_ERROR("throwing C++ JniException; caught Java Error: " << qThrowableMsg);
		throw CPPDEVTK_JNI_EXCEPTION(qThrowableMsg);
	}
	
	CPPDEVTK_ASSERT(detail::IsBaseOf(pJniEnv, "java/lang/Throwable", jThrowableClass));
	CPPDEVTK_LOG_ERROR("throwing C++ JniException; caught Java Throwable: " << qThrowableMsg);
	throw CPPDEVTK_JNI_EXCEPTION(qThrowableMsg);
}


namespace detail {


CPPDEVTK_JNI_API void ThrowJavaException(JNIEnv* pJniEnv, const char* excClassName, const char* excMsg) {
	CPPDEVTK_ASSERT(pJniEnv != NULL);
	CPPDEVTK_ASSERT(excClassName != NULL);
	CPPDEVTK_ASSERT(excMsg != NULL);
	CPPDEVTK_ASSERT(pJniEnv->ExceptionCheck() == JNI_FALSE);
	
	jclass jExcClass = pJniEnv->FindClass(excClassName);
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pJniEnv)(&jExcClass)) {
		pJniEnv->DeleteLocalRef(jExcClass);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	CPPDEVTK_THROW_JNI_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, jExcClass,
			QString("failed to find class '%1'").arg(excClassName), false);
	if (pJniEnv->ThrowNew(jExcClass, excMsg) != 0) {
		if (pJniEnv->ExceptionCheck() == JNI_FALSE) {
			throw CPPDEVTK_JNI_EXCEPTION(QString("failed to throw java exception: %1").arg(excClassName));
		}
	}
}

CPPDEVTK_JNI_API bool IsBaseOf(JNIEnv* pJniEnv, const QString& baseClassName, jclass jDerivedClass) {
	CPPDEVTK_ASSERT(pJniEnv != NULL);
	CPPDEVTK_ASSERT(!baseClassName.isEmpty());
	CPPDEVTK_ASSERT(jDerivedClass != NULL);
	CPPDEVTK_ASSERT(pJniEnv->ExceptionCheck() == JNI_FALSE);
	
	jclass jBaseClass = pJniEnv->FindClass(Q2Utf8(baseClassName).c_str());
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pJniEnv)(&jBaseClass)) {
		pJniEnv->DeleteLocalRef(jBaseClass);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	CPPDEVTK_THROW_JNI_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, jBaseClass,
			QString("failed to find class '%1'").arg(baseClassName), false);
	
	if (pJniEnv->IsSameObject(jBaseClass, jDerivedClass) == JNI_TRUE) {
		return true;
	}
	
	jclass jCurrSuperClass = NULL;
	jclass jCurrDerivedClass = jDerivedClass;
	while ((jCurrSuperClass = pJniEnv->GetSuperclass(jCurrDerivedClass)) != NULL) {
		if (jCurrDerivedClass != jDerivedClass) {
			pJniEnv->DeleteLocalRef(jCurrDerivedClass);
			jCurrDerivedClass = NULL;
		}
		
		if (pJniEnv->IsSameObject(jCurrSuperClass, jBaseClass) == JNI_TRUE) {
			pJniEnv->DeleteLocalRef(jCurrSuperClass);
			return true;
		}
		
		jCurrDerivedClass = jCurrSuperClass;
	}
	if (jCurrDerivedClass != jDerivedClass) {
		pJniEnv->DeleteLocalRef(jCurrDerivedClass);
	}
	
	return false;
}


}	// namespace detail


}	// namespace jni
}	// namespace cppdevtk
