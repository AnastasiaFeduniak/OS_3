#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <Windows.h>
#include <QTimer>
#include <functional>

vector<Process> activeProcesses;
std::thread timerThread;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    np = new NewProcesses(this);
    connect(np, &NewProcesses::returnProcesses, this, &MainWindow::getProcesses);
    DataNewerClock(FuncName);
    af = new AffinitySet(this);
    connect(af, &AffinitySet::getAff, this, &MainWindow::putAff);

    ui->InfoTable->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu = new QMenu(this);
    killAct = new QAction("Kill", this);
    suspendAct = new QAction("Suspend", this);
    startAct = new QAction("Resume", this);
    setAffAct = new QAction("Set Affinity", this);

    changePrio = new QMenu("Change priority");
    idleAct = new QAction("IDLE_PRIORITY_CLASS", this);
    belowAct = new QAction("BELOW_NORMAL_PRIORITY_CLASS", this);
    normalAct = new QAction("NORMAL_PRIORITY_CLASS", this);
    aboveAct = new QAction("ABOVE_NORMAL_PRIORITY_CLASS", this);
    highAct = new QAction("R_PRIORITY_CLASS", this);
    realtAct = new QAction("REAL_TIME_PRIORITY_CLASS", this);

    contextMenu->addAction(killAct);
    contextMenu->addAction(suspendAct);
    contextMenu->addAction(startAct);
    contextMenu->addAction(setAffAct);
    contextMenu->addMenu(changePrio);
    changePrio->addAction(idleAct);
    changePrio->addAction(belowAct);
    changePrio->addAction(normalAct);
    changePrio->addAction(aboveAct);
    changePrio->addAction(highAct);
    changePrio->addAction(realtAct);

     connect(ui->InfoTable, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
     connect(setAffAct, &QAction::triggered, this, &MainWindow::affPr);
     connect(killAct, &QAction::triggered, this, &MainWindow::killPr);
     connect(suspendAct, &QAction::triggered, this, &MainWindow::suspendPr);
     connect(startAct, &QAction::triggered, this, &MainWindow::startPr);
     connect(idleAct, &QAction::triggered, this, &MainWindow::idlePr);
     connect(belowAct, &QAction::triggered, this, &MainWindow::belowPr);
     connect(normalAct, &QAction::triggered, this, &MainWindow::normPr);
     connect(aboveAct, &QAction::triggered, this, &MainWindow::abovePr);
     connect(highAct, &QAction::triggered, this, &MainWindow::highPr);
     connect(realtAct, &QAction::triggered, this, &MainWindow::realtPr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getProcesses(vector<const char*>a){
    for(int i = 0; i < a.size(); i++){
        activeProcesses.push_back(*(new Process(a[i])));
    }
}

void MainWindow::on_CreateNewProcess_clicked()
{
     np->show();
}

void MainWindow::DataNewer(){
         ui->InfoTable->setRowCount(0);
         for(int i = 0; i < activeProcesses.size(); i++){
        if(activeProcesses[i].DeathCheck() == 1){
            activeProcesses.erase(activeProcesses.begin() + i);
            i--;
        } else {
            addTableInfo(i);}

         }

}
void MainWindow::addTableInfo(int i){
     ui->InfoTable->insertRow(ui->InfoTable->rowCount());
     activeProcesses[i].renewData();
     ui->InfoTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(to_string(static_cast<int>(activeProcesses[i].getProcessId())))));
     ui->InfoTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(activeProcesses[i].getProcessName())));
     ui->InfoTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(activeProcesses[i].getProcState())));
     ui->InfoTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(activeProcesses[i].getSProcessPriority())));
    // int temp = activeProcesses[i].getTime();
    // string TimeFormat = to_string(temp/3600) + ":" + to_string((temp%3600)/60) + ":" + to_string(temp%60);
     ui->InfoTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(activeProcesses[i].beautifulShow())));

     ui->InfoTable->setItem(i, 5, new QTableWidgetItem(QString::number(static_cast<qulonglong>(activeProcesses[i].getPageFaults()))));

     ui->InfoTable->setItem(i, 6, new QTableWidgetItem(QString::number(static_cast<qulonglong>(activeProcesses[i].getCommitSize()))));

     ui->InfoTable->setItem(i, 7, new QTableWidgetItem(QString::number(static_cast<qulonglong>(activeProcesses[i].getPeakWS()))));

     ui->InfoTable->setItem(i, 8, new QTableWidgetItem(QString::number(static_cast<qulonglong>(activeProcesses[i].getWS()))));

     ui->InfoTable->setItem(i, 9, new QTableWidgetItem(QString::number(static_cast<qulonglong>(activeProcesses[i].getPagedPool()))));

     ui->InfoTable->setItem(i, 10, new QTableWidgetItem(QString::number(static_cast<qulonglong>(activeProcesses[i].getNPagedPool()))));
    //ui->InfoTable->setItem(i, 11, new QTableWidgetItem(QString::number(static_cast<qulonglong>(activeProcesses[i].getProcessThreadCount()))));
}

void MainWindow::DataNewerClock(function<void(void)> a){
     int duration = 1000;
     std::thread([a, duration](){
         while(true){
         a();
         auto ms = std::chrono::steady_clock::now() + std::chrono::milliseconds(duration);
         std::this_thread::sleep_until(ms);
         }
     }).detach();
}

     void MainWindow::showContextMenu(const QPoint &pos)
     {
     rightClickedRow = ui->InfoTable->indexAt(pos).row();
     ui->InfoTable->selectRow(rightClickedRow);// Store the right-clicked row
     contextMenu->exec(ui->InfoTable->mapToGlobal(pos));
     }

     void MainWindow::killPr()
     {
       int tempo = findProcessByID(rightClickedRow);
       activeProcesses[tempo].terminateProcess();
        //activeProcesses.erase(activeProcesses.begin() + tempo);

     }

     void MainWindow::suspendPr()
     {
       if(activeProcesses[findProcessByID(rightClickedRow)].SuspendProcess()){
        activeProcesses[findProcessByID(rightClickedRow)].setRunning();
       }
     }

     void MainWindow::startPr()
     {
       if(activeProcesses[findProcessByID(rightClickedRow)].ResumeProcess()){
         activeProcesses[findProcessByID(rightClickedRow)].setRunning();
       }
     }

     int MainWindow::findProcessByID(int r){
     QString te = ui->InfoTable->item(r, 0)->text();
     for(int i = 0; i < activeProcesses.size(); i++){
        if(static_cast<DWORD>(stoi(te.toStdString())) == activeProcesses[i].getProcessId()){
                         return i;
                     }
     }
     return -1;
     }

     void MainWindow::idlePr(){
     activeProcesses[findProcessByID(rightClickedRow)].setPriority(IDLE_PRIORITY_CLASS);
     }
     void MainWindow::belowPr(){
     activeProcesses[findProcessByID(rightClickedRow)].setPriority(BELOW_NORMAL_PRIORITY_CLASS);
     }
     void MainWindow::normPr(){
     activeProcesses[findProcessByID(rightClickedRow)].setPriority(NORMAL_PRIORITY_CLASS);
     }
     void MainWindow::abovePr(){
          activeProcesses[findProcessByID(rightClickedRow)].setPriority(ABOVE_NORMAL_PRIORITY_CLASS);
     }
     void MainWindow::highPr(){
               activeProcesses[findProcessByID(rightClickedRow)].setPriority(HIGH_PRIORITY_CLASS);
     }
     void MainWindow::realtPr(){
                    activeProcesses[findProcessByID(rightClickedRow)].setPriority(REALTIME_PRIORITY_CLASS);
     }
     void MainWindow::affPr(){
                    af->show();
     }
     void MainWindow::putAff(std::string affin){
       int affinityValue = std::bitset<32>(affin).to_ulong();
        DWORD_PTR affinityMask = static_cast<DWORD_PTR>(affinityValue);
       if(activeProcesses[findProcessByID(rightClickedRow)].setAffinity(affinityMask)){

        }
     }
