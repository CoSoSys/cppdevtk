#****************************************************************************************************************************
# Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>
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


CPPDEVTK_PACKAGE_NAME = cppdevtk


# CppDevTk specific CONFIG options
CONFIG += cppdevtk_verbose
CONFIG += cppdevtk_enable_debuginfo_in_release
#CONFIG += cppdevtk_enable_target_suffix_qt_major_version
#CONFIG += cppdevtk_enable_app_target_debug_suffix
CONFIG += cppdevtk_disable_warnings
# TODO: keep in sync with CPPDEVTK_WITH_ZLIB in config/features.hpp
CONFIG += cppdevtk_with_zlib
win32 {
	CONFIG += targetxp
}


# TODO: keep in sync with CPPDEVTK_ENABLE_QT_SOLUTIONS in config/features.hpp
CPPDEVTK_ENABLE_QTSOLUTIONS = true


# target OS version
# TODO: keep in sync with features.hpp
unix {
	linux* {
		# nothing needed
	}
	else {
		macx {
			*g++* {
				CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED = 1050
				CPPDEVTK_MACOSX_DEPLOYMENT_TARGET = 10.5
			}
			else {
				*clang* {
					CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED = 101100
					CPPDEVTK_MACOSX_DEPLOYMENT_TARGET = 10.11
				}
				else {
					error("Unsupported compiler for Mac OS X platform!!!")
				}
			}
			CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED = $${CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED}
		}
		else {
			ios {
				CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED = 80000
				CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED = $${CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED}
				CPPDEVTK_IOS_DEPLOYMENT_TARGET = 8.0
			}
			else {
				error("Unsupported Unix platform!!!")
			}
		}
	}
}
else {
	win32 {
		CPPDEVTK_MSC_VER = $$find(QMAKE_COMPILER_DEFINES, _MSC_VER=.*)
		CPPDEVTK_MSC_VER = $$replace(CPPDEVTK_MSC_VER, "_MSC_VER=", "")
		isEmpty(CPPDEVTK_MSC_VER) {
			error("Qt must set _MSC_VER in QMAKE_COMPILER_DEFINES")
		}
		if(isEqual(CPPDEVTK_MSC_VER, "1700")|greaterThan(CPPDEVTK_MSC_VER, 1700)):!contains(DEFINES, _USING_V110_SDK71_) {
			!targetxp {
				CPPDEVTK_WIN32_WINNT = 0x0601
				CPPDEVTK_NTDDI_VERSION = 0x06010000
				CPPDEVTK_WINVER = $${CPPDEVTK_WIN32_WINNT}
				CPPDEVTK_WIN32_IE = 0x0800
			}
			else {
				DEFINES += _USING_V110_SDK71_
				CPPDEVTK_WIN32_WINNT = 0x0501
				CPPDEVTK_NTDDI_VERSION = 0x05010300
				CPPDEVTK_WINVER = $${CPPDEVTK_WIN32_WINNT}
				CPPDEVTK_WIN32_IE = 0x0603
			}
		}
		else {
			CPPDEVTK_WIN32_WINNT = 0x0501
			CPPDEVTK_NTDDI_VERSION = 0x05010300
			CPPDEVTK_WINVER = $${CPPDEVTK_WIN32_WINNT}
			CPPDEVTK_WIN32_IE = 0x0603
		}
	}
	else {
		error("Unsupported platform!!!")
	}
}

# compiler flags
*g++* {
	cppdevtk_enable_debuginfo_in_release {
		QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -g2
		QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -g2
		
		QMAKE_CFLAGS_RELEASE *= $${QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO}
		QMAKE_CXXFLAGS_RELEASE *= $${QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO}
	}
	else {
		QMAKE_CFLAGS_RELEASE *= -O2 -g2
		QMAKE_CXXFLAGS_RELEASE *= -O2 -g2
	}
}
else {
	*clang* {
		cppdevtk_enable_debuginfo_in_release {
			QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -g2
			QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -g2
			
			QMAKE_CFLAGS_RELEASE *= $${QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO}
			QMAKE_CXXFLAGS_RELEASE *= $${QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO}
		}
		else {
			QMAKE_CFLAGS_RELEASE *= -O2 -g2
			QMAKE_CXXFLAGS_RELEASE *= -O2 -g2
		}
	}
	else {
		*msvc* {
			cppdevtk_enable_debuginfo_in_release {
				QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO = -Zi
				QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO = -Zi
				QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO = /debugtype:cv
			}
		}
		else {
			error("Unsupported compiler!!!")
		}
	}
}

# CPPDEVTK_PREFIX
# Ex:
# linux: /usr/local/(static)
# android: C:/local-android/arch (/usr/local-android/arch)
# mac: /usr/local/arch/(static)
# iphonesimulator: /usr/local-ios/iphonesimulator
# iphoneos: /usr/local-ios/iphoneos
# windows: C:/local/qtver/compiler/arch/(static)

isEmpty(CPPDEVTK_PREFIX) {
	unix {
		linux* {
			android {
				isEqual(QMAKE_HOST.os, "Windows") {
					CPPDEVTK_PREFIX = /usr/local-android
				}
				else {
					CPPDEVTK_PREFIX = /usr/local-android
				}
			}
			else {
				CPPDEVTK_PREFIX = /usr/local
			}
		}
		else {
			macx {
				CPPDEVTK_PREFIX = /usr/local
			}
			else {
				ios {
					CPPDEVTK_PREFIX = /usr/local-ios
					CONFIG(iphonesimulator, iphonesimulator|iphoneos) {
						CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/iphonesimulator
					}
					else {
						CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/iphoneos
					}
				}
				else {
					error("Unsupported Unix platform!!!")
				}
			}
		}
	}
	else {
		win32 {
			CPPDEVTK_PREFIX = C:/local
		}
		else {
			error("Unsupported platform!!!")
		}
	}
	
	# qtver
	win32 {
		CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/qt$${QT_MAJOR_VERSION}
	}
	
	# compiler
	win32 {
		greaterThan(QT_MAJOR_VERSION, 4) {	# QMAKESPEC does not work on Qt 4
			isEmpty(QMAKESPEC) {
				error("QMAKESPEC is empty")
			}
			CPPDEVTK_QMAKESPEC_BASENAME = $$basename(QMAKESPEC)
			CPPDEVTK_QMAKESPEC_BASENAME_SPLIT = $$split(CPPDEVTK_QMAKESPEC_BASENAME, "-")
			CPPDEVTK_COMPILER = $$member(CPPDEVTK_QMAKESPEC_BASENAME_SPLIT, 1)
			isEmpty(CPPDEVTK_COMPILER) {
				error("CPPDEVTK_COMPILER is empty")
			}
			CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/$${CPPDEVTK_COMPILER}
		}
	}
	
	# arch
	unix {
		linux* {
			android {
				isEmpty(QT_ARCH) {
					error("QT_ARCH is empty!!!")
				}
				CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/$${QT_ARCH}
			}
			else {
				# we do not use arch on linux because we do not cross-compile (i586 on x86_64)
			}
		}
		else {
			macx {
				# QT_ARCH does not work in Qt 4 on mac.
				greaterThan(QT_MAJOR_VERSION, 4) {
					isEmpty(QT_ARCH) {
						error("QT_ARCH is empty!!!")
					}
					CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/$${QT_ARCH}
				}
				else {
					x86 {
						CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/x86
					}
					else {
						x86_64 {
							CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/x86_64
						}
						else {
							error("arch missing from CONFIG!!!")
						}
					}
				}
			}
			else {
				ios {
					# arch not used
				}
				else {
					error("Unsupported Unix platform!!!")
				}
			}
		}
	}
	else {
		win32 {
			# QT_ARCH does not work in Qt 4 on Windows; use QMAKE_TARGET.arch
			isEmpty(QMAKE_TARGET.arch) {
				error("QMAKE_TARGET.arch is empty!!!")
			}
			CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/$${QMAKE_TARGET.arch}
		}
		else {
			error("Unsupported platform!!!")
		}
	}

	# link
	!android:!ios {
		!static_and_shared|build_pass {
			CONFIG(static, static|shared) {
				CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/static
			}
		}
	}
}

# third-party
unix {
	linux* {
		android {
			CONFIG += cppdevtk_have_third_party
		}
		else {
			!static_and_shared|build_pass {
				CONFIG(static, static|shared) {
					#CONFIG += cppdevtk_have_third_party
				}
				else {
					#CONFIG += cppdevtk_have_third_party
				}
			}
		}
	}
	else {
		macx {
			!static_and_shared|build_pass {
				CONFIG(static, static|shared) {
					CONFIG += cppdevtk_have_third_party
				}
				else {
					CONFIG += cppdevtk_have_third_party
				}
			}
		}
		else {
			ios {
				CONFIG += cppdevtk_have_third_party
			}
			else {
				error("Unsupported Unix platform!!!")
			}
		}
	}
	
	cppdevtk_have_third_party {
		isEmpty(CPPDEVTK_THIRD_PARTY_PREFIX) {
			android:isEqual(QMAKE_HOST.os, "Windows") {
					CPPDEVTK_THIRD_PARTY_PREFIX = $$replace(CPPDEVTK_PREFIX, "/opt/", "C:/")
			}
			else {
				CPPDEVTK_THIRD_PARTY_PREFIX = $${CPPDEVTK_PREFIX}
			}
		}
	}
}
else {
	CONFIG += cppdevtk_have_third_party
	
	win32 {
		cppdevtk_have_third_party {
			isEmpty(CPPDEVTK_THIRD_PARTY_PREFIX) {
				CPPDEVTK_THIRD_PARTY_PREFIX = $${CPPDEVTK_PREFIX}
			}
		}
	}
	else {
		error("Unsupported platform!!!")
	}
}
