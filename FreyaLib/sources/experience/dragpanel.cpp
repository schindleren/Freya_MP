/*******************************************************************************
 File Name        : dragpanel.cpp
 Summary          : Less frame window with base funcation
 Create           : 2012-08-24
 Author           : RenDan
 Update history   : (Look up at dragpanel.h)
*******************************************************************************/

#include "dragpanel.h"

DragPanel::DragPanel(QWidget *parent, Qt::MouseButton mButton) :
    QWidget(parent)
{
    m_mButton = mButton;
    m_isPressed = false;
    m_FrameWidth = 10;
    m_isMaxSize = false;
    m_isMaxAble = true;
    m_ShadowWidth = 0;
    m_ShadowColor = Qt::black;
    m_ShadowAlpha = 50;

    setMouseTracking(true);
}

void DragPanel::SetResizeFrameWidth(const int &frameWidth)
{
    m_FrameWidth = frameWidth;

    if(frameWidth <= 0)
    {
        m_isMaxAble = false;
    }
}

void DragPanel::SetShowMaxAble(const bool &isEnable)
{
    m_isMaxAble = isEnable;
}

void DragPanel::SetObjectName(const QString &name)
{
    setObjectName(name);
}

void DragPanel::SetShadowStyle(int width, QColor color, int alpha)
{
    m_ShadowWidth = width;
    m_ShadowColor = color;
    m_ShadowAlpha = alpha;
}

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

void DragPanel::OnShowNormal()
{
    m_isMaxSize = false;
    setGeometry(m_CurrentRect);
    ToShowMaximized(m_isMaxSize);
}

void DragPanel::mousePressEvent(QMouseEvent *e)
{
    if (!m_isMaxSize && e->button() == m_mButton)
    {
        QPoint GreaterPos(mapToParent(e->pos()));
        m_DragPosition = GreaterPos - frameGeometry().topLeft();
        m_RBPoint = frameGeometry().bottomRight();
        m_isPressed = true;
        e->accept();
    }
    else//Do nothing
    {
        e->ignore();
    }
}

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
        QPoint GreaterPos(mapToParent(e->pos()));
        //Drag Left Top pos
        if(m_ResizeStyle == ResizeLT)
        {
            QSize rSize = QSize(m_RBPoint.x() - GreaterPos.x(), m_RBPoint.y() - GreaterPos.y());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            else
            {
                move(GreaterPos.x(), y());
            }
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            else
            {
                move(x(), GreaterPos.y());
            }
            resize(rSize);
        }
        //Drag Right Top pos
        else if(m_ResizeStyle == ResizeRT)
        {
            QSize rSize = QSize(GreaterPos.x() - frameGeometry().left(), m_RBPoint.y() - GreaterPos.y());
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
                move(x(), GreaterPos.y());
            }
            resize(rSize);
        }
        //Drag Left Bottom pos
        else if(m_ResizeStyle == ResizeLB)
        {
            QSize rSize = QSize(m_RBPoint.x() - GreaterPos.x(), GreaterPos.y() - frameGeometry().top());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            else
            {
                move(GreaterPos.x(), y());
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
            QSize rSize = QSize(GreaterPos.x() - frameGeometry().left(), GreaterPos.y() - frameGeometry().top());
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
            QSize rSize = QSize(m_RBPoint.x() - GreaterPos.x(), height());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            else
            {
                move(GreaterPos.x(), y());
            }
            resize(rSize);
        }
        //Drag Top line
        else if(m_ResizeStyle == ResizeTop)
        {
            QSize rSize = QSize(width(), m_RBPoint.y() - GreaterPos.y());
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            else
            {
                move(x(), GreaterPos.y());
            }
            resize(rSize);
        }
        //Drag Right line
        else if(m_ResizeStyle == ResizeRight)
        {
            QSize rSize = QSize(GreaterPos.x() - frameGeometry().left(), height());
            if(rSize.width() < minimumWidth())
            {
                rSize.setWidth(minimumWidth());
            }
            resize(rSize);
        }
        //Drag Bottom line
        else if(m_ResizeStyle == ResizeBottom)
        {
            QSize rSize = QSize(width(), GreaterPos.y() - frameGeometry().top());
            if(rSize.height() < minimumHeight())
            {
                rSize.setHeight(minimumHeight());
            }
            resize(rSize);
        }
        //Move
        else if(m_isPressed)
        {
            move(GreaterPos - m_DragPosition);
        }
        e->ignore();
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

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

void DragPanel::showEvent(QShowEvent *)
{
    repaint();
}

void DragPanel::paintEvent(QPaintEvent *)
{
    if(m_ShadowWidth > 0)
    {
        int iShadowW = m_ShadowWidth;
        if(isShowMaximized())
        {
            iShadowW = 0;
        }
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(iShadowW, iShadowW, width() - iShadowW * 2, height() - iShadowW * 2);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(path, palette().background());

        for(int i = 0; i < iShadowW; i++)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addRoundedRect(iShadowW - i, iShadowW - i, width() - (iShadowW - i) * 2, height() - (iShadowW - i) * 2, i, i);
            qreal alp = m_ShadowAlpha * qLn(iShadowW) - m_ShadowAlpha * qLn(1 + i);
            m_ShadowColor.setAlpha(alp > 0 ? (alp > 255 ? 255 : alp) : 0);
            painter.setPen(m_ShadowColor);
            painter.drawPath(path);
        }

        QBoxLayout *bLay = qobject_cast<QBoxLayout*>(layout());
        if(bLay)
        {
            bLay->setMargin(iShadowW);
        }
    }
}
