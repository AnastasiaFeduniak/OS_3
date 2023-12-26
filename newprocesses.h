#ifndef NEWPROCESSES_H
#define NEWPROCESSES_H
#include "process.h"
#include "fortabulation.h"
#include <QDialog>
#include <thread>
#include <cstring>
namespace Ui {
class NewProcesses;
}

class NewProcesses : public QDialog
{
    Q_OBJECT

public:
    explicit NewProcesses(QWidget *parent = nullptr);
    ~NewProcesses();

public slots:
    void getData(double a, double b, int st, int N);

private:
    Ui::NewProcesses *ui;
      ForTabulation* ft;

signals:
    void returnProcesses(std::vector<const char*>);

private slots:
    void ClearForm();
    void on_CreateButton_clicked();
    void on_CancelButton_clicked();
    void on_Tabulation_stateChanged(int arg1);
};

#endif // NEWPROCESSES_H
