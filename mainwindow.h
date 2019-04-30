#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class VLCPlayer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VLCPlayer* m_vplayer;
};

#endif // MAINWINDOW_H
