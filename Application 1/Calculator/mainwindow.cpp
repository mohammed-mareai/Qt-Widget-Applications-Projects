#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "calculate1.h"

// ************************************************ variables used bellow *****************************************
// these variables are style sheet of the pushbutton. They are used to update the color of the pushbutton temporily when clicked.
// numbers pushbuttons
QString PushbuttonStyleOld = "background-color:#ECECEC; border: 1px solid black;";
QString PushbuttonStyleNew = "background-color:#d9d9d9; border: 1px solid black;";

// operations pushbuttons
QString PushbuttonStyleOld1 = "background-color: #FAE4C2; border: 2px solid black; border-radius: 20px;";
QString PushbuttonStyleNew1 = "background-color: #f1a634; border: 2px solid black; border-radius: 20px;";

// AC, Del, (, ) pushbuttons
QString PushbuttonStyleOld2 = "background-color: #F0CEB4; border: 3px solid black; border-radius: 15px;";
QString PushbuttonStyleNew2 = "background-color: #CD5C08; border: 3px solid black; border-radius: 15px;";

// Equal pushbutton
QString PushbuttonStyleOld3 = "background-color: #9AE9CF; border: 1px solid black; border-radius: 20px;";
QString PushbuttonStyleNew3 = "background-color: #03C988; border: 3px solid black; border-radius: 20px;";


// ************************************************ functions used bellow *****************************************
// this function will change the color of the pushbutton when clicked for 100ms and then make it go back to normal
void changeTemporary(QPushButton *P, const QString &oldStyle, const QString &newStyle) {
    // Temporarily change the style of the push button
    P->setStyleSheet(oldStyle);

    // Start a timer to revert the style after a short delay
    QTimer::singleShot(100, P, [newStyle, P]() {
        P->setStyleSheet(newStyle); // Revert back to the original style
    });
}

// this function will take the input operation from the QTextEdit and append it to a QString variables
void MainWindow::ShowAppendStr(QTextEdit *LT, QString AppendValue){
    OptoShow.append(AppendValue);
    LT->setText(OptoShow);

    OptoCal.append(AppendValue);
    ui->result->clear(); // clear text from result becauese the user now is giving new input operation
}

// this function will check if the last element of the operation string is an operator
bool OperationCheck(QString operation){
    if(operation.isEmpty()){
        return true;
    }
    QChar lastChar = operation[operation.length() - 1];
    return lastChar == '+' || lastChar == '-' || lastChar == '/' || lastChar == '*' || lastChar == '.';
}

// this function will if adding a negative sign is allowed
bool NegativeSignCheck(QString operation){
    if(operation.isEmpty()){
        return false;
    }
    QString lastChar = operation[operation.length() - 1];
    return (lastChar != ".") && (lastChar == '+' || lastChar == "−" || lastChar == "×" || lastChar == "÷" || lastChar == '(');
}

// this function will display a quick result in the QLineEdit (result space) to notify the user if the input is Invalid
void NotifyUser(QLineEdit *LE, QString Note){

    LE-> setText(Note);
    QString v = " ";
    // Start a timer to show a note after a short delay
    QTimer::singleShot(400, LE, [v, LE]() {
        LE-> setText(v); // Clear the text
    });
}

// This function will activate equal pushbutton if the user clicked enter
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        ui->equal->click();
    } else {
        QMainWindow::keyPressEvent(event);  // Call base class implementation for other keys
    }
}

// ************************************************** Main Window Scope *********************************

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->result->setTextMargins(10, 5, 10, 5); // set margins of the QLineEdit (resutl section)
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ************************************************ Numbers Buttons *****************************************

void MainWindow::on_one_clicked()
{
    ShowAppendStr(ui->operation, "1");
    changeTemporary(ui->one, PushbuttonStyleOld, PushbuttonStyleNew);

}


void MainWindow::on_two_clicked()
{
    ShowAppendStr(ui->operation, "2");
    changeTemporary(ui->two, PushbuttonStyleOld, PushbuttonStyleNew);
}


void MainWindow::on_three_clicked()
{
    ShowAppendStr(ui->operation, "3");
    changeTemporary(ui->three, PushbuttonStyleOld, PushbuttonStyleNew);
}


void MainWindow::on_four_clicked()
{
    ShowAppendStr(ui->operation, "4");
    changeTemporary(ui->four, PushbuttonStyleOld, PushbuttonStyleNew);
}


void MainWindow::on_five_clicked()
{
    ShowAppendStr(ui->operation, "5");
    changeTemporary(ui->five, PushbuttonStyleOld, PushbuttonStyleNew);
}


void MainWindow::on_six_clicked()
{
    ShowAppendStr(ui->operation, "6");
    changeTemporary(ui->six, PushbuttonStyleOld, PushbuttonStyleNew);
}


void MainWindow::on_seven_clicked()
{
    ShowAppendStr(ui->operation, "7");
    changeTemporary(ui->seven, PushbuttonStyleOld, PushbuttonStyleNew);
}


void MainWindow::on_eight_clicked()
{
    ShowAppendStr(ui->operation, "8");
    changeTemporary(ui->eight, PushbuttonStyleOld, PushbuttonStyleNew);
}


void MainWindow::on_nine_clicked()
{
    ShowAppendStr(ui->operation, "9");
    changeTemporary(ui->nine, PushbuttonStyleOld, PushbuttonStyleNew);
}


void MainWindow::on_zero_clicked()
{
    ShowAppendStr(ui->operation, "0");
    changeTemporary(ui->zero, PushbuttonStyleOld, PushbuttonStyleNew);
}

// ************************************************************ Sign and Point ***********************************************


void MainWindow::on_sign_clicked()
{
    changeTemporary(ui->sign, PushbuttonStyleOld, PushbuttonStyleNew);

    // conditions to prevent the user from giving Invalid Negative Sign input
    if(NegativeSignCheck(OptoShow) || OptoCal.isEmpty()){
        OptoShow.append("-");
        ui->operation->setText(OptoShow);
        OptoCal.append("-");
    } else {
        NotifyUser(ui->result, "Can't Add Sign Here");
    }
}


void MainWindow::on_point_clicked()
{
    changeTemporary(ui->point, PushbuttonStyleOld, PushbuttonStyleNew);

    // conditions to prevent the user from giving Invalid decimal point
    if(!OperationCheck(OptoCal)){
        ShowAppendStr(ui->operation, ".");
    }else{
        NotifyUser(ui->result, "Can't Add Point Here");
    }
}

// ************************************************************ Operations ***********************************************

void MainWindow::on_add_clicked()
{
    changeTemporary(ui->add, PushbuttonStyleOld1, PushbuttonStyleNew1);

    // condition to prevent the user from inputing double operation, such as + +
    if(!OperationCheck(OptoCal)){
        ShowAppendStr(ui->operation, "+");
    }else{
        NotifyUser(ui->result, "Can't Add Operation Here");
    }
}


void MainWindow::on_subtract_clicked()
{
    changeTemporary(ui->subtract, PushbuttonStyleOld1, PushbuttonStyleNew1);

    // condition to prevent the user from inputing double operation, such as + +
    if(!OperationCheck(OptoCal)){
        OptoShow.append("−");
        ui->operation->setText(OptoShow);
        OptoCal.append("-");

        ui->result->clear(); // clear text as the user now is giving input
    }else{
        NotifyUser(ui->result, "Can't Add Operation Here");
    }
}


void MainWindow::on_multiply_clicked()
{
    changeTemporary(ui->multiply, PushbuttonStyleOld1, PushbuttonStyleNew1);

    // condition to prevent the user from inputing double operation, such as + +
    if(!OperationCheck(OptoCal)){
        OptoShow.append("×"); // make this symbol for display
        ui->operation->setText(OptoShow);
        OptoCal.append("*");  // make this symbol for calculation process

        ui->result->clear(); // clear text as the user now is giving input
    }else{
        NotifyUser(ui->result, "Can't Add Operation Here");
    }

}


void MainWindow::on_divide_clicked()
{
    changeTemporary(ui->divide, PushbuttonStyleOld1, PushbuttonStyleNew1);

    if(!OperationCheck(OptoCal)){
        OptoShow.append("÷"); // make this symbol for display
        ui->operation->setText(OptoShow);
        OptoCal.append("/"); // make this symbol for the claculation process

        ui->result->clear(); // clear text as the user now is giving input
    }else{
        NotifyUser(ui->result, "Can't Add Operation Here");
    }

}

// ************************************************************ Brackets ***********************************************


void MainWindow::on_Rbracket_clicked()
{
    changeTemporary(ui->Rbracket, PushbuttonStyleOld2, PushbuttonStyleNew2);

    if(OptoCal.isEmpty()){ // allow the user to input left bracket if the operation input is empty
        ShowAppendStr(ui->operation, "(");
    }
    // all the user to input is operation then left bracket, such as +(
    else if(OperationCheck(OptoCal) || OptoCal[OptoCal.length() - 1] == '('){
        ShowAppendStr(ui->operation, "(");
    }
    else{
        NotifyUser(ui->result, "Can't Add Bracket Here");
    }
}


void MainWindow::on_Lbracket_clicked()
{
    changeTemporary(ui->Lbracket, PushbuttonStyleOld2, PushbuttonStyleNew2);

    if(!OptoCal.isEmpty()){ // prevent the user from giving left bracket if the input operation is empty
        ShowAppendStr(ui->operation, ")");
    }

    // allow the user to give right bracket if the last input is a number such as 5)
    else if(!OperationCheck(OptoCal) || OptoCal[OptoCal.length() - 1] == '('){
        ShowAppendStr(ui->operation, ")");
    }
    else{
        NotifyUser(ui->result, "Can't Add Bracket Here");
    }
}

// ************************************************************ Clear and delete Operation ***********************************************

void MainWindow::on_Clear_clicked()
{
    // clear the entir input
    OptoShow.clear();
    ui->operation->setText(OptoShow);
    OptoCal.clear();

    changeTemporary(ui->Clear, PushbuttonStyleOld2, PushbuttonStyleNew2);
    ui->result->clear(); // clear text as the user now is giving input
}

void MainWindow::on_Delete_clicked()
{
    // remove the last letter from the input
    OptoShow.chop(1);
    ui->operation->setText(OptoShow);
    OptoCal.chop(1);

    changeTemporary(ui->Delete, PushbuttonStyleOld2, PushbuttonStyleNew2);
    ui->result->clear(); // clear text as the user now is giving input
}

// ************************************************************ Equal ***********************************************

void MainWindow::on_equal_clicked()
{
    changeTemporary(ui->equal, PushbuttonStyleOld3, PushbuttonStyleNew3);

    Calculate1 c; // access the functions stored in the class Calculate1.h

    // check if the last input is not an operation
    if(!OperationCheck(OptoCal)){

        // check if the input does not has brackets
        if(!c.hasBrackets(OptoCal)){
            double result = c.BasicOpCalc(OptoCal); // calculate directly if it doesn't have
            ui->result->setText(QString::number(result)); // display the result
        }
        // check if the input has brackets
        else if(c.hasBrackets(OptoCal)){
            // check if the input has equal left and right brackets
            if(c.bracketsBalanced(OptoCal)){
                QString strResult = c.BracketsOpCalc(OptoCal); // calculate the operations inside the brackets to remove brackets
                double result = c.BasicOpCalc(strResult); // calculate the processed operation without brackets
                ui->result->setText(QString::number(result)); // display the result
            }
            // notify the user to add correct number of brackets
            else{
                NotifyUser(ui->result, "Invalid Brackets Input");
            }

        }
    } else{
        NotifyUser(ui->result, "Invalid Operation"); // notify the user if the input is incompelete
    }

}


