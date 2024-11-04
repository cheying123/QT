#include "searchdialog.h"
#include "ui_searchdialog.h"

#include <QPlainTextEdit>
#include <QMessageBox>

SearchDialog::SearchDialog(QWidget *parent,QPlainTextEdit * textEdit)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    pTextEdit = textEdit;

    ui->rbDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btFindNext_clicked()
{
    QString target = ui->searchtext->text();    //目标文本
    if( target == "" || pTextEdit == nullptr ){
        return;
    }

    QString text = pTextEdit->toPlainText();    //全体文本
    QTextCursor c=pTextEdit->textCursor();      //当前光标
    int index = -1;

    if( ui->rbDown->isChecked() ){   //如果向下查询
        index = text.indexOf(target,c.position(),
                             ui->cbCaseSensetive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);          //从当前光标的位置开始
        if( index >= 0 ){
            c.setPosition(index);
            c.setPosition(index + target.length(),QTextCursor::KeepAnchor);     //移动锚点到目标后

            pTextEdit->setTextCursor(c);
        }
    }else if( ui->rbUp->isChecked() ){
        index = text.lastIndexOf(target,c.position() - text.length() -1,
                                 ui->cbCaseSensetive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);          //从当前光标的位置
        if( index >= 0 ){
            c.setPosition(index + target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);                           //移动锚点到目标前


            pTextEdit->setTextCursor(c);
        }
    }
    if( index < 0 ){
        QMessageBox::warning(this,"查找","查找失败");
    }
}


void SearchDialog::on_btCancel_clicked()        //"查找"中的取消
{
    accept();
}

