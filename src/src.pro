QT       += core gui widgets network xml
CONFIG   += c++14

TARGET = LazyFarmer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
VERSION = 0.1.0.0

COMPANY_NAME = "WololoPl"

DEFINES += "CURRENT_VERSION=\\\"$${VERSION}\\\""
DEFINES += "APPLICATION_NAME=\\\"$${TARGET}\\\""
DEFINES += "COMPANY_NAME=\\\"$${COMPANY_NAME}\\\""
DEFINES += "FUNCTION_NAME=__FUNCTION__"

win32 {
    QMAKE_TARGET_COMPANY = $${COMPANY_NAME}
}

INCLUDEPATH += $${PWD}/core $${PWD}/widgets

SOURCES += \
          $${PWD}/core/*.cpp \
          $${PWD}/widgets/*.cpp \
          main.cpp

HEADERS += \
          $${PWD}/core/*.h \
          $${PWD}/widgets/*.h

FORMS += \
        $${PWD}/widgets/*.ui

CONFIG(debug, debug|release) {
    DEFINES += DEBUG_MODE

    INCLUDEPATH += $${PWD}/helpers

    SOURCES += \
              $${PWD}/helpers/*.cpp
    HEADERS += \
              $${PWD}/helpers/*.h

} else {
  message("release")
}
