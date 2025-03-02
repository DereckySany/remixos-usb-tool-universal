/****************************************************************************
**
** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
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

/*!
    \headerfile <QtConcurrentFilter>
    \title Concurrent Filter and Filter-Reduce

    \brief The <QtConcurrentFilter> header provides concurrent Filter and
    Filter-Reduce.

    These functions are a part of the \l {threads.html#qtconcurrent-intro}{Qt Concurrent} framework.

    The QtConcurrent::filter(), QtConcurrent::filtered() and
    QtConcurrent::filteredReduced() functions filter items in a sequence such
    as a QList or a QVector in parallel. QtConcurrent::filter() modifies a
    sequence in-place, QtConcurrent::filtered() returns a new sequence
    containing the filtered content, and QtConcurrent::filteredReduced()
    returns a single result.

    \section1 Concurrent Filter

    QtConcurrent::filtered() takes an input sequence and a filter function.
    This filter function is then called for each item in the sequence, and a
    new sequence containing the filtered values is returned.

    The filter function must be of the form:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 0

    T must match the type stored in the sequence. The function returns true if
    the item should be kept, false if it should be discarded.

    This example shows how to keep strings that are all lower-case from a
    QStringList:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 1

    The results of the filter are made available through QFuture. See the
    QFuture and QFutureWatcher documentation for more information on how to
    use QFuture in your applications.

    If you want to modify a sequence in-place, use QtConcurrent::filter():

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 2

    Since the sequence is modified in place, QtConcurrent::filter() does not
    return any results via QFuture. However, you can still use QFuture and
    QFutureWatcher to monitor the status of the filter.

    \section1 Concurrent Filter-Reduce

    QtConcurrent::filteredReduced() is similar to QtConcurrent::filtered(),
    but instead of returing a sequence with the filtered results, the results
    are combined into a single value using a reduce function.

    The reduce function must be of the form:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 3

    T is the type of the final result, U is the type of items being filtered.
    Note that the return value and return type of the reduce function are not
    used.

    Call QtConcurrent::filteredReduced() like this:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 4

    The reduce function will be called once for each result kept by the filter
    function, and should merge the \e{intermediate} into the \e{result}
    variable. QtConcurrent::filteredReduced() guarantees that only one thread
    will call reduce at a time, so using a mutex to lock the result variable
    is not neccesary. The QtConcurrent::ReduceOptions enum provides a way to
    control the order in which the reduction is done.

    \section1 Additional API Features

    \section2 Using Iterators instead of Sequence

    Each of the above functions has a variant that takes an iterator range
    instead of a sequence. You use them in the same way as the sequence
    variants:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 5

    \section2 Blocking Variants

    Each of the above functions has a blocking variant that returns the final
    result instead of a QFuture. You use them in the same way as the
    asynchronous variants.

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 6

    Note that the result types above are not QFuture objects, but real result
    types (in this case, QStringList and QSet<QString>).

    \section2 Using Member Functions

    QtConcurrent::filter(), QtConcurrent::filtered(), and
    QtConcurrent::filteredReduced() accept pointers to member functions.
    The member function class type must match the type stored in the sequence:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 7

    Note that when using QtConcurrent::filteredReduced(), you can mix the use of
    normal and member functions freely:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 8

    \section2 Using Function Objects
    
    QtConcurrent::filter(), QtConcurrent::filtered(), and
    QtConcurrent::filteredReduced() accept function objects, which can be used to
    add state to a function call. The result_type typedef must define the 
    result type of the function call operator:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 13

    \section2 Using Bound Function Arguments

    Note that Qt does not provide support for bound functions. This is
    provided by 3rd party libraries like
    \l{http://www.boost.org/libs/bind/bind.html}{Boost} or
    \l{http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1836.pdf}
    {C++ TR1 Library Extensions}.

    If you want to use a filter function takes more than one argument, you can
    use boost::bind() or std::tr1::bind() to transform it onto a function that
    takes one argument.

    As an example, we use QString::contains():

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 9

    QString::contains() takes 2 arguments (including the "this" pointer) and
    can't be used with QtConcurrent::filtered() directly, because
    QtConcurrent::filtered() expects a function that takes one argument. To
    use QString::contains() with QtConcurrent::filtered() we have to provide a
    value for the \e regexp argument:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 10

    The return value from boost::bind() is a function object (functor) with
    the following signature:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 11

    This matches what QtConcurrent::filtered() expects, and the complete
    example becomes:

    \snippet doc/src/snippets/code/src.corelib.concurrent.qtconcurrentfilter.cpp 12
*/

/*!
    \fn QFuture<void> QtConcurrent::filter(Sequence &sequence, FilterFunction filterFunction)
    \relates <QtConcurrentFilter>

    Calls \a filterFunction once for each item in \a sequence. If
    \a filterFunction returns true, the item is kept in \a sequence;
    otherwise, the item is removed from \a sequence.
*/

/*!
    \fn QFuture<T> QtConcurrent::filtered(const Sequence &sequence, FilterFunction filterFunction)
    \relates <QtConcurrentFilter>

    Calls \a filterFunction once for each item in \a sequence and returns a
    new Sequence of kept items. If \a filterFunction returns true, a copy of
    the item is put in the new Sequence. Otherwise, the item will \e not
    appear in the new Sequence.
*/

/*!
    \fn QFuture<T> QtConcurrent::filtered(ConstIterator begin, ConstIterator end, FilterFunction filterFunction)
    \relates <QtConcurrentFilter>

    Calls \a filterFunction once for each item from \a begin to \a end and
    returns a new Sequence of kept items. If \a filterFunction returns true, a
    copy of the item is put in the new Sequence. Otherwise, the item will
    \e not appear in the new Sequence.
*/

/*!
    \fn QFuture<T> QtConcurrent::filteredReduced(const Sequence &sequence, FilterFunction filterFunction, ReduceFunction reduceFunction, QtConcurrent::ReduceOptions reduceOptions)
    \relates <QtConcurrentFilter>

    Calls \a filterFunction once for each item in \a sequence. If
    \a filterFunction returns true for an item, that item is then passed to
    \a reduceFunction. In other words, the return value is the result of
    \a reduceFunction for each item where \a filterFunction returns true.

    Note that while \a filterFunction is called concurrently, only one thread
    at a time will call \a reduceFunction. The order in which \a reduceFunction
    is called is undefined if \a reduceOptions is
    QtConcurrent::UnorderedReduce. If \a reduceOptions is
    QtConcurrent::OrderedReduce, \a reduceFunction is called in the order of
    the original sequence.
*/

/*!
    \fn QFuture<T> QtConcurrent::filteredReduced(ConstIterator begin, ConstIterator end, FilterFunction filterFunction, ReduceFunction reduceFunction, QtConcurrent::ReduceOptions reduceOptions)
    \relates <QtConcurrentFilter>

    Calls \a filterFunction once for each item from \a begin to \a end. If
    \a filterFunction returns true for an item, that item is then passed to
    \a reduceFunction. In other words, the return value is the result of
    \a reduceFunction for each item where \a filterFunction returns true.

    Note that while \a filterFunction is called concurrently, only one thread
    at a time will call \a reduceFunction. The order in which
    \a reduceFunction is called is undefined if \a reduceOptions is
    QtConcurrent::UnorderedReduce. If \a reduceOptions is
    QtConcurrent::OrderedReduce, the \a reduceFunction is called in the order
    of the original sequence.
*/
