/*******************************************************************************
 File Name        : dragpanel.h
 Summary          : Less frame window with base funcation
 Create           : 2012-08-24
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2012-08-24   Create New
 002     Ren Dan       2012-12-05   Add resize right&bottom funcation
 003     Ren Dan       2013-04-23   Delete constructor enable resize parameter
 004     Ren Dan       2013-04-23   Add full resize funcation
 005     Ren Dan       2013-10-25   Add press widget judge
 006     Ren Dan       2013-11-18   Fixed double click to show maxed or show normal
 007     Ren Dan       2014-01-01   Fixed maxed size move event
 008     Ren Dan       2015-04-28   Change base class to QDialog
 009     Ren Dan       2015-05-12   Remove SetPanelMinSize() and OnShowMiniSize()
 010     Ren Dan       2016-09-01   Fix the problem in terms of the coordinates of the non-window
 011     Ren Dan       2016-11-20   Add window SHADOW
 012     Ren Dan       2016-11-22   Fix due to the gap caused by SHADOW when show maximized And take over Margin of Boxlayout
*******************************************************************************/

#ifndef DRAGPANEL_H
#define DRAGPANEL_H

#include <QDialog>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

class DragPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DragPanel(QWidget *parent = 0, Qt::MouseButton mButton = Qt::NoButton);

    /***************************************************
      Author: RenDan
      Date: 2013-04-23
      Description: Set the panel resize frame width
      Input: int
      Output: void
      Return: void
    ****************************************************/
    void SetResizeFrameWidth(const int &frameWidth);

    /***************************************************
      Author: RenDan
      Date: 2013-11-12
      Description: Set the show maxsize enable or not
      Input: bool
      Output: void
      Return: void
    ****************************************************/
    void SetShowMaxAble(const bool &isEnable);

    /***************************************************
      Author: RenDan
      Date: 2013-04-23
      Description: Set the panel object name
      Input: QString
      Output: void
      Return: void
    ****************************************************/
    void SetObjectName(const QString &name);

    inline bool isShowMaximized() { return m_isMaxSize; }

    /***************************************************
      Author: RenDan
      Date: 2016-11-20
      Description: Set the panel shadow width, color and alpha
      Input: int, QColor, int(recommended value 0-160)
      Output: void
      Return: void
    ****************************************************/
    void SetShadowStyle(int width = 5, QColor color = Qt::black, int alpha = 50);

protected:

    /***************************************************
      Author: RenDan
      Date: 2012-08-24
      Description: Mouse Press Event
      Input: QMouseEvent*
      Output: void
      Return: void
    ****************************************************/
    void mousePressEvent(QMouseEvent *);

    /***************************************************
      Author: RenDan
      Date: 2012-08-24
      Modify: 2012-12-05
      Modify: 2013-04-23
      Description: Mouse Move Event
      Input: QMouseEvent*
      Output: void
      Return: void
    ****************************************************/
    void mouseMoveEvent(QMouseEvent *);

    /***************************************************
      Author: RenDan
      Date: 2012-08-24
      Description: Mouse Release Event
      Input: QMouseEvent*
      Output: void
      Return: void
    ****************************************************/
    void mouseReleaseEvent(QMouseEvent *);

    /***************************************************
      Author: RenDan
      Date: 2013-04-23
      Description: Mouse DoubleClick Event
      Input: QMouseEvent*
      Output: void
      Return: void
    ****************************************************/
    void mouseDoubleClickEvent(QMouseEvent *);

    /***************************************************
      Author: RenDan
      Date: 2013-04-26
      Description: When window show repaint it
      Input: QShowEvent*
      Output: void
      Return: void
    ****************************************************/
    void showEvent(QShowEvent *);

    /***************************************************
      Author: RenDan
      Date: 2016-11-20
      Description: When window repaint do this
                   like add shadow
      Input: QPaintEvent*
      Output: void
      Return: void
    ****************************************************/
    void paintEvent(QPaintEvent *);

public slots:

    /***************************************************
      Author: RenDan
      Date: 2013-04-23
      Description: Set the panel Show max size
      Input: void
      Output: void
      Return: void
    ****************************************************/
    void OnShowMaximized();

    /***************************************************
      Author: RenDan
      Date: 2013-04-23
      Description: Set the panel Show normal size
      Input: void
      Output: void
      Return: void
    ****************************************************/
    void OnShowNormal();

signals:
    void ToShowMaximized(bool isToMax);

private:
    enum ResizeStyle
    {
        NotResizing,
        ResizeLeft,
        ResizeRight,
        ResizeTop,
        ResizeBottom,
        ResizeLT,
        ResizeLB,
        ResizeRT,
        ResizeRB
    };

private:
    Qt::MouseButton     m_mButton;
    QPoint              m_DragPosition;
    int                 m_FrameWidth;
    QPoint              m_RBPoint;
    ResizeStyle         m_ResizeStyle;
    bool                m_isMaxAble;
    bool                m_isMaxSize;
    QRect               m_CurrentRect;
    bool                m_isPressed;
    int                 m_ShadowWidth;
    QColor              m_ShadowColor;
    int                 m_ShadowAlpha;
};
#endif // DRAGPANEL_H
