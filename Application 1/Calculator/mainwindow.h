#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>
#include<QTextEdit>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ShowAppendStr(QTextEdit *LT, QString AppendValue);

    QString OptoShow; // variable for the operation that will be displayed in the QTextEdit
    QString OptoCal; // variable for the operation that will be calculated by calculate1.h functions

private slots:

    // numbers
    void on_one_clicked();

    void on_two_clicked();

    void on_three_clicked();

    void on_four_clicked();

    void on_five_clicked();

    void on_six_clicked();

    void on_seven_clicked();

    void on_eight_clicked();

    void on_nine_clicked();

    void on_zero_clicked();

    // signs and point

    void on_sign_clicked();

    void on_point_clicked();

    // operations

    void on_add_clicked();

    void on_subtract_clicked();

    void on_multiply_clicked();

    void on_divide_clicked();

    // brackets

    void on_Rbracket_clicked();

    void on_Lbracket_clicked();

    // clear

    void on_Clear_clicked();

    void on_Delete_clicked();

    // equal

    void on_equal_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override; // special operation to make equal being clicked if the user clicked enter



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
