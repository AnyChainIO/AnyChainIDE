﻿#include "csharpCompile.h"

#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

#include "IDEUtil.h"
#include "DataDefine.h"

class csharpCompile::DataPrivate
{
public:
    DataPrivate()
    {

    }
public:
};

csharpCompile::csharpCompile(QObject *parent)
    :BaseCompile(parent)
    ,_p(new DataPrivate())
{

}

csharpCompile::~csharpCompile()
{
    delete _p;
    _p = nullptr;
}

void csharpCompile::initConfig(const QString &sourceFilePath)
{
    setTempDir( QCoreApplication::applicationDirPath()+QDir::separator()+
                DataDefine::CSHARP_COMPILE_TEMP_DIR + QDir::separator() + QFileInfo(sourceFilePath).baseName()
               );

    setSourceDir( IDEUtil::getNextDir(QCoreApplication::applicationDirPath()+QDir::separator()+DataDefine::CSHARP_DIR,
                                        sourceFilePath)
                 );

    //清空临时目录
    IDEUtil::deleteDir(getTempDir());

    readyBuild();

}

void csharpCompile::startCompileFile(const QString &sourceFilePath)
{
    initConfig(sourceFilePath);

    //设置控制台路径为当前路径
    getCompileProcess()->setWorkingDirectory(getTempDir());

    emit CompileOutput(QString("start Compile %1").arg(getSourceDir()));
    generateDllFile();
}

void csharpCompile::finishCompile(int exitcode, QProcess::ExitStatus exitStatus)
{
    if(QProcess::NormalExit != exitStatus)
    {
        //删除之前的文件
        QFile::remove(getDstByteFilePath());
        QFile::remove(getDstMetaFilePath());
        QFile::remove(getDstOutFilePath());

        //删除临时目录
        IDEUtil::deleteDir(getTempDir());

        emit CompileOutput(QString("compile error:stage %1").arg(getCompileStage()));
        emit errorCompileFile(getSourceDir());
        return;
    }

    switch (getCompileStage()) {
    case BaseCompile::StageOne:
        //生成.dll文件
        emit CompileOutput(QString(".dll file generate finish."));
        generateContractFile();
        break;
    case BaseCompile::StageTwo:
        dealFinishOperate();
        break;
    default:
        break;
    }
}

void csharpCompile::onReadStandardOutput()
{
    emit CompileOutput(QString::fromLocal8Bit(getCompileProcess()->readAll()));
}

void csharpCompile::onReadStandardError()
{
    emit CompileOutput(QString::fromLocal8Bit(getCompileProcess()->readAllStandardError()));
}

void csharpCompile::generateDllFile()
{
    setCompileStage(BaseCompile::StageOne);

    QStringList fileList;
    IDEUtil::GetAllFile(getSourceDir(),fileList,QStringList()<<DataDefine::CSHARP_SUFFIX);
    //将这些文件编译成dll文件
    QStringList params;
    params<<"/target:library"<<"-reference:"+QCoreApplication::applicationDirPath()+QDir::separator()+DataDefine::CSHARP_JSON_DLL_PATH
            +","+QCoreApplication::applicationDirPath()+QDir::separator()+DataDefine::CSHARP_CORE_DLL_PATH+""
          <<fileList<<"-out:"+getTempDir()+QDir::separator()+"result.dll"
          <<"-debug"<<"-pdb:"+getTempDir()+QDir::separator()+"result.pdb";

    qDebug()<<"c#-compile-generate-.dll: csc.exe "<<params;
    getCompileProcess()->start(QCoreApplication::applicationDirPath()+"/"+DataDefine::CSHARP_COMPILER_EXE_PATH,params);
}

void csharpCompile::generateContractFile()
{
    setCompileStage(BaseCompile::StageTwo);
    //将result编译成.pgc文件，需要先将工作目录指定为uvm_ass.exe所在目录
    getCompileProcess()->setWorkingDirectory(QCoreApplication::applicationDirPath()+QDir::separator()+DataDefine::CSHARP_COMPILE_DIR);
    QStringList params;
    params<<"--gpc"<<getTempDir()+QDir::separator()+"result.dll";
    qDebug()<<"c#-compiler-generate-.gpc: "<<QCoreApplication::applicationDirPath()+QDir::separator()+DataDefine::CSHARP_COMPILE_PATH<<params;
    getCompileProcess()->start(QCoreApplication::applicationDirPath()+QDir::separator()+DataDefine::CSHARP_COMPILE_PATH,params);

}

void csharpCompile::dealFinishOperate()
{
    //复制gpc meta.json文件到源目录
    QFile::copy(getTempDir()+"/result.gpc",getDstByteFilePath());
    QFile::copy(getTempDir()+"/result.meta.json",getDstMetaFilePath());
    QFile::copy(getTempDir()+"/result.out",getDstOutFilePath());

    //删除临时目录
    IDEUtil::deleteDir(getTempDir());

    if(QFile(getDstByteFilePath()).exists())
    {
        emit CompileOutput(QString("compile finish,see %1").arg(getDstByteFilePath()));
        emit finishCompileFile(getDstByteFilePath());
    }
    else
    {
        emit CompileOutput(QString("compile error,cann't find :%1").arg(getDstByteFilePath()));
        emit errorCompileFile(getSourceDir());
    }
}

