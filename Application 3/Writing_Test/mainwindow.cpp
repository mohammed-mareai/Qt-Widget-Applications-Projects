#include "mainwindow.h"
#include "ui_mainwindow.h"

// declare enter class variables in the mainWindow contructor
MainWindow::MainWindow(int wordCount, int testTime, QString savingPath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Extract the variables from enter class
    WordsNumber = wordCount;
    time = testTime * 60;
    savingPath2 = savingPath;

    // Set the context menu policy for the existing textEdit
    ui->textEdit->setContextMenuPolicy(Qt::NoContextMenu);

    // Install event filter on the textEdit widget
    ui->textEdit->installEventFilter(this);

    // Initialize timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer->start(1000); // 1 second interval
    timeInSeconds = time; // 60 minutes * 60 seconds
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Save the file if the window was closed.
void MainWindow::closeEvent(QCloseEvent *event)
{
    // Call the on_Submit_clicked() function before closing
    on_Submit_clicked();

    // Proceed with the default close behavior
    event->accept();
}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Update time and progress bar %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%555
void MainWindow::updateTimer() {
    if (timeInSeconds > 0) {
        --timeInSeconds;
        int minutes = timeInSeconds / 60;
        int seconds = timeInSeconds % 60;
        QString timeString = QString("%1:%2")
                                 .arg(minutes, 2, 10, QChar('0'))
                                 .arg(seconds, 2, 10, QChar('0'));
        ui->timerLabel->setText(timeString);


        QString text = ui->textEdit->toPlainText(); // Get the text from the textEdit widget
        QRegularExpression re("\\W+"); // Regular expression to split text into words
        currentWordCount = text.split(re, Qt::SkipEmptyParts).count();
        ui->label->setText(QString("Word Number: %1/%2").arg(currentWordCount).arg(WordsNumber));

        if (WordsNumber > currentWordCount){
            int progressPercent = (WordsNumber > 0) ? static_cast<int>(static_cast<float>(currentWordCount) / WordsNumber * 100) : 0;
            ui->progressBar->setValue(progressPercent);
        } else if (WordsNumber < currentWordCount){
            ui->progressBar->setValue(100);
        }

    } else {
        on_Submit_clicked(); // Trigger submit action when time reaches 00:00
    }
}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Filter Mouse and Shortcuts events %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        // Block Ctrl, Alt, and system keys like Alt+Tab
        if (keyEvent->modifiers() & (Qt::ControlModifier | Qt::AltModifier))
        {
            // Block Alt+Tab (Qt::Key_Tab) or any other system key combination
            if (keyEvent->key() == Qt::Key_Tab)
            {
                return true; // Ignore Alt+Tab
            }
            return true; // Ignore Ctrl or Alt combinations
        }
        // Allow Shift key only
        if (keyEvent->modifiers() == Qt::ShiftModifier)
        {
            return false;
        }
    }

    return QMainWindow::eventFilter(obj, event); // Call the base class implementation for other events
}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Encode the Text %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
QString MainWindow::encode(const QString& text) {
    QString modifiedText = text;
    // Replace English letters with Chinese symbols
    modifiedText.replace("a", "你"); // Replace 'a' with '你'
    modifiedText.replace("b", "好"); // Replace 'b' with '好'
    modifiedText.replace("c", "吗"); // Replace 'c' with '吗'
    modifiedText.replace("d", "是"); // Replace 'd' with '是'
    modifiedText.replace("e", "嗎"); // Replace 'e' with '嗎'
    modifiedText.replace("f", "誰"); // Replace 'f' with '誰'
    modifiedText.replace("g", "不"); // Replace 'g' with '不'

    QByteArray byteArray = modifiedText.toUtf8(); // Convert QString to QByteArray
    return byteArray.toBase64(); // Encode to Base64 and return as QString
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Submit the Text File %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void MainWindow::on_Submit_clicked()
{
    QString text = ui->textEdit->toPlainText(); // Get the text from the textEdit widget
    QString testResult;

    testResult.prepend(QString("This test was initialized with Word Count: %1 and Time: %2 minutes.")
                            .arg(WordsNumber)  // Insert wordCount at %1
                            .arg(time/60));  // Insert test time at %2);

    QString fileName;

    if (currentWordCount >= WordsNumber){
        testResult.append(QString("\nResult: Successful Try with word count: %2 words and time used: %1 minutes.\n\n\n")
                              .arg((time-timeInSeconds)/60).arg(currentWordCount));
        fileName = "Successful Try.txt";
    } else{
        testResult.append(QString("\nResult: Failed Try with word count: %2 words and time used: %1 minutes.\n\n\n")
                              .arg((time-timeInSeconds)/60).arg(currentWordCount));
        fileName = "Failed Try.txt";
    }

    text.prepend(testResult);

    // Create a file object and open it for writing
    QFile file(savingPath2 + "/" + fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writing.";
        return;
    }

    text = MainWindow::encode(text);

    // Write the text to the file
    QTextStream out(&file);
    out << text;

    // Close the file
    file.close();

    // Close the main window
    close();
}
