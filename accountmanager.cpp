#include "accountmanager.h"

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QMetaEnum>
#include <QtCore/QRegularExpression>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtDebug>

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
    , m_loggedIn(false)
{

}

void AccountManager::login()
{
    QNetworkRequest request(tokenUrl());

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);

    buildHeaders(request);

    QUrlQuery credentials;
    credentials.setQueryItems({
        { "server", m_options.server },
        { "username", m_options.login },
        { "password", m_options.password },
        { "ref", "" }, { "retid", "" }
    });

    auto reply = m_manager.post(request, credentials.toString().toLocal8Bit());
    connect(reply, &QNetworkReply::finished, [this, reply] () {
        const auto data = QJsonDocument::fromJson(reply->readAll());
        if (data.isArray()) {
            recursiveRedirect(data.array().last().toString(), [this] (QNetworkReply *reply) {
                if (extractRid(reply)) {
                    setLoggedIn(true);
                } else {
                    raiseError(RidNotParsed, tr("Unable to extract `rid`. Login failed."));
                }
            });
        } else {
            raiseError(NotLogged, tr("Unable to login. Invalid credentials."));
        }
    });
}

void AccountManager::logout()
{
    QNetworkRequest request(endpointUrl("main", {
        { "page", "logout" },
        { "logoutbutton", "1" }
    }, false));
    buildHeaders(request);

    qDebug() << request.url();

    auto reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, [this, reply] () {
        setLoggedIn(false);
        m_manager.clearAccessCache();
    });
}

void AccountManager::getFarmInfo()
{
    if (handleNotLogged(FUNCTION_NAME))
        return;
}

void AccountManager::recursiveRedirect(const QString &url, const std::function<void (QNetworkReply *)> &callback)
{
    QNetworkRequest request(url);
    buildHeaders(request);

    auto reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, [this, reply, callback] () {
        const auto redirectUrl = reply->header(QNetworkRequest::LocationHeader);
        if (redirectUrl.isValid()) {
            recursiveRedirect(redirectUrl.toString(), callback);
        } else {
            callback(reply);
        }
    });
}

void AccountManager::buildHeaders(QNetworkRequest &request) const
{
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
}

bool AccountManager::extractRid(QNetworkReply *reply)
{
    QString content = reply->read(1024);
    if (content.isEmpty())
        return false;

    QRegularExpression ridRegex = QRegularExpression("var rid = '(?<rid>.*)'");
    const QString rid = ridRegex.match(content).captured("rid");

    m_rid = rid;
    return !rid.isEmpty();
}

QUrl AccountManager::tokenUrl() const
{
    return QUrl(QString("https://www.%1/ajax/createtoken2.php?n=%2")
                .arg(m_options.domain)
                .arg(QDateTime::currentMSecsSinceEpoch()));
}

QUrl AccountManager::endpointUrl(const QString &endpoint,
                                 const QList<QPair<QString, QString> > &data,
                                 bool includeRid) const
{
    QUrlQuery query;
    query.setQueryItems(data);
    if (includeRid) {
        query.addQueryItem("rid", m_rid);
    }

    return QUrl(QString("http://s%1.%2/%3.php?%4")
                .arg(m_options.server)
                .arg(m_options.domain)
                .arg(endpoint)
                .arg(query.toString()));
}

void AccountManager::setLoggedIn(bool loggedIn)
{
    if (m_loggedIn != loggedIn) {
        m_loggedIn = loggedIn;
        emit loggedInChanged(m_loggedIn);
    }
}

bool AccountManager::handleNotLogged(const QString &operation)
{
    bool notLogged = !m_loggedIn;
    if (notLogged) {
        raiseError(NotLogged, tr("Action %1 requires to be logged in.").arg(operation));
    }

    return notLogged;
}

void AccountManager::raiseError(AccountManager::Errors error, const QString &message)
{
    emit errorRaised(error, message);

    const QMetaEnum errorEnumerator = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator(ErrorsName));
    const QString errorMessage = tr("Error `%1` raised. %2").arg(errorEnumerator.key(error)).arg(message);

    qCritical() << errorMessage;
}
