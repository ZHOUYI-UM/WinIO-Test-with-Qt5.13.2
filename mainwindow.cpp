#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <winioctl.h>
#include "WinIo.h"
#include <QtDebug>
#include <QMessageBox>
#include <QLibrary>

HINSTANCE hDLL; // Handle to DLL
typedef bool (*fun)();
typedef bool (*gff)(WORD, PDWORD, BYTE);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#if 0
    QLibrary qlib("WinIo64");
    if(qlib.load())
        {
            qDebug() << "\r\n";
            qDebug() << "qlib.load() success!" ;
            fun initWinIo=(fun)qlib.resolve("InitializeWinIo");
            if (initWinIo==nullptr)
            {
                qDebug() << "qlib.load() initWinIo fail";
            }
            bool bRet = initWinIo();
            if (!bRet) {
                qDebug()<<"Error In InitializeWinIo!";
            }
            gff fGetVal=(gff)qlib.resolve("GetPortVal");
            if (fGetVal==nullptr)
            {
                qDebug() << "qlib.load() GetPortVal fail";
            }

            WORD pbPhysAddr=0xA00;
            DWORD val = 0x0;
            bRet = fGetVal(pbPhysAddr,&val,(BYTE)1);
            qDebug() << QString::number(val, 16)<< " " << QString::number(val, 2);

            fGetVal(pbPhysAddr,&val,1);
            qDebug() << QString::number(val, 16)<< " " << QString::number(val, 2);

            qlib.unload();
    }
#else
    int retValue = 0;//非0代表的是失败，所以先暂时赋值为0
    retValue = InitializeWinIo();
    if (!retValue)
    {
        QMessageBox::critical(this, tr("Error"), tr("Can not load InitializeWinIo"));
    }
#endif
}

MainWindow::~MainWindow()
{
    ShutdownWinIo();  //关闭WinIo
    delete ui;
}

DWORD MainWindow::HTKWriteDo(WORD pbPhysAddr, int bitIndex)
{
    ////1 param: 写入的IO内存地址 ；2 param: 写入的值，单位为字；3 param:1 byte 2 WORD  3 DWORD
    //若写入成功，则返回0
    int flag = 0;
    WORD Data;
    DWORD dwPortVal2 = 0x0;
    DWORD temp = 1;

    temp = temp<<bitIndex;//将1左移bitIndex位，从而得到要与的值
    GetPortVal((WORD)pbPhysAddr,&dwPortVal2,1);
    QString str2 = "Previous write data:"+QString::number(dwPortVal2, 10);
    QMessageBox::information(NULL, "dwPortVal2", str2);
    DWORD flag2 = dwPortVal2&temp;

    if(flag2)//若为真，则第bitIndex位为1，灯是灭的，那么就要将其点亮
    {
        dwPortVal2 = ~dwPortVal2;
        Data = dwPortVal2 | temp;//取反->与->取反
        Data = ~Data;
    }
    else
    {
        Data = dwPortVal2 | temp;
    }

    QString str3 = "Write data:"+QString::number(Data, 10);
    QMessageBox::information(NULL, "Data", str3);

    SetPortVal((WORD)pbPhysAddr,(DWORD)Data,1);//进行设置
    dwPortVal2 = 0x0;//再初始化一次然后再获取
    GetPortVal((WORD)pbPhysAddr,&dwPortVal2,1);
    return dwPortVal2;//把那个地址获得的数据往上传
}

DWORD MainWindow::HTKReadDI(WORD pbPhysAddr, int index)
{
    //1 param: 要读出的IO内存地址 ；2 param: 读出的值，单位为字；3 param:1 byte 2 WORD  3 DWORD
    //dwPortVal指向双字变量的指针，接收从端口得到的数据。
    DWORD val = 0x0 ;
    GetPortVal((WORD)pbPhysAddr, &val, 1);
    qDebug() << QString::number(val, 16)<< " " << QString::number(val, 2);
    return val;
}

void MainWindow::on_pbGetValue_clicked()
{
    QString str = ui->leAddr->text();
    QByteArray array;
    array.append(str);
    WORD addr = array.toUShort(Q_NULLPTR,16);
    DWORD val = HTKReadDI(addr, 1);
    ui->leValue->setText(QString::number(val, 2));
}

void MainWindow::on_pbSetValue_clicked()
{

}
