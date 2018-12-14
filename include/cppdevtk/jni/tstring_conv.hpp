/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_JNI_TSTRING_CONV_HPP_INCLUDED_
#define CPPDEVTK_JNI_TSTRING_CONV_HPP_INCLUDED_


#include "config.hpp"
#include "string_conv.hpp"

#include <cppdevtk/base/string_conv.hpp>


#if (CPPDEVTK_DISABLE_UNICODE)
#	define CPPDEVTK_J2T(pJniEnv, str)	\
		::cppdevtk::jni::J2A(pJniEnv, str)

#	define CPPDEVTK_T2J(pJniEnv, str)	\
		::cppdevtk::jni::A2J(pJniEnv, str)
#elif (CPPDEVTK_DISABLE_WIDE)
#	define CPPDEVTK_J2T(pJniEnv, str)	\
		::cppdevtk::base::Q2Utf8(::cppdevtk::jni::J2Q(pJniEnv, str))

#	define CPPDEVTK_T2J(pJniEnv, str)	\
		::cppdevtk::jni::Q2J(pJniEnv, ::cppdevtk::base::Utf82Q(str))
#else
#	define CPPDEVTK_J2T(pJniEnv, str)	\
		::cppdevtk::jni::J2W(pJniEnv, str)

#	define CPPDEVTK_T2J(pJniEnv, str)	\
		::cppdevtk::jni::W2J(pJniEnv, str)
#endif


#endif	// CPPDEVTK_JNI_TSTRING_CONV_HPP_INCLUDED_
