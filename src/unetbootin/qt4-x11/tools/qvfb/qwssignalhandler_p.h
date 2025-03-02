/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWSSIGNALHANDLER_P_H
#define QWSSIGNALHANDLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qglobal.h>

#ifndef QT_NO_QWS_SIGNALHANDLER

#include <QtCore/qmap.h>
#include <QtCore/qvector.h>
#include <QtCore/qobjectcleanuphandler.h>

QT_BEGIN_NAMESPACE

typedef void (*qt_sighandler_t)(int);

class QWSSignalHandlerPrivate;

class Q_GUI_EXPORT QWSSignalHandler
{
public:
    static QWSSignalHandler* instance();

    ~QWSSignalHandler();

#ifndef QT_NO_QWS_MULTIPROCESS
    inline void addSemaphore(int semno) { semaphores.append(semno); }
    void removeSemaphore(int semno);
#endif
    inline void addObject(QObject *object) { (void)objects.add(object); }

private:
    QWSSignalHandler();
    static void handleSignal(int signal);
    QMap<int, qt_sighandler_t> oldHandlers;
#ifndef QT_NO_QWS_MULTIPROCESS
    QVector<int> semaphores;
#endif
    QObjectCleanupHandler objects;

    friend class QWSSignalHandlerPrivate;
};

QT_END_NAMESPACE

#endif // QT_NO_QWS_SIGNALHANDLER

#endif // QWSSIGNALHANDLER_P_H
