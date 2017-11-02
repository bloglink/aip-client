/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       警告弹窗
*******************************************************************************/
#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QVariant>
#include <QGridLayout>
#include <QMessageBox>
#include <QDialogButtonBox>

class LQMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit LQMessageBox(QWidget  *parent = 0, const QString &title = tr("Tip"),
                            const QString &text = "",
                            QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                            QMessageBox::StandardButton defaultButton = QMessageBox::Ok);

    ~LQMessageBox();
    void setText(QString s);
protected:
    void changeEvent(QEvent *event);
private slots:
    void onButtonClicked(QAbstractButton *button);
    void translateButtonText(void);
private:
    QDialogButtonBox *ButtonBox;
    QLabel *TextLabel;
};

#endif // MESSAGEBOX_H
