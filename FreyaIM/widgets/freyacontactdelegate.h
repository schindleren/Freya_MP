#ifndef FREYACONTACTDELEGATE_H
#define FREYACONTACTDELEGATE_H

#include <QStyledItemDelegate>
#include <QBoxLayout>
#include <QLabel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QWidget>

#define FREYAROLE_USERID      Qt::UserRole
#define FREYAROLE_USERDES     Qt::UserRole+1
#define FREYAROLE_USERNAME    Qt::UserRole+2
#define LITEHEIGHT      30
#define NORMALHEIGHT    50

class FreyaPortrait;
class FreyaContactWidget : public QWidget
{
    Q_OBJECT
public:
    FreyaContactWidget(QWidget * parent = 0);
    void SetContactPortrait(const QString &bareJid);
    void SetContactName(const QString &name);
    void SetContactDes(const QString &text);
    void SetWidgetState(bool isLite);

private:
    void initWidget();

    QBoxLayout              *m_ContactWidgetLay;
    FreyaPortrait           *m_ContactPortrait;
    QLabel                  *m_ContactNameLab;
    QLabel                  *m_ContactDesLab;
};

class FreyaContactDelegate : public QStyledItemDelegate
{
public:
    explicit FreyaContactDelegate(QWidget *parent = 0);
protected:
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // FREYACONTACTDELEGATE_H
