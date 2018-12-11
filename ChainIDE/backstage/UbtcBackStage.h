#ifndef UBTCBACKSTAGE_H
#define UBTCBACKSTAGE_H

#include "BackStageBase.h"

class UbtcBackStage : public BackStageBase
{
    Q_OBJECT
public:
    explicit UbtcBackStage(int type = 1,QObject *parent = nullptr);//链类型1==测试 2==正式
    ~UbtcBackStage()override final;
public:
    void startExe(const QString &appDataPath = "")override final;
    bool exeRunning()override final;
    QProcess *getProcess()const override final;
    void ReadyClose()override final;
public slots:
    void rpcPostedSlot(const QString &,const QString &)override final;
protected slots:
    void rpcReceivedSlot(const QString &id,const QString &message)override final;
private slots:
    void onNodeExeStateChanged();
    void testStartedFinish();
    void testStartReceiveSlot(const QString &id,const QString &message);

    void readNodeStandError();
    void readNodeStandOutput();
private:
    void initSocketManager();
private:
    class DataPrivate;
    DataPrivate *_p;
};

#endif // UBTCBACKSTAGE_H
