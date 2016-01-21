/*******************************************************************************
 File Name        : dragpanel.h
 Summary          : Less frame window with base funcation
 Create           : 2012-08-24
 Author           : RenDan
 Update history   :
 NO.     Author        Date         Content
 001     Ren Dan       2012-08-24   Create New
 002     Ren Dan       2012-12-05   Add resize right&bottom funcation
 003     Ren Dan       2013-04-23   Delete constructor Enable resize parameter
 004     Ren Dan       2013-04-23   Add full resize funcation
 005     Ren Dan       2013-10-25   Add Press widget judge
 006     Ren Dan       2013-11-18   Fixed double click to show maxed or show normal
 007     Ren Dan       2014-01-01   Fixed maxed size move event
 008     Ren Dan       2015-04-28   Change base class to QDialog
 009     Ren Dan       2015-05-12   Remove SetPanelMinSize and OnShowMiniSize
*******************************************************************************/

#ifndef DRAGPANEL_H
#define DRAGPANEL_H

#include "freyalib_global.h"
#include <QDialog>
#include <QDesktopWidget>
#include <QMouseEvent>

class FREYALIBSHARED_EXPORT DragPanel : public QDialog
{
    Q_OBJECT

public:
    explicit DragPanel(QWidget *parent = 0, Qt::MouseButton mButton = Qt::NoButton);
    void SetResizeFrameWidth(const int &frameWidth);
    void SetShowMaxAble(const bool &isEnable);
    void SetObjectName(const QString &name);
    inline bool isShowMaximized() { return m_isMaxSize; }

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void showEvent(QShowEvent *);

public slots:
    void OnShowMaximized();
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
};
#endif // DRAGPANEL_H
