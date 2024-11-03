#include "mainwindow.h"
#include "./ui/ui_mainwindow.h"
#include <xixi/DynamicStack.h> /* xixi_ 动态栈库 */
#include <QMetaEnum>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QSaveFile>
#include <QIcon>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowIcon(QIcon(":/appico.png"));
    ui->setupUi(this);
    setWindowTitle("记事本");
    connect(ui->XixiFileMenu,&QMenu::triggered,this,&MainWindow::XIXI_ActionTriggter);
    connect(ui->XixiEditMenu,&QMenu::triggered,this,&MainWindow::XIXI_ActionTriggter);
    connect(ui->XixiTextEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::XIXI_UpdateCursorPosition);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::XIXI_ActionTriggter(QAction *act)
{
    QMetaEnum me = QMetaEnum::fromType<XixiActKind::XixiActionKind>();
    bool IsOk;
    int KeyVal = me.keyToValue(act->text().toStdString().c_str(),&IsOk);
    printf("%d\n",KeyVal);
    if (!IsOk){
        return;
    }

    switch (static_cast<XixiActKind::XixiActionKind>(KeyVal)) {
    case XixiActKind::XixiActionKind::NewFile:
        XIXI_NewFile();
        break;
    case XixiActKind::XixiActionKind::OpenFile:
        XIXI_OpenFile();
        break;
    case XixiActKind::XixiActionKind::SaveFile:
        XIXI_SaveFile();
        break;
    case XixiActKind::XixiActionKind::SaveAsFile:
        XIXI_SaveAsFile();
        break;
    case XixiActKind::XixiActionKind::Undo:
        ui->XixiTextEdit->undo();
        break;
    case XixiActKind::XixiActionKind::Cut:
        ui->XixiTextEdit->cut();
        break;
    case XixiActKind::XixiActionKind::Copy:
        ui->XixiTextEdit->copy();
        break;
    case XixiActKind::XixiActionKind::Paste:
        ui->XixiTextEdit->paste();
        break;
    case XixiActKind::XixiActionKind::SelectAll:
        ui->XixiTextEdit->selectAll();
        break;
    case XixiActKind::XixiActionKind::Quit:
        QApplication::quit();
        break;
    default:
        break;
    }
}

void MainWindow::XIXI_NewFile()
{
    if (XIXI_MayBeSave()){
        ui->XixiTextEdit->clear();
        CurFilePath = QString();
        ui->XixiTextEdit->document()->setModified(false);
        setWindowTitle("Default - 记事本");
        statusBar()->showMessage("新建文件成功喵~",2000);
    }
}

void MainWindow::XIXI_OpenFile()
{
    if (XIXI_MayBeSave()){
        QString FileName = QFileDialog::getOpenFileName(this);
        if (!FileName.isEmpty()){
            XIXI_LoadFile(FileName);
            setWindowTitle(FileName + " - 记事本");
        }
    }
}

bool MainWindow::XIXI_SaveFile()
{
    if (CurFilePath.isEmpty()){
        return XIXI_SaveAsFile();
    } else {
        return XIXI_WriteFile(CurFilePath);
    }
}

bool MainWindow::XIXI_SaveAsFile()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted){
        return false;
    }

    const QStringList selectedFiles = dialog.selectedFiles();
    if (!selectedFiles.isEmpty()) {
        return XIXI_WriteFile(selectedFiles.first());
    } else {
        return false;
    }
}

void MainWindow::XIXI_UpdateCursorPosition()
{
    QTextCursor cursor = ui->XixiTextEdit->textCursor();
    /* 更新状态栏文本 */
    statusBar()->showMessage(QString("行: %1 列: %2").arg(cursor.blockNumber() + 1).arg(cursor.columnNumber() + 1));
}

bool MainWindow::XIXI_MayBeSave()
{
    if (!ui->XixiTextEdit->document()->isModified()){
        return true;
    }
    const QMessageBox::StandardButton BtnVal = QMessageBox::warning(this,qApp->applicationDisplayName(),"文件未保存!",QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
    switch (BtnVal) {
    case QMessageBox::Save:
        return XIXI_SaveFile();
    case QMessageBox::Cancel:
        return false;
    default :
        return true;
    }
}

void MainWindow::XIXI_LoadFile(const QString &FilePath)
{
    QFile TextContent(FilePath);
    if (!TextContent.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,qApp->applicationName(),"无法打开文件喵~" );
        return;
    }

    QTextStream TextStream(&TextContent);
    ui->XixiTextEdit->setPlainText(TextStream.readAll());

    CurFilePath = FilePath;
    ui->XixiTextEdit->document()->setModified(false);
    statusBar()->showMessage("文件打开成功喵~",2000);
}

bool MainWindow::XIXI_WriteFile(const QString &FilePath)
{
    QSaveFile File(FilePath);
    if (File.open(QFile::WriteOnly | QFile::Text)){
        QTextStream Write(&File);
        Write<<ui->XixiTextEdit->toPlainText();
        if (!File.commit()){
            QMessageBox::warning(this,qApp->applicationName(),"文件保存失败喵~");
            return false;
        }
    } else {
        return false;
    }

    CurFilePath = FilePath;
    ui->XixiTextEdit->document()->setModified(false);
    setWindowTitle(CurFilePath + " - 记事本");
    statusBar()->showMessage("文件保存成功喵~",2000);
    return true;
}
