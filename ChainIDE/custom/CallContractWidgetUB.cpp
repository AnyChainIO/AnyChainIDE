#include "CallContractWidgetUB.h"
#include "ui_CallContractWidgetUB.h"

#include <limits>
#include <QHeaderView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QCoreApplication>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

#include "ChainIDE.h"
#include "datamanager/DataManagerUB.h"
#include "IDEUtil.h"
#include "ConvenientOp.h"

CallContractWidgetUB::CallContractWidgetUB(QWidget *parent) :
    MoveableDialog(parent),
    ui(new Ui::CallContractWidgetUB)
{
    ui->setupUi(this);
    InitWidget();
}

CallContractWidgetUB::~CallContractWidgetUB()
{
    delete ui;
}

void CallContractWidgetUB::jsonDataUpdated(const QString &id, const QString &data)
{
    if("call_callcontract" == id)
    {//保存合约
        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
        if(json_error.error != QJsonParseError::NoError || !parse_doucment.isObject())
        {
            ConvenientOp::ShowSyncCommonDialog(data);
            close();
            return;
        }
        QString res = parse_doucment.object().value("result").toString();
        if(!res.isEmpty())
        {
            //发送交易
            ChainIDE::getInstance()->postRPC("callcontract-sendrawtransaction",IDEUtil::toJsonFormat("sendrawtransaction",
                                             QJsonArray()<<res));
        }
        else
        {
            ConvenientOp::ShowSyncCommonDialog(data);
            close();
        }
    }
    else if("callcontract-sendrawtransaction" == id)
    {
        if(ChainIDE::getInstance()->getCurrentChainType() == DataDefine::TEST)
        {
            //产一个块来确认
            ChainIDE::getInstance()->postRPC("generate",IDEUtil::toJsonFormat("generate",QJsonArray()<<1));
        }
        ConvenientOp::ShowSyncCommonDialog(data);
        close();
    }
    else if(id.startsWith("call-contractinfo_"))
    {
        QString ownerAddr = id.mid(QString("call-contractinfo_").length());
        parseContractInfo(ownerAddr,data);
    }
}

void CallContractWidgetUB::CallContract()
{
    if(ui->callAddress->currentText().isEmpty() || ui->contractAddress->currentText().isEmpty() || ui->function->currentText().isEmpty())
    {
        return;
    }
    if(ui->gaslimit->isEnabled() && ui->gasprice->isEnabled() && ui->fee->isEnabled())
    {
        ChainIDE::getInstance()->postRPC("call_callcontract",IDEUtil::toJsonFormat("callcontract",QJsonArray()<<
                                         ui->callAddress->currentText()<<ui->contractAddress->currentText()<<ui->function->currentText()
                                         <<ui->param->text()<<ui->gaslimit->value()<<ui->gasprice->value()<<ui->fee->value()));
    }
    else if(!ui->gaslimit->isEnabled() && !ui->gasprice->isEnabled() && !ui->fee->isEnabled())
    {
        ChainIDE::getInstance()->postRPC("call_callcontract",IDEUtil::toJsonFormat("invokecontractoffline",QJsonArray()<<
                                         ui->callAddress->currentText()<<ui->contractAddress->currentText()<<ui->function->currentText()
                                         <<ui->param->text()));
    }

}

void CallContractWidgetUB::contractAddressChanged()
{
    //查询合约对应的api
    ChainIDE::getInstance()->postRPC("call-contractinfo_"+ui->contractAddress->currentText(),
                                     IDEUtil::toJsonFormat("getcontractinfo",QJsonArray()<<ui->contractAddress->currentText()));
}

void CallContractWidgetUB::functionChanged()
{
    ui->fee->setEnabled(ui->function->currentData().toString() == "api");
    ui->gaslimit->setEnabled(ui->function->currentData().toString() == "api");
    ui->gasprice->setEnabled(ui->function->currentData().toString() == "api");
}

void CallContractWidgetUB::InitWidget()
{
    ui->gaslimit->setRange(0,std::numeric_limits<int>::max());
    ui->gaslimit->setSingleStep(1);
    ui->gasprice->setRange(10,std::numeric_limits<int>::max());
    ui->fee->setRange(0,std::numeric_limits<double>::max());
    ui->fee->setDecimals(8);
    ui->fee->setSingleStep(0.001);

    setWindowFlags(Qt::FramelessWindowHint);

    //ui->callAddress->setEditable(true);
    //ui->contractAddress->setEditable(true);

    //读取所有账户信息
    connect(DataManagerUB::getInstance(),&DataManagerUB::queryAccountFinish,this,&CallContractWidgetUB::InitAccountAddress);
    DataManagerUB::getInstance()->queryAccount();


    //初始化合约地址
    InitContractAddress();

    connect(ui->closeBtn,&QToolButton::clicked,this,&CallContractWidgetUB::close);
    connect(ui->cancelBtn,&QToolButton::clicked,this,&CallContractWidgetUB::close);
    connect(ui->okBtn,&QToolButton::clicked,this,&CallContractWidgetUB::CallContract);

    connect(ChainIDE::getInstance(),&ChainIDE::jsonDataUpdated,this,&CallContractWidgetUB::jsonDataUpdated);

    connect(ui->contractAddress,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::activated),this,
            &CallContractWidgetUB::contractAddressChanged);
    connect(ui->function,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::activated),this,
            &CallContractWidgetUB::functionChanged);
}

void CallContractWidgetUB::InitAccountAddress()
{
    QTreeWidget *tree = new QTreeWidget(this);
    tree->header()->setVisible(false);

    DataManagerStruct::AccountUB::AccountDataPtr data = DataManagerUB::getInstance()->getAccount();
    int number = 0;
    QString currentText;
    for(auto it = data->getAccount().begin();it != data->getAccount().end();++it)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList()<<(*it)->getAccountName());
        item->setFlags(Qt::ItemIsEnabled);
        tree->addTopLevelItem(item);

        for(auto add = (*it)->getAddressInfos().begin();add != (*it)->getAddressInfos().end();++add)
        {
            QTreeWidgetItem *childitem = new QTreeWidgetItem(QStringList()<<(*add)->GetAddress());
            item->addChild(childitem);
            if(0 == number)
            {
                currentText = (*add)->GetAddress();
            }
            ++number;
        }
    }
    tree->expandAll();
    ui->callAddress->setModel(tree->model());
    ui->callAddress->setView(tree);
}

void CallContractWidgetUB::InitContractAddress()
{

    DataManagerStruct::AddressContractDataPtr data = std::make_shared<DataManagerStruct::AddressContractData>();
    QString contractPath = ChainIDE::getInstance()->getCurrentChainType() == DataDefine::TEST? DataDefine::LOCAL_CONTRACT_TEST_PATH : DataDefine::LOCAL_CONTRACT_FORMAL_PATH;
    ConvenientOp::ReadContractFromFile(QCoreApplication::applicationDirPath()+QDir::separator()+contractPath,data);
    QTreeWidget *tree = new QTreeWidget(this);
    tree->header()->setVisible(false);
    for(auto it = data->getAllData().begin();it != data->getAllData().end();++it)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList()<<(*it)->GetOwnerAddr());
        item->setFlags(Qt::ItemIsEnabled);
        tree->addTopLevelItem(item);
        for(auto cont = (*it)->GetContracts().begin();cont != (*it)->GetContracts().end();++cont)
        {
            QTreeWidgetItem *childitem = new QTreeWidgetItem(QStringList()<<(*cont)->GetContractAddr());
            item->addChild(childitem);
        }

    }
    tree->expandAll();
    ui->contractAddress->setModel(tree->model());
    ui->contractAddress->setView(tree);

}

bool CallContractWidgetUB::parseContractInfo(const QString &addr, const QString &data)
{
    ui->function->clear();
    QTreeWidget *tree = new QTreeWidget(this);
    tree->header()->setVisible(false);
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList()<<"api");
    item->setFlags(Qt::ItemIsEnabled);
    tree->addTopLevelItem(item);

    QTreeWidgetItem *item1 = new QTreeWidgetItem(QStringList()<<"offline-api");
    item1->setFlags(Qt::ItemIsEnabled);
    tree->addTopLevelItem(item1);

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
    if(json_error.error != QJsonParseError::NoError || !parse_doucment.isObject())
    {
         qDebug()<<json_error.errorString();
         return false;
    }
    QJsonArray apisArr = parse_doucment.object().value("result").toObject().value("apis").toArray();
    foreach (QJsonValue val, apisArr) {
        if(!val.isObject()) continue;
        QTreeWidgetItem *itemChild = new QTreeWidgetItem(QStringList()<<val.toObject().value("name").toString());
        itemChild->setData(0,Qt::UserRole,"api");
        item->addChild(itemChild);
    }
    QJsonArray offapisArr = parse_doucment.object().value("result").toObject().value("offline_apis").toArray();
    foreach (QJsonValue val, offapisArr) {
        if(!val.isObject()) continue;
        QTreeWidgetItem *itemChild = new QTreeWidgetItem(QStringList()<<val.toObject().value("name").toString());
        itemChild->setData(0,Qt::UserRole,"offline-api");
        item1->addChild(itemChild);
    }


    tree->expandAll();
    ui->function->setModel(tree->model());
    ui->function->setView(tree);

    return true;
}
