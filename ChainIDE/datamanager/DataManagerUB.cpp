#include "datamanager/DataManagerUB.h"

#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QTextCodec>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include "ChainIDE.h"
#include "IDEUtil.h"
#include "ConvenientOp.h"
using namespace DataManagerStruct;

class DataManagerUB::DataPrivate
{
public:
    DataPrivate()
        :accountData(std::make_shared<AccountUB::AccountData>())
        ,contractData(std::make_shared<AddressContractData>())
    {

    }
public:
    DataManagerStruct::AccountUB::AccountDataPtr accountData;
    DataManagerStruct::AddressContractDataPtr contractData;
};

DataManagerUB *DataManagerUB::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new DataManagerUB();
    }
    return _instance;
}

DataManagerUB::DataManagerUB(QObject *parent)
    : QObject(parent)
    ,_p(new DataPrivate())
{

}
DataManagerUB * DataManagerUB::_instance = nullptr;
DataManagerUB::CGarbo DataManagerUB::Garbo;

DataManagerUB::~DataManagerUB()
{
    qDebug()<<"delete DataManagerUB";
    delete _p;
    _p = nullptr;
}

void DataManagerUB::queryAccount()
{
    _p->accountData->clear();

    ChainIDE::getInstance()->postRPC("query-listaccounts",IDEUtil::toJsonFormat("listaccounts",QJsonArray()));
}

const DataManagerStruct::AccountUB::AccountDataPtr &DataManagerUB::getAccount() const
{
    return _p->accountData;
}

void DataManagerUB::queryContract()
{
    QString contractPath = ChainIDE::getInstance()->getCurrentChainType() == DataDefine::TEST? DataDefine::LOCAL_CONTRACT_TEST_PATH : DataDefine::LOCAL_CONTRACT_FORMAL_PATH;
    ConvenientOp::ReadContractFromFile(QCoreApplication::applicationDirPath()+QDir::separator()+contractPath,_p->contractData);
    //查询合约信息
    std::for_each(_p->contractData->getAllData().begin(),_p->contractData->getAllData().end(),[](const DataManagerStruct::AddressContractPtr& data){
        std::for_each(data->GetContracts().begin(),data->GetContracts().end(),[](const DataManagerStruct::ContractInfoPtr &info){
            ChainIDE::getInstance()->postRPC("query_contractinfo_"+info->GetContractAddr(),IDEUtil::toJsonFormat("getcontractinfo",QJsonArray()<<info->GetContractAddr()));
        });
    });
    ChainIDE::getInstance()->postRPC("query-getcontract_info-finish",IDEUtil::toJsonFormat("finishquery",QJsonArray()));
}

const AddressContractDataPtr &DataManagerUB::getContract() const
{
    return _p->contractData;
}

void DataManagerUB::checkAddress(const QString &addr)
{
    ChainIDE::getInstance()->postRPC("data-checkaddress",IDEUtil::toJsonFormat("validateaddress",QJsonArray()<<addr));
}

void DataManagerUB::InitManager()
{
    connect(ChainIDE::getInstance(),&ChainIDE::jsonDataUpdated,this,&DataManagerUB::jsonDataUpdated);
}

void DataManagerUB::jsonDataUpdated(const QString &id, const QString &data)
{
    if("query-listaccounts" == id)
    {
        if(parseListAccount(data))
        {
            //查询每一个账户对应的地址
            std::for_each(_p->accountData->getAccount().begin(),_p->accountData->getAccount().end(),[](const AccountUB::AccountInfoPtr &account){
                ChainIDE::getInstance()->postRPC(QString("query-getaddressesbyaccount_%1").arg(account->getAccountName()),
                                                 IDEUtil::toJsonFormat("getaddressesbyaccount",
                                               QJsonArray()<<account->getAccountName()));
            });
            ChainIDE::getInstance()->postRPC("query-getaddresses-finish","finishquery");
        }
    }
    else if(id.startsWith("query-getaddressesbyaccount_"))
    {
        QString accountName = id.mid(QString("query-getaddressesbyaccount_").length());
        //查找地址对应的余额
        parseAddresses(accountName,data);
    }
    else if("query-getaddresses-finish" == id)
    {
        //查询每个地址对应的金额
        ChainIDE::getInstance()->postRPC("listunspent",IDEUtil::toJsonFormat("listunspent",QJsonArray()));
    }
    else if("listunspent" == id)
    {
        QString lidata = data;
        if(parseAddressBalances(lidata))
        {
            emit queryAccountFinish();
        }
    }
    //检测地址合法性
    else if("data-checkaddress" == id)
    {
        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
        if(json_error.error != QJsonParseError::NoError)
        {
            emit addressCheckFinish(false);
            return;
        }
        QJsonObject jsonObject = parse_doucment.object().value("result").toObject();
        emit addressCheckFinish(jsonObject.value("isvalid").toBool());
    }
    else if(id.startsWith("query_contractinfo_"))
    {
        QString contractAddr = id.mid(QString("query_contractinfo_").length());
        parseContractInfo(contractAddr,data);
    }
    else if("query-getcontract_info-finish" == id)
    {
        emit queryContractFinish();
    }
}

bool DataManagerUB::parseListAccount(const QString &data)
{
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug()<<json_error.errorString();
        return false;
    }
    QJsonObject jsonObject = parse_doucment.object().value("result").toObject();

    foreach (QString name, jsonObject.keys()) {
        _p->accountData->insertAccount(name,jsonObject.value(name).toDouble());
    }
    return true;
}

bool DataManagerUB::parseAddresses(const QString &accountName,const QString &data)
{
//    qDebug()<<"query getaddressesbyaccount"<<data << "accountname"<<accountName;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
    if(json_error.error != QJsonParseError::NoError)
    {
         qDebug()<<json_error.errorString();
         return false;
    }
    QJsonArray jsonArr = parse_doucment.object().value("result").toArray();

    foreach (QJsonValue addr, jsonArr) {
        _p->accountData->insertAddress(accountName,addr.toString(),0);
    }
    return true;

}

bool DataManagerUB::parseAddressBalances(const QString &data)
{
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
    if(json_error.error != QJsonParseError::NoError )
    {
         qDebug()<<json_error.errorString();
         return false;
    }
    QJsonArray jsonArr = parse_doucment.object().value("result").toArray();
    foreach(QJsonValue addr, jsonArr){
        if(!addr.isObject()) continue;
        QJsonObject obj = addr.toObject();
        _p->accountData->addAddressBalance(obj.value("address").toString(),obj.value("amount").toDouble());
    }
    return true;
}

bool DataManagerUB::parseContractInfo(const QString &contAddr, const QString &data)
{
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
    if(json_error.error != QJsonParseError::NoError || !parse_doucment.isObject() || parse_doucment.object().value("result").isNull())
    {
         qDebug()<<"contract_query_info_error:"<<json_error.errorString();
         ConvenientOp::DeleteContract(contAddr);
         return false;
    }
    DataManagerStruct::ContractInfoPtr contractInfo = _p->contractData->getContractInfo(contAddr);
    contractInfo->SetContractName(parse_doucment.object().value("result").toObject().value("name").toString());
    contractInfo->SetContractName(parse_doucment.object().value("result").toObject().value("description").toString());
    if(!contractInfo) return false;
    DataDefine::ApiEventPtr apis = contractInfo->GetInterface();

    QJsonArray apisArr = parse_doucment.object().value("result").toObject().value("apis").toArray();
    foreach (QJsonValue val, apisArr) {
        if(!val.isObject()) continue;
        apis->addApi(val.toObject().value("name").toString());
    }
    QJsonArray offapisArr = parse_doucment.object().value("result").toObject().value("offline_apis").toArray();
    foreach (QJsonValue val, offapisArr) {
        if(!val.isObject()) continue;
        apis->addOfflineApi(val.toObject().value("name").toString());
    }
}


