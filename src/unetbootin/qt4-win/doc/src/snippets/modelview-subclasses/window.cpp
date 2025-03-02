/****************************************************************************
**
** Copyright (C) 2004-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the documentation of the Qt Toolkit.
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

#include <QAction>
#include <QDataStream>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QListView>

#include "window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Model/View example");

    setupModelView();

    QAction *openAction = new QAction(tr("&Open"), this);
    QAction *quitAction = new QAction(tr("E&xit"), this);
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAction);
    fileMenu->addAction(quitAction);
    menuBar()->addMenu(fileMenu);

    connect(openAction, SIGNAL(triggered()), this, SLOT(selectOpenFile()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    setCentralWidget(view);
}

void MainWindow::setupModelView()
{
    model = new LinearModel(this);
    view = new LinearView(this);
    view->setModel(model);
}

void MainWindow::selectOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select a file to open"), "", tr("Sound files (*.wav)"));
    
    if (!fileName.isEmpty())
        openFile(fileName);
}

void MainWindow::openFile(const QString &fileName)
{
    QFile file(fileName);
    int length = file.size();

    if (file.open(QFile::ReadOnly)) {
        model->removeRows(0, model->rowCount());

        int rows = (length - 0x2c)/2;
        model->insertRows(0, rows);

        // Perform some dodgy tricks to extract the data from the file.
        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        Q_INT16 left;
        Q_INT16 right;

        for (int row = 0; row < rows; ++row) {
            QModelIndex index = model->index(row);

            stream >> left >> right;
            model->setData(index, int(left / 256));
        }
    }
}
