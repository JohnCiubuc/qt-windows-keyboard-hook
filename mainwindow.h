#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <qt_windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyDown(DWORD key);
    void keyUp(DWORD key);
private slots:
    void doMultimedia();
private:
    Ui::MainWindow *ui;
    HHOOK hhkLowLevelKybd;
    bool bWinKey;
    bool bF9;
    bool bF10;
};

#endif // MAINWINDOW_H
