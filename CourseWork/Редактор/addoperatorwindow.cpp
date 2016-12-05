#include "addoperatorwindow.h"
#include "ui_addoperatorwindow.h"
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>

AddOperatorWindow::AddOperatorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOperatorWindow)
{
    ui->setupUi(this);
}

AddOperatorWindow::~AddOperatorWindow()
{
    delete ui;
}

void AddOperatorWindow::checkInput()
{
    if((ui->surnameField->text().length()==0 ||
            ui->nameField->text().length()==0 ||
            ui->fathersNameField->text().length()==0 ||
            ui->loginField->text().length()==0 ||
            ui->passwordField->text().length()==0))

    {
        this->ui->addBtn->setEnabled(false);
    }

    else
    {
        this->ui->addBtn->setEnabled(true);
    }
}

void AddOperatorWindow::on_cancelBtn_clicked()
{
    this->close();
}

void AddOperatorWindow::on_nameField_textChanged()
{
    checkInput();
}

void AddOperatorWindow::on_surnameField_textChanged()
{
    checkInput();
}

void AddOperatorWindow::on_fathersNameField_textChanged()
{
    checkInput();
}

void AddOperatorWindow::on_loginField_textChanged()
{
    checkInput();
}

void AddOperatorWindow::on_passwordField_textChanged()
{
    checkInput();
}

void AddOperatorWindow::on_addBtn_clicked()
{
      QSqlQuery query;

      query.prepare("INSERT INTO Оператори (Логін, Пароль, ПІБ) "
                    "VALUES (:login, :password, :name)");

      QString name = ui->surnameField->text() + ' '
                   + ui->nameField->text() + ' ' + ui->fathersNameField->text();

      query.bindValue(":login", ui->loginField->text());
      query.bindValue(":password", ui->passwordField->text());
      query.bindValue(":name", name);

      query.exec();

      this->close();
}
