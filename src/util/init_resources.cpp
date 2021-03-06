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


#include <cppdevtk/util/init_resources.hpp>

#include <QtCore/QDir>


CPPDEVTK_UTIL_API void CppDevTkUtilInitResources() {
#	ifndef CPPDEVTK_SHARED
	Q_INIT_RESOURCE(cppdevtk_util);
	Q_INIT_RESOURCE(cppdevtk_util_customization);
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	Q_INIT_RESOURCE(cppdevtk_util_qt5qm);
#	else
	Q_INIT_RESOURCE(cppdevtk_util_qt4qm);
#	endif
#	endif
}

CPPDEVTK_UTIL_API int cppdevtk_util_init_resources(void) try {
	CppDevTkUtilInitResources();
	return 0;
}
catch (...) {
	return -1;
}
