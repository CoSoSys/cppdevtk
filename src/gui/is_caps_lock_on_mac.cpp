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


#include <cppdevtk/gui/is_caps_lock_on.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif
#include <cppdevtk/base/unused.hpp>

#if (!CPPDEVTK_PLATFORM_IOS)
#	include <Carbon/Carbon.h>
#endif

namespace cppdevtk {
namespace gui {


CPPDEVTK_GUI_API bool IsCapsLockOn(bool& value) {
#	if (!CPPDEVTK_PLATFORM_IOS)
	
	//value = (GetCurrentEventKeyModifiers() & alphaLock);
	value = (::GetCurrentKeyModifiers() & alphaLock);
	
	return true;
	
#	else
	// TODO: iOS port
	::cppdevtk::base::SuppressUnusedWarning(value);
	CPPDEVTK_COMPILER_WARNING("IsCapsLockOn() not ported on iOS");
	return false;
#	endif
}


}	// namespace gui
}	// namespace cppdevtk
