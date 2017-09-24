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

#include "logindialog.h"
#include "ui_logindialog.h"

#include <QtCore/QSettings>

LoginDialog::LoginDialog(const QSharedPointer<Player> &player, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , m_player(player)
{
    ui->setupUi(this);
    loadDefaultData();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QVariantMap LoginDialog::loginInfo() const
{
    return QVariantMap({
        { "login", ui->lineEdit_username->text() },
        { "password", ui->lineEdit_password->text() },
        { "domain", ui->lineEdit_domain->text() },
        { "server", ui->spinBox_server->text() }
    });
}

void LoginDialog::accept()
{
    storeDefaultData();
    QDialog::accept();
}

void LoginDialog::initializeConnections() const
{
    connect(ui->buttonBox,  &QDialogButtonBox::accepted,
            this,           &LoginDialog::accept);

    connect(ui->buttonBox,  &QDialogButtonBox::rejected,
            this,           &LoginDialog::reject);
}

void LoginDialog::loadDefaultData()
{
    QSettings settings;
    settings.beginGroup(DefaultLogin);

    bool saveDefaults = settings.value(SaveDefaults, false).toBool();
    ui->checkBox_saveAsDefaults->setChecked(saveDefaults);

    if (saveDefaults) {
        ui->lineEdit_username->setText(settings.value(Login, QString()).toString());
        ui->lineEdit_domain->setText(settings.value(Domain, QString()).toString());
        ui->spinBox_server->setValue(settings.value(Server, 1).toInt());
    }
}

void LoginDialog::storeDefaultData() const
{
    QSettings settings;
    settings.beginGroup(DefaultLogin);

    bool saveDefaults = ui->checkBox_saveAsDefaults->isChecked();
    settings.setValue(SaveDefaults, saveDefaults);

    if (saveDefaults) {
        settings.setValue(Login, ui->lineEdit_username->text());
        settings.setValue(Domain, ui->lineEdit_domain->text());
        settings.setValue(Server, ui->spinBox_server->value());
    }
}
