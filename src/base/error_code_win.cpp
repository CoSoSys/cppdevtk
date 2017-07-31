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


#include <cppdevtk/base/error_code.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/error_category.hpp>

#include <windows.h>


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API ErrorCode GetLastSystemErrorCode() {
	return MakeSystemErrorCode(::GetLastError());
}

CPPDEVTK_BASE_API void SetLastSystemErrorCode(const ErrorCode& systemErrorCode) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((systemErrorCode.GetCategory() == GetSystemCategory()),
			"systemErrorCode does not have SystemCategory");
	
	::SetLastError(systemErrorCode.GetValue());
}


}	// namespace base
}	// namespace cppdevtk