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


#include "config.hpp"
#include "init_resources.hpp"
#include "widget.hpp"
#include <cppdevtk/gui/invisible_widget.hpp>
#include <cppdevtk/gui/application.hpp>
#include <cppdevtk/gui/message_box.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/qiostream.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/info_tr.hpp>

#include <QtCore/QtDebug>
#include <QtCore/QString>
#include <QtCore/QDir>
#ifndef CPPDEVTK_SHARED
#	include <QtCore/QtPlugin>
#endif
#include <QtGui/QIcon>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
#	include <QtCore/Qt>
#endif

#include <cstdlib>


#ifndef CPPDEVTK_SHARED
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
Q_IMPORT_PLUGIN(QICOPlugin)
#else
Q_IMPORT_PLUGIN(qico)
#endif
#endif


#if (!CPPDEVTK_PLATFORM_ANDROID)
#define CPPDEVTK_COUT ::cppdevtk::base::qcout
#define CPPDEVTK_CERR ::cppdevtk::base::qcerr
#else
#define CPPDEVTK_COUT qDebug()
#define CPPDEVTK_CERR qCritical()
#endif


using ::cppdevtk::gui::Application;
using ::cppdevtk::gui::MessageBox;
using ::cppdevtk::base::qcerr;
using ::cppdevtk::base::Exception;
using ::std::exception;


#if (CPPDEVTK_PLATFORM_ANDROID)
__attribute__((visibility("default")))
#endif
int main(int argc, char* argv[]) try {
#	if (CPPDEVTK_ENABLE_LOG_TO_FILE)
	const bool kIsLogFileMsgHandlerInstalled = ::cppdevtk::base::InstallLogFileMsgHandler(::cppdevtk::base::GetLogFileName());
#	endif
	
	::cppdevtk::test_invisible_widget::InitResources();
	
	CPPDEVTK_ASSERT(Application::quitOnLastWindowClosed());
	Application::SetInfo(CPPDEVTK_COMPANY_SHORT_NAME_SANITIZED, CPPDEVTK_COMPANY_HOMEPAGE,
			CPPDEVTK_SHORT_NAME_SANITIZED, CPPDEVTK_VERSION_STRING, CPPDEVTK_TEST_INVISIBLE_WIDGET_SHORT_NAME_SANITIZED);
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
	Application::setAttribute(Qt::AA_EnableHighDpiScaling);
#	endif
	
	Application application(argc, argv);
	
	Application::setWindowIcon(QIcon(":/cppdevtk/test_invisible_widget/res/ico/application.ico"));
	
#	if (CPPDEVTK_ENABLE_LOG_TO_FILE)
	if (kIsLogFileMsgHandlerInstalled) {
		MessageBox::Information(application.GetDefaultWindow(), CPPDEVTK_TEST_INVISIBLE_WIDGET_SHORT_NAME,
				QString("Log file: %1").arg(QDir::toNativeSeparators(::cppdevtk::base::GetLogFileName())));
	}
	else {
		MessageBox::Critical(application.GetDefaultWindow(), CPPDEVTK_TEST_INVISIBLE_WIDGET_SHORT_NAME,
				"InstallLogFileMsgHandler() failed");
	}
#	endif
	
	try {
		::cppdevtk::gui::InvisibleWidget invisibleWidget;
		::cppdevtk::test_invisible_widget::Widget widget;
		
		invisibleWidget.show();
		widget.show();
		
		return application.exec();
	}
	catch (const exception& exc) {
		CPPDEVTK_LOG_ERROR("caught ::std::exception: " << Exception::GetDetailedInfo(exc));
		MessageBox::Critical(application.GetDefaultWindow(),
				CPPDEVTK_TEST_INVISIBLE_WIDGET_SHORT_NAME,
				QString("Caught exception: %1").arg(exc.what()), exc);
		
		CPPDEVTK_ASSERT((dynamic_cast<const ::cppdevtk::base::LogicException*>(&exc) == NULL)
				&& (dynamic_cast<const ::std::logic_error*>(&exc) == NULL));
		
		return EXIT_FAILURE;
	}
	catch (...) {
		CPPDEVTK_LOG_ERROR("caught unknown exception!");
		MessageBox::Critical(application.GetDefaultWindow(),
				CPPDEVTK_TEST_INVISIBLE_WIDGET_SHORT_NAME,
				QString("Caught unknown exception!"));
		
		return EXIT_FAILURE;
	}
}
catch (const exception& exc) {
	const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
			exc.what(), Exception::GetDetailedInfo(exc));
	CPPDEVTK_LOG_ERROR(kErrMsg);
	CPPDEVTK_CERR << "Error: " << kErrMsg << endl;
	
	CPPDEVTK_ASSERT((dynamic_cast<const ::cppdevtk::base::LogicException*>(&exc) == NULL)
			&& (dynamic_cast<const ::std::logic_error*>(&exc) == NULL));
	
	return EXIT_FAILURE;
}
catch (...) {
	const QString kErrMsg("caught unknown exception!!!");
	CPPDEVTK_LOG_ERROR(kErrMsg);
	CPPDEVTK_CERR << "Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}
