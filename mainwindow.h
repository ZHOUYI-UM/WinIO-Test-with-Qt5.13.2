#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <WINDEF.H>

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
    DWORD HTKWriteDo(WORD pbPhysAddr ,int bitIndex);
    DWORD HTKReadDI(WORD pbPhysAddr ,int index);

private slots:
    void on_pbGetValue_clicked();

    void on_pbSetValue_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
