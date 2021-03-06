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


#ifndef CPPDEVTK_GUI_APPLICATION_HPP_INCLUDED_
#define CPPDEVTK_GUI_APPLICATION_HPP_INCLUDED_


#include "config.hpp"
#include "application_base.hpp"

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


namespace cppdevtk {
namespace gui {


class CPPDEVTK_GUI_API Application: public QApplication, public ApplicationBase {
	friend class ApplicationBase;
	
	Q_OBJECT
public:
	Application(int& argc, char** argv);
	virtual ~Application();
public Q_SLOTS:
	/// \note We can not use ::cppdevtk::util::CoreApplicationBase::NotifyThrowAction here because enum must
	/// be registered to meta-object system and CoreApplicationBase does not inherit QObject, so we use int.
	void SetNotifyThrowAction(int notifyThrowAction);
private:
	Q_DISABLE_COPY(Application)
	
	// private to prevent overriding in derived classes.
	virtual bool notify(QObject* pReceiver, QEvent* pEvent);
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline bool Application::notify(QObject* pReceiver, QEvent* pEvent) {
	return ApplicationBase::notify(pReceiver, pEvent);
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_APPLICATION_HPP_INCLUDED_
