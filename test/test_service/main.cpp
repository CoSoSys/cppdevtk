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
#include "http_service.hpp"
#include <cppdevtk/util/core_application.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/qiostream.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QtDebug>
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <QtCore/QSettings>
#include <QtCore/QDir>

#include <cstdlib>


#if (!CPPDEVTK_PLATFORM_ANDROID)
#define CPPDEVTK_COUT ::cppdevtk::base::qcout
#define CPPDEVTK_CERR ::cppdevtk::base::qcerr
#else
#define CPPDEVTK_COUT qDebug()
#define CPPDEVTK_CERR qCritical()
#endif


using ::cppdevtk::base::qcerr;
using ::cppdevtk::base::qcout;
using ::cppdevtk::base::Exception;
using ::std::exception;

using namespace ::cppdevtk::util;


#if (CPPDEVTK_PLATFORM_ANDROID)
__attribute__((visibility("default")))
#endif
int main(int argc, char* argv[]) try {
	::cppdevtk::test_service::InitResources();
	
#	if (CPPDEVTK_ENABLE_LOG_TO_FILE)
	const QString kLogFileName = ::cppdevtk::base::GetLogFileName();
	if (::cppdevtk::base::InstallLogFileMsgHandler(kLogFileName)) {
		CPPDEVTK_COUT << "Logs will be placed in file: " << QDir::toNativeSeparators(kLogFileName) << endl;
	}
	else {
		CPPDEVTK_CERR << "Failed to setup logging to file: " << QDir::toNativeSeparators(kLogFileName) << endl;
	}
#	endif
	
#	if (CPPDEVTK_PLATFORM_UNIX)
	// QtService stores service settings in SystemScope, which normally require root privileges.
	// To allow testing this example as non-root, we change the directory of the SystemScope settings file.
	QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
	CPPDEVTK_LOG_INFO("(Example uses dummy settings file: " << QDir::tempPath() << "/QtSoftware.conf");
	CPPDEVTK_COUT << "(Example uses dummy settings file: " << QDir::tempPath() << "/QtSoftware.conf";
#	endif
	
	::cppdevtk::util::CoreApplication::SetInfo(CPPDEVTK_COMPANY_SHORT_NAME_SANITIZED, CPPDEVTK_COMPANY_HOMEPAGE,
			CPPDEVTK_SHORT_NAME_SANITIZED, CPPDEVTK_VERSION_STRING, CPPDEVTK_TEST_SERVICE_SHORT_NAME_SANITIZED);
	
	::cppdevtk::test_service::HttpService httpService(argc, argv);
	
	return httpService.exec();
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
