/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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
#include "private/qharfbuzz_p.h"
#include <QtCore/qtextboundaryfinder.h>
#include <QtCore/qvarlengtharray.h>
#include <private/qunicodetables_p.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

class QTextBoundaryFinderPrivate
{
public:
    HB_CharAttributes attributes[1];
};

static void init(QTextBoundaryFinder::BoundaryType type, const QChar *chars, int length, HB_CharAttributes *attributes)
{
    QVarLengthArray<HB_ScriptItem> scriptItems;

    const ushort *string = reinterpret_cast<const ushort *>(chars);
    const ushort *unicode = string;
    // correctly assign script, isTab and isObject to the script analysis
    const ushort *uc = unicode;
    const ushort *e = uc + length;
    int script = QUnicodeTables::Common;
    int lastScript = QUnicodeTables::Common;
    const ushort *start = uc;
    while (uc < e) {
        int s = QUnicodeTables::script(*uc);
        if (s != QUnicodeTables::Inherited)
            script = s;
        if (*uc == QChar::ObjectReplacementCharacter || *uc == QChar::LineSeparator || *uc == 9) 
            script = QUnicodeTables::Common;
        if (script != lastScript) {
            if (uc != start) {
                HB_ScriptItem item;
                item.pos = start - string;
                item.length = uc - start;
                item.script = (HB_Script)lastScript;
                item.bidiLevel = 0; // ### what's the proper value?
                scriptItems.append(item);
                start = uc;
            }
            lastScript = script;
        }
        ++uc;
    }
    if (uc != start) {
        HB_ScriptItem item;
        item.pos = start - string;
        item.length = uc - start;
        item.script = (HB_Script)lastScript;
        item.bidiLevel = 0; // ### what's the proper value?
        scriptItems.append(item);
    }

    qGetCharAttributes(string, length, scriptItems.data(), scriptItems.count(), attributes);
    if (type == QTextBoundaryFinder::Word)
        HB_GetWordBoundaries(string, length, scriptItems.data(), scriptItems.count(), attributes);
    else if (type == QTextBoundaryFinder::Sentence)
        HB_GetSentenceBoundaries(string, length, scriptItems.data(), scriptItems.count(), attributes);
}

/*! \class QTextBoundaryFinder

    \brief The QTextBoundaryFinder class provides a way of finding Unicode text boundaries in a string.

    \since 4.4
    \ingroup tools
    \ingroup shared
    \ingroup text
    \reentrant

    QTextBoundaryFinder allows to find Unicode text boundaries in a
    string, similar to the Unicode text boundary specification (see
    http://www.unicode.org/reports/tr29/tr29-11.html).

    QTextBoundaryFinder can operate on a QString in four possible
    modes depending on the value of \a BoundaryType.

    Units of Unicode characters that make up what the user thinks of
    as a character or basic unit of the language are here called
    Grapheme clusters. The two unicode characters 'A' + diaeresis do
    for example form one grapheme cluster as the user thinks of them
    as one character, yet it is in this case represented by two
    unicode code points.

    Word boundaries are there to locate the start and end of what a
    language considers to be a word.

    Line break boundaries give possible places where a line break
    might happen and sentence boundaries will show the beginning and
    end of whole sentences.
*/

/*!
    \enum QTextBoundaryFinder::BoundaryType

    \value Grapheme Finds a grapheme which is the smallest boundary. It
    including letters, punctation marks, numerals and more.
    \value Word Finds a word.
    \value Line Finds possible positions for breaking the text into multiple
    lines.
    \value Sentence Finds sentence boundaries. These include periods, question
    marks etc.
*/

/*!
  \enum QTextBoundaryFinder::BoundaryReason

  \value NotAtBoundary  The boundary finder is not at a boundary position.
  \value StartWord  The boundary finder is at the start of a word.
  \value EndWord  The boundary finder is at the end of a word.
*/

/*!
  Constructs an invalid QTextBoundaryFinder object.
*/
QTextBoundaryFinder::QTextBoundaryFinder()
    : t(Grapheme)
    , chars(0)
    , length(0)
    , freePrivate(true)
    , d(0)
{
}

/*!
  Copies the QTextBoundaryFinder object, \a other.
*/
QTextBoundaryFinder::QTextBoundaryFinder(const QTextBoundaryFinder &other)
    : t(other.t)
    , s(other.s)
    , chars(other.chars)
    , length(other.length)
    , pos(other.pos)
    , freePrivate(true)
{
    d = (QTextBoundaryFinderPrivate *) malloc(length*sizeof(HB_CharAttributes));
    memcpy(d, other.d, length*sizeof(HB_CharAttributes));
}

/*!
  Assigns the object, \a other, to another QTextBoundaryFinder object.
*/
QTextBoundaryFinder &QTextBoundaryFinder::operator=(const QTextBoundaryFinder &other)
{
    if (&other == this)
        return *this;

    t = other.t;
    s = other.s;
    chars = other.chars;
    length = other.length;
    pos = other.pos;
    freePrivate = true;
    
    d = (QTextBoundaryFinderPrivate *) realloc(d, length*sizeof(HB_CharAttributes));
    memcpy(d, other.d, length*sizeof(HB_CharAttributes));

    return *this;
}

/*!
  Destructs the QTextBoundaryFinder object.
*/
QTextBoundaryFinder::~QTextBoundaryFinder()
{
    if (freePrivate)
        free(d);
}

/*!
  Creates a QTextBoundaryFinder object of \a type operating on \a string.
*/
QTextBoundaryFinder::QTextBoundaryFinder(BoundaryType type, const QString &string)
    : t(type)
    , s(string)
    , chars(string.unicode())
    , length(string.length())
    , pos(0)
    , freePrivate(true)
{
    d = (QTextBoundaryFinderPrivate *) malloc(length*sizeof(HB_CharAttributes));
    init(t, chars, length, d->attributes);
}

/*!
  Creates a QTextBoundaryFinder object of \a type operating on \a chars
  with \a length.

  \a buffer is an optional working buffer of size \a bufferSize you can pass to
  the QTextBoundaryFinder. If the buffer is large enough to hold the working
  data required, it will use this instead of allocating its own buffer.

  \warning QTextBoundaryFinder does not create a copy of \a chars. It is the
  application programmer's responsability to ensure the array is allocated for
  as long as the QTextBoundaryFinder object stays alive. The same applies to
  \a buffer.
*/
QTextBoundaryFinder::QTextBoundaryFinder(BoundaryType type, const QChar *chars, int length, unsigned char *buffer, int bufferSize)
    : t(type)
    , chars(chars)
    , length(length)
    , pos(0)
{
    if (buffer && (uint)bufferSize >= length*sizeof(HB_CharAttributes)) {
        d = (QTextBoundaryFinderPrivate *)buffer;
        freePrivate = false;
    } else {
        d = (QTextBoundaryFinderPrivate *) malloc(length*sizeof(HB_CharAttributes));
        freePrivate = true;
    }
    init(t, chars, length, d->attributes);
}

/*!
  Moves the finder to the start of the string. This is equivalent to setPosition(0).

  \sa setPosition(), position()
*/
void QTextBoundaryFinder::toStart()
{
    pos = 0;
}

/*!
  Moves the finder to the end of the string. This is equivalent to setPosition(string.length()).

  \sa setPosition(), position()
*/
void QTextBoundaryFinder::toEnd()
{
    pos = length;
}

/*!
  Returns the current position of the QTextBoundaryFinder.

  The range is from 0 (the beginning of the string) to the length of
  the string inclusive.

  \sa setPosition()
*/
int QTextBoundaryFinder::position() const
{
    return pos;
}

/*!
  Sets the current position of the QTextBoundaryFinder to \a position.

  If \a position is out of bounds, it will be bound to only valid
  positions. In this case, valid positions are from 0 to the length of
  the string inclusive.

  \sa position()
*/
void QTextBoundaryFinder::setPosition(int position)
{
    pos = qBound(0, position, length);
}

/*! \fn QTextBoundaryFinder::BoundaryType QTextBoundaryFinder::type() const

  Returns the type of the QTextBoundaryFinder.
*/

/*! \fn bool QTextBoundaryFinder::isValid() const

   Returns true if the text boundary finder is valid; otherwise returns false.
   A default QTextBoundaryFinder is invalid.
*/

/*!
  Returns the string  the QTextBoundaryFinder object operates on.
*/
QString QTextBoundaryFinder::string() const
{
    if (chars == s.unicode() && length == s.length())
        return s;
    return QString(chars, length);
}


/*!
  Moves the QTextBoundaryFinder to the next boundary position and returns that position.

  Returns -1 is there is no next boundary.
*/
int QTextBoundaryFinder::toNextBoundary()
{
    if (!d) {
        pos = -1;
        return pos;
    }

    if (pos < 0 || pos >= length) {
        pos = -1;
        return pos;
    }
    ++pos;
    if (pos == length)
        return pos;
    
    switch(t) {
    case Grapheme:
        while (pos < length && !d->attributes[pos].charStop)
            ++pos;
        break;
    case Word:
        while (pos < length && !d->attributes[pos].wordBoundary)
            ++pos;
        break;
    case Sentence:
        while (pos < length && !d->attributes[pos].sentenceBoundary)
            ++pos;
        break;
    case Line:
        while (pos < length && d->attributes[pos].lineBreakType < HB_Break)
            ++pos;
        break;
    }

    return pos;
}

/*!
  Moves the QTextBoundaryFinder to the previous boundary position and returns that position.

  Returns -1 is there is no previous boundary.
*/
int QTextBoundaryFinder::toPreviousBoundary()
{
    if (!d) {
        pos = -1;
        return pos;
    }

    if (pos <= 0 || pos > length) {
        pos = -1;
        return pos;
    }
    --pos;
    if (pos == 0)
        return pos;

    switch(t) {
    case Grapheme:
        while (pos > 0 && !d->attributes[pos].charStop)
            --pos;
        break;
    case Word:
        while (pos > 0 && !d->attributes[pos].wordBoundary)
            --pos;
        break;
    case Sentence:
        while (pos > 0 && !d->attributes[pos].sentenceBoundary)
            --pos;
        break;
    case Line:
        while (pos > 0 && d->attributes[pos].lineBreakType < HB_Break)
            --pos;
        break;
    }

    return pos;
}

/*!
  Returns true if the object's position() is currently at a valid text boundary.
*/
bool QTextBoundaryFinder::isAtBoundary() const
{
    if (!d || pos < 0)
        return false;

    if (pos == length)
        return true;

    switch(t) {
    case Grapheme:
        return d->attributes[pos].charStop;
    case Word:
        return d->attributes[pos].wordBoundary;
    case Line:
        return d->attributes[pos].lineBreakType >= HB_Break;
    case Sentence:
        return d->attributes[pos].sentenceBoundary;
    }
    return false;
}

/*!
  Returns the reasons for the boundary finder to have chosen the current position as a boundary.
*/
QTextBoundaryFinder::BoundaryReasons QTextBoundaryFinder::boundaryReasons() const
{
    if (!d)
        return NotAtBoundary;
    if (! isAtBoundary())
        return NotAtBoundary;
    if (pos == 0) {
        if (d->attributes[pos].whiteSpace)
            return NotAtBoundary;
        return StartWord;
    }
    if (pos >= length - 1) {
        if (d->attributes[length-1].whiteSpace)
            return NotAtBoundary;
        return EndWord;
    }

    BoundaryReasons answer;
    const bool nextIsSpace = d->attributes[pos + 1].whiteSpace;
    const bool prevIsSpace = d->attributes[pos - 1].whiteSpace;

    if (d->attributes[pos].whiteSpace)
        answer = EndWord;
    else if (!prevIsSpace) {
        answer = StartWord;
        answer |= EndWord;
    }

    if (prevIsSpace)
        answer |= StartWord;
    if (nextIsSpace)
        answer |= EndWord;
    if (answer == 0) {
        answer = StartWord;
        answer |= EndWord;
    }

    return answer;
}

QT_END_NAMESPACE
