/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtScriptTools module of the Qt Toolkit.
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

#ifndef QSCRIPTEDIT_P_H
#define QSCRIPTEDIT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGui/qplaintextedit.h>

#include <QtCore/qhash.h>
#include <QtCore/qset.h>

QT_BEGIN_NAMESPACE

class QScriptEditExtraArea;

class Q_AUTOTEST_EXPORT QScriptEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    QScriptEdit(QWidget *parent = 0);
    ~QScriptEdit();

    int baseLineNumber() const;
    void setBaseLineNumber(int base);

    int executionLineNumber() const;
    void setExecutionLineNumber(int lineNumber, bool error);
    void setExecutableLineNumbers(const QSet<int> &lineNumbers);

    int currentLineNumber() const;
    void gotoLine(int lineNumber);

    void setBreakpoint(int lineNumber);
    void setBreakpointEnabled(int lineNumber, bool enable);
    void deleteBreakpoint(int lineNumber);

    int extraAreaWidth() const;

signals:
    void breakpointToggleRequest(int lineNumber, bool on);
    void breakpointEnableRequest(int lineNumber, bool enable);

protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);

    void extraAreaPaintEvent(QPaintEvent *e);
    void extraAreaMouseEvent(QMouseEvent *e);
    bool extraAreaEvent(QEvent *e);

private slots:
    void updateExtraAreaWidth();
    void updateExtraArea(const QRect &, int);
    void highlightCurrentLine();

private:
    QTextEdit::ExtraSelection currentLineSelection() const;
    QTextEdit::ExtraSelection currentExecutionLineSelection() const;
    void updateExtraSelections();

private:
    QScriptEditExtraArea *m_extraArea;
    int m_baseLineNumber;
    int m_executionLineNumber;
    QSet<int> m_executableLineNumbers;
    bool m_executionLineNumberHasError;
    int m_extraAreaToggleBlockNumber;

    struct BreakpointData {
        BreakpointData() : enabled(true) {}
        bool enabled;
    };
    QHash<int, BreakpointData> m_breakpoints;

private:
    Q_DISABLE_COPY(QScriptEdit)
    friend class QScriptEditExtraArea;
};

QT_END_NAMESPACE

#endif
