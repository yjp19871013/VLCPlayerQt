#include <vlcplayer.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_vplayer = new VLCPlayer(this);

    m_vplayer->Play("/home/yjp/下载/1.mp4", static_cast<uint32_t>(ui->videoFrame->winId()));
}

MainWindow::~MainWindow()
{
    m_vplayer->Stop();
    delete m_vplayer;

    delete ui;
}
