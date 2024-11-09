#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textchange = false;             //初始化，判断是否改变

    statusLabel.setMaximumWidth(150);
    statusLabel.setText("长度:" + QString::number(0) + "    行号: " + QString::number(1));          //文本长度
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("行:" + QString::number(1) + "    列: " + QString::number(1));        //设置光标位置
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("黄文轩"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);

//---------------------------------------------------------------------------------
//这一段代码的目的是初始化换行，否则要点击两次才能正常
    QPlainTextEdit::LineWrapMode mode = ui->plainTextEdit->lineWrapMode();

    if( mode == QTextEdit::NoWrap ){
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionWrap->setChecked(true);
    }else{
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->actionWrap->setChecked(false);
    }
//-----------------------------------------------------------------------------------

    ui->actionToolbar->setChecked(true);
    ui->actionStatusbar->setChecked(true);

    ui->actionShowLineNumber->setChecked(false);
    on_actionShowLineNumber_triggered(false);
    connect(ui->actionShowLineNumber,SIGNAL(triggered(bool)),
            ui->plainTextEdit,SLOT(hideLineNumberArea(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dlg(this,ui->plainTextEdit);
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg(this,ui->plainTextEdit);
    dlg.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed()){           //最后总会返回true，所以里面的return是不会运行的
        return;
    }

    file_path = "";
    ui->plainTextEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));

    textchange = false;
}



void MainWindow::on_actionOpen_triggered()
{

    if(!userEditConfirmed()){           //最后总会返回true，所以里面的return是不会运行的
        return;
    }

    QString file_name = QFileDialog::getOpenFileName(this,"打开文件",".",                       //文件打开窗口
                                                     tr("Text file (*.txt) ;; ALL (*.*)"));

    file_path = file_name;                                                                    //路径保存
    //当前窗口，标题，文件路径，过滤器
    QFile file(file_name);                                  //文件打开

    if( !file.open(QFile::ReadOnly | QFile::Text) ){        //打开检测
        QMessageBox::warning(this,"警告","打开文件失败");
        return;
    }

    QTextStream in(&file);                                  //输入流建立，且读取到输入流
    QString text = in.readAll();                            //输入流里的文本读取
    ui->plainTextEdit->setPlainText(text);                  //文本放入
    file.close();                                           //关闭文件

    this->setWindowTitle(file_name);                        //文件标题改变
    // this->setWindowTitle(QFileInfo(file_name).absoluteFilePath());

    textchange = false;

}


void MainWindow::on_actionSave_triggered()
{
    if( file_path == "" ){
        QString file_name = QFileDialog::getSaveFileName(this,"保存","新建文本文件",
                                                         tr("Text file (*.txt) ;; ALL (*.*)"));
        file_path = file_name;
    }

    QFile file(file_path);          //打开文件路径的文件，在读取文件时就已经将路径导入

    if( !file.open(QFile::WriteOnly | QFile::Text) ){
        QMessageBox::warning(this,"警告","文件保存失败");
        return;
    }

    QTextStream out(&file);         //建立输出流
    QString text = ui->plainTextEdit->toPlainText();    //将text里的文件读取
    out<<text;                      //将text的文件放入缓冲区
    file.flush();                   //将缓冲区的所有文本返还到file中
    file.close();

    textchange = false;
    this->setWindowTitle(file_path);                    //文件标题改变(去掉“*”)

}


void MainWindow::on_actionSaveAs_triggered()
{
    QFileInfo fileInfo(file_path);                  //获取文件信息
    QString name = fileInfo.fileName();             //获取文件名字


    QString file_name = QFileDialog::getSaveFileName(this,"另存为",name,
                                                     tr("Text file (*.txt) ;; ALL (*.*)"));
    //当前文件，保存窗口标题，文件初始名字，过滤器

    QFile file(file_name);          //打开文件路径的文件，在读取文件时就已经将路径导入
    if( !file.open(QFile::WriteOnly | QFile::Text) ){
        QMessageBox::warning(this,"警告","保存失败");
        return;
    }
    file_path = file_name;
    QTextStream out(&file);         //建立输出流
    QString text = ui->plainTextEdit->toPlainText();    //将text里的文件读取
    out<<text;                      //将text的文件放入缓冲区
    file.flush();                   //将缓冲区的所有文本返还到file中
    file.close();

    textchange = false;
    this->setWindowTitle(file_name);                        //文件标题改变
}


void MainWindow::on_plainTextEdit_textChanged()             //文本栏状态改变
{
    if( !textchange ){
        this->setWindowTitle("*" + this->windowTitle() );
        textchange = true;
    }
    statusLabel.setText("总长度:" + QString::number(ui->plainTextEdit->toPlainText().length()) +
                        "    总行号: " + QString::number(ui->plainTextEdit->document()->lineCount()));          //设置文本信息
}

bool MainWindow::userEditConfirmed()
{
    if( textchange ){
        QString path = file_path != "" ? file_path : "新建文本文件.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("文件保存");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n") + "\"" + path + "\" ?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int r = msg.exec();
        switch(r){
        case QMessageBox::Yes:
            on_actionSave_triggered();                  //是的话会跳转到保存界面
            break;
        case QMessageBox::No:
            textchange = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }
    return true;
}


void MainWindow::on_actionUndo_triggered()
{
    ui->plainTextEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->plainTextEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->plainTextEdit->copy();
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionPaste_triggered()
{
    ui->plainTextEdit->paste();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->plainTextEdit->redo();
}


//可行性判断
void MainWindow::on_plainTextEdit_undoAvailable(bool b)     //能撤销就将撤销显示，（下面以此类推）
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_plainTextEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_plainTextEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_actionFontColor_triggered()    //字体颜色
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");   //初始化颜色
    if(color.isValid()){
        ui->plainTextEdit->setStyleSheet(QString("QPlainTextEdit {color:%1}").arg(color.name()));
    }
}


void MainWindow::on_actionFontBackColar_triggered() //字体背景色
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");   //初始化颜色
    if(color.isValid()){
        ui->plainTextEdit->setStyleSheet(QString("QPlainTextEdit {background-color:%1}").arg(color.name()));
    }
}


void MainWindow::on_actionEditBackColor_triggered()
{

}


void MainWindow::on_actionWrap_triggered()  //自动换行
{
    QPlainTextEdit::LineWrapMode mode = ui->plainTextEdit->lineWrapMode();

    if( mode == QTextEdit::NoWrap ){
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionWrap->setChecked(true);
    }else{
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->actionWrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);

    if( ok ){
        ui->plainTextEdit->setFont(font);
    }
}


void MainWindow::on_actionToolbar_triggered()       //工具栏
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolbar->setChecked(!visible);
}


void MainWindow::on_actionStatusbar_triggered()     //状态栏
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusbar->setChecked(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->plainTextEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if( userEditConfirmed() ){
        exit(0);
    }
}


void MainWindow::on_plainTextEdit_cursorPositionChanged()           //光标位置显示
{
    int pos = ui->plainTextEdit->textCursor().position();           //光标位置(处于的字符串位置)
    int ln = 0;
    int fla = -1;
    QString text = ui->plainTextEdit->toPlainText();

    for(int i=0;i<pos;i++){                 //找行数和当前行开头对应的是第几个字符串
        if( text[i] == '\n' ){
            ln += 1;
            fla = i;
        }
    }
    fla += 1;                               //由于最后是匹配'\n',所以要+1
    int col = pos - fla;
    statusCursorLabel.setText("行:" + QString::number(ln) +
                              "    列: " + QString::number(col + 1));        //设置光标位置


}


void MainWindow::on_actionShowLineNumber_triggered(bool checked)
{
    ui->plainTextEdit->hideLineNumberArea(!checked);
}

