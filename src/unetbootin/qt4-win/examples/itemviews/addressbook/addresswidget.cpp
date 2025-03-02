/****************************************************************************
**
** Copyright (C) 2008-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
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

#include <QtGui>
#include "addresswidget.h"
#include "adddialog.h"

//! [0]
AddressWidget::AddressWidget(QWidget *parent)
    : QTabWidget(parent)
{
    table = new TableModel(this);
    newAddressTab = new NewAddressTab(this);
    connect(newAddressTab, SIGNAL(sendDetails(QString, QString)),
        this, SLOT(addEntry(QString, QString))); 

    addTab(newAddressTab, "Address Book");    

    setupTabs();
}
//! [0]

//! [2]
void AddressWidget::addEntry()
{    
    AddDialog aDialog;

    if (aDialog.exec()) {
        QString name = aDialog.nameText->text();
        QString address = aDialog.addressText->toPlainText();
        
        addEntry(name, address);
    }
}
//! [2]

//! [3]
void AddressWidget::addEntry(QString name, QString address)
{    
    QList< QPair<QString, QString> >list = table->getList();
    QPair<QString, QString> pair(name, address);

    if (!list.contains(pair)) {
        table->insertRows(0, 1, QModelIndex());

        QModelIndex index = table->index(0, 0, QModelIndex());
        table->setData(index, name, Qt::EditRole);
        index = table->index(0, 1, QModelIndex());
        table->setData(index, address, Qt::EditRole);
        removeTab(indexOf(newAddressTab));
    } else {
        QMessageBox::information(this, tr("Duplicate Name"),
            tr("The name \"%1\" already exists.").arg(name));
    }
}
//! [3]

//! [4a]
void AddressWidget::editEntry()
{
    QTableView *temp = static_cast<QTableView*>(currentWidget());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index, i;        
    QString name;
    QString address;
    int row;

    foreach (index, indexes) {
        row = index.row();
        i = table->index(row, 0, QModelIndex());
        QVariant varName = table->data(i, Qt::DisplayRole);
        name = varName.toString();
    
        i = table->index(row, 1, QModelIndex());
        QVariant varAddr = table->data(i, Qt::DisplayRole);
        address = varAddr.toString();
    }
//! [4a]
    
//! [4b]    
    AddDialog aDialog;
    aDialog.setWindowTitle(tr("Edit a Contact"));

    aDialog.nameText->setReadOnly(true);
    aDialog.nameText->setText(name);
    aDialog.addressText->setText(address);

    if (aDialog.exec()) {
        QString newAddress = aDialog.addressText->toPlainText();
        if (newAddress != address) {
            i = table->index(row, 1, QModelIndex());
            table->setData(i, newAddress, Qt::EditRole);
        }
    }
}
//! [4b]

//! [5]
void AddressWidget::removeEntry()
{
    QTableView *temp = static_cast<QTableView*>(currentWidget());
    QItemSelectionModel *selectionModel = temp->selectionModel();
    
    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;

    foreach (index, indexes) {
        int row = index.row();
        table->removeRows(row, 1, QModelIndex());
    }

    if (table->rowCount(QModelIndex()) == 0) {
        insertTab(0, newAddressTab, "Address Book");
    }
}
//! [5]

//! [1]
void AddressWidget::setupTabs()
{
    QStringList groups;
    groups << "ABC" << "DEF" << "GHI" << "JKL" << "MNO" << "PQR" << "STU" << "VW" << "XYZ";

    for (int i = 0; i < groups.size(); ++i) {
        QString str = groups.at(i);
        
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(table);
        proxyModel->setDynamicSortFilter(true);
    
        QTableView *tableView = new QTableView;
        tableView->setModel(proxyModel);
        tableView->setSortingEnabled(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->hide();
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);

        QString newStr = QString("^[%1].*").arg(str);

        proxyModel->setFilterRegExp(QRegExp(newStr, Qt::CaseInsensitive));
        proxyModel->setFilterKeyColumn(0);
        proxyModel->sort(0, Qt::AscendingOrder);
    
        connect(tableView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SIGNAL(selectionChanged(const QItemSelection &)));

        addTab(tableView, str);
    }
}
//! [1]

//! [7]
void AddressWidget::readFromFile(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        return;
    }

    QList< QPair<QString, QString> > pairs = table->getList();    
    QDataStream in(&file);
    in >> pairs;

    if (pairs.isEmpty()) {
        QMessageBox::information(this, tr("No contacts in file"),
            tr("The file you are attempting to open contains no contacts."));  
    } else {
        for (int i=0; i<pairs.size(); ++i) {
            QPair<QString, QString> p = pairs.at(i);
            addEntry(p.first, p.second);
        }
    }
}
//! [7]

//! [6]
void AddressWidget::writeToFile(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QList< QPair<QString, QString> > pairs = table->getList();    
    QDataStream out(&file);
    out << pairs;
}
//! [6]
