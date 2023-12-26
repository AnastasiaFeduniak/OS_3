#include "newprocesses.h"
#include "ui_newprocesses.h"
char* paths[] = {"C:\\Users\\Lenovo\\AppData\\Roaming\\Telegram Desktop\\Telegram.exe", "C:\\Windows\\System32\\Taskmgr.exe", "D:\\LABS_ALGORITHMS\\OC_FILES\\Palindromes\\x64\\Debug\\Palindromes.exe"};
string Tabul = "D:\\\\LABS_ALGORITHMS\\\\OC_FILES\\\\TabulationOC\\\\x64\\\\Debug\\\\TabulationOC.exe";
vector<const char*> tmp;
NewProcesses::NewProcesses(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProcesses)
{
    ft = new ForTabulation(this);
    connect(ft, &ForTabulation::returnData, this, &NewProcesses::getData);
    ft->setModal(false);
    this->setWindowTitle("New Processes");
    ui->setupUi(this);
}

NewProcesses::~NewProcesses()
{
    delete ui;
}

void NewProcesses::on_CreateButton_clicked()
{
    if(ui->Telegram->isChecked()){
        tmp.push_back(paths[0]);
    }
    if(ui->Palindrome->isChecked()){
        //Process a(paths[3]);
        tmp.push_back(paths[2]);
    }
    if(ui->Taskmgr->isChecked()){
        tmp.push_back(paths[1]);
    }
    emit returnProcesses(tmp);
    tmp.clear();
    ClearForm();
    close();
}
void NewProcesses::getData(double a, double b, int st, int N){
    string a1 = Tabul;
    a1 += " " + std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(st) + " " + std::to_string(N);
    tmp.push_back(strdup(a1.c_str()));

}

void NewProcesses::on_CancelButton_clicked()
{
    ClearForm();
    close();
}

void NewProcesses::ClearForm(){
    ui->Telegram->setCheckState(Qt::Unchecked);
    ui->Tabulation->setCheckState(Qt::Unchecked);
    ui->Taskmgr->setCheckState(Qt::Unchecked);
    ui->Palindrome->setCheckState(Qt::Unchecked);
}

void NewProcesses::on_Tabulation_stateChanged(int arg1)
{
    if(arg1 !=0){
        ft->show();}
}

