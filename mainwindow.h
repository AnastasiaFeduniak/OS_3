#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "newprocesses.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QMenu>
#include <QClipboard>
#include <bitset>
#include "affinityset.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
 //   setWindowTitle("Task Maneger");
    void DataNewer();
    void DataNewerClock(std::function<void()>);
    ~MainWindow();
    std::function<void(void)> FuncName = [this](){DataNewer();};
    void addTableInfo(int i);
    void updateProcessData();


public slots:
    void getProcesses(vector<const char*>);
    void putAff(std::string affin);
private slots:
    void on_CreateNewProcess_clicked();
     void killPr();
     void suspendPr();
     void startPr();
     void idlePr();
     void belowPr();
     void normPr();
     void abovePr();
     void highPr();
     void affPr();
     void realtPr();
     int findProcessByID(int Row);
private:
    AffinitySet * af;
    Ui::MainWindow *ui;
    NewProcesses *np;
    std::thread timerThread;
    bool stopThread = false;
    QMenu *contextMenu;
    QAction *setAffAct;
    QAction *killAct;
    QAction *suspendAct;
    QAction *startAct;
    QMenu *changePrio;
    QAction *idleAct;
    QAction *normalAct;
    QAction *belowAct;
    QAction *aboveAct;
    QAction *realtAct;
    QAction *highAct;
  int rightClickedRow;
     void showContextMenu(const QPoint &pos);
};
#endif // MAINWINDOW_H
