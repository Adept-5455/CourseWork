#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_surnameCheckBox_clicked()
{
    if(ui->surnameCheckBox->isChecked()) ui->surnameFindField->setEnabled(true);
    else ui->surnameFindField->setEnabled(false);
}

void MainMenu::on_specialityCheckBox_clicked()
{
    if(ui->specialityCheckBox->isChecked()) ui->specialityFindBox->setEnabled(true);
    else ui->specialityFindBox->setEnabled(false);
}

void MainMenu::on_dateCheckBox_clicked()
{
    if(ui->dateCheckBox->isChecked()) ui->dateFindBox->setEnabled(true);
    else ui->dateFindBox->setEnabled(false);
}
