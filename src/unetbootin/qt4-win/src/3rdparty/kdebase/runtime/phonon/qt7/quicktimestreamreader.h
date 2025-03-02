/*  This file is part of the KDE project.

    Copyright (C) 2007 Trolltech ASA. All rights reserved.

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 or 3 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef Phonon_QT7_QUICKTIMESTREAMREADER_H
#define Phonon_QT7_QUICKTIMESTREAMREADER_H

#include <QuickTime/QuickTime.h>
#undef check // avoid name clash;

#include <phonon/mediasource.h>
#include <phonon/streaminterface.h>
#include <QtCore>
#include <QReadWriteLock>

QT_BEGIN_NAMESPACE

namespace Phonon 
{
namespace QT7
{
    class QuickTimeStreamReader : public QObject, Phonon::StreamInterface
    {
        Q_OBJECT
        Q_INTERFACES(Phonon::StreamInterface)
        
    public:
        QuickTimeStreamReader(const Phonon::MediaSource &source);
        ~QuickTimeStreamReader();

        int readData(long offset, long size, void *data);
        bool readAllDataIntoBuffer();
        void writeData(const QByteArray &data);
        void endOfData();
        void setStreamSize(qint64 newSize);
        qint64 streamSize() const;
        void setStreamSeekable(bool s);
        bool streamSeekable() const;
        void setCurrentPos(qint64 pos);
        qint64 currentPos() const;
        int currentBufferSize() const;
        Movie QuickTimeStreamReader::movieRef();

        QByteArray m_buffer;
        void *m_QTMovieRef;
        mutable QReadWriteLock m_lock;
        bool m_seekable;
        qint64 m_pos;
        qint64 m_size;
    };

}} //namespace Phonon::QT7

QT_END_NAMESPACE

#endif // Phonon_QT7_QUICKTIMESTREAMREADER_H
