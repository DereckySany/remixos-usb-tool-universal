/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the qmake spec of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QPLATFORMDEFS_H
#define QPLATFORMDEFS_H

// ### TODO Following are only SDK specific defines.
// As StandardSDK has this one defined, we should avoid ifdefing the qfunctions_wce for specific mkspecs
// although it's ugly, let the mkspec to it
#define WINCE_SMARTPHONE

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

// Get Qt defines/settings

#include "qglobal.h"
#include "qfunctions_wince.h"

#define _POSIX_
#include <limits.h>
#undef _POSIX_

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define Q_FS_FAT
#ifdef QT_LARGEFILE_SUPPORT
#define QT_STATBUF		struct _stati64		// non-ANSI defs
#define QT_STATBUF4TSTAT	struct _stati64		// non-ANSI defs
#define QT_STAT			::_stati64
#define QT_FSTAT		::_fstati64
#else
#define QT_STATBUF		struct stat		// non-ANSI defs
#define QT_STATBUF4TSTAT	struct stat		// non-ANSI defs
#define QT_STAT			::stat
#define QT_FSTAT		::_fstat
#endif
#define QT_STAT_REG		_S_IFREG
#define QT_STAT_DIR		_S_IFDIR
#define QT_STAT_MASK		_S_IFMT
#if defined(_S_IFLNK)
#  define QT_STAT_LNK		_S_IFLNK
#endif
#define QT_FILENO		::__fileno
#define QT_OPEN			::open
#define QT_CLOSE		::_close
#ifdef QT_LARGEFILE_SUPPORT
#define QT_LSEEK		::_lseeki64
#define QT_TSTAT		::_tstati64
#else
#define QT_LSEEK		::_lseek
#define QT_TSTAT		::_tstat
#endif
#define QT_READ			::_read
#define QT_WRITE		::_write
#define QT_ACCESS		::_access
#define QT_GETCWD		::_getcwd
#define QT_CHDIR		::_chdir
#define QT_MKDIR		::_mkdir
#define QT_RMDIR		::_rmdir
#define QT_OPEN_RDONLY		_O_RDONLY
#define QT_OPEN_WRONLY		_O_WRONLY
#define QT_OPEN_RDWR		_O_RDWR
#define QT_OPEN_CREAT		_O_CREAT
#define QT_OPEN_TRUNC		_O_TRUNC
#define QT_OPEN_APPEND		_O_APPEND
# define QT_OPEN_TEXT		_O_TEXT
# define QT_OPEN_BINARY		_O_BINARY

#define QT_FOPEN                ::fopen
#define QT_FSEEK                ::fseek
#define QT_FTELL                ::ftell
#define QT_FGETPOS              ::fgetpos
#define QT_FSETPOS              ::fsetpos
#define QT_FPOS_T               fpos_t
#define QT_OFF_T                long

#define QT_SIGNAL_ARGS		int

#define QT_VSNPRINTF(buffer, count, format, arg) \
    _vsnprintf(buffer, count, format, arg)

#define QT_SNPRINTF		::_snprintf

# define F_OK	0
# define X_OK	1
# define W_OK	2
# define R_OK	4

typedef int mode_t;

#endif // QPLATFORMDEFS_H
