#ifndef SUBMITATTENDANCE_H
#define SUBMITATTENDANCE_H

#include <QDialog>
#include "functions.h"

namespace Ui {
class submitAttendance;
}

class submitAttendance : public QDialog
{
    Q_OBJECT

public:
    explicit submitAttendance(QWidget *parent = nullptr);
    ~submitAttendance();

    QString tableName;

private slots:

    void on_submit_accepted();

    void on_reload_clicked();

private:
    Ui::submitAttendance *ui;
    Functions fcn;
};

#endif // SUBMITATTENDANCE_H
