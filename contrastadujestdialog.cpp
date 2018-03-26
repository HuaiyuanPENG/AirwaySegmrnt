#include "contrastadujestdialog.h"
#include "ui_contrastadujestdialog.h"
#include <QRegExpValidator>
#include <QRegExp>

ContrastAdujestDialog::ContrastAdujestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContrastAdujestDialog)
{
    ui->setupUi(this);
    //设置输入框只能输入整数
    ui->window->setValidator(new QRegExpValidator(QRegExp("^-\d+$")));
    ui->level->setValidator(new QRegExpValidator(QRegExp("^-\d+$")));
}

ContrastAdujestDialog::~ContrastAdujestDialog()
{
    delete ui;
}

void ContrastAdujestDialog::on_buttonBox_accepted()
{
    float w = ui->window->text().toFloat();
    float l = ui->level->text().toFloat();
    emit sentWindowLevel( w, l );
}
