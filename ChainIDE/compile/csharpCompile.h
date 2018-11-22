#ifndef CSHARPCOMPILE_H
#define CSHARPCOMPILE_H

#include "BaseCompile.h"
//C#合约编译器
class csharpCompile : public BaseCompile
{
    Q_OBJECT
public:
    explicit csharpCompile(QObject *parent = nullptr);
    ~csharpCompile()override final;
public:
    void startCompileFile(const QString &sourceFilePath)override final;
protected slots:
    void finishCompile(int exitcode,QProcess::ExitStatus exitStatus)override final;
    void onReadStandardOutput()override final;
    void onReadStandardError()override final;
private:
    void generateDllFile();
    void generateContractFile();
    void dealFinishOperate();
    void initConfig(const QString &sourceFilePath);
private:
    class DataPrivate;
    DataPrivate *_p;
};

#endif // CSHARPCOMPILE_H
