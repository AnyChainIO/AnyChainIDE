#include "consoledialog.h"
#include "ui_consoledialog.h"

#include <QKeyEvent>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPainter>
#include <QCompleter>
#include <QAbstractItemView>

#include "ChainIDE.h"
#include "IDEUtil.h"

ConsoleDialog::ConsoleDialog(QWidget *parent) :
    MoveableDialog(parent),
    ui(new Ui::ConsoleDialog),
    cmdIndex(0)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    ui->consoleLineEdit->installEventFilter(this);

    ui->consoleLineEdit->setFocus();

    connect( ChainIDE::getInstance(),&ChainIDE::jsonDataUpdated, this, &ConsoleDialog::jsonDataUpdated);

    //查询帮助，自动提示功能解析
    ChainIDE::getInstance()->postRPC("completer_help",IDEUtil::toJsonFormat("help",QJsonArray()));

}

ConsoleDialog::~ConsoleDialog()
{    
    delete ui;    
}

bool ConsoleDialog::eventFilter(QObject *watched, QEvent *e)
{
    if(watched == ui->consoleLineEdit && e->type() == QEvent::KeyPress)
    {
        QKeyEvent* event = dynamic_cast<QKeyEvent*>(e);
        if( event->key() == Qt::Key_Up)
        {
            ModifyIndex(Up);
        }
        else if( event->key() == Qt::Key_Down)
        {
            ModifyIndex(Down);
        }
    }

    return QWidget::eventFilter(watched,e);
}

void ConsoleDialog::on_closeBtn_clicked()
{
    close();
}

void ConsoleDialog::on_consoleLineEdit_returnPressed()
{
    QString consoleText = ui->consoleLineEdit->text().simplified();
    if(consoleText.isEmpty()) return;

    if(((ChainIDE::getInstance()->getChainClass() == DataDefine::UB && (consoleText.startsWith("stop")) ))||
        (ChainIDE::getInstance()->getChainClass() == DataDefine::HX && (consoleText.startsWith("witness_node_stop")||consoleText.startsWith("lock")))||
        (ChainIDE::getInstance()->getChainClass() == DataDefine::CTC && (consoleText.startsWith("stop")))
       )
    {
        ui->consoleBrowser->append(ui->consoleLineEdit->text());
        ui->consoleBrowser->append(QStringLiteral("operate is forbidden!\n"));
        ui->consoleLineEdit->clear();
        return;
    }

    cmds.removeAll(consoleText);
    cmds.append(consoleText);
    cmdIndex = cmds.size();

    QStringList paramaters = consoleText.split(' ');
    QString command = paramaters.at(0);
    paramaters.removeFirst();    

    QJsonArray array;
    if(!paramaters.empty())
    {
        if(1 == paramaters.size() && paramaters.first().startsWith("[") && paramaters.first().endsWith("]"))
        {
            array = QJsonDocument::fromJson(paramaters.first().toUtf8()).array();
        }
        else
        {
            QString data = "[";
            foreach (QString param, paramaters)
            {
                data.append(param).append(",");
            }
            data.remove(data.length()-1,1);
            data.append("]");
            array = QJsonDocument::fromJson(data.toUtf8()).array();
        }
    }
    ChainIDE::getInstance()->postRPC( "console-" + ui->consoleLineEdit->text().simplified(), IDEUtil::toJsonFormat( command, array )/*str*/);

    ui->consoleLineEdit->clear();
}

void ConsoleDialog::jsonDataUpdated(const QString &id,const QString &data)
{
    if( id.startsWith("console-"))
    {
        ui->consoleBrowser->append(">>>" + id.mid(8));
        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
        if(json_error.error != QJsonParseError::NoError || !parse_doucment.isObject())
        {
            ui->consoleBrowser->append(json_error.errorString());
            ui->consoleBrowser->append("\n");
            return;
        }
        ui->consoleBrowser->append(parse_doucment.toJson());
        ui->consoleBrowser->append("\n");
    }
    else if("completer_help" == id)
    {
        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(),&json_error);
        if(json_error.error != QJsonParseError::NoError || !parse_doucment.isObject())return;

        QStringList wordList;
        parseHelpCommand(parse_doucment.object().value("result").toString(),wordList);
        QCompleter *completer = new QCompleter(wordList, this);
        completer->popup()->setObjectName("completer");
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->consoleLineEdit->setCompleter(completer);
    }
}

void ConsoleDialog::on_clearBtn_clicked()
{
    ui->consoleBrowser->clear();
}

void ConsoleDialog::ModifyIndex(ConsoleDialog::IndexType indexType)
{
    switch (indexType) {
    case Up:
        --cmdIndex;
        break;
    case Down:
        ++cmdIndex;
        break;
    default:
        break;
    }
    cmdIndex = std::max<int>(std::min<int>(cmds.size(),cmdIndex),0);

    if(cmds.isEmpty() || cmdIndex == cmds.size())
    {
        ui->consoleLineEdit->clear();
    }
    else
    {
        ui->consoleLineEdit->setText(cmds.at(cmdIndex));
    }
}

void ConsoleDialog::parseHelpCommand(const QString &helpStr, QStringList &helpList)
{
    helpList.clear();
    QStringList enter = helpStr.split("\n");
    foreach (QString str, enter) {
        if(-1 == str.indexOf(QRegExp("^[a-zA-Z]"))) continue;
        if(DataDefine::HX != ChainIDE::getInstance()->getChainClass())
        {
            QStringList temp = str.split(QRegExp("\\s+"));
            if(!temp.isEmpty())
            {
                helpList.append(temp.front());
            }
        }
        else if(DataDefine::HX == ChainIDE::getInstance()->getChainClass())
        {
            QStringList temp = str.split("(");
            if(!temp.isEmpty())
            {
                QStringList space = temp.front().split(QRegExp("\\s+"));
                if(space.size() >= 2)
                {
                    helpList.append(space.last());
                }
            }
        }
    }
}
