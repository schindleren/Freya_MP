#include "freyatest.h"
#include "freyatest_global.h"
#include "freyabasecontrol.h"
#include "freyabaseextension.h"

#include "localclienta.h"
#include "localclientb.h"
#include "localclients.h"

FreyaTest::FreyaTest(QWidget *parent)
    : QWidget(parent)
{
    FreyaBaseExtension *pluginExtension = new FreyaBaseExtension(FYTS_ID_EXT, FreyaBaseControl::GetFreyaControl(), FYTS_OBJ_EXTENSION);
    pluginExtension->DefineAuthCode(QStringList() << FYTS_AUTH_OUT_1 << FYTS_AUTH_OUT_2 << FYTS_AUTH_OUT_4 << FYTS_AUTH_OUT_8,
                                    QStringList() << FYTS_AUTH_IN_1 << FYTS_AUTH_IN_2 << FYTS_AUTH_IN_4 << FYTS_AUTH_IN_8);

    m_SignalOne = new QPushButton("One", this);
    m_SignalTwo = new QPushButton("Two", this);
    m_SignalThr = new QPushButton("Thr", this);
    m_SignalOne->setGeometry(10, 10, 50, 30);
    m_SignalTwo->setGeometry(100, 10, 50, 30);
    m_SignalThr->setGeometry(190, 10, 50, 30);
    connect(m_SignalOne, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
    connect(m_SignalTwo, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
    connect(m_SignalThr, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));

    (new LocalClientA(this))->setGeometry(10, 50, 300, 500);
    new LocalClientB();
    new LocalClientS();
}

FreyaTest::~FreyaTest()
{
    FreyaBaseControl::GetFreyaControl()->DeleteAllAction();
}

void FreyaTest::OnButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    if(btn)
    {
        if(btn == m_SignalOne)
        {
            FREYA_REQUESTEXECUTION(FYTS_CMD_HAPPY);
        }
        else if(btn == m_SignalTwo)
        {
            FreyaData date = FreyaBaseData::CreateDate();
            date->command = FYTS_CMD_SAD;
            QVariantMap varMap;
            varMap.insert("T_1", "I am not happy");
            varMap.insert("T_2", "I am not wanna cry");
            varMap.insert("T_3", 37140);
            date->arguments = varMap;
            FREYA_REQUESTEXECUTION(date);
        }
        else if(btn == m_SignalThr)
        {
            FreyaData date = FreyaBaseData::CreateDate();
            date->command = FYTS_CMD_CRY;
            date->arguments = "Just wanna cry";
            FREYA_REQUESTEXECUTION(date);
        }
    }
}
