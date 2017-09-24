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

#pragma once

#include <QtCore/QVariantMap>
#include <QtWidgets/QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

    static constexpr auto DefaultLogin = "DefaultLogin";

    static constexpr auto Login = "Login";
    static constexpr auto Domain = "Domain";
    static constexpr auto Server = "Server";
    static constexpr auto SaveDefaults = "SaveDefaults";

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QVariantMap loginInfo() const;

public slots:
    void accept();

private:
    void initializeConnections() const;
    void loadDefaultData();
    void storeDefaultData() const;

private:
    Ui::LoginDialog *ui;
};
