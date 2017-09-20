#ifndef FREYATEST_H
#define FREYATEST_H

#include <QtWidgets>

class FreyaTest : public QWidget
{
    Q_OBJECT

public:
    FreyaTest(QWidget *parent = 0);
    ~FreyaTest();

private slots:
    void OnButtonClicked();

private:
    QPushButton *m_SignalOne;
    QPushButton *m_SignalTwo;
    QPushButton *m_SignalThr;
};

#endif // FREYATEST_H
