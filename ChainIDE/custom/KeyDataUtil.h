#ifndef KEYDATAUTIL_H
#define KEYDATAUTIL_H

#include <vector>
#include <memory>
#include <map>
#include <mutex>

#include <QDebug>
#include <QJsonDocument>

#include "AES/aesencryptor.h"

//hx账户私钥管理
struct KeyDataInfo
{
    KeyDataInfo()
    {

    }

    QString HXAddr;//link地址
    QString AccountName;//账户名称
    std::map<QString,QString> info_key;//"BTC" + privatekey

    std::map<QString,std::pair<QString,QString>> all_key;//"addr + public + peivate
    std::mutex dataMutex;
public:
    void insertIntoAll(const QString & addr,const QString &pub,const QString &pri)
    {
        std::lock_guard<std::mutex> loc(dataMutex);
        all_key[addr] = std::make_pair(pub,pri);
    }

    void MatchPrivateKey(const QString &symbol,const QString &tunnelAddr)
    {
        auto it = all_key.find(tunnelAddr);
        if(it != all_key.end())
        {
            std::lock_guard<std::mutex> loc(dataMutex);
            info_key[symbol] = (*it).second.second;
        }
    }

    //aes加密
    void EncryptAES(const QString &str)
    {
        unsigned char key2[16] = {0};
        memcpy(key2,str.toUtf8().data(),static_cast<size_t>(str.toUtf8().size()));
        AesEncryptor aes(key2);

        //进行aes加密
        for(auto it = info_key.begin();it != info_key.end();++it)
        {
            std::lock_guard<std::mutex> loc(dataMutex);
            (*it).second = QString::fromStdString( aes.EncryptString( (*it).second.toStdString()) );
        }
    }
    void DecryptAES(const QString &str)
    {
        unsigned char key2[16] = {0};
        memcpy(key2,str.toUtf8().data(),static_cast<size_t>(str.toUtf8().size()));
        AesEncryptor aes(key2);

        //解密
        for(auto it = info_key.begin();it != info_key.end();++it)
        {
            std::lock_guard<std::mutex> loc(dataMutex);
            (*it).second = QString::fromStdString( aes.DecryptString( (*it).second.toStdString()) );
        }

    }

    //base64加密
    void EncryptBase64()
    {
        for(auto it = info_key.begin();it != info_key.end();++it)
        {
            std::lock_guard<std::mutex> loc(dataMutex);
            (*it).second = QString((*it).second.toUtf8().toBase64());
        }
    }
    void DecryptBase64()
    {
        for(auto it = info_key.begin();it != info_key.end();++it)
        {
            std::lock_guard<std::mutex> loc(dataMutex);
            (*it).second = QString(QByteArray::fromBase64((*it).second.toUtf8()));
        }
    }
};
typedef std::shared_ptr<KeyDataInfo> KeyDataPtr;

class KeyDataUtil
{
public:
    KeyDataUtil();
public:
    static bool ReadaPrivateKeyFromPath(const QString &path,KeyDataPtr &data);
    static bool WritePrivateKetToPath(const KeyDataPtr &data,const QString &path);
public:
    static bool ReadPrivateKeyFromJson(const QString &jsonStr,KeyDataPtr &data);

    static bool WritePrivateKeyToJson(const KeyDataPtr &data,QJsonDocument &obj);
};

#endif // KEYDATAUTIL_H
