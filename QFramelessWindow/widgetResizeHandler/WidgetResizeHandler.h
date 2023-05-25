#pragma once
#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>

class TitleBar;
class WidgetResizeHandler : public QObject
{
	Q_OBJECT
public:
	enum Action
	{
		Move = 0x01,
		Resize = 0x02,
		Any = Move | Resize
	};
	explicit WidgetResizeHandler(QWidget *parent, QWidget *cw = 0);
	void setTitleBar(TitleBar *tBar);
	TitleBar *getTitleBar() const;

	void setResizeEnabled(bool isEnabled);
	bool getResizeEnabled() const;

	void setActive(bool b)
	{
		setActive(Any, b);
	}
	void setActive(Action ac, bool b);
	bool isActive() const
	{
		return isActive(Any);
	}
	bool isActive(Action ac) const;
	void setMovingEnabled(bool b)
	{
		m_movingEnabled = b;
	}
	bool isMovingEnabled() const
	{
		return m_movingEnabled;
	}

	bool isButtonDown() const
	{
		return m_buttonDown;
	}

	void setExtraHeight(int h)
	{
		m_extrahei = h;
	}
	void setSizeProtection(bool b)
	{
		m_sizeprotect = b;
	}

	void setFrameWidth(int w)
	{
		m_fw = w;
	}

	void setTitleBarHeight(int h)
	{
		m_titleBarHeight = h;
	}
	void doResize();
	void doMove();

Q_SIGNALS:
	void activate();

protected:
	bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);

private:
	Q_DISABLE_COPY(WidgetResizeHandler)
		QSize qSmartMinSize(const QSize &sizeHint, const QSize &minSizeHint,
			const QSize &minSize, const QSize &maxSize,
			const QSizePolicy &sizePolicy);
	enum MousePosition
	{
		Nowhere,
		TopLeft, BottomRight, BottomLeft, TopRight,
		Top, Bottom, Left, Right,
		Center
	};

	QWidget *m_widget;
	QWidget *m_childWidget;
	QPoint m_moveOffset;
	QPoint m_invertedMoveOffset;
	MousePosition m_mode;
	int m_fw;
	int m_extrahei;
	int m_range;
	uint m_buttonDown = 1;
	uint m_moveResizeMode = 1;
	uint m_activeForResize = 1;
	uint m_sizeprotect = 1;
	uint m_movingEnabled = 1;
	uint m_activeForMove = 1;
	uint m_titleBarHeight = 0;
	TitleBar *m_titleBar;

	void setMouseCursor(MousePosition m);
	bool isMove() const
	{
		return m_moveResizeMode && m_mode == Center;
	}
	bool isResize() const
	{
		return m_moveResizeMode && !isMove();
	}
	
};

