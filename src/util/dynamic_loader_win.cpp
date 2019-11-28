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


#include <cppdevtk/util/dynamic_loader.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif
#include <cppdevtk/util/dynamic_loader_exception.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>

#include <QtCore/QDir>


namespace cppdevtk {
namespace util {


DynamicLibrary::Handle DynamicLoader::Load(const QString& dynLibName) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(dynLibName), "dynLibName must be valid");
	
	DynamicLibrary::Handle handle = ::LoadLibrary(CPPDEVTK_Q2T(QDir::toNativeSeparators(dynLibName)).c_str());
	if (handle == 0) {
		throw CPPDEVTK_DYNAMIC_LOADER_EXCEPTION("failed to load library " + dynLibName);
	}
	
	return handle;
}

void DynamicLoader::Unload(DynamicLibrary::Handle dynLibHandle) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((dynLibHandle != 0), "dynLibHandle is 0");
	
	if (!::FreeLibrary(dynLibHandle)) {
		throw CPPDEVTK_DYNAMIC_LOADER_EXCEPTION("failed to unload library");
	}
}


}	// namespace util
}	// namespace cppdevtk
