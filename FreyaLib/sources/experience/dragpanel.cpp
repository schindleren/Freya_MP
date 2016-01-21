/*******************************************************************************
 File Name        : dragpanel.cpp
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

#include "dragpanel.h"

DragPanel::DragPanel(QWidget *parent, Qt::MouseButton mButton) :
    QDialog(parent)
{
    m_mButton = mButton;
    m_isPressed = false;
    m_FrameWidth = 10;
    m_isMaxSize = false;
    m_isMaxAble = true;

    setMouseTracking(true);
}

/***************************************************
  Author: RenDan
  Date: 2013-04-23
  Description: Set the panel resize frame width
  Input: int
  Output: void
  Return: void
****************************************************/
void DragPanel::SetResizeFrameWidth(const int &frameWidth)
{
    m_FrameWidth = frameWidth;

    if(frameWidth <= 0)
    {
        m_isMaxAble = false;
    }
}

/***************************************************
  Author: RenDan
  Date: 2013-11-12
  Description: Set the show maxsize enable or not
  Input: bool
  Output: void
  Return: void
****************************************************/
void DragPanel::SetShowMaxAble(const bool &isEnable)
{
    m_isMaxAble = isEnable;
}

/***************************************************
  Author: RenDan
  Date: 2013-04-23
  Description: Set the panel object name
  Input: QString
  Output: void
  Return: void
****************************************************/
void DragPanel::SetObjectName(const QString &name)
{
    setObjectName(name);
}

/***************************************************
  Author: RenDan
  Date: 2013-04-23
  Description: Set the panel Show max size
  Input: void
  Output: void
  Return: void
****************************************************/
void DragPanel::OnShowMaximized()
{
    if(m_isMaxAble)
    {
        if(m_isMaxSize)
        {
            OnShowNormal();
        }
        else
        {
            m_isMaxSize = true;
            m_CurrentRect = frameGeometry();
            QDesktopWidget screenWidget;
            setGeometry(screenWidget.availableGeometry(0));
            ToShowMaximized(m_isMaxSize);
        }
    }
}

/***************************************************
  Author: RenDan
  Date: 2013-04-23
  Description: Set the panel Show normal size
  Input: void
  Output: void
  Return: void
****************************************************/
void DragPanel::OnShowNormal()
{
    m_isMaxSize = false;
    setGeometry(m_CurrentRect);
    ToShowMaximized(m_isMaxSize);
}

/***************************************************
  Author: RenDan
  Date: 2012-08-24
  Description: Mouse Press Event
  Input: QMouseEvent*
  Output: void
  Return: void
****************************************************/
void DragPanel::mousePressEvent(QMouseEvent *e)
{
    if (!m_isMaxSize && e->button() == m_mButton)
    {
        m_DragPosition = e->globalPos() - frameGeometry().topLeft();
        m_RBPoint = frameGeometry().bottomRight();
        m_isPressed = true;
        e->accept();
    }
    else//Do nothing
    {
        e->ignore();
    }
}

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
void DragPanel::mouseMoveEvent(QMouseEvent *e)
{
    if(m_isMaxSize)
    {
        e->accept();
        return;
    }
    //Set mouse State
    if(!(e->buttons() & m_mButton) && (m_FrameWidth > 0))
    {
        //Left Top
        if(e->pos().x() < m_FrameWidth && e->pos().y() < m_FrameWidth)
        {
            setCursor(QCursor(Qt::SizeFDiagCursor));
            m_ResizeStyle = ResizeLT;
        }
        //Right Top
        else if(e->pos().x() > width() - m_FrameWidth && e->pos().y() < m_FrameWidth)
        {
            setCursor(QCursor(Qt::SizeBDiagCursor));
            m_ResizeStyle = ResizeRT;
        }
        //Left Bottom
        else if(e->pos().x() < m_FrameWidth && e->pos().y() > (height() - m_FrameWidth))
        {
            setCursor(QCursor(Qt::SizeBDiagCursor));
            m_ResizeStyle = ResizeLB;
        }
        //Right Bottom
        else if(e->pos().x() > (width() - m_FrameWidth) && e->pos().y() > (height() - m_FrameWidth))
        {
            setCursor(QCursor(Qt::SizeFDiagCursor));
            m_ResizeStyle = ResizeRB;
        }
        //Left
        else if(e->pos().x() < m_FrameWidth)
        {
            setCursor(QCursor(Qt::SizeHorCursor));
            m_ResizeStyle = ResizeLeft;
        }
        //Top
        else if(e->pos().y() < m_FrameWidth)
        {
            setCursor(QCursor(Qt::SizeVerCursor));
            m_ResizeStyle = ResizeTop;
        }
        //Right
        else if(e->pos().x() > width() - m_FrameWidth)
        {
            setCursor(QCursor(Qt::SizeHorCursor));
            m_ResizeStyle = ResizeRight;
        }
        //Bottom
        else if(e->pos().y() > height() - m_FrameWidth)
        {
            setCursor(QCursor(Qt::SizeVerCursor));
            m_ResizeStyle = ResizeBottom;
        }
        //Move
        else
        {
            setCursor(QCursor(Qt::ArrowCursor));
            m_ResizeStyle = NotResizing;
        }
    }
    //On Resize & Move
    else if(e->buttons() & m_mButton)
    {
        //Drag Left Top pos
        if(m_ResizeStyle == ResizeLT)
        {
            QSize rSize = QSize(m_RBPoint.x() - e->globalPos().x(), m_RBPoint.y() - e->globalPos().y());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            else
            {
                move(e->globalPos().x(), y());
            }
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            else
            {
                move(x(), e->globalPos().y());
            }
            resize(rSize);
        }
        //Drag Right Top pos
        else if(m_ResizeStyle == ResizeRT)
        {
            QSize rSize = QSize(e->globalPos().x() - frameGeometry().left(), m_RBPoint.y() - e->globalPos().y());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            else
            {
                move(x(), e->globalPos().y());
            }
            resize(rSize);
        }
        //Drag Left Bottom pos
        else if(m_ResizeStyle == ResizeLB)
        {
            QSize rSize = QSize(m_RBPoint.x() - e->globalPos().x(), e->globalPos().y() - frameGeometry().top());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            else
            {
                move(e->globalPos().x(), y());
            }
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            resize(rSize);
        }
        //Drag Right Bottom pos
        else if(m_ResizeStyle == ResizeRB)
        {
            QSize rSize = QSize(e->globalPos().x() - frameGeometry().left(), e->globalPos().y() - frameGeometry().top());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            resize(rSize);
        }
        //Drag Left line
        else if(m_ResizeStyle == ResizeLeft)
        {
            QSize rSize = QSize(m_RBPoint.x() - e->globalPos().x(), height());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            else
            {
                move(e->globalPos().x(), y());
            }
            resize(rSize);
        }
        //Drag Top line
        else if(m_ResizeStyle == ResizeTop)
        {
            QSize rSize = QSize(width(), m_RBPoint.y() - e->globalPos().y());
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            else
            {
                move(x(), e->globalPos().y());
            }
            resize(rSize);
        }
        //Drag Right line
        else if(m_ResizeStyle == ResizeRight)
        {
            QSize rSize = QSize(e->globalPos().x() - frameGeometry().left(), height());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            resize(rSize);
        }
        //Drag Bottom line
        else if(m_ResizeStyle == ResizeBottom)
        {
            QSize rSize = QSize(width(), e->globalPos().y() - frameGeometry().top());
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            resize(rSize);
        }
        //Move
        else if(m_isPressed)
        {
            move(e->globalPos() - m_DragPosition);
        }
        e->ignore();
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

/***************************************************
  Author: RenDan
  Date: 2012-08-24
  Description: Mouse Release Event
  Input: QMouseEvent*
  Output: void
  Return: void
****************************************************/
void DragPanel::mouseReleaseEvent(QMouseEvent *e)
{
    m_ResizeStyle = NotResizing ;

    if (m_mButton == Qt::NoButton)
    {
        e->ignore();
    }
    else
    {
        m_isPressed = false;
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

/***************************************************
  Author: RenDan
  Date: 2013-04-23
  Description: Mouse DoubleClick Event
  Input: QMouseEvent*
  Output: void
  Return: void
****************************************************/
void DragPanel::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == m_mButton)
    {
        if(m_isMaxAble)
        {
            if(m_isMaxSize)
            {
                OnShowNormal();
            }
            else
            {
                OnShowMaximized();
            }
        }
        else
        {
            e->ignore();
        }
    }
}

/***************************************************
  Author: RenDan
  Date: 2013-04-26
  Description: When window show repaint it
  Input: QShowEvent*
  Output: void
  Return: void
****************************************************/
void DragPanel::showEvent(QShowEvent *)
{
    repaint();
}
