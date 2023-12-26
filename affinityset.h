#ifndef AFFINITYSET_H
#define AFFINITYSET_H
#include <QDialog>

namespace Ui {
class AffinitySet;
}

class AffinitySet : public QDialog
{
    Q_OBJECT

public:
    explicit AffinitySet(QWidget *parent = nullptr);
    ~AffinitySet();

private slots:
 //   void on_SetButton_clicked();

    void on_SetButton_clicked();

signals:
    void getAff(std::string wtvr);

private:
    Ui::AffinitySet *ui;
};

#endif // AFFINITYSET_H
