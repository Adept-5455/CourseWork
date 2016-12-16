#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QFile>
#include <QDebug>

#include "loginwindow.h"
#include "addoperatorwindow.h"
#include "dbinterface.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    DBInterface DBI;
    void setHeadersData(QAbstractItemModel*, QAbstractItemModel*);
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();


private slots:
    void on_addBtn_clicked();
    void generateXML();
    void connectToDB();
    void on_operatorsTable_clicked(const QModelIndex &index);
    void on_delBtn_clicked();
    void on_delBtn_2_clicked();
    void on_ratingTable_clicked(const QModelIndex &index);

public slots:
        void updateTables();

signals:
    needUpdate();
    needClearFields();

private:

    Ui::MainMenu *ui;

    QSqlTableModel *operatorsModel;
    QSqlTableModel *ratingModel;

    AddOperatorWindow AOW;
    LoginWindow LW;
    QModelIndexList indexes;
};

#endif // MAINMENU_H
