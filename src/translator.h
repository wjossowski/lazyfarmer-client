/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2018 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QTranslator>

class Translator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString r READ emptyString NOTIFY languageChanged)
    Q_PROPERTY(QStringList translations READ translations)

public:
    explicit Translator(QObject *parent = nullptr);
    ~Translator() override = default;

    Q_INVOKABLE void setLanguage(const QString &language);
    Q_INVOKABLE QStringList translations() const;

    QString emptyString () const { return QString(); }
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void languageChanged() const;

private:
    void initializeTranslations();

private:
    QMap<QString, QString> m_availableTranslations;
    QTranslator m_translator;
};
