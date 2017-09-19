#pragma once

#include "player.h"

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

#include <functional>

class ApiGateway : public QObject
{
    Q_OBJECT

public:
    static constexpr auto ErrorsName = "Errors";

    enum Errors {
        NotConfigured,
        NotLogged,
        RidNotParsed
    };
    Q_ENUM(Errors)

    explicit ApiGateway(QObject *parent = nullptr);
    void setOptions(const QVariantMap &options);

    void login();
    void logout();

    void getFarmInfo();

    inline bool isLoggedIn() const { return m_loggedIn; }
    inline bool isConfigured() const { return m_configured; }

signals:
    void loggedInChanged(bool changed);
    void errorRaised(ApiGateway::Errors error, const QString &message);

private:
    void recursiveRedirect(const QString &url, const std::function<void (QNetworkReply *)> &callback);
    void buildHeaders(QNetworkRequest &request) const;
    bool extractRid(QNetworkReply *reply);

    QUrl tokenUrl() const;
    QUrl endpointUrl(const QString &endpoint, const QList<QPair<QString, QString>> &data, bool includeRid = true) const;
    QUrl endpointAjaxUrl(const QString &endpoint, const QList<QPair<QString, QString>> &data, bool includeRid = true) const;

    void setLoggedIn (bool loggedIn);
    bool handleNotLogged(const QString &operation);

    void raiseError(ApiGateway::Errors error, const QString &message);

private:
    bool m_loggedIn;
    bool m_configured;

    struct {
        QString login;
        QString password;
        QString domain;
        QString server;
    } m_options;
    QString m_rid;

    QNetworkAccessManager m_manager;
    Player m_player;

    static bool s_firstRun;
};
