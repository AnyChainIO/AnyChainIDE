#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include "popwidget/MoveableDialog.h"

namespace Ui {
class ConfigWidget;
}

class ConfigWidget : public MoveableDialog
{
    Q_OBJECT

public:
    explicit ConfigWidget(QWidget *parent = nullptr);
    ~ConfigWidget();
public:
    bool pop();
private slots:
    void ConfirmSlots();
    void ChangeDataPath();
private:
    void InitWidget();
private:
    Ui::ConfigWidget *ui;
    bool isOk;
};

#endif // CONFIGWIDGET_H
