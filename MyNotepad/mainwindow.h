#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_plainTextEdit_textChanged();

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString file_path;
    bool textchange;            //判断是否改变
    bool userEditConfirmed();       //判断是否确定改变
};
#endif // MAINWINDOW_H
