#include "replacedialog.h"
#include "ui_replacedialog.h"

#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent,QPlainTextEdit * textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);

    pTextEdit = textEdit;               //获取全部文本

    ui->rbDown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}



void ReplaceDialog::on_btFindNext_clicked()
{
    QString target = ui->searchText->text();    //目标文本

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

void ReplaceDialog::on_btCancel_clicked()
{
    accept();
}


void ReplaceDialog::on_takeplace_clicked()
{
    QString target = ui->searchText->text();            //查找的目标
    QString to = ui->targetText->text();                //替换的文本
    if( (pTextEdit != nullptr) && (target != "") && (to != "") ){
        QString selText = pTextEdit->textCursor().selectedText();
        if( selText == target ){
            pTextEdit->insertPlainText(to);                 //将替换的加进去
        }
    }
    on_btFindNext_clicked();
}


void ReplaceDialog::on_takeallplace_clicked()
{
    QString target = ui->searchText->text();            //查找的目标
    QString to = ui->targetText->text();                //替换的文本
    if( (pTextEdit != nullptr) && (target != "") && (to != "") ){
        QString text = pTextEdit->toPlainText();
        text.replace(target,to,
                     ui->cbCaseSensetive->isChecked()? Qt::CaseSensitive : Qt::CaseInsensitive);

        pTextEdit->clear();                 //因为全部替换了，需要将文本进行更新
        pTextEdit->insertPlainText(text);   //实际是插入，因为里面已经没东西了，插入text约等于替换
    }
}

