#include "fortabulation.h"
#include "ui_fortabulation.h"

ForTabulation::ForTabulation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForTabulation)
{
    ui->setupUi(this);
}

ForTabulation::~ForTabulation()
{
    delete ui;
}

void ForTabulation::on_pushButton_clicked()
{
    emit returnData(ui->lineEdit->text().toDouble(), ui->lineEdit_2->text().toDouble(), ui->lineEdit_3->text().toInt(), ui->lineEdit_4->text().toInt());
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    close();

}

