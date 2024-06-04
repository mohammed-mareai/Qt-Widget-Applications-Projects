#include "submitattendance.h"
#include "ui_submitattendance.h"
#include <QButtonGroup>
#include <QRadioButton>

submitAttendance::submitAttendance(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::submitAttendance)
{
    ui->setupUi(this);

    // Get the current date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Set the current date and time to the QDateTimeEdit widget
    ui->dateTime->setCalendarPopup(true);
    ui->dateTime->setDateTime(currentDateTime);
}

submitAttendance::~submitAttendance()
{
    delete ui;
}


void submitAttendance::on_submit_accepted()
{
    QMap<QString, bool> state; // Define state QMap to store the state of radio buttons
    QMap<QString, int> attendanceStates; // Define a map to store the true column for each student

    // Iterate through the table and check the state of each radio button
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        // Get the student name from the first column
        QString studentName = ui->tableWidget->item(i, 0)->text();

        // Initialize the true column for the current row to -1
        int trueColumn = -1;

        // Iterate over the columns to find the true column
        for (int j = 0; j < 3; ++j) {
            QRadioButton *radioButton = qobject_cast<QRadioButton*>(ui->tableWidget->cellWidget(i, j + 1));
            if (radioButton && radioButton->isChecked()) {
                // If a radio button is checked, set the true column for this row
                trueColumn = j + 1;
                break; // Exit the loop once a checked radio button is found
            }
        }

        // Store the true column for the current row in the map
        if (trueColumn != -1) {
            attendanceStates[studentName] = trueColumn;
        }
    }

    int rowCount = ui->tableWidget->rowCount();

    if (attendanceStates.size() != rowCount) {
        qDebug() << "A Student is not Registered";
    }

    QDateTime dateTime = ui->dateTime->dateTime();
    QString columnName = dateTime.toString("yyyy-MM-dd hh:mm AP");

    columnName.prepend("Date_");
    columnName.replace(" ", "_");
    columnName.replace("-", "_");
    columnName.replace(":", "_");

    fcn.appendNewColumn(tableName, columnName);

    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    // Iterate over the attendanceStates QMap
    for (auto it = attendanceStates.constBegin(); it != attendanceStates.constEnd(); ++it) {
        const QString& name = it.key();
        int value = it.value();

        QString state;
        if (value == 1) {
            state = "Present";
        } else if (value == 2) {
            state = "Absent";
        } else if (value == 3) {
            state = "Excused";
        } else{
            state = "Error";
        }

        // Update the value in the database table for the corresponding name
        QString updateQuery = QString("UPDATE %1 SET %2 = '%3' WHERE name = '%4'")
                                  .arg(tableName)
                                  .arg(columnName)
                                  .arg(state)
                                  .arg(name);

        QSqlQuery query(sDataDB);
        if (!query.exec(updateQuery)) {
            qDebug() << "Error updating attendance for" << name << ":" << query.lastError().text();
        }
    }
    sDataDB.close();
}


void submitAttendance::on_reload_clicked()
{
    // Extract student Names from the database

    // Sample list of students will be fetched from the database
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    QStringList students;

    // Execute the query to fetch student names
    QSqlQuery query(sDataDB);
    if (query.exec("SELECT Name FROM "+tableName+"")) {
        while (query.next()) {
            QString studentName = query.value(0).toString();
            students << studentName;
        }
    } else {
        qDebug() << "Error fetching student names:" << query.lastError().text();
    }

    sDataDB.close();

    // upload the names to the tablewidget

    // Create a QTableWidget for displaying students and their attendance
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Stretch columns to fill the available space
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Make cells non-editable

    // Populate the student table with the list of students and radio buttons for attendance
    ui->tableWidget->setRowCount(students.size());
    for (int i = 0; i < students.size(); ++i) {
        QTableWidgetItem *nameItem = new QTableWidgetItem(students[i]);
        ui->tableWidget->setItem(i, 0, nameItem);

        // Create a local button group for each row
        QButtonGroup *buttonGroup = new QButtonGroup(this);

        // Add radio buttons for attendance
        for (int j = 0; j < 3; ++j) {
            QRadioButton *radioButton = new QRadioButton(this);
            ui->tableWidget->setCellWidget(i, j + 1, radioButton);

            // Check the first radio button in the group
            if (j == 0) {
                radioButton->setChecked(true);
            }

            // Add the radio button to the local button group
            buttonGroup->addButton(radioButton);
        }
        // Set exclusive for each row
        buttonGroup->setExclusive(true);
    }

    // Sort the table by the first column
    ui->tableWidget->sortItems(0, Qt::AscendingOrder);
}
