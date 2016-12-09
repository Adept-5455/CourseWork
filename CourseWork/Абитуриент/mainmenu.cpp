#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    connect(ui->loadFromDB, SIGNAL(triggered(bool)), this, SLOT(connectToDB()));
    connect(LW.ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(updateTable()));

    connect(ui->surnameField, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(ui->nameField, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(ui->fathersNameField, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(ui->passportField, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(ui->idField, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(ui->adressField, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(ui->telephoneField, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(ui->nameField, SIGNAL(textChanged(QString)), this, SLOT(checkInput()));
    connect(ui->surnameFindField, SIGNAL(textChanged(QString)), this, SLOT(changeFindBtnState()));

    QRegExp RegEx( "^[А-ЯІ][а-яі]+$" );
    QValidator *validator = new QRegExpValidator(RegEx, this);
    ui->surnameField->setValidator(validator);
    ui->nameField->setValidator(validator);
    ui->fathersNameField->setValidator(validator);
    ui->surnameFindField->setValidator(validator);

    RegEx.setPattern("^[А-ЯІ]{2}[0-9]{6}$");
    validator = new QRegExpValidator(RegEx, this);
    ui->passportField->setValidator(validator);

    RegEx.setPattern("380[0-9]{9}$");
    validator = new QRegExpValidator(RegEx, this);
    ui->telephoneField->setValidator(validator);

    RegEx.setPattern("[0-9]{10}$");
    validator = new QRegExpValidator(RegEx, this);
    ui->idField->setValidator(validator);

    for(int i=0; i<ui->specialityBox->count(); i++)
    {
        ui->specialityFindBox->addItem(ui->specialityBox->itemText(i));
    }
}

void MainMenu::setHeaderData()
{
    abitListModel->setHeaderData(0, Qt::Horizontal, tr("Прізвище"));
    abitListModel->setHeaderData(1, Qt::Horizontal, tr("Ім'я"));
    abitListModel->setHeaderData(2, Qt::Horizontal, tr("По-батькові"));
    abitListModel->setHeaderData(3, Qt::Horizontal, tr("Дата народження"));
    abitListModel->setHeaderData(4, Qt::Horizontal, tr("Паспорт"));
    abitListModel->setHeaderData(5, Qt::Horizontal, tr("ІД номер"));
    abitListModel->setHeaderData(6, Qt::Horizontal, tr("Телефон"));
    abitListModel->setHeaderData(7, Qt::Horizontal, tr("Адреса"));
    abitListModel->setHeaderData(8, Qt::Horizontal, tr("ОКР"));
    abitListModel->setHeaderData(9, Qt::Horizontal, tr("Бал"));
    abitListModel->setHeaderData(10, Qt::Horizontal, tr("Форма навчання"));
    abitListModel->setHeaderData(11, Qt::Horizontal, tr("Спеціальність"));
    abitListModel->setHeaderData(12, Qt::Horizontal, tr("ПК"));
    abitListModel->setHeaderData(13, Qt::Horizontal, tr("Гуртожиток"));
    abitListModel->setHeaderData(14, Qt::Horizontal, tr("Дата внесення"));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::updateTable()
{
    if(LW.successEnter == false)
    {
        return;
    }

    abitListModel = new QSqlQueryModel(this);

    abitListModel->setQuery(" SELECT [Прізвище], [Ім'я], [По-батькові],"
                            "strftime('%d/%m/%Y', [Дата народження]), [Паспорт], [Ідентифікаційний номер],"
                            "[Контактний телефон], [Домашня адреса], [Освітньо-кваліфікаційний рівень],"
                            "[Рейтинговий бал], [Форма навчання], [Спеціальність],"
                            "[Поза конкурсом], [Потребує гуртожитку],"
                            "strftime('%d/%m/%Y', [Дата внесення]) FROM Абітурієнти ORDER BY [Рейтинговий бал] DESC");

    abitListModel->query().exec();
    qDebug() << abitListModel->query().lastError();

    setHeaderData();

    ui->abitList->setModel(abitListModel);
    ui->abitList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->abitList->resizeColumnsToContents();
    ui->abitList->resizeRowsToContents();

    ui->tabWidget->setEnabled(true);
    ui->addBtn->setEnabled(false);
    ui->findBtn->setEnabled(false);
}

void MainMenu::connectToDB()
{
    QString fileName = "";
    fileName = QFileDialog::getOpenFileName(this,
        tr("Відкриття бази даних"), "/home/ZNTU_Abiturient_DB.db", tr("База даних (*.db)"));

    if(fileName == "")
    {
        return;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    qDebug() << db.databaseName();

    if(db.open())
    {
        qDebug() << "Success!";

        foreach(QString t, db.tables())
        {
            qDebug() << t ;
        }
    }

    else
    {
        qDebug() << "Fail!";
    }


    if(db.databaseName()!="")
    {
        ui->abitList->setModel(NULL);

        ui->tabWidget->setEnabled(false);

        LW.LPVector.clear();

        QSqlQuery query;
        query.exec("SELECT Логін, Пароль FROM Оператори");

        while(query.next())
        {

            LogPass tmpLogPass;

            tmpLogPass.login = query.value(0).toString();
            tmpLogPass.pass =  query.value(1).toString();

            LW.LPVector.push_back(tmpLogPass);

            qDebug() << tmpLogPass.login << ' ' << tmpLogPass.pass;
        }

        LW.successEnter = false;

        LW.ui->loginField->clear();
        LW.ui->passwordField->clear();

        LW.setModal(this);
        LW.show();
    }

}

void MainMenu::checkInput()
{
    bool inputIsOk = (  (ui->nameField->text().length() >= 3)
                      &&(ui->surnameField->text().length() >= 3)
                      &&(ui->fathersNameField->text().length() >= 3)
                      &&(ui->telephoneField->text().length() == 12)
                      &&(ui->idField->text().length() == 10)
                      &&(ui->passportField->text().length() == 8)
                      &&(ui->adressField->text().length() >= 10)   );

    if(inputIsOk) ui->addBtn->setEnabled(true);
    else ui->addBtn->setEnabled(false);
}

void MainMenu::on_surnameCheckBox_clicked()
{
    if(ui->surnameCheckBox->isChecked()) ui->surnameFindField->setEnabled(true);
    else ui->surnameFindField->setEnabled(false);

    changeFindBtnState();
}

void MainMenu::on_specialityCheckBox_clicked()
{
    if(ui->specialityCheckBox->isChecked()) ui->specialityFindBox->setEnabled(true);
    else ui->specialityFindBox->setEnabled(false);

    changeFindBtnState();
}

void MainMenu::on_dateCheckBox_clicked()
{
    if(ui->dateCheckBox->isChecked()) ui->dateFindBox->setEnabled(true);
    else ui->dateFindBox->setEnabled(false);

    changeFindBtnState();
}

void MainMenu::changeFindBtnState()
{
    bool stateToSet = (ui->dateCheckBox->isChecked()) ||
                      (ui->specialityCheckBox->isChecked() ||
                       (ui->surnameCheckBox->isChecked() &&
                        ui->surnameFindField->text().length() > 0));

    ui->findBtn->setEnabled(stateToSet);
}

void MainMenu::on_addBtn_clicked()
{
    QMessageBox* acceptDialog = new QMessageBox(QMessageBox::Question,"Підтвердіть дію",
                                                 "Підтвердіть внесення нового абітурієнта!",
                                                 QMessageBox::Yes |
                                                 QMessageBox::No);
    acceptDialog->setButtonText(QMessageBox::Yes, "Так");
    acceptDialog->setButtonText(QMessageBox::No, "Ні");
    acceptDialog->show();

    connect(acceptDialog->button(QMessageBox::Yes), SIGNAL(clicked(bool)) ,this, SLOT(insertData()));
}

void MainMenu::insertData()
{
    QSqlQuery query;

    query.prepare(" INSERT INTO [Абітурієнти] "
                  "([Прізвище], [Ім'я], [По-батькові],"
                  "[Дата народження], [Паспорт], [Ідентифікаційний номер],"
                  "[Контактний телефон], [Домашня адреса], [Освітньо-кваліфікаційний рівень],"
                  "[Рейтинговий бал], [Форма навчання], [Спеціальність],"
                  "[Поза конкурсом], [Потребує гуртожитку], [Дата внесення])"

                  " VALUES "
                  "(:surname, :name, :fathersname, "
                  ":dateofbirth, :passport, :id, :telephone,"
                  ":adress, :edlevel, :rating, :edform, :spec,"
                  ":outside, :chumneed, :dateofadding)");

    QString name = ui->nameField->text();
    QString surname = ui->surnameField->text();
    QString fathersname = ui->fathersNameField->text();
    QDate dateofbirth = ui->dateOfBirth->date();
    QString passport = ui->passportField->text();
    QString id = ui->idField->text();
    QString telephone = ui->telephoneField->text();
    QString adress = ui->adressField->text();
    QString edlevel = ui->levelBox->currentText();
    double rating = ui->ratingBox->value();
    QString edform = ui->formBox->currentText();
    QString spec = ui->specialityBox->currentText();
    QString outside = (ui->outsideCheckBox->isChecked()) ? "Так" : "Ні" ;
    qDebug() << outside;
    QString chumneed = (ui->chummeryCheckBox->isChecked()) ? "Так" : "Ні" ;
    qDebug() << chumneed;
    QDate dateofadding = QDate::currentDate();

    query.bindValue(":surname", surname);
    query.bindValue(":name", name);
    query.bindValue(":fathersname", fathersname);
    query.bindValue(":dateofbirth", dateofbirth);
    query.bindValue(":passport", passport);
    query.bindValue(":id", id);
    query.bindValue(":telephone", telephone);
    query.bindValue(":adress", adress);
    query.bindValue(":edlevel", edlevel);
    query.bindValue(":rating", rating);
    query.bindValue(":edform", edform);
    query.bindValue(":spec", spec);
    query.bindValue(":outside", outside);
    query.bindValue(":chumneed", chumneed);
    query.bindValue(":dateofadding", dateofadding);

    if(query.exec())
    {
        qDebug() << "Inserted succesfully!";
    }
    else qDebug() << query.lastError();

    ui->nameField->clear();
    ui->surnameField->clear();
    ui->fathersNameField->clear();
    ui->passportField->clear();
    ui->idField->clear();
    ui->telephoneField->setText("380");
    ui->adressField->clear();
    ui->levelBox->currentText();
    ui->ratingBox->setValue(100.00);
    ui->outsideCheckBox->setChecked(false);
    ui->chummeryCheckBox->setChecked(false);

    updateTable();
}

void MainMenu::on_findBtn_clicked()
{
    QString queryText = "SELECT [Прізвище], [Ім'я], [По-батькові],"
            "strftime('%d/%m/%Y', [Дата народження]), [Паспорт], [Ідентифікаційний номер],"
            "[Контактний телефон], [Домашня адреса], [Освітньо-кваліфікаційний рівень],"
            "[Рейтинговий бал], [Форма навчання], [Спеціальність],"
            "[Поза конкурсом], [Потребує гуртожитку], strftime('%d/%m/%Y', [Дата внесення]) FROM Абітурієнти WHERE ";

    bool onlyOneParameter = true;

    if(ui->specialityCheckBox->isChecked())
    {
        queryText += "Спеціальність = '" + ui->specialityBox->currentText() + "' ";
        onlyOneParameter = false;
    }

    if(ui->surnameCheckBox->isChecked())
    {
        if(!onlyOneParameter) queryText += "AND";
        queryText += " Прізвище = '" + ui->surnameFindField->text() + "' ";
    }

    if(ui->dateCheckBox->isChecked())
    {
        if(!onlyOneParameter) queryText += "AND";
        queryText += " [Дата внесення] = '" + ui->dateFindBox->date().toString("yyyy-MM-dd") + "' ";
    }

    qDebug() << queryText;

    QSqlQuery query(queryText);

    if(query.exec())
        qDebug() << "Succesfully selected!";
    else qDebug() << query.lastError();

    abitListModel = new QSqlQueryModel(this);
    abitListModel->setQuery(query);
    setHeaderData();

    ui->abitList->setModel(abitListModel);
    ui->abitList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->abitList->resizeColumnsToContents();
    ui->abitList->resizeRowsToContents();
}

void MainMenu::on_refreshBtn_clicked()
{
    ui->surnameCheckBox->setChecked(false);
    ui->surnameFindField->clear();

    ui->specialityCheckBox->setChecked(false);
    ui->dateCheckBox->setChecked(false);

    updateTable();
}
