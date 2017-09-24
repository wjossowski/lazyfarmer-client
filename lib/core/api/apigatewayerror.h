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

struct ApiGatewayError
{
    enum Type {
        NotConfigured,
        InvalidCredentials,
        NotLogged,
        RidNotParsed
    } type;

    QString toString() const
    {
        switch (type) {
        case NotConfigured: return "NotConfigured";
        case InvalidCredentials: return "InvalidCredentials";
        case NotLogged: return "Action %1 requires to be logged in.";
        case RidNotParsed: return "Unable to extract `rid`. Login failed.";
        default: return "Unknown error";
        }
    }

    QString message() const
    {
        switch (type) {
        case NotConfigured:
            return qApp->translate("ApiGatewayError",
                                   "Unable to login with unspecified credentials.");
        case InvalidCredentials:
            return qApp->translate("ApiGatewayError",
                                   "Unable to extract `rid`. Login failed.");
        case NotLogged:
            return qApp->translate("ApiGatewayError",
                                   "Action %1 requires to be logged in.");
        case RidNotParsed:
            return qApp->translate("ApiGatewayError",
                                   "Unable to extract `rid`. Login failed.");
        default:
            return qApp->translate("ApiGatewayError",
                                   "Unknown error");
        }
    }

    ApiGatewayError (Type exceptionType) : type(exceptionType) {}
};
