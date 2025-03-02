/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt3Support module of the Qt Toolkit.
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

#ifndef Q3ASCIIDICT_H
#define Q3ASCIIDICT_H

#include <Qt3Support/q3gdict.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Qt3SupportLight)

template<class type>
class Q3AsciiDict
#ifdef qdoc
	: public Q3PtrCollection
#else
	: public Q3GDict
#endif
{
public:
    Q3AsciiDict(int size=17, bool caseSensitive=true, bool copyKeys=true)
	: Q3GDict(size,AsciiKey,caseSensitive,copyKeys) {}
    Q3AsciiDict(const Q3AsciiDict<type> &d) : Q3GDict(d) {}
   ~Q3AsciiDict()			{ clear(); }
    Q3AsciiDict<type> &operator=(const Q3AsciiDict<type> &d)
			{ return (Q3AsciiDict<type>&)Q3GDict::operator=(d); }
    uint  count()   const		{ return Q3GDict::count(); }
    uint  size()    const		{ return Q3GDict::size(); }
    bool  isEmpty() const		{ return Q3GDict::count() == 0; }

    void  insert(const char *k, const type *d)
					{ Q3GDict::look_ascii(k,(Item)d,1); }
    void  replace(const char *k, const type *d)
					{ Q3GDict::look_ascii(k,(Item)d,2); }
    bool  remove(const char *k)	{ return Q3GDict::remove_ascii(k); }
    type *take(const char *k)		{ return (type *)Q3GDict::take_ascii(k); }
    type *find(const char *k) const
		{ return (type *)((Q3GDict*)this)->Q3GDict::look_ascii(k,0,0); }
    type *operator[](const char *k) const
		{ return (type *)((Q3GDict*)this)->Q3GDict::look_ascii(k,0,0); }

    void  clear()			{ Q3GDict::clear(); }
    void  resize(uint n)		{ Q3GDict::resize(n); }
    void  statistics() const		{ Q3GDict::statistics(); }

#ifdef qdoc
protected:
    virtual QDataStream& read(QDataStream &, Q3PtrCollection::Item &);
    virtual QDataStream& write(QDataStream &, Q3PtrCollection::Item) const;
#endif

private:
    void  deleteItem(Item d);
};

#if !defined(Q_BROKEN_TEMPLATE_SPECIALIZATION)
template<> inline void Q3AsciiDict<void>::deleteItem(Q3PtrCollection::Item)
{
}
#endif

template<class type> inline void Q3AsciiDict<type>::deleteItem(Q3PtrCollection::Item d)
{
    if (del_item) delete (type *)d;
}

template<class type>
class Q3AsciiDictIterator : public Q3GDictIterator
{
public:
    Q3AsciiDictIterator(const Q3AsciiDict<type> &d)
	: Q3GDictIterator((Q3GDict &)d) {}
   ~Q3AsciiDictIterator()      {}
    uint  count()   const     { return dict->count(); }
    bool  isEmpty() const     { return dict->count() == 0; }
    type *toFirst()	      { return (type *)Q3GDictIterator::toFirst(); }
    operator type *() const   { return (type *)Q3GDictIterator::get(); }
    type   *current() const   { return (type *)Q3GDictIterator::get(); }
    const char *currentKey() const { return Q3GDictIterator::getKeyAscii(); }
    type *operator()()	      { return (type *)Q3GDictIterator::operator()(); }
    type *operator++()	      { return (type *)Q3GDictIterator::operator++(); }
    type *operator+=(uint j)  { return (type *)Q3GDictIterator::operator+=(j);}
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // Q3ASCIIDICT_H
