#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

#include <functional>

class AccountManager : public QObject
{
    Q_OBJECT

public:
    static constexpr auto ErrorsName = "Errors";

    enum Errors {
        NotLogged,
        RidNotParsed
    };
    Q_ENUM(Errors)

    struct Options {
        QString login;
        QString password;
        QString domain;
        QString server;
    };

    explicit AccountManager(QObject *parent = nullptr);
    inline void setOptions(const Options &options) { m_options = options; }

    void login();
    void logout();

    void getFarmInfo();

    inline bool isLoggedIn() const { return m_loggedIn; }

signals:
    void loggedInChanged(bool changed);
    void errorRaised(AccountManager::Errors error, const QString &message);

public slots:

private:
    void recursiveRedirect(const QString &url, const std::function<void (QNetworkReply *)> &callback);
    void buildHeaders(QNetworkRequest &request) const;
    bool extractRid(QNetworkReply *reply);

    QUrl tokenUrl() const;
    QUrl endpointUrl(const QString &endpoint, const QList<QPair<QString, QString>> &data, bool includeRid = true) const;

    void setLoggedIn (bool loggedIn);
    bool handleNotLogged(const QString &operation);

    void raiseError(AccountManager::Errors error, const QString &message);

private:
    QNetworkAccessManager m_manager;
    Options m_options;
    QString m_rid;

    bool m_loggedIn;
};

#endif // ACCOUNTMANAGER_H
