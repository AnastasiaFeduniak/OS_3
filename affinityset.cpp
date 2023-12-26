#include "affinityset.h"
#include "ui_affinityset.h"

AffinitySet::AffinitySet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AffinitySet)
{
    ui->setupUi(this);
}

AffinitySet::~AffinitySet()
{
    delete ui;
}

//void AffinitySet::on_SetButton_clicked()
//{
 //   if(){} else {}
//}


void AffinitySet::on_SetButton_clicked()
{
    std::string a;
    a += (ui->chb11->isChecked()) ? "1":"0";
    a += (ui->chb10->isChecked()) ? "1":"0";
    a += (ui->chb9->isChecked()) ? "1":"0";
    a += (ui->chb8->isChecked()) ? "1":"0";
    a += (ui->chb7->isChecked()) ? "1":"0";
    a += (ui->chb6->isChecked()) ? "1":"0";
    a += (ui->chb5->isChecked()) ? "1":"0";
    a += (ui->chb4->isChecked()) ? "1":"0";
    a += (ui->chb3->isChecked()) ? "1":"0";
    a += (ui->chb2->isChecked()) ? "1":"0";
    a += (ui->chb1->isChecked()) ? "1":"0";
    a += (ui->chb0->isChecked()) ? "1":"0";
    emit getAff(a);
    ui->chb0->setCheckState(Qt::Unchecked);
    ui->chb1->setCheckState(Qt::Unchecked);
    ui->chb2->setCheckState(Qt::Unchecked);
    ui->chb3->setCheckState(Qt::Unchecked);
    ui->chb4->setCheckState(Qt::Unchecked);
    ui->chb5->setCheckState(Qt::Unchecked);
    ui->chb6->setCheckState(Qt::Unchecked);
    ui->chb7->setCheckState(Qt::Unchecked);
    ui->chb8->setCheckState(Qt::Unchecked);
    ui->chb9->setCheckState(Qt::Unchecked);
    ui->chb10->setCheckState(Qt::Unchecked);
    ui->chb11->setCheckState(Qt::Unchecked);
    close();
}
