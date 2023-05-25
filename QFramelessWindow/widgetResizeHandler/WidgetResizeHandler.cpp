#include "WidgetResizeHandler.h"
#include <QFrame>
#include <QApplication>
#include <QDesktopWidget>
#include <QCursor>

#if QT_CONFIG(sizegrip)
#include <QSizeGrip>
#endif
#include <QEvent>
#include <QDebug>

#include "../titlebar/TitleBar.h"

#define RANGE 8

static bool resizeHorizontalDirectionFixed = false;
static bool resizeVerticalDirectionFixed = false;

WidgetResizeHandler::WidgetResizeHandler(QWidget *parent, QWidget *cw)
	: QObject(parent), m_widget(parent), m_childWidget(cw ? cw : parent),
	m_fw(0), m_extrahei(0), m_buttonDown(false),
	m_moveResizeMode(false), m_sizeprotect(true), m_movingEnabled(true)
{
	m_mode = Nowhere;
	m_widget->installEventFilter(this);
	m_widget->setMouseTracking(true);
	QFrame *frame = qobject_cast<QFrame*>(m_widget);
	m_range = frame ? frame->frameWidth() : RANGE;
	m_range = qMax(RANGE, m_range);
	m_activeForMove = m_activeForResize = true;

	m_widget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);

	m_titleBar = new TitleBar(m_widget);

	m_widget->resize(500, 500);
	m_titleBar->raise();
	setTitleBarHeight(m_titleBar->height());
}

void WidgetResizeHandler::setTitleBar(TitleBar * tBar)
{
	if (tBar != m_titleBar) {
		m_titleBar->deleteLater();
		m_titleBar = tBar;
		m_titleBar->setParent(m_widget);
		m_titleBar->raise();
		setTitleBarHeight(m_titleBar->height());
	}
}

TitleBar * WidgetResizeHandler::getTitleBar() const
{
	return m_titleBar;
}

void WidgetResizeHandler::setResizeEnabled(bool isEnabled)
{
	setActive(Resize, isEnabled);
}

bool WidgetResizeHandler::getResizeEnabled() const
{
	return m_activeForResize;
}

void WidgetResizeHandler::setActive(Action ac, bool b)
{
	if (ac & Move)
		m_activeForMove = b;
	if (ac & Resize)
		m_activeForResize = b;

	if (!isActive())
		setMouseCursor(Nowhere);
}

bool WidgetResizeHandler::isActive(Action ac) const
{
	bool b = false;
	if (ac & Move) b = m_activeForMove;
	if (ac & Resize) b |= m_activeForResize;

	return b;
}

bool WidgetResizeHandler::eventFilter(QObject *o, QEvent *ee)
{
	if (o == m_widget)
	{
		qDebug() << ee;
	}
	if (!isActive()
		|| (ee->type() != QEvent::MouseButtonPress
			&& ee->type() != QEvent::MouseButtonRelease
			&& ee->type() != QEvent::MouseMove
			&& ee->type() != QEvent::KeyPress
			&& ee->type() != QEvent::ShortcutOverride
			&& ee->type() != QEvent::MouseButtonDblClick
			&& ee->type() != QEvent::Resize)
		)
		return false;

	Q_ASSERT(o == m_widget);
	QWidget *w = m_widget;
	if (QApplication::activePopupWidget())
	{
		if (m_buttonDown && ee->type() == QEvent::MouseButtonRelease)
			m_buttonDown = false;
		return false;
	}

	switch (ee->type())
	{
	case QEvent::MouseButtonPress:
	{
		QMouseEvent *e = static_cast<QMouseEvent *>(ee);
		if (w->isMaximized())
			break;
		const QRect widgetRect = m_widget->rect().marginsAdded(QMargins(m_range, m_range, m_range, m_range));
		const QPoint cursorPoint = m_widget->mapFromGlobal(e->globalPos());
		if (!widgetRect.contains(cursorPoint) || m_mode == Nowhere)
			return false;
		if (e->button() == Qt::LeftButton)
		{
#if 0 // Used to be included in Qt4 for Q_WS_X11
			/*
				Implicit grabs do not stop the X server from changing
				the cursor in children, which looks *really* bad when
				doing resizingk, so we grab the cursor. Note that we do
				not do this on Windows since double clicks are lost due
				to the grab (see change 198463).
			*/
			if (e->spontaneous())
#  if !defined(QT_NO_CURSOR)
				widget->grabMouse(widget->cursor());
#  else
				widget->grabMouse();
#  endif // QT_NO_CURSOR
#endif
			m_buttonDown = false;
			emit activate();
			bool me = m_movingEnabled;
			m_movingEnabled = (me && o == m_widget);
			mouseMoveEvent(e);
			m_movingEnabled = me;
			m_buttonDown = true;
			m_moveOffset = m_widget->mapFromGlobal(e->globalPos());
			m_invertedMoveOffset = m_widget->rect().bottomRight() - m_moveOffset;
			if (m_mode == Center)
			{
				if (m_movingEnabled)
					return true;
			}
			else
			{
				return true;
			}
		}
	}
	break;
	case QEvent::MouseButtonRelease:
		if (w->isMaximized())
			break;
		if (w->pos().ry() <= 0)
		{
			w->move(QPoint(w->pos().rx(), 0));
		}
		if (static_cast<QMouseEvent *>(ee)->button() == Qt::LeftButton)
		{
			m_moveResizeMode = false;
			m_buttonDown = false;
			m_widget->releaseMouse();
			m_widget->releaseKeyboard();
			if (m_mode == Center)
			{
				if (m_movingEnabled)
					return true;
			}
			else
			{
				return true;
			}
		}
		break;
	case QEvent::MouseMove:
	{
		if (w->isMaximized())
		{
			break;
		}
		QMouseEvent *e = static_cast<QMouseEvent *>(ee);
		m_buttonDown = m_buttonDown && (e->buttons() & Qt::LeftButton); // safety, state machine broken!
		bool me = m_movingEnabled;
		m_movingEnabled = (me && o == m_widget && (m_buttonDown || m_moveResizeMode));
		mouseMoveEvent(e);
		m_movingEnabled = me;
		if (m_mode == Center)
		{
			if (m_movingEnabled)
				return true;
		}
		else
		{
			return true;
		}
	}
	break;
	case QEvent::KeyPress:
		keyPressEvent(static_cast<QKeyEvent *>(ee));
		break;
	case QEvent::ShortcutOverride:
		if (m_buttonDown)
		{
			ee->accept();
			return true;
		}
		break;
	case QEvent::MouseButtonDblClick:
	{
		QMouseEvent *e = static_cast<QMouseEvent *>(ee);
		if (e && e->pos().y() <= m_titleBarHeight)
		{
			if (w->isMaximized())
			{
				w->showNormal();
			}
			else
			{
				w->showMaximized();
			}
		}
		break;
	}
	case QEvent::Resize:
	{
		m_titleBar->resize(m_widget->width(), m_titleBar->height());
	}
	default:
		break;
	}

	return false;
}

void WidgetResizeHandler::mouseMoveEvent(QMouseEvent *e)
{
	QPoint pos = m_widget->mapFromGlobal(e->globalPos());
	if (!m_moveResizeMode && !m_buttonDown)
	{
		if (pos.y() <= m_range && pos.x() <= m_range)
			m_mode = TopLeft;
		else if (pos.y() >= m_widget->height() - m_range && pos.x() >= m_widget->width() - m_range)
			m_mode = BottomRight;
		else if (pos.y() >= m_widget->height() - m_range && pos.x() <= m_range)
			m_mode = BottomLeft;
		else if (pos.y() <= m_range && pos.x() >= m_widget->width() - m_range)
			m_mode = TopRight;
		else if (pos.y() <= m_range)
			m_mode = Top;
		else if (pos.y() >= m_widget->height() - m_range)
			m_mode = Bottom;
		else if (pos.x() <= m_range)
			m_mode = Left;
		else if (pos.x() >= m_widget->width() - m_range)
			m_mode = Right;
		else if (m_widget->rect().contains(pos))
			m_mode = Center;
		else
			m_mode = Nowhere;

		if (m_widget->isMinimized() || !isActive(Resize))
			m_mode = Center;
#ifndef QT_NO_CURSOR
		setMouseCursor(m_mode);
#endif
		return;
	}

	if (m_mode == Center && !m_movingEnabled)
		return;

	if (m_widget->testAttribute(Qt::WA_WState_ConfigPending))
		return;


	QPoint globalPos = (!m_widget->isWindow() && m_widget->parentWidget()) ?
		m_widget->parentWidget()->mapFromGlobal(e->globalPos()) : e->globalPos();
	if (!m_widget->isWindow() && !m_widget->parentWidget()->rect().contains(globalPos))
	{
		if (globalPos.x() < 0)
			globalPos.rx() = 0;
		if (globalPos.y() < 0)
			globalPos.ry() = 0;
		if (m_sizeprotect && globalPos.x() > m_widget->parentWidget()->width())
			globalPos.rx() = m_widget->parentWidget()->width();
		if (m_sizeprotect && globalPos.y() > m_widget->parentWidget()->height())
			globalPos.ry() = m_widget->parentWidget()->height();
	}

	QPoint p = globalPos + m_invertedMoveOffset;
	QPoint pp = globalPos - m_moveOffset;

	// Workaround for window managers which refuse to move a tool window partially offscreen.
	if (QGuiApplication::platformName() == QLatin1String("xcb"))
	{
		const QRect desktop = qApp->desktop()->availableGeometry(m_widget);
		pp.rx() = qMax(pp.x(), desktop.left());
		pp.ry() = qMax(pp.y(), desktop.top());
		p.rx() = qMin(p.x(), desktop.right());
		p.ry() = qMin(p.y(), desktop.bottom());
	}

	QSize ms = qSmartMinSize(m_childWidget->sizeHint(), m_childWidget->minimumSizeHint(),
		m_childWidget->minimumSize(), m_childWidget->maximumSize(),
		m_childWidget->sizePolicy());
	int mw = ms.width();
	int mh = ms.height();
	if (m_childWidget != m_widget)
	{
		mw += 2 * m_fw;
		mh += 2 * m_fw + m_extrahei;
	}

	QSize maxsize(m_childWidget->maximumSize());
	if (m_childWidget != m_widget)
		maxsize += QSize(2 * m_fw, 2 * m_fw + m_extrahei);
	QSize mpsize(m_widget->geometry().right() - pp.x() + 1,
		m_widget->geometry().bottom() - pp.y() + 1);
	mpsize = mpsize.expandedTo(m_widget->minimumSize()).expandedTo(QSize(mw, mh))
		.boundedTo(maxsize);
	QPoint mp(m_widget->geometry().right() - mpsize.width() + 1,
		m_widget->geometry().bottom() - mpsize.height() + 1);

	QRect geom = m_widget->geometry();

	switch (m_mode)
	{
	case TopLeft:
		geom = QRect(mp, m_widget->geometry().bottomRight());
		break;
	case BottomRight:
		geom = QRect(m_widget->geometry().topLeft(), p);
		break;
	case BottomLeft:
		geom = QRect(QPoint(mp.x(), m_widget->geometry().y()), QPoint(m_widget->geometry().right(), p.y()));
		break;
	case TopRight:
		geom = QRect(QPoint(m_widget->geometry().x(), mp.y()), QPoint(p.x(), m_widget->geometry().bottom()));
		break;
	case Top:
		geom = QRect(QPoint(m_widget->geometry().left(), mp.y()), m_widget->geometry().bottomRight());
		break;
	case Bottom:
		geom = QRect(m_widget->geometry().topLeft(), QPoint(m_widget->geometry().right(), p.y()));
		break;
	case Left:
		geom = QRect(QPoint(mp.x(), m_widget->geometry().top()), m_widget->geometry().bottomRight());
		break;
	case Right:
		geom = QRect(m_widget->geometry().topLeft(), QPoint(p.x(), m_widget->geometry().bottom()));
		break;
	case Center:
		geom.moveTopLeft(pp);
		break;
	default:
		break;
	}

	geom = QRect(geom.topLeft(),
		geom.size().expandedTo(m_widget->minimumSize())
		.expandedTo(QSize(mw, mh))
		.boundedTo(maxsize));

	if (geom != m_widget->geometry() &&
		(m_widget->isWindow() || m_widget->parentWidget()->rect().intersects(geom)))
	{
		if (m_mode == Center)
		{
			m_widget->move(geom.topLeft());
		}
		else
			m_widget->setGeometry(geom);
	}
}

void WidgetResizeHandler::setMouseCursor(MousePosition m)
{
#ifdef QT_NO_CURSOR
	Q_UNUSED(m);
#else
	QObjectList children = m_widget->children();
	for (int i = 0; i < children.size(); ++i)
	{
		if (QWidget *w = qobject_cast<QWidget*>(children.at(i)))
		{
			if (!w->testAttribute(Qt::WA_SetCursor))
			{
				w->setCursor(Qt::ArrowCursor);
			}
		}
	}

	switch (m)
	{
	case TopLeft:
	case BottomRight:
		m_widget->setCursor(Qt::SizeFDiagCursor);
		break;
	case BottomLeft:
	case TopRight:
		m_widget->setCursor(Qt::SizeBDiagCursor);
		break;
	case Top:
	case Bottom:
		m_widget->setCursor(Qt::SizeVerCursor);
		break;
	case Left:
	case Right:
		m_widget->setCursor(Qt::SizeHorCursor);
		break;
	default:
		m_widget->setCursor(Qt::ArrowCursor);
		break;
	}
#endif // QT_NO_CURSOR
}

void WidgetResizeHandler::keyPressEvent(QKeyEvent * e)
{
	if (!isMove() && !isResize())
		return;
	bool is_control = e->modifiers() & Qt::ControlModifier;
	int delta = is_control ? 1 : 8;
	QPoint pos = QCursor::pos();
	switch (e->key())
	{
	case Qt::Key_Left:
		pos.rx() -= delta;
		if (pos.x() <= qApp->desktop()->geometry().left())
		{
			if (m_mode == TopLeft || m_mode == BottomLeft)
			{
				m_moveOffset.rx() += delta;
				m_invertedMoveOffset.rx() += delta;
			}
			else
			{
				m_moveOffset.rx() -= delta;
				m_invertedMoveOffset.rx() -= delta;
			}
		}
		if (isResize() && !resizeHorizontalDirectionFixed)
		{
			resizeHorizontalDirectionFixed = true;
			if (m_mode == BottomRight)
				m_mode = BottomLeft;
			else if (m_mode == TopRight)
				m_mode = TopLeft;
#ifndef QT_NO_CURSOR
			setMouseCursor(m_mode);
			m_widget->grabMouse(m_widget->cursor());
#else
			widget->grabMouse();
#endif
		}
		break;
	case Qt::Key_Right:
		pos.rx() += delta;
		if (pos.x() >= qApp->desktop()->geometry().right())
		{
			if (m_mode == TopRight || m_mode == BottomRight)
			{
				m_moveOffset.rx() += delta;
				m_invertedMoveOffset.rx() += delta;
			}
			else
			{
				m_moveOffset.rx() -= delta;
				m_invertedMoveOffset.rx() -= delta;
			}
		}
		if (isResize() && !resizeHorizontalDirectionFixed)
		{
			resizeHorizontalDirectionFixed = true;
			if (m_mode == BottomLeft)
				m_mode = BottomRight;
			else if (m_mode == TopLeft)
				m_mode = TopRight;
#ifndef QT_NO_CURSOR
			setMouseCursor(m_mode);
			m_widget->grabMouse(m_widget->cursor());
#else
			widget->grabMouse();
#endif
		}
		break;
	case Qt::Key_Up:
		pos.ry() -= delta;
		if (pos.y() <= qApp->desktop()->geometry().top())
		{
			if (m_mode == TopLeft || m_mode == TopRight)
			{
				m_moveOffset.ry() += delta;
				m_invertedMoveOffset.ry() += delta;
			}
			else
			{
				m_moveOffset.ry() -= delta;
				m_invertedMoveOffset.ry() -= delta;
			}
		}
		if (isResize() && !resizeVerticalDirectionFixed)
		{
			resizeVerticalDirectionFixed = true;
			if (m_mode == BottomLeft)
				m_mode = TopLeft;
			else if (m_mode == BottomRight)
				m_mode = TopRight;
#ifndef QT_NO_CURSOR
			setMouseCursor(m_mode);
			m_widget->grabMouse(m_widget->cursor());
#else
			widget->grabMouse();
#endif
		}
		break;
	case Qt::Key_Down:
		pos.ry() += delta;
		if (pos.y() >= qApp->desktop()->geometry().bottom())
		{
			if (m_mode == BottomLeft || m_mode == BottomRight)
			{
				m_moveOffset.ry() += delta;
				m_invertedMoveOffset.ry() += delta;
			}
			else
			{
				m_moveOffset.ry() -= delta;
				m_invertedMoveOffset.ry() -= delta;
			}
		}
		if (isResize() && !resizeVerticalDirectionFixed)
		{
			resizeVerticalDirectionFixed = true;
			if (m_mode == TopLeft)
				m_mode = BottomLeft;
			else if (m_mode == TopRight)
				m_mode = BottomRight;
#ifndef QT_NO_CURSOR
			setMouseCursor(m_mode);
			m_widget->grabMouse(m_widget->cursor());
#else
			widget->grabMouse();
#endif
		}
		break;
	case Qt::Key_Space:
	case Qt::Key_Return:
	case Qt::Key_Enter:
	case Qt::Key_Escape:
		m_moveResizeMode = false;
		m_widget->releaseMouse();
		m_widget->releaseKeyboard();
		m_buttonDown = false;
		break;
	default:
		return;
	}
	QCursor::setPos(pos);
}

QSize WidgetResizeHandler::qSmartMinSize(const QSize &sizeHint, const QSize &minSizeHint, const QSize &minSize, const QSize &maxSize, const QSizePolicy &sizePolicy)
{
	QSize s(0, 0);

	if (sizePolicy.horizontalPolicy() != QSizePolicy::Ignored)
	{
		if (sizePolicy.horizontalPolicy() & QSizePolicy::ShrinkFlag)
			s.setWidth(minSizeHint.width());
		else
			s.setWidth(qMax(sizeHint.width(), minSizeHint.width()));
	}

	if (sizePolicy.verticalPolicy() != QSizePolicy::Ignored)
	{
		if (sizePolicy.verticalPolicy() & QSizePolicy::ShrinkFlag)
		{
			s.setHeight(minSizeHint.height());
		}
		else
		{
			s.setHeight(qMax(sizeHint.height(), minSizeHint.height()));
		}
	}

	s = s.boundedTo(maxSize);
	if (minSize.width() > 0)
		s.setWidth(minSize.width());
	if (minSize.height() > 0)
		s.setHeight(minSize.height());

	return s.expandedTo(QSize(0, 0));
}

void WidgetResizeHandler::doResize()
{
	if (!m_activeForResize)
		return;

	m_moveResizeMode = true;
	m_moveOffset = m_widget->mapFromGlobal(QCursor::pos());
	if (m_moveOffset.x() < m_widget->width() / 2)
	{
		if (m_moveOffset.y() < m_widget->height() / 2)
			m_mode = TopLeft;
		else
			m_mode = BottomLeft;
	}
	else
	{
		if (m_moveOffset.y() < m_widget->height() / 2)
			m_mode = TopRight;
		else
			m_mode = BottomRight;
	}
	m_invertedMoveOffset = m_widget->rect().bottomRight() - m_moveOffset;
#ifndef QT_NO_CURSOR
	setMouseCursor(m_mode);
	m_widget->grabMouse(m_widget->cursor());
#else
	widget->grabMouse();
#endif
	m_widget->grabKeyboard();
	resizeHorizontalDirectionFixed = false;
	resizeVerticalDirectionFixed = false;
}

void WidgetResizeHandler::doMove()
{
	if (!m_activeForMove)
		return;

	m_mode = Center;
	m_moveResizeMode = true;
	m_moveOffset = m_widget->mapFromGlobal(QCursor::pos());
	m_invertedMoveOffset = m_widget->rect().bottomRight() - m_moveOffset;
#ifndef QT_NO_CURSOR
	m_widget->grabMouse(Qt::SizeAllCursor);
#else
	widget->grabMouse();
#endif
	m_widget->grabKeyboard();
}
