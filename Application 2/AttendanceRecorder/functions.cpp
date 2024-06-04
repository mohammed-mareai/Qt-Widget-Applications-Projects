#include "functions.h"

Functions::Functions(QObject *parent)
    : QObject{parent}
{}


// ******************************************** Open Database *********************************************
QString Functions::dataDir()
{
    QDir dir;
    dir.mkdir(QDir::currentPath() + "\\Data");
    return QDir::currentPath() + "\\Data";
}

void Functions::openDB(QSqlDatabase &sDataDB, const QString &connectionName)
{
    if (!QSqlDatabase::contains(connectionName)) {
        sDataDB = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        QString path = dataDir() + "\\SData";
        sDataDB.setDatabaseName(path);
    } else {
        sDataDB = QSqlDatabase::database(connectionName);
    }

    if (!sDataDB.open()) {
        qDebug() << "Error: Unable to open the database!" << sDataDB.lastError().text();
    }
}

void Functions::createTable(QString name, QList<QString> columns)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    if (sDataDB.tables().contains(name)) {
        sDataDB.close();
        return;
    }

    // Construct the SQL command
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS " + name + " (";
    createTableQuery += columns.join(", ");
    createTableQuery += ")";

    QSqlQuery query(sDataDB);
    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table:" << query.lastError().text();
    }

    sDataDB.close();
}

// ************************************************** Classes Functions *****************************************
QList<QString> Functions::tablesNames()
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    QList<QString> tableNames = sDataDB.tables();

    sDataDB.close();
    return tableNames;
}

void Functions::renameTable(const QString& oldName, const QString& newName)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    QSqlQuery query(sDataDB);
    QString queryString = QString("ALTER TABLE %1 RENAME TO %2").arg(oldName).arg(newName);
    if (!query.exec(queryString)) {
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), query.lastError().text());
    }

    sDataDB.close();
}

void Functions::deleteTable(const QString &tableName)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    QSqlQuery query(sDataDB);
    QString queryString = QString("DROP TABLE %1").arg(tableName);
    if (!query.exec(queryString)) {
        QMessageBox::critical(nullptr, QObject::tr("Database Error"), query.lastError().text());
    }

    sDataDB.close();
}

// *************************************************** Student Functions ********************************
QSet<int> Functions::selectedRows(QTableView *view)
{
    // Get the selection model from the table view
    QItemSelectionModel *selectionModel = view->selectionModel();
    // Get the selected indexes
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

    if (selectedIndexes.isEmpty()) {
        qDebug() << "No selection made.";
    }

    // Collect the row indexes to delete
    QSet<int> selection;
    for (const QModelIndex &index : selectedIndexes) {
        selection.insert(index.row());
    }
    return selection;
}

QSet<int> Functions::selectedColumns(QTableView *view)
{
    // Get the selection model from the table view
    QItemSelectionModel *selectionModel = view->selectionModel();

    // Get the selected indexes
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

    if (selectedIndexes.isEmpty()) {
        qDebug() << "No selection made.";
    }

    // Collect the column indexes
    QSet<int> selectedColumns;
    for (const QModelIndex &index : selectedIndexes) {
        selectedColumns.insert(index.column());
    }
    return selectedColumns;
}

// ************************************* Import and Export Functions ********************************************
void Functions::importTable(const QString &csvFilePath, const QString &tableName)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    if (!sDataDB.isOpen()) {
        qDebug() << "Database connection failed!";
        return;
    }

    QFile file(csvFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    QStringList fields;
    while (!in.atEnd()) {
        QString line = in.readLine();
        fields = line.split(',');

        if (fields.size() >= 1) {  // Ensure there's at least one column to insert
            QString name = fields.at(0).trimmed(); // Assuming the first column is 'Name'

            QSqlQuery query(sDataDB);
            QString queryString = QString("INSERT INTO %1 (Name) VALUES (:name)").arg(tableName);
            query.prepare(queryString);
            query.bindValue(":name", name);

            if (!query.exec()) {
                qDebug() << "Failed to insert row:" << query.lastError().text();
            }
        }
    }

    file.close();
    sDataDB.close();
}

void Functions::exportTable(const QString &tableName, const QString &filePath)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    QSqlQuery query(sDataDB);
    QString selectQuery = QString("SELECT * FROM %1").arg(tableName);
    if (!query.exec(selectQuery)) {
        qDebug() << "Error executing query:" << query.lastError().text();
        sDataDB.close();
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file for writing:" << file.errorString();
        sDataDB.close();
        return;
    }

    QTextStream out(&file);

    // Write the header
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        if (i > 0)
            out << ",";
        out << record.fieldName(i);
    }
    out << "\n";

    // Write the data
    while (query.next()) {
        for (int i = 0; i < record.count(); ++i) {
            if (i > 0)
                out << ",";
            out << query.value(i).toString();
        }
        out << "\n";
    }

    file.close();
    sDataDB.close();
}

// ****************************************************** Record Attendance Functions ***********************************************
int Functions::tableColumnsCount(QString tableName)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    QSqlQuery query(sDataDB);
    if (!query.exec("SELECT * FROM " + tableName + " LIMIT 1")) {
        qDebug() << "Error: Could not execute query" << query.lastError().text();
        sDataDB.close();
        return -1; // Indicate error in executing query
    }

    QSqlRecord record = query.record();
    int columnCount = record.count();

    sDataDB.close();

    return columnCount;


    sDataDB.close();
}

void Functions::appendNewColumn(QString tableName, QString columnName)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    QSqlQuery query(sDataDB);
    QString alterTableQuery = QString("ALTER TABLE %1 ADD COLUMN %2 TEXT").arg(tableName).arg(columnName);

    if (!query.exec(alterTableQuery)) {
        qDebug() << "Error: Could not add new column" << query.lastError().text();
    }

    sDataDB.close();
}

QList<int> Functions::totalStates(QString tableName, QString studentName)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    QList<int> totals = {0, 0, 0}; // Initialize totals for Present, Absent, and Excused

    if (sDataDB.isValid() && sDataDB.isOpen()) {
        QSqlQuery query(sDataDB);
        QString queryString = "SELECT * FROM " + tableName + " WHERE Name = '" + studentName+ "'";
        if (query.exec(queryString)) {
            if (query.next()) {
                // Start counting from the second column
                for (int i = 1; i < query.record().count(); ++i) {
                    QString value = query.value(i).toString();
                    if (value == "Present") {
                        totals[0]++; // Increment total present count
                    } else if (value == "Absent") {
                        totals[1]++; // Increment total absent count
                    } else if (value == "Excused") {
                        totals[2]++; // Increment total excused count
                    }
                }
            } else {
                qDebug() << "No data found for student:" << studentName;
            }
        } else {
            qDebug() << "Query execution failed:" << query.lastError().text();
        }
    } else {
        qDebug() << "Database connection is not valid or open.";
    }

    sDataDB.close();
    return totals;
}

void Functions::appendValue(QString tableName, QString studentName, QString columnName, double value)
{
    QString connectionName = "SData_Connection";
    QSqlDatabase sDataDB;
    openDB(sDataDB, connectionName);

    QSqlQuery query(sDataDB);
    QString queryString = "UPDATE " + tableName + " SET " + columnName + " = :value WHERE Name = :studentName";

    query.prepare(queryString);
    query.bindValue(":value", value);
    query.bindValue(":studentName", studentName);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    sDataDB.close();
}



