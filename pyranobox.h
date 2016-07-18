#ifndef PYRANOBOX_H
#define PYRANOBOX_H

#include <QWidget>
#include <QLineEdit>

class PyranoBox : public QWidget
{
    Q_OBJECT
public:
    explicit PyranoBox(QString name, QWidget *parent = 0);

    void writeWatt(int watt);
    void writeTemp(int Temp);
    void clear();

    void setEnabled(bool enable);

signals:

public slots:

private:
 QLineEdit *showWatt;
 QLineEdit *showTemp;

};

#endif // PYRANOBOX_H
