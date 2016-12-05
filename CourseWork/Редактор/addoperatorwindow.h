#ifndef ADDOPERATORWINDOW_H
#define ADDOPERATORWINDOW_H

#include <QDialog>

namespace Ui {
class AddOperatorWindow;
}

class AddOperatorWindow : public QDialog
{
    Q_OBJECT
    void checkInput();

public:
    explicit AddOperatorWindow(QWidget *parent = 0);
    ~AddOperatorWindow();

private slots:

    void on_cancelBtn_clicked();
    void on_nameField_textChanged();
    void on_surnameField_textChanged();
    void on_fathersNameField_textChanged();
    void on_loginField_textChanged();
    void on_passwordField_textChanged();
    void on_addBtn_clicked();

private:
    Ui::AddOperatorWindow *ui;
};

#endif // ADDOPERATORWINDOW_H
