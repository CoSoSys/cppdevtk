#****************************************************************************************************************************
# Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Please see the file COPYING.
#
# Author(s): Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
#****************************************************************************************************************************


QT -= gui
QT *= core
android {
	QT *= androidextras
}


TEMPLATE = app


include(./test_config_customization.pri)
include(./../../common.pri)


# CONFIG
CONFIG -= app_bundle
CONFIG *= console
CONFIG *= link_prl


# TARGET + INSTALLS

TEMPLATE += cppdevtkphonylib
cppdevtk_enable_target_suffix_qt_major_version {
	CPPDEVTK_UTIL_TARGET = $${qtLibraryTarget($${CPPDEVTK_UTIL_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET}_qt$${QT_MAJOR_VERSION})}
}
else {
	CPPDEVTK_UTIL_TARGET = $${qtLibraryTarget($${CPPDEVTK_UTIL_TARGET})}
	CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET})}
}
TEMPLATE -= cppdevtkphonylib

!android:!ios {
	target.path = $${CPPDEVTK_BIN_DIR}
	INSTALLS += target
}


# LIBS + PRE_TARGETDEPS
!debug_and_release|build_pass {
	LIBS += -l$${CPPDEVTK_UTIL_TARGET} -l$${CPPDEVTK_BASE_TARGET}
	!static_and_shared|build_pass {
		unix {
			cppdevtk_enable_android_destdir_workaround {
				PRE_TARGETDEPS += $${OUT_PWD}/../../src/util/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				PRE_TARGETDEPS += $${OUT_PWD}/../../src/base/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
		}
		else {
			win32 {
				PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				error("Unsupported platform!!!")
			}
		}
	}
}


# Strip
!debug_and_release|build_pass {
	CONFIG(release, debug|release) {
		unix {
			isEqual(CPPDEVTK_ENABLE_STRIP_APP, "true") {
				!static_and_shared|build_pass {
					QMAKE_POST_LINK += $${CPPDEVTK_STRIP_APP_CMD} $${DESTDIR}/$${TARGET}
				}
			}
		}
	}
}




# Files


RESOURCES += cppdevtk_test_config.qrc	\
	cppdevtk_test_config_customization.qrc

win32 {
	RC_FILE = cppdevtk_test_config.rc
	HEADERS += cppdevtk_test_config_res_win.hpp
}


HEADERS += info.hpp \
    init_resources.hpp \
    config.hpp


SOURCES += main.cpp