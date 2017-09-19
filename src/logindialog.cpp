#include "logindialog.h"
#include "ui_logindialog.h"

#include <QtCore/QSettings>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    loadDefaultData();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QVariantMap LoginDialog::loginInfo() const
{
    return QVariantMap({
        { "login", ui->lineEdit_username->text() },
        { "password", ui->lineEdit_password->text() },
        { "domain", ui->lineEdit_domain->text() },
        { "server", ui->spinBox_server->text() }
                       });
}

void LoginDialog::accept()
{
    storeDefaultData();
    QDialog::accept();
}

void LoginDialog::initializeConnections() const
{
    connect(ui->buttonBox,  &QDialogButtonBox::accepted,
            this,           &LoginDialog::accept);

    connect(ui->buttonBox,  &QDialogButtonBox::rejected,
            this,           &LoginDialog::reject);
}

void LoginDialog::loadDefaultData()
{
    QSettings settings;
    settings.beginGroup(DefaultLogin);

    bool saveDefaults = settings.value(SaveDefaults, false).toBool();
    ui->checkBox_saveAsDefaults->setChecked(saveDefaults);

    if (saveDefaults) {
        ui->lineEdit_username->setText(settings.value(Login, QString()).toString());
        ui->lineEdit_domain->setText(settings.value(Domain, QString()).toString());
        ui->spinBox_server->setValue(settings.value(Server, 1).toInt());
    }
}

void LoginDialog::storeDefaultData() const
{
    QSettings settings;
    settings.beginGroup(DefaultLogin);

    bool saveDefaults = ui->checkBox_saveAsDefaults->isChecked();
    settings.setValue(SaveDefaults, saveDefaults);

    if (saveDefaults) {
        settings.setValue(Login, ui->lineEdit_username->text());
        settings.setValue(Domain, ui->lineEdit_domain->text());
        settings.setValue(Server, ui->spinBox_server->value());
    }
}
