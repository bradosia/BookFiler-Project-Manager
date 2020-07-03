/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__APPLE__)
#include "FilesystemSignalApple.cpp"
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "FilesystemSignalWin.cpp"
#else
#include "FilesystemSignalPosix.cpp"
#endif