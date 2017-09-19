#pragma once

#include <QtCore/QVariantMap>
#include <QtWidgets/QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

    static constexpr auto DefaultLogin = "DefaultLogin";

    static constexpr auto Login = "Login";
    static constexpr auto Domain = "Domain";
    static constexpr auto Server = "Server";
    static constexpr auto SaveDefaults = "SaveDefaults";

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QVariantMap loginInfo() const;

public slots:
    void accept();

private:
    void initializeConnections() const;
    void loadDefaultData();
    void storeDefaultData() const;

private:
    Ui::LoginDialog *ui;
};
