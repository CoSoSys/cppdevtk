/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_TEST_LOCALIZATION_APPLICATION_HPP_INCLUDED_
#define CPPDEVTK_TEST_LOCALIZATION_APPLICATION_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/gui/single_application.hpp>

#include <QtCore/QTranslator>


namespace cppdevtk {
namespace test_localization {


class Application: public ::cppdevtk::gui::SingleApplication {
	Q_OBJECT
public:
	Application(int& argc, char** argv);
	
	
	static void SetQmInfo();
	static void SetInfo();
protected:
	virtual bool SetupTranslators();
private:
	Q_DISABLE_COPY(Application)
	
	
	QTranslator translator_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline void Application::SetQmInfo() {
	SingleApplication::SetQmInfo(":/cppdevtk/test_localization/res/tr", "tr_");
}

inline void Application::SetInfo() {
	::cppdevtk::gui::SingleApplication::SetInfo(CPPDEVTK_COMPANY_SHORT_NAME_SANITIZED, CPPDEVTK_COMPANY_HOMEPAGE,
			(CPPDEVTK_SHORT_NAME_SANITIZED + "_" + CPPDEVTK_TEST_LOCALIZATION_NAME_SANITIZED), CPPDEVTK_VERSION_STRING);
}


}	// namespace test_localization
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_LOCALIZATION_APPLICATION_HPP_INCLUDED_
