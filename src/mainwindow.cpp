/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2017 Wojciech Ossowski <w.j.ossowski@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "logindialog.h"
#include "aboutdialog.h"

#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();
    initializeConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    insertNewPlayer();
}

void MainWindow::initializeConnections() const
{
    connect(ui->actionAbout_Qt,     &QAction::triggered,
            qApp,                   &QApplication::aboutQt);

    connect(ui->actionAbout,        &QAction::triggered,
            this,                   &MainWindow::showAboutDialog);

    connect(ui->tabWidgetAccounts,  &QTabWidget::tabCloseRequested,
            this,                   &MainWindow::removePlayer);
}

void MainWindow::showAboutDialog() const
{
    AboutDialog about;
    about.exec();
}

void MainWindow::insertNewPlayer()
{
    ui->tabWidgetAccounts->insertTab(ui->tabWidgetAccounts->count(), new LoginDialog(QSharedPointer<Player>(), this), "LD");
}

void MainWindow::removePlayer(int tabNumber)
{

}
