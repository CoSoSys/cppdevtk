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


#ifndef CPPDEVTK_UTIL_SINGLE_CORE_APPLICATION_HPP_INCLUDED_
#define CPPDEVTK_UTIL_SINGLE_CORE_APPLICATION_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_ENABLE_QT_SOLUTIONS)
#error "This file require QtSolutions enabled!!!"
#endif

#include "core_application_base.hpp"
#include <cppdevtk/QtSolutions/QtSingleCoreApplication/QtSingleCoreApplication>


namespace cppdevtk {
namespace util {


class CPPDEVTK_UTIL_API SingleCoreApplication: public ::cppdevtk::qtsol::QtSingleCoreApplication, public CoreApplicationBase {
	friend class CoreApplicationBase;
	
	Q_OBJECT
public:
	/// \pre !organizationName().isEmpty()
	/// \pre !applicationName().isEmpty()
	/// \remark The application identifier will be GetUniqueId()
	SingleCoreApplication(int& argc, char** argv);
	SingleCoreApplication(int& argc, char** argv, const QString& id);
	virtual ~SingleCoreApplication();
public Q_SLOTS:
	/// \note We can not use ::cppdevtk::util::CoreApplicationBase::NotifyThrowAction here because enum must
	/// be registered to meta-object system and CoreApplicationBase does not inherit QObject, so we use int.
	void SetNotifyThrowAction(int notifyThrowAction);
private:
	Q_DISABLE_COPY(SingleCoreApplication)
	
	// private to prevent overriding in derived classes.
	virtual bool notify(QObject* pReceiver, QEvent* pEvent);
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline bool SingleCoreApplication::notify(QObject* pReceiver, QEvent* pEvent) {
	return CoreApplicationBase::notify(pReceiver, pEvent);
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_SINGLE_CORE_APPLICATION_HPP_INCLUDED_
