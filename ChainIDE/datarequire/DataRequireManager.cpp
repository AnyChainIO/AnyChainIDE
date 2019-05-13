﻿#include "DataRequireManager.h"

#include <mutex>
#include <QTimer>
#include "RequireBase.h"
#include "websocketRequire.h"
#include "tcpsocketRequire.h"
#include "httpRequire.h"

static const QString SPLITFLAG = "****";
static const int MAXLOOPNUMBER = 3000;

class DataRequireManager::DataPrivate
{
public:
    DataPrivate(const QString &ip,const QString & port)
        :isBusy(false)
        ,requireTimer(new QTimer())
        ,requireBase(nullptr)
        ,connectPort(port)
        ,connectIP(ip)
        ,loopNumber(0)
    {

    }
    ~DataPrivate()
    {
        if(requireTimer)
        {
            delete requireTimer;
            requireTimer = nullptr;
        }
        if(requireBase)
        {
            delete requireBase;
            requireBase = nullptr;
        }
    }

public:
    QStringList pendingRpcs;//请求内容
    std::mutex dataMutex;//数据锁
    QTimer *requireTimer;//请求时间控制器，自动循环
    bool isBusy;//当前是否有未返回请求

    RequireBase *requireBase;//请求主体
    QString connectPort;
    QString connectIP;
    int loopNumber;//循环次数，超出一定次数就跳过该请求

    //多余信息--http使用
    QByteArray header;
    QByteArray values;
    QString additionalPath;
};

DataRequireManager::DataRequireManager(const QString &ip,const QString & connectPort,QObject *parent)
    : QObject(parent)
    ,_p(new DataPrivate(ip,connectPort))
{
    InitManager();
}

DataRequireManager::~DataRequireManager()
{
    qDebug()<<QString("delete datarequiremanager with ip:%1 port:%2").arg(_p->connectIP).arg(_p->connectPort);
    delete _p;
    _p = nullptr;
}

void DataRequireManager::requirePosted(const QString &_rpcId, const QString & _rpcCmd)
{
    std::lock_guard<std::mutex> loc(_p->dataMutex);
    _p->pendingRpcs.append(_rpcId + SPLITFLAG + _rpcCmd);
}

void DataRequireManager::receiveResponse(const QString &message)
{
    QString id = _p->pendingRpcs.empty()?"":_p->pendingRpcs.front().split(SPLITFLAG).front();

    {
        std::lock_guard<std::mutex> loc(_p->dataMutex);
        _p->isBusy = false;
        _p->loopNumber = 0;
        if(!_p->pendingRpcs.empty())
        {//将第一个数据移除，发送收到回复的消息
            _p->pendingRpcs.removeFirst();
        }
    }

    emit requireResponse(id,message);
}

void DataRequireManager::requireClear()
{
    std::lock_guard<std::mutex> loc(_p->dataMutex);
    _p->isBusy = false;
    _p->loopNumber = 0;
    _p->pendingRpcs.clear();
}

void DataRequireManager::setHTTPHeaderAdditional(const QByteArray &headerName, const QByteArray &value)
{
    _p->header = headerName;
    _p->values = value;
}

void DataRequireManager::setHTTPPathAdditional(const QString &additionalPath)
{
    _p->additionalPath = additionalPath;
}

bool DataRequireManager::isConnected() const
{
    if(_p->requireBase)
    {
        return _p->requireBase->isConnected();
    }
    return false;
}

void DataRequireManager::startManager(ConnectType connecttype)
{
    if(connecttype == WEBSOCKET)
    {
        _p->requireBase = new websocketRequire(_p->connectIP,_p->connectPort);
    }
    else if(connecttype == TCP)
    {
        _p->requireBase = new tcpsocketRequire(_p->connectIP,_p->connectPort);
    }
    else if(connecttype == HTTP)
    {
        _p->requireBase = new httpRequire(_p->connectIP,_p->connectPort);
    }
    else if(connecttype == HTTPWITHUSER)
    {
        _p->requireBase = new httpRequire(_p->connectIP,_p->connectPort);
        dynamic_cast<httpRequire*>(_p->requireBase)->setRawHeader(_p->header,_p->values);
        dynamic_cast<httpRequire*>(_p->requireBase)->setAdditionalPath(_p->additionalPath);
    }

    connect(_p->requireBase,&RequireBase::receiveData,this,&DataRequireManager::receiveResponse);
    connect(_p->requireBase,&RequireBase::connectFinish,this,&DataRequireManager::connectFinish);

    qDebug()<<"start connect datarequire "<<_p->connectIP<<_p->connectPort;
    _p->requireBase->startConnect();
    _p->requireTimer->start(30);
}

void DataRequireManager::processRequire()
{
    if(_p->isBusy)
    {
        {
            std::lock_guard<std::mutex> loc(_p->dataMutex);
            ++_p->loopNumber;
        }
        if(_p->loopNumber >= MAXLOOPNUMBER)
        {
            //请求超时，清除请求
            if(!_p->pendingRpcs.empty())
            {
                qDebug()<<"require outtime "<<_p->pendingRpcs.front();
                emit requireOvertime(_p->pendingRpcs.front().split(SPLITFLAG).front(),_p->pendingRpcs.front().split(SPLITFLAG).back());
                //清空所有请求
                requireClear();
                return;
            }
        }
    }

    if(_p->isBusy || _p->pendingRpcs.empty()) return;

    {
        std::lock_guard<std::mutex> loc(_p->dataMutex);
        _p->isBusy = true;
        _p->loopNumber = 1;
    }
    _p->requireBase->postData(_p->pendingRpcs.front().split(SPLITFLAG).back());
}

void DataRequireManager::InitManager()
{
    connect(_p->requireTimer,&QTimer::timeout,this,&DataRequireManager::processRequire);
}
