/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QDATETIMEEDIT_P_H
#define QDATETIMEEDIT_P_H

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

#include "QtGui/qcombobox.h"
#include "QtGui/qcalendarwidget.h"
#include "QtGui/qspinbox.h"
#include "QtGui/qtoolbutton.h"
#include "QtGui/qmenu.h"
#include "QtGui/qlabel.h"
#include "QtGui/qdatetimeedit.h"
#include "QtGui/private/qabstractspinbox_p.h"
#include "QtCore/private/qdatetime_p.h"

#include "qdebug.h"

#ifndef QT_NO_DATETIMEEDIT

QT_BEGIN_NAMESPACE

class QCalendarPopup;
class QDateTimeEditPrivate : public QAbstractSpinBoxPrivate, public QDateTimeParser
{
    Q_DECLARE_PUBLIC(QDateTimeEdit)
public:
    QDateTimeEditPrivate();

    void init(const QVariant &var);
    void readLocaleSettings();

    void emitSignals(EmitPolicy ep, const QVariant &old);
    QString textFromValue(const QVariant &f) const;
    QVariant valueFromText(const QString &f) const;
    virtual void _q_editorCursorPositionChanged(int oldpos, int newpos);
    virtual void interpret(EmitPolicy ep);
    virtual void clearCache() const;

    QDateTime validateAndInterpret(QString &input, int &, QValidator::State &state,
                                   bool fixup = false) const;
    void clearSection(int index);
    virtual QString displayText() const { return edit->displayText(); } // this is from QDateTimeParser

    int absoluteIndex(QDateTimeEdit::Section s, int index) const;
    int absoluteIndex(const SectionNode &s) const;
    void updateEdit();
    QDateTime stepBy(int index, int steps, bool test = false) const;
    int sectionAt(int pos) const;
    int closestSection(int index, bool forward) const;
    int nextPrevSection(int index, bool forward) const;
    void setSelected(int index, bool forward = false);

    void updateCache(const QVariant &val, const QString &str) const;

    void updateTimeSpec();
    virtual QDateTime getMinimum() const { return minimum.toDateTime(); }
    virtual QDateTime getMaximum() const { return maximum.toDateTime(); }
    virtual QLocale locale() const { return q_func()->locale(); }
    QString valueToText(const QVariant &var) const { return textFromValue(var); }
    QString getAmPmText(AmPm ap, Case cs) const;
    int cursorPosition() const { return edit ? edit->cursorPosition() : -1; }

    virtual QStyle::SubControl newHoverControl(const QPoint &pos);
    virtual void updateEditFieldGeometry();
    virtual QVariant getZeroVariant() const;
    virtual void setRange(const QVariant &min, const QVariant &max);

    void _q_resetButton();
    void updateArrow(QStyle::StateFlag state);
    bool calendarPopupEnabled() const;
    void syncCalendarWidget();

    bool isSeparatorKey(const QKeyEvent *k) const;

    static QDateTimeEdit::Sections convertSections(QDateTimeParser::Sections s);
    static QDateTimeEdit::Section convertToPublic(QDateTimeParser::Section s);

    void initCalendarPopup(QCalendarWidget *cw = 0);
    void positionCalendarPopup();

    QDateTimeEdit::Sections sections;
    mutable bool cacheGuard;

    QString defaultDateFormat, defaultTimeFormat, defaultDateTimeFormat, unreversedFormat;
    Qt::LayoutDirection layoutDirection;
    mutable QVariant conflictGuard;
    bool hasHadFocus, formatExplicitlySet, calendarPopup;
    QStyle::StateFlag arrowState;
    QCalendarPopup *monthCalendar;

#ifdef QT_KEYPAD_NAVIGATION
    bool focusOnButton;
#endif
};


class QCalendarPopup : public QWidget
{
    Q_OBJECT
public:
    QCalendarPopup(QWidget *parent = 0, QCalendarWidget *cw = 0);
    QDate selectedDate() { return calendar->selectedDate(); }
    void setDate(const QDate &date);
    void setDateRange(const QDate &min, const QDate &max);
    void setFirstDayOfWeek(Qt::DayOfWeek dow) { calendar->setFirstDayOfWeek(dow); }
    QCalendarWidget *calendarWidget() const { return calendar; }
    void setCalendarWidget(QCalendarWidget *cw);
Q_SIGNALS:
    void activated(const QDate &date);
    void newDateSelected(const QDate &newDate);
    void hidingCalendar(const QDate &oldDate);
    void resetButton();

private Q_SLOTS:
    void dateSelected(const QDate &date);
    void dateSelectionChanged();

protected:
    void hideEvent(QHideEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool event(QEvent *e);

private:
    QCalendarWidget *calendar;
    QDate oldDate;
    bool dateChanged;
};

QT_END_NAMESPACE

#endif // QT_NO_DATETIMEEDIT

#endif // QDATETIMEEDIT_P_H
