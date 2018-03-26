#ifndef CONTRASTADUJESTDIALOG_H
#define CONTRASTADUJESTDIALOG_H

#include <QDialog>

/*
 * 调整窗位，窗宽的对话框
*/

namespace Ui {
class ContrastAdujestDialog;
}

class ContrastAdujestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContrastAdujestDialog(QWidget *parent = 0);
    ~ContrastAdujestDialog();


private:
    Ui::ContrastAdujestDialog *ui;

signals:
    void sentWindowLevel(float w, float l);


private slots:
    void on_buttonBox_accepted();

};

#endif // CONTRASTADUJESTDIALOG_H
