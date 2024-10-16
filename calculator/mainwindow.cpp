#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(binaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(binaryOperatorClicked()));
    connect(ui->btnMutli,SIGNAL(clicked()),this,SLOT(binaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(binaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(){                          //计算处理
    double result = NULL;
    QString Error = "除数不能为0";

    double operand1 = operands.front().toDouble();          //第一个数
    operands.pop_front();
    double operand2 = operands.front().toDouble();          //第二个数
    operands.pop_front();

    QString code = opcode;

    qDebug()<<operand1<<code<<operand2;

    if( code == "+" ){
        result = operand1 + operand2;
    }else if( code == "-"){
            result = operand1 - operand2;
    }else if( code == "×" ){
        result = operand1 * operand2;
    }else if( code == "/"){
        if( operand2 == 0 ){
            return Error;
        }else{
            result = operand1 / operand2;
        }
    }
    QString message = QString("%1 %2 %3 = %4").arg(operand1).arg(code).arg(operand2).arg(result);

    ui->statusbar->showMessage(message);

    return QString::number(result);
}


void MainWindow::on_btnEqual_clicked()                      // "="的处理
{
    if( operand != "" ){
        operands.push_back(operand);
        operand = "";
    }

    QString result = "";
    if(operands.size() == 2 && opcode != ""){
        result = calculation();                         //结果输出

        //operands.push_back(result);                     //将结果放入
        if( result != "除数不能为0" ){
            operand = result;
        }
        ui->display->setText(result);
    }

}

void MainWindow::binaryOperatorClicked(){                   // 加减乘除 操作

    opcode = qobject_cast<QPushButton*>(sender())->text();  //读取操作符
    qDebug()<<"读取的操作符1:"<<opcode;


    if( operand != "" ){                                    //数据读取及其存储
        operands.push_back(operand);                        //将符号前的数字放入栈中

        operand = "";
        QString result = "";
        if(operands.size() == 2 && opcode != ""){
            result = calculation();                         //结果输出

            //operands.push_back(result);                     //将结果放入
            if( result != "除数不能为0" ){
                operand = result;
            }

            ui->display->setText(result);
        }

        qDebug()<<"读取的操作符:"<<opcode;
        qDebug()<<"储存的数据数:"<<operands.size();

    }


}

void MainWindow::btnNumClicked()            //数字点击
{
    QString digit = qobject_cast<QPushButton*>(sender())->text();       //将要添加的 字符 。

    if( operand == "0" ){               //当前只有0  （后面应该只用判断是否小数点就行了）
        if( digit == "0" ){             //输入为0
            digit = "";
        }
        else if( digit != "." ){        //输入不为0且不为小数点
            digit = "";
        }
    }

    operand += digit;

    ui->display->setText(operand);
    QString last_num = "    the last num: ";
    if( !operands.empty() ){
        last_num += operands.top();
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked" + last_num );
    //staatusbar表示的是下面的状态栏
    //qobject_cast<QPushButton*>(sender())->text() 是获取点击按钮的text文本，并且返回

}




void MainWindow::on_btnPeriod_clicked()         //小数点
{
    if(operand.contains(".") || operand == ""){             //如果包含了小数点 或者 当前输入栏目为空
        return ;
    }
    operand += qobject_cast<QPushButton*>(sender())->text();

    ui->display->setText(operand);
    QString last_num = "    the last num: ";
    if( !operands.empty() ){
        last_num += operands.top();
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked" + last_num );
}


void MainWindow::on_btnDel_clicked()            //删除最右边的一个数字
{
    operand = operand.left(operand.length() - 1);
    ui->display->setText(operand);
    QString last_num = "    the last num: ";
    if( !operands.empty() ){
        last_num += operands.top();
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked" + last_num );
}


void MainWindow::on_btnClear_clicked()          //全部清除
{
    operand.clear();                            //当前数字清除
    opcode.clear();                             //当前操作符清除
    ui->display->setText(operand);
    while( !operands.empty() ){                 //将存储的全部清除
        operands.pop();
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked");
}

void MainWindow::on_btnClearError_clicked()
{
    operand.clear();                            //当前数字清除
    ui->display->setText(operand);
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked");
}


void MainWindow::on_btnSquare_clicked()         //将当前文本的数字进行平方
{
    if(operand != ""){
        double number = operand.toDouble();
        number = pow(number,2);
        operand = QString::number(number);
        ui->display->setText(operand);
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked");
}


void MainWindow::on_btnSqtr_clicked()           //对当前数进行开根号
{
    if(operand != ""){
        double number = operand.toDouble();
        number = sqrt(number);
        operand = QString::number(number);
        ui->display->setText(operand);
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked");
}


void MainWindow::on_btnInverse_clicked()        //对当前数进行倒数处理
{
    if(operand != ""){
        double number = operand.toDouble();
        number = 1.0/number;
        operand = QString::number(number);
        ui->display->setText(operand);
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked");
}


void MainWindow::on_btnPercentage_clicked()     //变成百分比数
{
    if(operand != ""){
        double number = operand.toDouble();
        number = number/100.0;
        operand = QString::number(number);
        ui->display->setText(operand);
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked");
}


void MainWindow::on_btnSign_clicked()
{
    if(operand != ""){
        double number = operand.toDouble();
        number = -number;
        operand = QString::number(number);
        ui->display->setText(operand);
    }
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text() + "  btn clicked");
}




