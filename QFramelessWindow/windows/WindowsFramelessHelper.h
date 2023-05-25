#pragma once
#include <QAbstractNativeEventFilter>
#include <QWindow>

class WindowsFramelessHelper;
class TitleBar;
class NativeWindowFilter : public QAbstractNativeEventFilter
{
public:
	static void deliver(QWindow *window, WindowsFramelessHelper *helper);
protected:
	bool nativeEventFilter(const QByteArray &eventType,
		void *message, long *result) final;
private:
	static QScopedPointer<NativeWindowFilter> s_instance;
	static QHash<WindowsFramelessHelper *, WId> s_windows;
	static QHash<QWindow *, WId> s_winIds;
	static QHash<WId, WindowsFramelessHelper *> s_helpers;
};

class WindowsFramelessHelper : public QObject
{
	Q_OBJECT
public:
	explicit WindowsFramelessHelper(QWidget *widget);
	~WindowsFramelessHelper();

	void setMaximizedMargins(int left, int top, int right, int bottom);
	void setMaximizedMargins(const QMargins &margins);

	void setDraggableMargins(int left, int top, int right, int bottom);
	void setDraggableMargins(const QMargins &margins);

	void setTitleBar(TitleBar* titleBar);

	inline TitleBar* titleBar() const
	{
		return m_titleBar;
	}

	void setResizeEnabled(bool isEnabled);
	bool getResizeEnabled() const;

public:
    qreal scaleFactor() const;
    bool nativeEventFilter(const QByteArray & eventType, void *msg, long *result);

protected:
	bool eventFilter(QObject *obj, QEvent *ev) final;

signals:
	void scaleFactorChanged(qreal factor);
private:
	void initWindow();

	void updateWindowStyle();
	
	bool isMaximized() const;
	
	QRect availableGeometry() const;
	
	int hitTest(int x, int y) const;
		
	inline QMargins maximizedMargins() const
	{
		return m_maximizedMargins * m_scaleFactor;
	}
	
	inline QMargins draggableMargins() const
	{
		return m_draggableMargins * m_scaleFactor;
	}
private:
	bool m_isResizeEnabled = true;
	QWidget* m_widget = nullptr;
	QWindow* m_window = nullptr;
	qreal m_scaleFactor = 1.0;
	HWND m_oldWindow = NULL;
	QMargins m_maximizedMargins;
	QMargins m_draggableMargins;
	TitleBar* m_titleBar = nullptr;
	int m_titleBarHeight = 35;
};

