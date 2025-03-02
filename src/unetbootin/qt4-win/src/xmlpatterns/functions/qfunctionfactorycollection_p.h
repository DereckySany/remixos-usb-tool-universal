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


#ifndef Patternist_FunctionFactoryCollection_H
#define Patternist_FunctionFactoryCollection_H

#include "qfunctionfactory_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * A FunctionFactoryCollection instance is a FunctionFactory in its own right,
     * but looks in its contained collection of factories for requested functions.
     *
     * @note the order of adding function libraries is significant.
     *
     * @ingroup Patternist_functions
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class Q_AUTOTEST_EXPORT FunctionFactoryCollection: public FunctionFactory
                                                     , public FunctionFactory::List
    {
    public:

        typedef QExplicitlySharedDataPointer<FunctionFactoryCollection> Ptr;

        /**
         * Creates a function call node.
         */
        virtual Expression::Ptr createFunctionCall(const QXmlName,
                                                   const Expression::List &arguments,
                                                   const StaticContext::Ptr &context,
                                                   const SourceLocationReflection *const r);
        virtual bool isAvailable(const NamePool::Ptr &np, const QXmlName name, const xsInteger arity);

        virtual FunctionSignature::Hash functionSignatures() const;

        virtual FunctionSignature::Ptr retrieveFunctionSignature(const NamePool::Ptr &np, const QXmlName name);

        /**
         * @return a FunctionFactory containing all core functions and constructor
         * functions required for XPath 2.0, plus Patternist's
         * implementation defined functions. The functions specified for XQuery 1.0
         * are the same as for XPath 2.0 so this FunctionFactory work well for XQuery
         * as well.
         */
        static FunctionFactory::Ptr xpath20Factory(const NamePool::Ptr &np);

        /**
         * @return a FunctionFactory containing all core functions required for XPath 1.0
         * plus Patternist's implementation defined functions.
         */
        static FunctionFactory::Ptr xpath10Factory();
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
