#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_reloadClass_clicked(); // relaod the classes (table names) to the combo box
    ui->studentInfo->setEditTriggers(QAbstractItemView::NoEditTriggers); // prevent the table from being editable.
}

MainWindow::~MainWindow()
{
    delete ui;
}


// ************************************** Subject Pushbuttons **************************************
void MainWindow::on_newClass_clicked()
{
    bool ok;
    QString className = QInputDialog::getText(this, tr("Insert Class Name"),
                                                tr("Class Name:"), QLineEdit::Normal,
                                                "", &ok);
    if (ok && !className.isEmpty()) {
        QMessageBox::information(this, tr("Class Name"), tr("The class name is: %1").arg(className));
    } else {
        QMessageBox::warning(this, tr("No Input"), tr("You did not enter any class name."));
    }

    className.replace(" ", "_$$_"); // filter spaces from the table names.

    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    QList<QString> columns = {
        "Name TEXT",
        "Total FLOAT",
        "Percentage FLOAT"
    };
    fcn.createTable(className, columns);

    sDataDB.close();

    on_reloadClass_clicked();
}


void MainWindow::on_reloadClass_clicked()
{
    ui->selectClass->clear();

    QList<QString> tableNames = fcn.tablesNames();

    // Return the spaces back for the user to view them in the combo box
    if (!tableNames.isEmpty()) {
        for(int i = 0; i<tableNames.size(); ++i){
            QString currentTable = tableNames[i];
            currentTable.replace("_$$_", " ");
            tableNames[i] = currentTable;
        }
    }
    ui->selectClass->addItems(tableNames);
    on_reload_clicked();
}


void MainWindow::on_renameClass_clicked()
{
    bool ok;
    QString newClassName = QInputDialog::getText(this, tr("Insert New Class Name"),
                                              tr("New Class Name:"), QLineEdit::Normal,
                                              "", &ok);
    if (ok && !newClassName.isEmpty()) {
        QMessageBox::information(this, tr("New Class Name"), tr("The new class name is: %1").arg(newClassName));
    } else {
        QMessageBox::warning(this, tr("No Input"), tr("You did not enter any class name."));
    }

    QString currentTable = ui->selectClass->currentText();
    currentTable.replace(" ", "_$$_");
    fcn.renameTable(currentTable, newClassName.replace(" ", "_$$_"));
    on_reloadClass_clicked();
}

void MainWindow::on_deleteClass_clicked()
{
    QString currentTable = ui->selectClass->currentText();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion", tr("Are you sure you want to delete %1 table?").arg(currentTable),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        currentTable.replace(" ", "_$$_");
        fcn.deleteTable(currentTable);
        on_reloadClass_clicked();
    }
}


// ****************************************************** Student Functions ****************************************
void MainWindow::on_reload_clicked()
{
    QString currentTable = ui->selectClass->currentText();
    currentTable.replace(" ", "_$$_");

    if(currentTable.isEmpty()){
        return;
    }
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    // Create a QSqlTableModel
    QSqlTableModel *model = new QSqlTableModel(this, sDataDB);
    model->setTable(currentTable); // Set the table name
    model->setSort(0, Qt::AscendingOrder); // Set the sorting column and order

    // Load data into the model
    if (model->select()) {
        // Set the model
        ui->studentInfo->setModel(model);
        ui->studentInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Show the table view
        ui->studentInfo->show();

        // Iterate through each row to find total states for each student
        for (int row = 0; row < model->rowCount(); ++row) {
            QString studentName = model->index(row, 0).data().toString(); // student name is in the first column
            QList<int> states = fcn.totalStates(currentTable, studentName);

            // append totol attendance for the students
            int totalStatesSum = states[0] + states[2];
            double totalStatesDouble = static_cast<double>(totalStatesSum);
            fcn.appendValue(currentTable, studentName, "Total", totalStatesDouble);

            // append attendcan percentage
            int totalAttColumns = fcn.tableColumnsCount(currentTable) - 3;
            double totalAttColumnsDouble = static_cast<double>(totalAttColumns);
            if(!(totalAttColumnsDouble==0)){
                double percentage = (totalStatesDouble/totalAttColumnsDouble)*100;
                fcn.appendValue(currentTable, studentName, "Percentage", percentage);
            } else if(totalAttColumnsDouble==0){
                double percentage = 0;
                fcn.appendValue(currentTable, studentName, "Percentage", percentage);
            }
        }
    } else {
        qDebug() << "Error loading data:" << model->lastError().text();
    }

    sDataDB.close();
}

void MainWindow::on_selectClass_currentIndexChanged()
{
    on_reload_clicked();
}

void MainWindow::on_insertStudent_clicked()
{
    QString currentTable = ui->selectClass->currentText();
    currentTable.replace(" ", "_$$_");

    bool ok;
    QString studentName = QInputDialog::getText(this, tr("Insert Student Name"),
                                                 tr("Student Name:"), QLineEdit::Normal,
                                                 "", &ok);
    if (ok && !studentName.isEmpty()) {
        QMessageBox::information(this, tr("Student Name"), tr("The student name is: %1").arg(studentName));
    } else {
        QMessageBox::warning(this, tr("No Input"), tr("You did not enter any student name."));
    }

    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    QSqlQuery query(sDataDB);
    QString queryString = QString("INSERT INTO %1 (Name) VALUES (:name)").arg(currentTable);
    query.prepare(queryString);
    query.bindValue(":name", studentName);

    if (!query.exec()) {
        qDebug() << "Failed to insert student name:" << query.lastError().text();
    }
    sDataDB.close();

    on_reload_clicked();
}

void MainWindow::on_removeStudent_clicked()
{
    QSet<int> rowsToDelete = fcn.selectedRows(ui->studentInfo);

    // Confirm deletion with the user
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Confirmation",
                                  "Are you sure you want to delete the selected students?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return; // User chose not to delete
    }

    // Convert the QSet to a QList and sort in descending order
    QList<int> sortedRows = rowsToDelete.values();
    std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

    // Check if the model is valid and is a QSqlTableModel
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(ui->studentInfo->model());
    if (!model) {
        qDebug() << "Model is not a QSqlTableModel.";
        return;
    }

    // Open the database connection
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    // Remove rows from the model starting from the last row to the first row
    bool allRowsRemoved = true;
    for (int row : sortedRows) {
        if (!model->removeRow(row)) {
            qDebug() << "Error removing row:" << model->lastError().text() << "at row" << row;
            allRowsRemoved = false;
        }
    }

    // Submit the changes to the database if all rows were removed successfully
    if (allRowsRemoved) {
        if (!model->submitAll()) {
            qDebug() << "Error submitting changes:" << model->lastError().text();
        }
    } else {
        qDebug() << "Some rows were not removed. Changes are not submitted.";
        model->revertAll();
    }

    // Close the database connection
    sDataDB.close();
    on_reload_clicked();
}


// ******************************************** Import and Export Actions **********************************
void MainWindow::on_actionImport_Data_triggered()
{
    QString filter = "CSV Files (*.csv)";
    QString path = QFileDialog::getOpenFileName(this, "Open file","C:\\", filter);

    if(path.isEmpty()){
        return;
    }

    QString currentTable = ui->selectClass->currentText();
    currentTable.replace(" ", "_$$_");

    fcn.importTable(path, currentTable);
    on_reload_clicked();
}


void MainWindow::on_actionExport_Data_triggered()
{
    QString currentTable = ui->selectClass->currentText();
    currentTable.replace(" ", "_$$_");

    // Open QFileDialog to select folder and file name for saving
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save CSV File"), "", tr("CSV Files (*.csv)"));
    if (filePath.isEmpty()) {
        return;
    }

    fcn.exportTable(currentTable, filePath);
}


// ****************************************** Allow Editing on the Database ********************************************
void MainWindow::on_notAllowEdit_clicked()
{
    on_reload_clicked();
    ui->studentInfo->setEditTriggers(QAbstractItemView::NoEditTriggers); // prevent the table from being editable.

    // close the database connection
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);
    sDataDB.close();

    // Get all children of the main window
    QList<QPushButton *> buttons = this->findChildren<QPushButton *>();

    // Disable each QPushButton
    for (QPushButton *button : buttons) {
        button->setDisabled(false);
    }
}

// Slot to handle dataChanged signal
void MainWindow::handleDataChanged(const QModelIndex &topLeft)
{
    // Check if the modified data falls within the columns after the third one
    if (topLeft.column() > 2) {
        // Get the modified data
        QString newData = topLeft.data().toString();

        // Check if the modified data is not "Present", "Absent", or "Excused"
        if (newData != "Present" && newData != "Absent" && newData != "Excused") {
            // Show a warning
            QMessageBox::warning(this, "Warning", "Invalid input! Please enter either 'Present', 'Absent', or 'Excused'.");
            // Set the value to "Present" automatically
            QSqlTableModel *model = qobject_cast<QSqlTableModel*>(sender());
            if (model) {
                model->setData(topLeft, "Present");// Set value to "Present"
            }
        }
    }
}

void MainWindow::on_allowEdit_clicked()
{
    on_reload_clicked();

    QString currentTable = ui->selectClass->currentText();
    currentTable.replace(" ", "_$$_");

    // Open the database connection
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    QSqlTableModel *model = new QSqlTableModel(this, sDataDB);
    // Set the table name
    model->setTable(currentTable);
    // Set the edit strategy to automatically submit changes
    ui->studentInfo->setEditTriggers(QAbstractItemView::AllEditTriggers);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setSort(0, Qt::AscendingOrder); // Set the sorting column and order
    // Populate the model with data
    model->select();

    // Set the model to the table view
    ui->studentInfo->setModel(model);

    // Get all children of the main window
    QList<QPushButton *> buttons = this->findChildren<QPushButton *>();

    // Disable each QPushButton
    for (QPushButton *button : buttons) {
        button->setDisabled(true);
    }

    // Connect the dataChanged signal to a slot in MainWindow class
    connect(model, &QSqlTableModel::dataChanged, this, &MainWindow::handleDataChanged);
}

// ******************************************* Searching ***************************************

void MainWindow::on_Search_clicked()
{
    QString currentTable = ui->selectClass->currentText();
    currentTable.replace(" ", "_$$_");

    QString searchWord = ui->SearchWord->text();

    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    // Create a QSqlTableModel
    QSqlTableModel *model = new QSqlTableModel(this, sDataDB);
    model->setTable(currentTable); // Set the table name

    QString filter = QString("Name LIKE '%%1%'").arg(searchWord);
    model->setFilter(filter);

    // Fetch the data
    model->select();
    // Set the model to your table view
    ui->studentInfo->setModel(model);

    sDataDB.close();
}

// ******************************************* Add Attendance Column ***************************************

void MainWindow::on_addAttendance_clicked()
{
    QString currentTable = ui->selectClass->currentText();
    currentTable.replace(" ", "_$$_");

    submitAttendance *attendanceForm = new submitAttendance(this);
    attendanceForm->tableName = currentTable;

    attendanceForm->show();
}

void MainWindow::on_removeDay_clicked()
{
    // Get the columns to delete
    QSet<int> columnsToDelete = fcn.selectedColumns(ui->studentInfo);

    foreach (int column, columnsToDelete) {
        // Check if the column index is within the first three columns
        if (column >= 0 && column < 3) {
            // Display a warning message
            QMessageBox::warning(this, "Warning", "Cannot delete this column.");
            return;
        }
    }

    // Confirm deletion with the user
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Confirmation",
                                  "Are you sure you want to delete the selected days?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return; // User chose not to delete
    }

    // Open the database connection
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    fcn.openDB(sDataDB, connectionName);

    // Prepare the ALTER TABLE statement to drop columns
    QSqlQuery query(sDataDB);
    QString tableName = ui->selectClass->currentText();
    tableName.replace(" ", "_$$_");

    QString alterTableQuery;
    QTextStream(&alterTableQuery) << "ALTER TABLE \"" << tableName << "\" ";
    bool first = true;
    for (int column : columnsToDelete) {
        if (!first) {
            alterTableQuery += ",";
        }
        alterTableQuery += " DROP COLUMN \"" + ui->studentInfo->model()->headerData(column, Qt::Horizontal).toString() + "\"";
        first = false;
    }

    // Execute the ALTER TABLE statement
    if (!query.exec(alterTableQuery)) {
        qDebug() << "Error: Could not remove columns" << query.lastError().text();
    }

    // Close the database connection
    sDataDB.close();

    // Reload the table
    on_reload_clicked();
}


