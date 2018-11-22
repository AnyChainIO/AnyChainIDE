#ifndef GLUACOMPILE_H
#define GLUACOMPILE_H

#include "BaseCompile.h"
//glua编译器
class gluaCompile : public BaseCompile
{
    Q_OBJECT
public:
    explicit gluaCompile(QObject *parent = nullptr);
    ~gluaCompile()override final;
public:
    void startCompileFile(const QString &sourceFilePath)override final;
protected slots:
    void finishCompile(int exitcode,QProcess::ExitStatus exitStatus)override final;
    void onReadStandardOutput()override final;
    void onReadStandardError()override final;
private:
    void generateOutFile();
    void generateContractFile();
    void dealFinishOperate();
private:
    void initConfig(const QString &sourceFilePath);
private:
    class DataPrivate;
    DataPrivate *_p;
};

#endif // GLUACOMPILE_H
