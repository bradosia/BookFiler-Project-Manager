/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILESYSTEM_SIGNAL_H
#define FSDB_FILESYSTEM_SIGNAL_H

/*
 * determine the event change:
 * POSIX: read
 * WINAPI: ReadDirectoryChangesW
 */

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) ||     \
    defined(__APPLE__)
#include "FilesystemSignalApple.hpp"
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "FilesystemSignalWin.hpp"
#else
#include "FilesystemSignalPosix.hpp"
#endif

#endif // FSDB_FILESYSTEM_SIGNAL_H
