#ifndef CSHARPCOMPILE_UNIX_H
#define CSHARPCOMPILE_UNIX_H

#include "BaseCompile.h"
//C#合约编译器_unix

class csharpCompile_unix : public BaseCompile
{
    Q_OBJECT
public:
    explicit csharpCompile_unix(QObject *parent = nullptr);
    ~csharpCompile_unix()override final;
public:
    void startCompileFile(const QString &sourceFilePath)override final;
protected slots:
    void finishCompile(int exitcode,QProcess::ExitStatus exitStatus)override final;
    void onReadStandardOutput()override final;
    void onReadStandardError()override final;
private:
    void generateProject();
    void generateDllFile();
    void generateUVMFile();
    void generateOutFile();
    void generateContractFile();
    void dealFinishOperate();
    void initConfig(const QString &sourceFilePath);
private:
    class DataPrivate;
    DataPrivate *_p;
};

#endif // CSHARPCOMPILE_UNIX_H
