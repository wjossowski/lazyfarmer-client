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

#include "api/apigateway.h"
#include "data/storage.h"
#include "data/common.h"
#include "data/buildinglist.h"

#include <QtCore/QObject>

namespace Core {

    class GlobalGameData;

    class Player : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(int level MEMBER m_level NOTIFY levelChanged)
        Q_PROPERTY(QString levelDescription MEMBER m_levelDescription NOTIFY levelDescriptionChanged)
        Q_PROPERTY(int levelPercentage MEMBER m_levelPercentage NOTIFY levelPercentageChanged)
        Q_PROPERTY(qreal money MEMBER m_money NOTIFY moneyChanged)

        Q_PROPERTY(QString description READ playerDescription NOTIFY playerDescriptionChanged)
        Q_PROPERTY(QString job READ currentJob NOTIFY currentJobChanged)

        Q_PROPERTY(QString lastError MEMBER m_lastError NOTIFY lastErrorChanged)

    public:
        using Ptr = QSharedPointer<Player>;

        explicit Player(QObject *parent = nullptr);
        ~Player() override;

        GlobalGameData::Ptr gameData() const;

        Api::ApiGateway::Ptr gateway() { return m_gateway; }
        Data::Storage::Ptr storage() const { return m_storage; }
        Data::BuildingList::Ptr buildings() const { return m_buildingList; }

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &json);

        QString playerDescription() const;
        QString currentJob() const;

        void setApiOptions(const Api::ApiOptions &options);

        Q_INVOKABLE inline QString login () const { return m_gateway->login(); }
        Q_INVOKABLE inline QString domain () const { return m_gateway->serverDomain(); }
        Q_INVOKABLE inline QString server () const { return m_gateway->serverId(); }

        Q_INVOKABLE void setApiOptions(const QString &domain,
                                       const QString &serverId,
                                       const QString &login,
                                       const QString &password);

    public slots:
        void update(const QByteArray &info);

    signals:
        void updateBuildingRequested(const Data::BuildingDetails &details, const Data::BuildingType &type) const;

        void levelChanged(int) const;
        void levelDescriptionChanged(const QString&) const;
        void levelPercentageChanged(int) const;
        void moneyChanged(qreal) const;

        void playerDescriptionChanged() const;
        void currentJobChanged() const;

        void lastErrorChanged() const;

        void dataChanged() const;

    private:
        void updateBasicInfo(const QVariantMap &basicInfo);
        void initialize();
        void initializeConnections() const;

    private slots:
        void handleGatewayError(const QString &errorMessage);

    private:
        int m_level;
        int m_levelPercentage;

        qreal m_money;
        QString m_levelDescription;

        Data::Storage::Ptr m_storage;
        Data::BuildingList::Ptr m_buildingList;
        Api::ApiGateway::Ptr m_gateway;

        QString m_lastError;
    };

}
