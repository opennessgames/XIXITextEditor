/*
 * @Author: xixi_
 * @Date: 2024-11-02 17-07-37
 * @LastEditors: xixi_
 * @LastEditTime: 2026-07-08 02:49:36
 * @FilePath: /XIXITextEditor/src/main.cpp
 * Copyright (c) 2017-2026 by xixi_ , All Rights Reserved.
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
