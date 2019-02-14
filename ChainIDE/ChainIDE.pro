#-------------------------------------------------
#
# Project created by QtCreator 2018-06-19T11:31:05
#
#-------------------------------------------------

QT       += core gui webengine webenginewidgets webchannel websockets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AnyChain
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    ChainIDE.cpp \
    DataDefine.cpp \
    IDEUtil.cpp \
    compile/BaseCompile.cpp \
    compile/CompileManager.cpp \
    popwidget/commondialog.cpp \
    selectpathwidget.cpp \
    waitingforsync.cpp \
    filewidget/FileWidget.cpp \
    filewidget/FileView.cpp \
    outputwidget.cpp \
    contentwidget/aceeditor.cpp \
    control/contextmenu.cpp \
    control/editorcontextmenu.cpp \
    contentwidget/bridge.cpp \
    popwidget/NewFileDialog.cpp \
    filewidget/InterfaceWidget.cpp \
    popwidget/consoledialog.cpp \
    datarequire/RequireBase.cpp\
    datarequire/DataRequireManager.cpp \
    datarequire/websocketRequire.cpp \
    datarequire/tcpsocketRequire.cpp \
    datarequire/httpRequire.cpp \
    contentwidget/ContextWidget.cpp \
    backstage/BackStageBase.cpp \
    backstage/LinkBackStage.cpp \
    backstage/UbtcBackStage.cpp \
    backstage/CTCBackStage.cpp\
    datamanager/DataManagerHX.cpp \
    datamanager/DataManagerUB.cpp \
    custom/AccountWidgetHX.cpp \
    custom/NameDialogHX.cpp \
    popwidget/MoveableDialog.cpp \
    custom/RegisterContractDialogHX.cpp \
    custom/TransferWidgetHX.cpp \
    custom/CallContractWidgetHX.cpp \
    ConvenientOp.cpp \
    contractwidget/ContractWidget.cpp \
    contractwidget/FunctionWidget.cpp \
    popwidget/ConfigWidget.cpp \
    contentwidget/Editor.cpp \
    contentwidget/codeeditor.cpp \
    custom/PasswordVerifyWidgetHX.cpp \
    custom/ImportDialogHX.cpp \
    custom/AccountWidgetUB.cpp \
    custom/CallContractWidgetUB.cpp \
    custom/ImportDialogUB.cpp \
    custom/NameDialogUB.cpp \
    custom/RegisterContractDialogUB.cpp \
    custom/TransferWidgetUB.cpp \
    update/UpdateNetWork.cpp \
    update/UpdateProcess.cpp \
    update/UpdateProgressUtil.cpp \
    popwidget/AboutWidget.cpp \
    AES/aes.cpp \
    AES/aesencryptor.cpp \
    custom/KeyDataUtil.cpp \
    custom/ImportEnterPWDDialog.cpp \
    debugwidget/DebugWidget.cpp \
    LocalPeer.cpp \
    StatusBar.cpp \
    filewidget/FileProxyModel.cpp \
    backstage/BackStageManager.cpp \
    datamanager/DataManagerStruct.cpp \
    debugwidget/DebugManager.cpp \
    notifyWidget/MessageNotifyWidget.cpp \
    custom/UpgradeContractDialogHX.cpp \
    debugwidget/DebugDataStruct.cpp \
    datamanager/DataManagerCTC.cpp \
    custom/AccountWidgetCTC.cpp \
    custom/TransferWidgetCTC.cpp \
    custom/RegisterContractDialogCTC.cpp \
    custom/CallContractWidgetCTC.cpp \
    custom/UpgradeContractDialogCTC.cpp \
    custom/NameDialogCTC.cpp \
    custom/ImportDialogCTC.cpp \
    debugwidget/BaseTreeItem.cpp \
    debugwidget/DataTreeItemModel.cpp \
    debugwidget/DebugUtil.cpp \
    debugwidget/DebugFunctionWidget.cpp \
    control/tabcontextmenu.cpp \
    control/CustomShadowEffect.cpp \
    popwidget/AboutWidgetGit.cpp \
    custom/TransferToContractHX.cpp \
    custom/TransferToContractCTC.cpp \
    popwidget/MoveableWidget.cpp \
    popwidget/MessageTypeShowWidget.cpp

HEADERS  += MainWindow.h \
    ChainIDE.h \
    DataDefine.h \
    IDEUtil.h \
    compile/BaseCompile.h \
    compile/CompileManager.h \
    popwidget/commondialog.h \
    selectpathwidget.h \
    waitingforsync.h \
    filewidget/FileWidget.h \
    filewidget/FileView.h \
    outputwidget.h \
    contentwidget/aceeditor.h \
    control/contextmenu.h \
    control/editorcontextmenu.h \
    contentwidget/bridge.h \
    popwidget/NewFileDialog.h \
    filewidget/InterfaceWidget.h \
    popwidget/consoledialog.h \
    datarequire/RequireBase.h\
    datarequire/DataRequireManager.h \
    datarequire/websocketRequire.h \
    datarequire/tcpsocketRequire.h \
    datarequire/httpRequire.h \
    contentwidget/ContextWidget.h \
    backstage/BackStageBase.h \
    backstage/LinkBackStage.h \
    backstage/UbtcBackStage.h \
    backstage/CTCBackStage.h\
    datamanager/DataManagerHX.h \
    datamanager/DataManagerUB.h \
    custom/AccountWidgetHX.h \
    custom/NameDialogHX.h \
    popwidget/MoveableDialog.h \
    custom/RegisterContractDialogHX.h \
    custom/TransferWidgetHX.h \
    custom/CallContractWidgetHX.h \
    ConvenientOp.h \
    contractwidget/ContractWidget.h \
    contractwidget/FunctionWidget.h \
    popwidget/ConfigWidget.h \
    contentwidget/Editor.h \
    contentwidget/codeeditor.h \
    custom/PasswordVerifyWidgetHX.h \
    custom/ImportDialogHX.h \
    custom/AccountWidgetUB.h \
    custom/CallContractWidgetUB.h \
    custom/ImportDialogUB.h \
    custom/NameDialogUB.h \
    custom/RegisterContractDialogUB.h \
    custom/TransferWidgetUB.h \
    update/UpdateNetWork.h \
    update/UpdateProcess.h \
    update/UpdateProgressUtil.h \
    popwidget/AboutWidget.h \
    AES/aes.h \
    AES/aesencryptor.h \
    custom/KeyDataUtil.h \
    custom/ImportEnterPWDDialog.h \
    debugwidget/DebugWidget.h \
    LocalPeer.h \
    StatusBar.h \
    filewidget/FileProxyModel.h \
    backstage/BackStageManager.h \
    datamanager/DataManagerStruct.h \
    debugwidget/DebugManager.h \
    notifyWidget/MessageNotifyWidget.h \
    custom/UpgradeContractDialogHX.h \
    debugwidget/DebugDataStruct.h \
    datamanager/DataManagerCTC.h \
    custom/AccountWidgetCTC.h \
    custom/TransferWidgetCTC.h \
    custom/RegisterContractDialogCTC.h \
    custom/CallContractWidgetCTC.h \
    custom/UpgradeContractDialogCTC.h \
    custom/NameDialogCTC.h \
    custom/ImportDialogCTC.h \
    debugwidget/BaseTreeItem.h \
    debugwidget/DataTreeItemModel.h \
    debugwidget/DebugUtil.h \
    debugwidget/DebugFunctionWidget.h \
    control/tabcontextmenu.h \
    control/CustomShadowEffect.h \
    popwidget/AboutWidgetGit.h \
    custom/TransferToContractHX.h \
    custom/TransferToContractCTC.h \
    popwidget/MoveableWidget.h \
    popwidget/MessageTypeShowWidget.h

FORMS    += MainWindow.ui \
    popwidget/commondialog.ui \
    selectpathwidget.ui \
    waitingforsync.ui \
    filewidget/FileWidget.ui \
    outputwidget.ui \
    contentwidget/aceeditor.ui \
    popwidget/NewFileDialog.ui \
    filewidget/InterfaceWidget.ui \
    popwidget/consoledialog.ui \
    contentwidget/ContextWidget.ui \
    custom/AccountWidgetHX.ui \
    custom/NameDialogHX.ui \
    custom/RegisterContractDialogHX.ui \
    custom/TransferWidgetHX.ui \
    custom/CallContractWidgetHX.ui \
    contractwidget/ContractWidget.ui \
    contractwidget/FunctionWidget.ui \
    popwidget/ConfigWidget.ui \
    contentwidget/Editor.ui \
    contentwidget/codeeditor.ui \
    custom/PasswordVerifyWidgetHX.ui \
    custom/ImportDialogHX.ui \
    custom/AccountWidgetUB.ui \
    custom/CallContractWidgetUB.ui \
    custom/ImportDialogUB.ui \
    custom/NameDialogUB.ui \
    custom/RegisterContractDialogUB.ui \
    custom/TransferWidgetUB.ui \
    popwidget/AboutWidget.ui \
    custom/ImportEnterPWDDialog.ui \
    debugwidget/DebugWidget.ui \
    notifyWidget/MessageNotifyWidget.ui \
    custom/UpgradeContractDialogHX.ui \
    custom/AccountWidgetCTC.ui \
    custom/TransferWidgetCTC.ui \
    custom/RegisterContractDialogCTC.ui \
    custom/CallContractWidgetCTC.ui \
    custom/UpgradeContractDialogCTC.ui \
    custom/NameDialogCTC.ui \
    custom/ImportDialogCTC.ui \
    debugwidget/DebugFunctionWidget.ui \
    popwidget/AboutWidgetGit.ui \
    custom/TransferToContractHX.ui \
    custom/TransferToContractCTC.ui \
    popwidget/MessageTypeShowWidget.ui

RESOURCES += \
    chain.qrc

TRANSLATIONS +=   IDE_Simplified_Chinese.ts  IDE_English.ts

CONFIG(release,debug|release){
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

win32{
RC_FILE += logo.rc

SOURCES +=    compile/gluaCompile.cpp \
    compile/javaCompile.cpp \
    compile/csharpCompile.cpp \
    compile/kotlinCompile.cpp

HEADERS +=    compile/gluaCompile.h \
    compile/javaCompile.h \
    compile/csharpCompile.h \
    compile/kotlinCompile.h

CONFIG(debug, debug|release) {
    INCLUDEPATH += VisualLeakDetector/include
    LIBS += -L$$PWD/VisualLeakDetector/lib/Win64 -lvld
}
CONFIG(release, debug|release) {
    LIBS += -lDbgHelp
}
}

unix{
SOURCES += compile/gluaCompile.cpp \
    compile/javaCompile.cpp \
    compile/kotlinCompile.cpp\
    compile/csharpCompile_unix.cpp

HEADERS += compile/gluaCompile.h \
    compile/javaCompile.h \
    compile/kotlinCompile.h\
    compile/csharpCompile_unix.h
}

macx{
ICON = idelogo.icns
}
