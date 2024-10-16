#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qlabel>
#include <Qstack>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString opcode;             //加减乘除符号存储

    QString operand;            //数字存储
    QStack<QString> operands;

    QString calculation();      //计算

private slots:
    void on_btnEqual_clicked();

    void binaryOperatorClicked();

    void btnNumClicked();

    void on_btnPeriod_clicked();

    void on_btnDel_clicked();

    void on_btnClear_clicked();

    void on_btnSquare_clicked();

    void on_btnSqtr_clicked();

    void on_btnInverse_clicked();

    void on_btnPercentage_clicked();

    void on_btnSign_clicked();

    void on_btnClearError_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
