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

#include <QtCore/QString>
#include <QtCore/QCoreApplication>

namespace Core {

    namespace Api {

        namespace Messages {

            enum class QueryType {
                Post,
                Get,

                Unknown
            };

            enum class MessageType {
                Login, // Done
                CheckCredentials, // Done
                Logout, // Done

                GetConstantData, // Done

                GetFarmInfo, // Todo
                GetFieldInfo,

                SetPlant, // Done
                SetPour, // Done
                GetCollect, // Done

                SetFeed, // Done
                GetFeed,
                GetFeedInfo,

                SetProduction, // Done
                GetProduction, // Done
                GetProductionInfo, //Done

                SetBuyer,
                GetBuyer,

                GetPricesOnMarket,
                SetOfferOnMarket,

                Unknown
            };

            struct MessageHelper
            {

                static QString toString(MessageType type)
                {
                    switch (type) {
                    case MessageType::Login:                return qApp->translate("MessageTypes", "Login");
                    case MessageType::CheckCredentials:     return qApp->translate("MessageTypes", "CheckCredentials");
                    case MessageType::Logout:               return qApp->translate("MessageTypes", "Logout");
                    case MessageType::GetConstantData:      return qApp->translate("MessageTypes", "GetConstantData");
                    case MessageType::GetFarmInfo:          return qApp->translate("MessageTypes", "GetFarmInfo");
                    case MessageType::GetFieldInfo:         return qApp->translate("MessageTypes", "GetFieldInfo");
                    case MessageType::SetPlant:             return qApp->translate("MessageTypes", "SetPlant");
                    case MessageType::SetPour:              return qApp->translate("MessageTypes", "SetPour");
                    case MessageType::GetCollect:           return qApp->translate("MessageTypes", "GetCollect");
                    case MessageType::SetFeed:              return qApp->translate("MessageTypes", "SetFeed");
                    case MessageType::GetFeed:              return qApp->translate("MessageTypes", "GetFeed");
                    case MessageType::GetFeedInfo:          return qApp->translate("MessageTypes", "GetFeedInfo");
                    case MessageType::SetProduction:        return qApp->translate("MessageTypes", "SetProduction");
                    case MessageType::GetProduction:        return qApp->translate("MessageTypes", "GetProduction");
                    case MessageType::GetProductionInfo:    return qApp->translate("MessageTypes", "GetProductionInfo");
                    case MessageType::SetBuyer:             return qApp->translate("MessageTypes", "SetBuyer");
                    case MessageType::GetBuyer:             return qApp->translate("MessageTypes", "GetBuyer");
                    case MessageType::GetPricesOnMarket:    return qApp->translate("MessageTypes", "GetPricesOnMarket");
                    case MessageType::SetOfferOnMarket:     return qApp->translate("MessageTypes", "SetOfferOnMarket");
                    default:                                return qApp->translate("MessageTypes", "Unknown");
                    }
                }
            };

        }

    }

}
