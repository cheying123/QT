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

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionRedo_triggered();

    void on_actionPaste_triggered();

    void on_plainTextEdit_undoAvailable(bool b);

    void on_plainTextEdit_copyAvailable(bool b);

    void on_plainTextEdit_redoAvailable(bool b);

    void on_actionFontColor_triggered();

    void on_actionFontBackColar_triggered();

    void on_actionEditBackColor_triggered();

    void on_actionWrap_triggered();

    void on_actionFont_triggered();

    void on_actionToolbar_triggered();

    void on_actionStatusbar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

    void on_plainTextEdit_cursorPositionChanged();



    //void on_actionShowLineNumber_triggered();

    void on_actionShowLineNumber_triggered(bool checked);

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString file_path;
    bool textchange;            //判断文本是否改变
    bool userEditConfirmed();       //判断是否确定文本改变
};
#endif // MAINWINDOW_H
