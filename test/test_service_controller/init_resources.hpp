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


#ifndef CPPDEVTK_TEST_SERVICE_CONTROLLER_INIT_RESOURCES_HPP_INCLUDED_
#define CPPDEVTK_TEST_SERVICE_CONTROLLER_INIT_RESOURCES_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/base/init_resources.hpp>
#include <cppdevtk/util/init_resources.hpp>
#include <cppdevtk/QtSolutions/QtCopyDialog/init_resources.hpp>
#include <cppdevtk/gui/init_resources.hpp>

#include <QtCore/QDir>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialize resources for cppdevtk/test_service_controller subproject.
/// \remark Not in namespace.
void CppDevTkTestServiceControllerInitResources();


namespace cppdevtk {
namespace test_service_controller {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialize all resources for cppdevtk/test_service_controller subproject.
void InitResources();


}	// namespace test_service_controller
}	// namespace cppdevtk




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline void CppDevTkTestServiceControllerInitResources() {
#	ifndef CPPDEVTK_SHARED
	Q_INIT_RESOURCE(cppdevtk_test_service_controller);
	Q_INIT_RESOURCE(cppdevtk_test_service_controller_customization);
#	endif
}


namespace cppdevtk {
namespace test_service_controller {


inline void InitResources() {
	CppDevTkBaseInitResources();
	CppDevTkUtilInitResources();
	CppDevTkQtSolutionsQtCopyDialogInitResources();
	CppDevTkGuiInitResources();
	
	CppDevTkTestServiceControllerInitResources();
}


}	// namespace test_service_controller
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_SERVICE_CONTROLLER_INIT_RESOURCES_HPP_INCLUDED_
