#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "functions.h"
#include "submitattendance.h"

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

private slots:
    void on_newClass_clicked();

    void on_reloadClass_clicked();

    void on_selectClass_currentIndexChanged();

    void on_renameClass_clicked();

    void on_deleteClass_clicked();

    void on_reload_clicked();

    void on_actionImport_Data_triggered();

    void on_insertStudent_clicked();

    void on_removeStudent_clicked();

    void on_actionExport_Data_triggered();

    void on_notAllowEdit_clicked();

    void on_allowEdit_clicked();

    void on_Search_clicked();



    void on_addAttendance_clicked();

    void on_removeDay_clicked();

    void handleDataChanged(const QModelIndex &topLeft);

private:
    Ui::MainWindow *ui;

    Functions fcn;
};
#endif // MAINWINDOW_H
