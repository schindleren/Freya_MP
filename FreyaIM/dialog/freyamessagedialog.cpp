#include "freyaim_global.h"
#include "freyamessagedialog.h"
#include "freyadefaultclient.h"

FreyaMessageDialog::FreyaMessageDialog(const char *dialogID, FreyaWindow *parent) :
    FreyaBackWindow(parent, dialogID)
{
    InitWidget();
}

FreyaMessageDialog::~FreyaMessageDialog()
{
}

void FreyaMessageDialog::DefinitionContent(const QString &content)
{
    QFontMetrics fm = m_ContentLab->fontMetrics();
    int w = fm.maxWidth()+20;
    int h = fm.height();
    int l = content.length();
    int c = qSqrt(l*w*1.0 / (3.0 * h));
    int l1 = l/c;
    m_ContentLab->setText(content);
    m_ContentLab->setFixedSize(w*l1, h*c);
}

void FreyaMessageDialog::AppendDecisionBtn(const QString &name, const FreyaBaseData &freyaData)
{
    QToolButton *decisionBtn = new QToolButton(this);
    decisionBtn->setObjectName("MsgDecBtn");
    decisionBtn->setText(name);
    decisionBtn->setFixedSize(80, 26);
    m_BtnHLay->addWidget(decisionBtn);
    connect(decisionBtn, SIGNAL(clicked()), this, SLOT(OnDecisionBtnClicked()));
    m_DecisionBtnMap.insert(decisionBtn, freyaData);
}

void FreyaMessageDialog::AppendMoreBtn(const QString &name, const FreyaBaseData &freyaData)
{
    QToolButton *tempBtn = new QToolButton(this);
    tempBtn->setObjectName("MsgMoreBtn");
    tempBtn->setText(name);
    m_BtnHLay->addWidget(tempBtn);
    connect(tempBtn, SIGNAL(clicked()), this, SLOT(OnOtherBtnClicked()));
    m_BtnMap.insert(tempBtn, freyaData);
}

void FreyaMessageDialog::InitWidget()
{
    SetShowMaxAble(false);
    SetResizeFrameWidth(0);
    needMaxBtn(false);
    SetTitle(tr("FreyaMessageDialog"));

    QWidget *MainWidget = MainBackWidget();
    QVBoxLayout *MainVLay = new QVBoxLayout(MainWidget);
    MainVLay->setSpacing(10);
    MainVLay->setMargin(20);

    m_ContentLab = new QLabel(MainWidget);
    m_ContentLab->setObjectName("MessageContentLab");
    m_ContentLab->setWordWrap(true);
    MainVLay->addWidget(m_ContentLab, 0, Qt::AlignCenter);

    m_BtnHLay = new QHBoxLayout;
    m_BtnHLay->setSpacing(10);
    m_BtnHLay->setMargin(10);
    m_BtnHLay->setSizeConstraint(QHBoxLayout::SetMinAndMaxSize);
    MainVLay->addLayout(m_BtnHLay);
}

void FreyaMessageDialog::Execute(const quint64 &/*command*/)
{
}

void FreyaMessageDialog::Execute(FreyaBaseData */*pData*/)
{
}

void FreyaMessageDialog::showEvent(QShowEvent *e)
{
    if(m_BtnHLay->count() == 0)
    {
        QToolButton *tempOKBtn = new QToolButton(this);
        tempOKBtn->setObjectName("MessageOKBtn");
        tempOKBtn->setFixedSize(80, 26);
        tempOKBtn->setText(tr("OK"));
        m_BtnHLay->addWidget(tempOKBtn);
        connect(tempOKBtn, SIGNAL(clicked()), this, SLOT(accept()));
    }
    resize(qMax(m_BtnHLay->minimumSize().width(), m_ContentLab->width()) + 20,
           m_ContentLab->height() + 60);
    FreyaBackWindow::showEvent(e);
}

void FreyaMessageDialog::OnDecisionBtnClicked()
{
    if(m_DecisionBtnMap.contains(sender()))
    {
        FreyaBaseData data = m_DecisionBtnMap.value(sender());
        FREYA_REQUESTEXECUTION(&data);
        if(QVariant::Map == data.arguments.type())
        {
            done(data.arguments.toMap().value(FREYA_CLS_MESSAGEBOXDECISION).toBool());
        }
        else
        {
            close();
        }
    }
}

void FreyaMessageDialog::OnOtherBtnClicked()
{
    if(m_BtnMap.contains(sender()))
    {
        FreyaBaseData data = m_BtnMap.value(sender());
        FREYA_REQUESTEXECUTION(&data);
    }
}
