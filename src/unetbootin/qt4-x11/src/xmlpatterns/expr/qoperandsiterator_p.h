/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtXMLPatterns module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Patternist_OperandsIterator_H
#define Patternist_OperandsIterator_H

#include <QPair>
#include <QStack>

#include "qexpression_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short A helper class that iterates a tree of Expression instances. It
     * is not a sub-class of QAbstractXmlForwardIterator.
     *
     * The OperandsIterator delivers all Expression instances that are children at any
     * depth of the Expression passed in the constructor.
     * The order is delivered in a defined way, from left to right and depth
     * first.
     *
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class OperandsIterator
    {
        /**
         * The second value, the int, is the current position in the first.
         */
        typedef QPair<Expression::List, int> Level;

    public:
        enum TreatParent
        {
            ExcludeParent,
            IncludeParent
        };

        /**
         * if @p treatParent is @c IncludeParent, @p start is excluded.
         *
         * @p start must be a valid Expression.
         */
        inline OperandsIterator(const Expression::Ptr &start,
                                const TreatParent treatParent)
        {
            Q_ASSERT(start);
            if(treatParent == IncludeParent)
            {
                Expression::List l;
                l.append(start);
                m_exprs.push(qMakePair(l, -1));
            }

            m_exprs.push(qMakePair(start->operands(), -1));
        }

        /**
         * @short Returns the current Expression and advances the iterator.
         *
         * If the end has been reached, a default constructed pointer is
         * returned.
         *
         * We intentionally return by reference.
         */
        inline Expression::Ptr next()
        {
            if(m_exprs.isEmpty())
                return Expression::Ptr();

            Level &lvl = m_exprs.top();
            ++lvl.second;

            if(lvl.second == lvl.first.size())
            {
                /* Resume iteration above us. */
                m_exprs.pop();

                if(m_exprs.isEmpty())
                    return Expression::Ptr();

                while(true)
                {
                    Level &previous = m_exprs.top();
                    ++previous.second;

                    if(previous.second < previous.first.count())
                    {
                        const Expression::Ptr &op = previous.first.at(previous.second);
                        m_exprs.push(qMakePair(op->operands(), -1));
                        return op;
                    }
                    else
                    {
                        // We have already reached the end of this level.
                        m_exprs.pop();
                        if(m_exprs.isEmpty())
                            return Expression::Ptr();
                    }
                }
            }
            else
            {
                const Expression::Ptr &op = lvl.first.at(lvl.second);
                m_exprs.push(qMakePair(op->operands(), -1));
                return op;
            }
        }

        /**
         * Advances this iterator by the current expression and its operands.
         */
        Expression::Ptr skipOperands()
        {
            if(m_exprs.isEmpty())
                return Expression::Ptr();

            Level &lvl = m_exprs.top();
            ++lvl.second;

            if(lvl.second == lvl.first.size())
            {
                /* We've reached the end of this level, at least. */
                m_exprs.pop();
            }

            return next();
        }

    private:
        Q_DISABLE_COPY(OperandsIterator)

        QStack<Level> m_exprs;
    };
}


QT_END_NAMESPACE
QT_END_HEADER

#endif
