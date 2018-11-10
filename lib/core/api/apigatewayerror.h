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

#include <QtCore/QString>
#include <QtCore/QCoreApplication>

namespace Core {

    namespace Api {

        struct ApiGatewayError
        {
            enum class ErrorType {
                NotConfigured,
                InvalidCredentials,
                NotLogged,
                RidNotParsed
            } type;

            QString toString() const
            {
                switch (type) {
                case ErrorType::NotConfigured: return "NotConfigured";
                case ErrorType::InvalidCredentials: return "InvalidCredentials";
                case ErrorType::NotLogged: return "Action %1 requires to be logged in.";
                case ErrorType::RidNotParsed: return "Unable to extract `rid`. Login failed.";
                }

                return "Unknown error";
            }

            QString message() const
            {
                switch (type) {
                case ErrorType::NotConfigured:
                    return qApp->translate("ApiGatewayError",
                                           "Unable to login with unspecified credentials.");
                case ErrorType::InvalidCredentials:
                    return qApp->translate("ApiGatewayError",
                                           "Unable to extract `rid`. Login failed.");
                case ErrorType::NotLogged:
                    return qApp->translate("ApiGatewayError",
                                           "Action %1 requires to be logged in.");
                case ErrorType::RidNotParsed:
                    return qApp->translate("ApiGatewayError",
                                           "Unable to extract `rid`. Login failed.");
                }

                return qApp->translate("ApiGatewayError", "Unknown error");
            }

            ApiGatewayError (ErrorType exceptionType) : type(exceptionType) { }
        };

    }

}

