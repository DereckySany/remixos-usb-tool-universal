/****************************************************************************
**
** Copyright (C) 2006-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtXMLPatterns module of the Qt Toolkit.
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
