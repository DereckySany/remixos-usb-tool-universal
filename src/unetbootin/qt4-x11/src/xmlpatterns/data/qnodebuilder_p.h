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

#ifndef Patternist_NodeBuilder_H
#define Patternist_NodeBuilder_H

#include "qitem_p.h"
#include "qabstractxmlreceiver.h"
#include "qautoptr_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short Receives QAbstractXmlReceiver events and builds a node tree
     * in memory that afterwards can be retrieved via builtNode()
     *
     * @ingroup Patternist_xdm
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class NodeBuilder : public QAbstractXmlReceiver
    {
    public:
        typedef AutoPtr<NodeBuilder> Ptr;

        inline NodeBuilder()
        {
        }

        /**
         * @short Returns the document that has been built.
         *
         * If this function is called before any events have been received, the result is undefined.
         *
         * The top node that was constructed can be retrieved by calling
         * NodeModel::root() on the returned NodeModel.
         *
         * This function is not @c const, because some implementations delay
         * the node construction until the node is needed. Also, text nodes are
         * difficult, at best, to construct until one knows that all text content
         * has been received(which a call to this function in a natural way
         * signals).
         */
        virtual QAbstractXmlNodeModel::Ptr builtDocument() = 0;

        /**
         * @short Creates a copy of this NodeBuilder, that operates independently of
         * this NodeBuilder.
         *
         * The caller owns the returned instance.
         */
        virtual NodeBuilder::Ptr create(const QUrl &baseURI) const = 0;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
