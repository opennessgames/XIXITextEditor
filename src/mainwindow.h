/*
 * @Author: xixi_
 * @Date: 2024-11-02 17-07-37
 * @LastEditors: xixi_
 * @LastEditTime: 2026-07-08 02:56:59
 * @FilePath: /XIXITextEditor/src/mainwindow.h
 * Copyright (c) 2017-2026 by xixi_ , All Rights Reserved.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

namespace XixiActKind
{
    Q_NAMESPACE

    typedef enum
    {
        // File
        NewFile,
        OpenFile,
        SaveFile,
        SaveAsFile,
        // Quit
        Quit,
        // Edit
        Cut,
        Copy,
        Paste,
        Undo,
        SelectAll,
    } XixiActionKind;

    Q_ENUM_NS(XixiActionKind);
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void XIXI_ActionTriggter(QAction *act);
    void XIXI_NewFile();
    void XIXI_OpenFile();
    bool XIXI_SaveFile();
    bool XIXI_SaveAsFile();

private:
    void XIXI_UpdateCursorPosition();
    bool XIXI_MayBeSave();
    void XIXI_LoadFile(const QString &FilePath);
    bool XIXI_WriteFile(const QString &FilePath);

private:
    Ui::MainWindow *ui;
    QString CurFilePath;
};

#endif // MAINWINDOW_H
