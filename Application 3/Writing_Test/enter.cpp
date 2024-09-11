#include "enter.h"
#include "ui_enter.h"
#include "mainwindow.h"

enter::enter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::enter)
{
    ui->setupUi(this);


    ui->savePath->setReadOnly(true);
    ui->savePath->setText(savingPath);

    // Disable maximize button
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
}

enter::~enter()
{
    delete ui;
}


// Update the saving path from disktop to another location
void enter::on_changeSavePath_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), savingPath,
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!filePath.isEmpty()) {
        ui->savePath->setText(filePath);
        savingPath = filePath;  // Update the savingPath variable
    }
}

//  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Open mainWindow to start the test %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void enter::on_startTest_clicked()
{
    // take the word number input
    bool okWordNum;
    bool okTime;
    int inputWordNum = ui->wordNum->text().toInt(&okWordNum);

    // take the time in minutes input
    int inputTime = ui->timeMinutes->text().toInt(&okTime);

    if ((okWordNum && inputWordNum > 0) && (okTime && inputTime > 0)) {
        wordCount = inputWordNum;  // Save the valid integer to wordCount
        testTime = inputTime;

        // Create and show the MainWindow instance
        MainWindow *w = new MainWindow(wordCount, testTime, savingPath); // passs variables to mainWindow contructor
        w->setWindowIcon(QIcon(":/new/prefix1/icons/mainWindow.jpg"));
        w->showFullScreen();

    } else {
        ui->warning->setText("Please enter a valid word Number and Time!");
        ui->wordNum->clear();  // Clear the input if invalid
        ui->timeMinutes->clear();
    }
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Define Decode Function %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
QString enter::decode(const QString& encodedText) {
    QByteArray byteArray = QByteArray::fromBase64(encodedText.toUtf8()); // Decode from Base64
    QString decodedText = QString::fromUtf8(byteArray); // Convert QByteArray to QString

    // Replace Chinese symbols back to English letters
    decodedText.replace("你", "a"); // Replace '你' with 'a'
    decodedText.replace("好", "b"); // Replace '好' with 'b'
    decodedText.replace("吗", "c"); // Replace '吗' with 'c'
    decodedText.replace("是", "d"); // Replace '是' with 'd'
    decodedText.replace("嗎", "e"); // Replace '嗎' with 'e'
    decodedText.replace("誰", "f"); // Replace '誰' with 'f'
    decodedText.replace("不", "g"); // Replace '不' with 'g'

    return decodedText;
}

QString enter::readFileToString(const QString& filePath) {
    QFile file(filePath);
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        content = in.readAll();
        file.close();
    } else {
        qDebug() << "Failed to open file for reading: " << filePath;
    }

    return content;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Select file and Decode it to Disktop %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void enter::on_openEncodedFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Text File"),
                                                    savingPath,
                                                    tr("Text Files (*.txt);;All Files (*)"));

    QString writing;

    if (!fileName.isEmpty()) {
        writing = readFileToString(fileName);
        writing = decode(writing);

        // Create a file object and open it for writing
        QFile file(savingPath + "/Decoded writing.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing.";
            return;
        }

        // Write the text to the file
        QTextStream out(&file);
        out << writing;

        // Close the file
        file.close();

        ui->warning->setText(QString("File is saved in: %1").arg(savingPath + "/Decoded writing.txt"));
    } else {
        // Show a warning message box if no file was selected
        QMessageBox::warning(this,
                             tr("No File Selected"),
                             tr("You did not select a file. Please choose a file to proceed."));
    }
}
