#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "enter.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QTextEdit>
#include <QContextMenuEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // declare enter class variables in the mainWindow contructor
    MainWindow(int wordCount, int testTime, QString savingPath, QWidget *parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject *obj, QEvent *event);
    void updateTimer();
    static QString encode(const QString& text);


    // Program Parameters
    int time;
    int WordsNumber;

    // program calculations
    int currentWordCount;


private slots:
    void on_Submit_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;

    QString savingPath2;

    QTimer *timer;
    int timeInSeconds;

    // Subclass QTextEdit to customize context menu behavior
    class CustomTextEdit : public QTextEdit
    {
    public:
        CustomTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {}

    protected:

        // Override contextMenuEvent to prevent the context menu from appearing
        void contextMenuEvent(QContextMenuEvent *event) override
        {
            // Prevent the context menu from appearing
            event->ignore();
        }
    };

};
#endif // MAINWINDOW_H
