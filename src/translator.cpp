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

#include "translator.h"
#include "core/data/common.h"

#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QLocale>
#include <QtCore/QTimer>

#include <QtDebug>

Translator::Translator(QObject *parent)
    : QObject(parent)
{
    installEventFilter(this);
    initializeTranslations();
}

void Translator::setLanguage(const QString &language)
{
    if (m_availableTranslations.contains(language)) {
        qDebug() << "Changing language name to" << language << m_availableTranslations[language];

        m_translator.load(m_availableTranslations[language]);
        qApp->installTranslator(&m_translator);

        QSettings settings;
        settings.setValue(S_TranslationsPath, language);

        emit languageChanged();
    }
}

QStringList Translator::translations() const
{
    return m_availableTranslations.keys();
}

bool Translator::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        emit languageChanged();
    }

    return QObject::eventFilter(watched, event);
}

void Translator::initializeTranslations()
{
#ifdef DEBUG_MODE
    QDir assetsDirectory(ASSETS_DIRECTORY);
#else
    QDir assetsDirectory(qApp->applicationDirPath());
#endif
    if (!assetsDirectory.cd("translations")) {
        throw std::ios_base::failure(qApp->translate("main", "Could not find translations directory").toStdString());
    }

    const auto translations = assetsDirectory.entryInfoList({"*.qm"}, QDir::Files | QDir::Readable, QDir::Name);
    for (const auto &t : translations) {
        m_availableTranslations.insert(t.baseName(), t.absoluteFilePath());
    }

    qApp->setProperty("AvailableTranslations", QStringList(m_availableTranslations.keys()));

    QSettings settings;
    const QString language = settings.value(S_TranslationsPath).toString();
    setLanguage(language.isEmpty() ? QLocale::system().name() : language);
}
