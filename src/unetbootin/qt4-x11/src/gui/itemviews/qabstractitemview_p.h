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

#ifndef QABSTRACTITEMVIEW_P_H
#define QABSTRACTITEMVIEW_P_H

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

#include "private/qabstractscrollarea_p.h"
#include "private/qabstractitemmodel_p.h"
#include "QtGui/qapplication.h"
#include "QtCore/qdatetime.h"
#include "QtGui/qevent.h"
#include "QtGui/qmime.h"
#include "QtGui/qpainter.h"
#include "QtCore/qpair.h"
#include "QtCore/qtimer.h"
#include "QtCore/qtimeline.h"
#include "QtGui/qregion.h"
#include "QtCore/qdebug.h"
#include "QtGui/qpainter.h"

#ifndef QT_NO_ITEMVIEWS

QT_BEGIN_NAMESPACE

struct QEditorInfo
{
    QEditorInfo() : isStatic(false)
    {
    }

    QEditorInfo(const QPersistentModelIndex &i, QWidget *e, bool b) : index(i), editor(e), isStatic(b)
    {
    }

    QPersistentModelIndex index;
    QPointer<QWidget> editor;
    bool isStatic; //true when called from setIndexWidget

};

class QEmptyModel : public QAbstractItemModel
{
public:
    explicit QEmptyModel(QObject *parent = 0) : QAbstractItemModel(parent) {}
    QModelIndex index(int, int, const QModelIndex &) const { return QModelIndex(); }
    QModelIndex parent(const QModelIndex &) const { return QModelIndex(); }
    int rowCount(const QModelIndex &) const { return 0; }
    int columnCount(const QModelIndex &) const { return 0; }
    bool hasChildren(const QModelIndex &) const { return false; }
    QVariant data(const QModelIndex &, int) const { return QVariant(); }
};

class Q_GUI_EXPORT QAbstractItemViewPrivate : public QAbstractScrollAreaPrivate
{
    Q_DECLARE_PUBLIC(QAbstractItemView)

public:
    QAbstractItemViewPrivate();
    virtual ~QAbstractItemViewPrivate();

    void init();

    void _q_rowsRemoved(const QModelIndex &parent, int start, int end);
    void _q_columnsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void _q_columnsRemoved(const QModelIndex &parent, int start, int end);
    void _q_columnsInserted(const QModelIndex &parent, int start, int end);
    void _q_modelDestroyed();
    void _q_layoutChanged();
    void _q_fetchMore();

    bool shouldEdit(QAbstractItemView::EditTrigger trigger, const QModelIndex &index) const;
    bool shouldForwardEvent(QAbstractItemView::EditTrigger trigger, const QEvent *event) const;
    bool shouldAutoScroll(const QPoint &pos) const;
    void doDelayedItemsLayout(int delay = 0);
    void interruptDelayedItemsLayout() const;

    bool dropOn(QDropEvent *event, int *row, int *col, QModelIndex *index);
    bool droppingOnItself(QDropEvent *event, const QModelIndex &index);

    QWidget *editor(const QModelIndex &index, const QStyleOptionViewItem &options);
    bool sendDelegateEvent(const QModelIndex &index, QEvent *event) const;
    bool openEditor(const QModelIndex &index, QEvent *event);
    void updateEditorData(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    QItemSelectionModel::SelectionFlags multiSelectionCommand(const QModelIndex &index,
                                                              const QEvent *event) const;
    QItemSelectionModel::SelectionFlags extendedSelectionCommand(const QModelIndex &index,
                                                                 const QEvent *event) const;
    QItemSelectionModel::SelectionFlags contiguousSelectionCommand(const QModelIndex &index,
                                                                   const QEvent *event) const;
    virtual void selectAll(QItemSelectionModel::SelectionFlags command);

    inline QItemSelectionModel::SelectionFlags selectionBehaviorFlags() const
    {
        switch (selectionBehavior) {
        case QAbstractItemView::SelectRows: return QItemSelectionModel::Rows;
        case QAbstractItemView::SelectColumns: return QItemSelectionModel::Columns;
        case QAbstractItemView::SelectItems: default: return QItemSelectionModel::NoUpdate;
        }
    }

#ifndef QT_NO_DRAGANDDROP
    QAbstractItemView::DropIndicatorPosition position(const QPoint &pos, const QRect &rect, const QModelIndex &idx) const;
    inline bool canDecode(QDropEvent *e) const {
        QStringList modelTypes = model->mimeTypes();
        const QMimeData *mime = e->mimeData();
        for (int i = 0; i < modelTypes.count(); ++i)
            if (mime->hasFormat(modelTypes.at(i))
                && (e->dropAction() & model->supportedDropActions()))
                return true;
        return false;
    }

    inline void paintDropIndicator(QPainter *painter)
    {
        if (showDropIndicator && state == QAbstractItemView::DraggingState
#ifndef QT_NO_CURSOR
            && viewport->cursor().shape() != Qt::ForbiddenCursor
#endif
            ) {
            QStyleOption opt;
            opt.init(q_func());
            opt.rect = dropIndicatorRect;
            q_func()->style()->drawPrimitive(QStyle::PE_IndicatorItemViewItemDrop, &opt, painter, q_func());
        }
    }
#endif

    inline void releaseEditor(QWidget *editor) const {
        if (editor) {
            QObject::disconnect(editor, SIGNAL(destroyed(QObject*)),
                                q_func(), SLOT(editorDestroyed(QObject*)));
            editor->removeEventFilter(itemDelegate);
            editor->hide();
            editor->deleteLater();
        }
    }

    inline void executePostedLayout() const {
        if (delayedPendingLayout && state != QAbstractItemView::CollapsingState) {
            interruptDelayedItemsLayout();
            const_cast<QAbstractItemView*>(q_func())->doItemsLayout();
        }
    }

    inline void setDirtyRegion(const QRegion &visualRegion) {
        updateRegion += visualRegion;
        if (!updateTimer.isActive())
            updateTimer.start(0, q_func());
    }

    inline void scrollDirtyRegion(int dx, int dy) {
        scrollDelayOffset = QPoint(-dx, -dy);
        updateDirtyRegion();
        scrollDelayOffset = QPoint(0, 0);
    }

    inline void scrollContentsBy(int dx, int dy) {
        scrollDirtyRegion(dx, dy);
        viewport->scroll(dx, dy);
    }

    void updateDirtyRegion() {
        updateTimer.stop();
        viewport->update(updateRegion);
        updateRegion = QRegion();
    }

    void clearOrRemove();
    void checkPersistentEditorFocus();

    QPixmap renderToPixmap(const QModelIndexList &indexes, QRect *r = 0) const;

    inline QPoint offset() const {
        const Q_Q(QAbstractItemView);
        return QPoint(q->isRightToLeft() ? -q->horizontalOffset()
                      : q->horizontalOffset(), q->verticalOffset());
    }

    QEditorInfo editorForIndex(const QModelIndex &index) const;
    inline bool hasEditor(const QModelIndex &index) const {
        return editorForIndex(index).editor != 0;
    }

    QModelIndex indexForEditor(QWidget *editor) const;
    void addEditor(const QModelIndex &index, QWidget *editor, bool isStatic);
    void removeEditor(QWidget *editor);

    inline bool isAnimating() const {
        return state == QAbstractItemView::AnimatingState;
    }

    inline QAbstractItemDelegate *delegateForIndex(const QModelIndex &index) const {
	QAbstractItemDelegate *del;
	if ((del = rowDelegates.value(index.row(), 0))) return del;
	if ((del = columnDelegates.value(index.column(), 0))) return del;
	return itemDelegate;
    }

    inline bool isIndexValid(const QModelIndex &index) const {
         return (index.row() >= 0) && (index.column() >= 0) && (index.model() == model);
    }
    inline bool isIndexSelectable(const QModelIndex &index) const {
        return (model->flags(index) & Qt::ItemIsSelectable);
    }
    inline bool isIndexEnabled(const QModelIndex &index) const {
        return (model->flags(index) & Qt::ItemIsEnabled);
    }
    inline bool isIndexDropEnabled(const QModelIndex &index) const {
        return (model->flags(index) & Qt::ItemIsDropEnabled);
    }
    inline bool isIndexDragEnabled(const QModelIndex &index) const {
        return (model->flags(index) & Qt::ItemIsDragEnabled);
    }

    virtual bool selectionAllowed(const QModelIndex &index) const {
        // in some views we want to go ahead with selections, even if the index is invalid
        return isIndexValid(index) && isIndexSelectable(index);
    }

    // reimplemented from QAbstractScrollAreaPrivate
    virtual QPoint contentsOffset() const {
        Q_Q(const QAbstractItemView);
        return QPoint(q->horizontalOffset(), q->verticalOffset());
    }

    /**
     * For now, assume that we have few editors, if we need a more efficient implementation
     * we should add a QMap<QAbstractItemDelegate*, int> member.
     */
    int delegateRefCount(const QAbstractItemDelegate *delegate) const
    {
        int ref = 0;
        if (itemDelegate == delegate)
            ++ref;

        for (int maps = 0; maps < 2; ++maps) {
            const QMap<int, QPointer<QAbstractItemDelegate> > *delegates = maps ? &columnDelegates : &rowDelegates;
            for (QMap<int, QPointer<QAbstractItemDelegate> >::const_iterator it = delegates->begin();
                it != delegates->end(); ++it) {
                    if (it.value() == delegate) {
                        ++ref;
                        // optimization, we are only interested in the ref count values 0, 1 or >=2
                        if (ref >= 2) {
                            return ref;
                        }
                    }
            }
        }
        return ref;
    }
    
    /**
     * return true if the index is registered as a QPersistentModelIndex
     */
    inline bool isPersistent(const QModelIndex &index) const
    {
        return static_cast<QAbstractItemModelPrivate *>(model->d_ptr)->persistent.indexes.contains(index);
    }

    QModelIndexList selectedDraggableIndexes() const;

    QStyleOptionViewItemV4 viewOptionsV4() const;

    QAbstractItemModel *model;
    QPointer<QAbstractItemDelegate> itemDelegate;
    QMap<int, QPointer<QAbstractItemDelegate> > rowDelegates;
    QMap<int, QPointer<QAbstractItemDelegate> > columnDelegates;
    QPointer<QItemSelectionModel> selectionModel;

    QAbstractItemView::SelectionMode selectionMode;
    QAbstractItemView::SelectionBehavior selectionBehavior;

    QList<QEditorInfo> editors;
    QSet<QWidget*> persistent;
    QWidget *currentlyCommittingEditor;

    QPersistentModelIndex enteredIndex;
    QPersistentModelIndex pressedIndex;
    Qt::KeyboardModifiers pressedModifiers;
    QPoint pressedPosition;
    bool pressedAlreadySelected;
    
    //forces the next mouseMoveEvent to send the viewportEntered signal 
    //if the mouse is over the viewport and not over an item
    bool viewportEnteredNeeded;

    QAbstractItemView::State state;
    QAbstractItemView::EditTriggers editTriggers;
    QAbstractItemView::EditTrigger lastTrigger;

    QPersistentModelIndex root;
    QPersistentModelIndex hover;

    bool tabKeyNavigation;

#ifndef QT_NO_DRAGANDDROP
    bool showDropIndicator;
    QRect dropIndicatorRect;
    bool dragEnabled;
    QAbstractItemView::DragDropMode dragDropMode;
    bool overwrite;
    QAbstractItemView::DropIndicatorPosition dropIndicatorPosition;
#endif

    QString keyboardInput;
    QTime keyboardInputTime;

    bool autoScroll;
    QBasicTimer autoScrollTimer;
    int autoScrollMargin;
    int autoScrollCount;

    bool alternatingColors;

    QSize iconSize;
    Qt::TextElideMode textElideMode;

    QRegion updateRegion; // used for the internal update system
    QPoint scrollDelayOffset;

    QBasicTimer updateTimer;
    QBasicTimer delayedEditing;
    QBasicTimer delayedAutoScroll; //used when an item is clicked
    QTimeLine timeline;

    QAbstractItemView::ScrollMode verticalScrollMode;
    QAbstractItemView::ScrollMode horizontalScrollMode;

    bool currentIndexSet;

    bool wrapItemText;
    mutable bool delayedPendingLayout;

private:
    mutable QBasicTimer delayedLayout;
};

QT_BEGIN_INCLUDE_NAMESPACE
#include <qvector.h>
QT_END_INCLUDE_NAMESPACE

template <typename T>
inline int qBinarySearch(const QVector<T> &vec, const T &item, int start, int end)
{
    int i = (start + end + 1) >> 1;
    while (end - start > 0) {
        if (vec.at(i) > item)
            end = i - 1;
        else
            start = i;
        i = (start + end + 1) >> 1;
    }
    return i;
}

QT_END_NAMESPACE

#endif // QT_NO_ITEMVIEWS

#endif // QABSTRACTITEMVIEW_P_H
