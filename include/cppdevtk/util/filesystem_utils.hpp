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


#ifndef CPPDEVTK_UTIL_FILESYSTEM_UTILS_HPP_INCLUDED_
#define CPPDEVTK_UTIL_FILESYSTEM_UTILS_HPP_INCLUDED_


/// Cross-platform \c NAME_MAX
/// \attention Does not include the terminating null.
#define CPPDEVTK_NAME_MAX 0
#undef CPPDEVTK_NAME_MAX

/// Cross-platform \c PATH_MAX
/// \attention Does include the terminating null.
#define CPPDEVTK_PATH_MAX 0
#undef CPPDEVTK_PATH_MAX


#include "config.hpp"
#include "filesystem_exception.hpp"
#include "no_such_file_or_directory_exception.hpp"
#if (CPPDEVTK_PLATFORM_WINDOWS)
#include <windows.h>
#undef DeleteFile
#undef CopyFile
#undef RemoveDirectory
#endif
#if (CPPDEVTK_PLATFORM_UNIX)
#	include "filesystem_utils_unx.hpp"
#	if (CPPDEVTK_PLATFORM_LINUX)
#		include "filesystem_utils_lnx.hpp"
#	elif (CPPDEVTK_PLATFORM_MACOSX)
#		include "filesystem_utils_mac.hpp"
#	else
#		error "Unsupported Unix platform!!!"
#	endif
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	include "filesystem_utils_win.hpp"
#else
#	error "Unsupported platform!!!"
#endif
#include <cppdevtk/base/cstdint.hpp>
#include <cppdevtk/base/static_assert.hpp>

#include <QtCore/QString>
#include <QtCore/QLocale>

#include <ios>


namespace cppdevtk {
namespace util {


/// File size type (signed) (::std::streamsize is 32 bits instead of 64 bits on many 32 bits platforms...).
typedef ::std::streamoff fsize_t;
CPPDEVTK_STATIC_ASSERT(sizeof(fsize_t) == sizeof(int64_t));

/// File offset type (signed)
typedef ::std::streamoff foff_t;
CPPDEVTK_STATIC_ASSERT(sizeof(foff_t) == sizeof(int64_t));


struct FileSystemSpaceInfo {
	// Std file size is signed, but:
	// - Ms GetDiskFreeSpaceEx() is using ULARGE_INTEGER
	// - Unix statvfs's f_bsize, f_frsize, f_blocks, f_bfree and f_bavail are unsigned long
	typedef ::cppdevtk::base::uint64_t size_type;
	
	
	size_type size_;
	size_type totalFree_;
	size_type userFree_;
};


/// \return \c true if path is not empty, is in Qt format and does not include invalid characters, \c false otherwise.
CPPDEVTK_UTIL_API bool IsValidPath(const QString& path, bool ignorePathSeparator = true);

/// \pre IsValidPath()
/// \throw ::cppdevtk::util::FilesystemException
/// \remarks
/// - \c EINTR is handled on Unix
/// - \c FILE_ATTRIBUTE_READONLY is handled on Windows
/// - \c "\\?\" is handled on Windows + Unicode
CPPDEVTK_UTIL_API void DeleteFile(const QString& fileName, bool failIfNotExists = false);
CPPDEVTK_UTIL_API bool DeleteFile(const QString& fileName, bool failIfNotExists, ::cppdevtk::base::ErrorCode& errorCode);

/// \pre IsValidPath()
/// \throw ::cppdevtk::util::FilesystemException
/// \remarks
/// - \c EINTR is handled on Unix
/// - \c FILE_ATTRIBUTE_HIDDEN and \c FILE_ATTRIBUTE_READONLY are handled on Windows
/// - \c "\\?\" is handled on Windows + Unicode
CPPDEVTK_UTIL_API void CopyFile(const QString& srcFileName, const QString& dstFileName, bool failIfExists = true);
CPPDEVTK_UTIL_API bool CopyFile(const QString& srcFileName, const QString& dstFileName, bool failIfExists,
		::cppdevtk::base::ErrorCode& errorCode);

/// \pre IsValidPath()
/// \throw ::cppdevtk::util::FilesystemException
/// \remark Does not create intermediate directories.
CPPDEVTK_UTIL_API void MakeDirectory(const QString& dirName, bool failIfExists = true);
CPPDEVTK_UTIL_API bool MakeDirectory(const QString& dirName, bool failIfExists,
		::cppdevtk::base::ErrorCode& errorCode);

/// \pre IsValidPath()
/// \throw ::cppdevtk::util::FilesystemException
/// \remark Does create intermediate directories.
CPPDEVTK_UTIL_API void MakePath(const QString& dirPath, bool failIfExists = true);
CPPDEVTK_UTIL_API bool MakePath(const QString& dirPath, bool failIfExists,
		::cppdevtk::base::ErrorCode& errorCode);

/// \pre IsValidPath()
/// \throw ::cppdevtk::util::FilesystemException
/// \remark Directory must be empty
CPPDEVTK_UTIL_API void RemoveDirectory(const QString& path, bool failIfNotExists = false);
CPPDEVTK_UTIL_API bool RemoveDirectory(const QString& path, bool failIfNotExists,
		::cppdevtk::base::ErrorCode& errorCode);

/// \brief Removes the directory, including all its contents.
/// \pre IsValidPath()
/// \note If a file or directory cannot be removed, it keeps going and attempts to delete as many files
/// and sub-directories as possible, then returns \c false.
/// \remarks
/// - \c EINTR is handled on Unix
/// - \c FILE_ATTRIBUTE_READONLY is handled on Windows
/// - \c "\\?\" is handled on Windows + Unicode
CPPDEVTK_UTIL_API bool RemoveDirectoryRecursively(const QString& path, bool failIfNotExists,
		::cppdevtk::base::ErrorCode& errorCode);

/// \brief Copy the directory, including all its contents.
/// \pre IsValidPath()
/// \note If a file or directory cannot be copied, it keeps going and attempts to copy as many files
/// and sub-directories as possible, then returns \c false.
/// \remarks
/// - \c EINTR is handled on Unix
/// - \c FILE_ATTRIBUTE_HIDDEN and \c FILE_ATTRIBUTE_READONLY are handled on Windows
/// - \c "\\?\" is handled on Windows + Unicode
CPPDEVTK_UTIL_API bool CopyDirectoryRecursively(const QString& srcPath, const QString& dstPath, bool failIfDstPathExists,
		bool failIfFileExists, ::cppdevtk::base::ErrorCode& errorCode);




/// \return
/// - Drives on Windows (ex: C:/, D:/)
/// - mount point on Unix (ex: /, /home/)
/// - empty list if no error and no mount point found
/// \throw ::cppdevtk::util::FilesystemException
/// \note
/// - Mount points ends with '/'
/// - On Windows drive letters are uppercase
CPPDEVTK_UTIL_API QStringList GetMountPoints(bool ignoreSpecialFileSystems = true);
CPPDEVTK_UTIL_API QStringList GetMountPoints(bool ignoreSpecialFileSystems, ::cppdevtk::base::ErrorCode& errorCode);

/// \pre IsValidPath()
/// \return
/// - Drive on Windows (ex: C:/)
/// - mount point on Unix (ex: /home/)
/// - empty list if no error and no mount point found
/// \throw ::cppdevtk::util::FilesystemException
/// \note
/// - Returned mount point ends with '/'
/// - On Windows drive letter is uppercase
CPPDEVTK_UTIL_API QStringList GetMountPointsFromPath(const QString& path);
CPPDEVTK_UTIL_API QStringList GetMountPointsFromPath(const QString& path, ::cppdevtk::base::ErrorCode& errorCode);




/// \pre IsValidPath()
/// \throw ::cppdevtk::util::FilesystemException
CPPDEVTK_UTIL_API void GetFileSystemSpaceInfo(const QString& path, FileSystemSpaceInfo& fileSystemSpaceInfo);
CPPDEVTK_UTIL_API bool GetFileSystemSpaceInfo(const QString& path, FileSystemSpaceInfo& fileSystemSpaceInfo,
		::cppdevtk::base::ErrorCode& errorCode);


/// \pre IsValidPath()
CPPDEVTK_UTIL_API bool ReadTextFile(const QString& textFilePath, QString& textFileContent);

/// \return localized filename, empty if fails.
CPPDEVTK_UTIL_API QString GetLocalizedFileName(const QString& fileNamePrefix, const QString& fileExt,
		const QLocale& locale);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_FILESYSTEM_UTILS_HPP_INCLUDED_
