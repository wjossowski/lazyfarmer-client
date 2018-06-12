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

        }

    }

}
