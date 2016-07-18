#ifndef SENSORDIALOG_H
#define SENSORDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QLineEdit>
#include <QList>

namespace Ui {
class SensorDialog;
}

class MyLineEdit : public QLineEdit
{
public:
    explicit MyLineEdit(QWidget *parent = 0);
protected:
    virtual void keyPressEvent(QKeyEvent *e);
};

class SensorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SensorDialog(QWidget *parent = 0);
    ~SensorDialog();

public slots:
    void putSerial(QString serial);

private slots:
    void apply();
    void cancel();

private:
    void setOrder();
    void generateList();
    Ui::SensorDialog *ui;
    QList<MyLineEdit*> *lineList;

};

#endif // SENSORDIALOG_H
