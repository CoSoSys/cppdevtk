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


#ifndef CPPDEVTK_UTIL_INFO_TR_HPP_INCLUDED_
#define CPPDEVTK_UTIL_INFO_TR_HPP_INCLUDED_


#include <cppdevtk/base/info_tr.hpp>
#include "info_tr_customization.hpp"
#include "info.hpp"


#define CPPDEVTK_UTIL_SHORT_NAME_TR_SANITIZED QString(CPPDEVTK_UTIL_SHORT_NAME_TR).toLower().replace(' ', '_')
#define CPPDEVTK_UTIL_LONG_NAME_TR_SANITIZED QString(CPPDEVTK_UTIL_LONG_NAME_TR).toLower().replace(' ', '_')


#endif	// CPPDEVTK_UTIL_INFO_TR_HPP_INCLUDED_
