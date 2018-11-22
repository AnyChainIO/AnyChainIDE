#ifndef RegisterContractDialogHX_H
#define RegisterContractDialogHX_H

#include <QDialog>
#include "popwidget/MoveableDialog.h"

namespace Ui {
class RegisterContractDialogHX;
}

class RegisterContractDialogHX : public MoveableDialog
{
    Q_OBJECT

public:
    explicit RegisterContractDialogHX(QWidget *parent = nullptr);
    ~RegisterContractDialogHX();

private slots:
    void jsonDataUpdated(const QString &id,const QString &data);

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

    void on_closeBtn_clicked();

    void testRegister();
private:
    void InitWidget();
    void InitAccountAddress();

    double parseTestRegisterFee(const QString &data)const;
private:
    Ui::RegisterContractDialogHX *ui;
};

#endif // RegisterContractDialogHX_H
