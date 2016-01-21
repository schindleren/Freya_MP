#ifndef FREYAMESSAGEDIALOG_H
#define FREYAMESSAGEDIALOG_H

#include "freyabackwindow.h"
#include "QXmppRegisterIq.h"
#include <QToolButton>
#include <QLabel>
#include <QBoxLayout>
#include <QShowEvent>
#include <QFontMetrics>
#include <QtMath>

class FreyaMessageDialog : public FreyaBackWindow
{
    Q_OBJECT
public:
    explicit FreyaMessageDialog(const char *dialogID, FreyaWindow *parent = 0);
    ~FreyaMessageDialog();
    void DefinitionContent(const QString &content);
    void AppendDecisionBtn(const QString &name, const FreyaBaseData &freyaData);
    void AppendMoreBtn(const QString &name, const FreyaBaseData &freyaData);

private:
    void InitWidget();

protected:
    virtual void Execute(const quint64 &command);
    virtual void Execute(FreyaBaseData *pData);

    void showEvent(QShowEvent *);

private slots:
    void OnDecisionBtnClicked();
    void OnOtherBtnClicked();

private:
    QLabel                                  *m_ContentLab;
    QHBoxLayout                             *m_BtnHLay;
    QMap<QObject*, FreyaBaseData>           m_BtnMap;
    QMap<QObject*, FreyaBaseData>           m_DecisionBtnMap;
};

#endif // FREYAMESSAGEDIALOG_H
