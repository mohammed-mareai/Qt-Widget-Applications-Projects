#ifndef CALCULATE1_H
#define CALCULATE1_H

#include <QObject>

#include <QMap>
#include <QList>
#include <QDebug>
#include <QRegularExpression>

#include <QString>
#include <QPair>
#include <stack>


class Calculate1 : public QObject
{
    Q_OBJECT
public:
    explicit Calculate1(QObject *parent = nullptr);

    void parseExpression(const QString& expression, QList<QString>& operators, QList<double>& values);

    void reduceSigns(QList<QString> &op, QList<double> &vals);

    void MapForMultiplication (QMap<int, QList<double>> &sequences, QList <QString> &op, QList <double> &vals);

    void multiplyResults(const QMap<int, QList<double>>& sequences, const QList<double>& vals, QList<double>& NewVals);

    double sumValues(const QList<double>& values);

    double BasicOpCalc(const QString operation);

    // ***************************** Brackets **********************************
    bool hasBrackets(const QString &str);
    bool bracketsBalanced(const QString &str);
    QString BracketsOpCalc(QString op);

signals:
};

#endif // CALCULATE1_H
