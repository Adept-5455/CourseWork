#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include <QXmlStreamWriter>

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    QFile styleF;

    styleF.setFileName(":/Style/Style/QSS.css");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();

    qApp->setStyleSheet(qssStr);

    connect(ui->loadFromDB, SIGNAL(triggered(bool)), this, SLOT(connectToDB()));
    connect(ui->generateXML, SIGNAL(triggered(bool)), this, SLOT(generateXML()));
    connect(ui->exit, SIGNAL(triggered(bool)), this, SLOT(close()));
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
    connect(ui->surnameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeFindBtnState()));
    connect(ui->specialityCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeFindBtnState()));
    connect(ui->dateCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeFindBtnState()));

    QRegExp RegEx("^[А-ЯІЄЇ][а-яієї]+$");
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

    ui->generateXML->setEnabled(false);
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

void MainMenu::generateXML()
{
    QString fileName = "";
    fileName = QFileDialog::getSaveFileName(this,
        tr("Збереження XML"), "/home/ZNTU_Abiturient_XML.xml", tr("XML (*.xml)"));

    if(fileName == "")
    {
        return;
    }

    qDebug() << "XML to save:" << fileName;

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.setCodec("windows-1251");
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("database");

    for(int i=0; i<ui->abitList->model()->rowCount(); i++)
    {
        xmlWriter.writeStartElement("abiturient");
        xmlWriter.writeAttribute("surname",         ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 0)).toString());
        xmlWriter.writeAttribute("name",            ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 1)).toString());
        xmlWriter.writeAttribute("fathersname",     ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 2)).toString());
        xmlWriter.writeAttribute("dateofbirth",     ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 3)).toString());
        xmlWriter.writeAttribute("passport",        ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 4)).toString());
        xmlWriter.writeAttribute("id",              ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 5)).toString());
        xmlWriter.writeAttribute("telephone",       ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 6)).toString());
        xmlWriter.writeAttribute("address",         ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 7)).toString());
        xmlWriter.writeAttribute("level",           ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 8)).toString());
        xmlWriter.writeAttribute("rating",          ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 9)).toString());
        xmlWriter.writeAttribute("form",            ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 10)).toString());
        xmlWriter.writeAttribute("speciality",      ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 11)).toString());
        xmlWriter.writeAttribute("outsider",        ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 12)).toString());
        xmlWriter.writeAttribute("chumneed",        ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 13)).toString());
        xmlWriter.writeAttribute("dateofadding",    ui->abitList->model()->data
                                                    (ui->abitList->model()->index(i, 14)).toString());

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    file.close();
}

void MainMenu::updateTable()
{
    if(LW.successEnter == false)
    {
        return;
    }

    QSqlQuery query;
    query.exec(" SELECT [ПІБ] FROM Оператори WHERE [Логін] = '" + LW.correct.login + "' ");
    query.next();
    ui->statusLabel->setText("Оператор: " + query.value(0).toString());

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
    ui->generateXML->setEnabled(true);
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

    DBI.connectToDB(fileName);

    if(DBI.DB().databaseName()!="")
    {
        ui->abitList->setModel(NULL);

        ui->tabWidget->setEnabled(false);
        ui->generateXML->setEnabled(false);
        ui->statusLabel->setText("Слід обрати БД для завантаження в меню \"Файл\".");

        LW.LPVector.clear();
        LW.correct.clear();

        QSqlQuery query = DBI.execQuery("SELECT Логін, Пароль FROM Оператори");

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
    bool stateToSet =  ui->dateCheckBox->isChecked() ||
                       ui->specialityCheckBox->isChecked() ||
                       ui->surnameCheckBox->isChecked();

    if(ui->surnameFindField->text().length() == 0 && ui->surnameCheckBox->isChecked()) stateToSet = false;
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
    QStringList parameterName;

    parameterName << "Прізвище" <<  "Ім'я" << "По-батькові"
                  << "Дата народження" << "Паспорт" << "Ідентифікаційний номер"
                  << "Контактний телефон" <<  "Домашня адреса" << "Освітньо-кваліфікаційний рівень"
                  << "Рейтинговий бал" << "Форма навчання" << "Спеціальність"
                  << "Поза конкурсом" << "Потребує гуртожитку" << "Дата внесення";



    QVariantList parameterValue;

    parameterValue << ui->surnameField->text()
                   << ui->nameField->text()
                   << ui->fathersNameField->text()
                   << ui->dateOfBirth->date()
                   << ui->passportField->text()
                   << ui->idField->text()
                   << ui->telephoneField->text()
                   << ui->adressField->text()
                   << ui->levelBox->currentText()
                   << ui->ratingBox->value()
                   << ui->formBox->currentText()
                   << ui->specialityBox->currentText()
                   << ((ui->outsideCheckBox->isChecked()) ? "Так" : "Ні")
                   << ((ui->chummeryCheckBox->isChecked()) ? "Так" : "Ні")
                   << QDate::currentDate();

    DBI.addToDB("Абітурієнти", parameterName, parameterValue);

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
    QStringList filterName;
    QVariantList filterValue;
    QString defaultQueryText = "SELECT [Прізвище], [Ім'я], [По-батькові],"
            "strftime('%d/%m/%Y', [Дата народження]), [Паспорт], [Ідентифікаційний номер],"
            "[Контактний телефон], [Домашня адреса], [Освітньо-кваліфікаційний рівень],"
            "[Рейтинговий бал], [Форма навчання], [Спеціальність],"
            "[Поза конкурсом], [Потребує гуртожитку], strftime('%d/%m/%Y', [Дата внесення]) FROM Абітурієнти";

    if(ui->specialityCheckBox->isChecked())
    {
        filterName << "Спеціальність";
        filterValue << ui->specialityFindBox->currentText();
    }

    if(ui->surnameCheckBox->isChecked())
    {
        filterName << "Прізвище";
        filterValue << ui->surnameFindField->text();
    }

    if(ui->dateCheckBox->isChecked())
    {
        filterName << "Дата внесення";
        filterValue <<  ui->dateFindBox->date().toString("yyyy-MM-dd");
    }

    abitListModel = new QSqlQueryModel(this);
    abitListModel->setQuery(DBI.selectWithFilters(defaultQueryText, filterName, filterValue));
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

    ui->surnameFindField->setEnabled(false);
    ui->specialityFindBox->setEnabled(false);
    ui->dateFindBox->setEnabled(false);

    updateTable();
}
