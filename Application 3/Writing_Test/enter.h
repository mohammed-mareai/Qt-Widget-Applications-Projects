#ifndef ENTER_H
#define ENTER_H

#include <QWidget>
#include <QFileDialog>
#include <qstandardpaths.h>
#include <QMessageBox>

namespace Ui {
class enter;
}

class enter : public QWidget
{
    Q_OBJECT

public:
    explicit enter(QWidget *parent = nullptr);
    ~enter();

    // File parameters
    int wordCount = 0;
    int testTime = 0;
    QString savingPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);



private slots:


    void on_changeSavePath_clicked();

    void on_startTest_clicked();

    void on_openEncodedFile_clicked();

private:
    Ui::enter *ui;

    // file functions
    static QString decode(const QString& encodedText);
    QString readFileToString(const QString& filePath);
};

#endif // ENTER_H
