#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QObject>

#include <QtSql>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

#include <QTableView>

class Functions : public QObject
{
    Q_OBJECT
public:
    explicit Functions(QObject *parent = nullptr);

    QString dataDir ();
    void openDB(QSqlDatabase &sDataDB, const QString &connectionName);
    void createTable(QString name, QList <QString> columns);

    QList<QString> tablesNames();
    void renameTable(const QString& oldName, const QString& newName);
    void deleteTable(const QString& tableName);
    QSet<int> selectedRows (QTableView *view);
    QSet<int> selectedColumns(QTableView *view);

    void importTable(const QString &csvFilePath, const QString &tableName);
    void exportTable(const QString &tableName, const QString &filePath);

    int tableColumnsCount(QString tableName);
    void appendNewColumn (QString tableName, QString columnName);
    QList <int> totalStates(QString tableName, QString studentName);
    void appendValue(QString tableName, QString studentName, QString columnName, double value);

signals:
};

#endif // FUNCTIONS_H
