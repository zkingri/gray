#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"打开","~","所有文件(*)");
    if(!filename.compare(""))
        return;
    QImage image(filename);
    if (image.isNull())
        QMessageBox::information(this,"提示","未能打开文件 "+filename+" 。");
    else
    {
        if(!image.allGray())
            QMessageBox::information(this,"提示","该文件并非灰度图片，计算结果可能有误差。");
        unsigned int count[256]={0};
        unsigned int gray;
        QColor color;
        for(int i=0;i<image.width();++i)
            for(int j=0;j<image.height();++j)
            {
                color=image.pixelColor(i,j);
                gray=(color.red()+color.green()+color.blue())/3;
                ++count[gray];
            }
        unsigned int sum=0;
        ui->plainTextEdit->clear();
        ui->plainTextEdit->appendPlainText("灰度值\t像素数");
        for(int i=0;i<256;++i)
        {
            ui->plainTextEdit->appendPlainText(QString::number(i)+"\t"+QString::number(count[i]));
            sum += i * count[i];
        }
        ui->plainTextEdit->appendPlainText("图像加权平均灰度值为\n"+QString::number((double)sum/(image.width()*image.height()),'g',10)+"。");
    }
}
