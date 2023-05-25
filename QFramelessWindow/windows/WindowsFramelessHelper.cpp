#include "WindowsFramelessHelper.h"

#include <Windows.h>
#include <windowsx.h>
#include <QCoreApplication>
#include <QScreen>
#include <QOperatingSystemVersion>
#include <QGraphicsDropShadowEffect>
#include <QtWin>
#include <qdebug.h>

#include "../titlebar/TitleBar.h"

QScopedPointer<NativeWindowFilter> NativeWindowFilter::s_instance;

QHash<WindowsFramelessHelper *, WId> NativeWindowFilter::s_windows;
QHash<QWindow *, WId> NativeWindowFilter::s_winIds;
QHash<WId, WindowsFramelessHelper *> NativeWindowFilter::s_helpers;

void NativeWindowFilter::deliver(QWindow * window, WindowsFramelessHelper * helper)
{
	Q_CHECK_PTR(window);

	if (!s_instance) {
		QCoreApplication *appc = QCoreApplication::instance();
		if (appc) {
			auto filter = new NativeWindowFilter();
			appc->installNativeEventFilter(filter);
		}
	}

	if (helper) {
		WId newId = window->winId();
		WId oldId = s_windows.value(helper);
		if (newId != oldId) {
			s_helpers.insert(newId, helper);
			s_helpers.remove(oldId);
			s_windows.insert(helper, newId);

			s_winIds.insert(window, newId);
		}
	}
	else {
		WId oldId = s_winIds.take(window);
		WindowsFramelessHelper *helper = s_helpers.take(oldId);
		s_windows.remove(helper);
	}
}

bool NativeWindowFilter::nativeEventFilter(const QByteArray & eventType, void * message, long * result)
{
	LPMSG msg = reinterpret_cast<LPMSG>(message);
	if (auto h = s_helpers.value(reinterpret_cast<WId>(msg->hwnd)))
		return h->nativeEventFilter(eventType, msg, result);
	return false;
}

WindowsFramelessHelper::WindowsFramelessHelper(QWidget *widget)
	: QObject(widget)
{
	Q_CHECK_PTR(widget);
	m_widget = widget;

	if (QOperatingSystemVersion::current().majorVersion() != QOperatingSystemVersion::Windows7.majorVersion())
	{
		m_widget->setWindowFlags(m_widget->windowFlags() | Qt::FramelessWindowHint);
	}
	else if (m_widget->parentWidget())
	{
		m_widget->setWindowFlags(m_widget->parentWidget()->windowFlags() | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	}
	else
	{
		m_widget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	}

	m_widget->installEventFilter(this);

	m_titleBar = new TitleBar(m_widget);

	m_widget->resize(500, 500);
	m_titleBar->raise();
	m_titleBarHeight = m_titleBar->height();
}

WindowsFramelessHelper::~WindowsFramelessHelper()
{
	if (m_window)
		NativeWindowFilter::deliver(m_window, nullptr);
}

void WindowsFramelessHelper::setMaximizedMargins(int left, int top, int right, int bottom)
{
	m_maximizedMargins = QMargins(left, top, right, bottom);
}

void WindowsFramelessHelper::setMaximizedMargins(const QMargins & margins)
{
	m_maximizedMargins = margins;
}

void WindowsFramelessHelper::setDraggableMargins(int left, int top, int right, int bottom)
{
	m_draggableMargins = QMargins(left, top, right, bottom);
}

void WindowsFramelessHelper::setDraggableMargins(const QMargins & margins)
{
	m_draggableMargins = margins;
}

void WindowsFramelessHelper::setTitleBar(TitleBar * titleBar)
{
	if (titleBar != m_titleBar) {
		m_titleBar->deleteLater();
		m_titleBar = titleBar;
		m_titleBar->setParent(m_widget);
		m_titleBar->raise();

		m_titleBarHeight = m_titleBar->height();
	}
}

void WindowsFramelessHelper::setResizeEnabled(bool isEnabled)
{
	m_isResizeEnabled = isEnabled;
}

bool WindowsFramelessHelper::getResizeEnabled() const
{
	return m_isResizeEnabled;
}

bool WindowsFramelessHelper::nativeEventFilter(const QByteArray & eventType, void * msg, long * result)
{
	Q_CHECK_PTR(m_window);
	LPMSG lpMsg = reinterpret_cast<LPMSG>(msg);
	WPARAM wParam = lpMsg->wParam;
	LPARAM lParam = lpMsg->lParam;

	if (WM_NCHITTEST == lpMsg->message) {
		*result = hitTest(GET_X_LPARAM(lParam),
			GET_Y_LPARAM(lParam));
		return true;
	}
	else if (WM_NCACTIVATE == lpMsg->message) {
		if (!QtWin::isCompositionEnabled()) {
			if (result) *result = 1;
			return true;
		}
	}
	else if (WM_NCCALCSIZE == lpMsg->message) {
		if (TRUE == wParam) {
			if (isMaximized()) {
				NCCALCSIZE_PARAMS &params = *reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);

				QRect g = availableGeometry();
				QMargins m = maximizedMargins();

				params.rgrc[0].top = g.top() - m.top();
				params.rgrc[0].left = g.left() - m.left();
				params.rgrc[0].right = g.right() + m.right() + 1;
				params.rgrc[0].bottom = g.bottom() + m.bottom() + 1;
			}

			if (result) *result = 0;
			return true;
		}
	}
	else if (WM_GETMINMAXINFO == lpMsg->message) {
		LPMINMAXINFO lpMinMaxInfo = reinterpret_cast<LPMINMAXINFO>(lParam);

		QRect g = availableGeometry();
		QMargins m = maximizedMargins();

		lpMinMaxInfo->ptMaxPosition.x = -m.left();
		lpMinMaxInfo->ptMaxPosition.y = -m.top();
		lpMinMaxInfo->ptMaxSize.x = g.right() - g.left() + 1 + m.left() + m.right();
		lpMinMaxInfo->ptMaxSize.y = g.bottom() - g.top() + 1 + m.top() + m.bottom();

		lpMinMaxInfo->ptMinTrackSize.x = m_window->minimumWidth();
		lpMinMaxInfo->ptMinTrackSize.y = m_window->minimumHeight();
		lpMinMaxInfo->ptMaxTrackSize.x = m_window->maximumWidth();
		lpMinMaxInfo->ptMaxTrackSize.y = m_window->maximumHeight();

		if (result) *result = 0;
		return true;
	}
	else if (WM_NCLBUTTONDBLCLK == lpMsg->message) {
		auto minimumSize = m_window->minimumSize();
		auto maximumSize = m_window->maximumSize();
		if ((minimumSize.width() >= maximumSize.width())
			|| (minimumSize.height() >= maximumSize.height())) {
			if (result) *result = 0;
			return true;
		}
	}
	else if (WM_DPICHANGED == lpMsg->message) {
		qreal old = m_scaleFactor;
		if (HIWORD(wParam) < 144) {
			m_scaleFactor = 1.0;
		}
		else {
			m_scaleFactor = 2.0;
		}

		if (!qFuzzyCompare(old, m_scaleFactor)) {
			emit scaleFactorChanged(m_scaleFactor);
		}

		auto hWnd = reinterpret_cast<HWND>(m_window->winId());
		auto rect = reinterpret_cast<LPRECT>(lParam);
		SetWindowPos(hWnd,
			NULL,
			rect->left,
			rect->top,
			rect->right - rect->left,
			rect->bottom - rect->top,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	return false;
}

bool WindowsFramelessHelper::eventFilter(QObject * obj, QEvent * ev)
{
	if (obj == m_widget && ev->type() == QEvent::WinIdChange)
	{
		initWindow();
	}
	else if (obj == m_widget && ev->type() == QEvent::Resize)
	{
		m_titleBar->resize(m_widget->width(), m_titleBar->height());
		m_titleBar->raise();
	}

	if (m_window == obj && ev->type() == QEvent::WinIdChange) {
		updateWindowStyle();
	}
	else if (m_window == obj && ev->type() == QEvent::Show) {
		updateWindowStyle();
	}

	return QObject::eventFilter(obj, ev);
}

qreal WindowsFramelessHelper::scaleFactor() const
{
	return m_scaleFactor;
}

void WindowsFramelessHelper::initWindow()
{
	QWindow* window = m_widget->windowHandle();

	Q_CHECK_PTR(window);
	m_window = window;


	if (m_window) {
		m_scaleFactor = m_window->screen()->devicePixelRatio();

		if (m_window->flags() & Qt::FramelessWindowHint) {
			m_window->installEventFilter(this);
			updateWindowStyle();
		}
	}
}

void WindowsFramelessHelper::updateWindowStyle()
{
	Q_CHECK_PTR(m_window);

	HWND hWnd = reinterpret_cast<HWND>(m_window->winId());
	if (NULL == hWnd)
		return;
	else if (m_oldWindow == hWnd) {
		return;
	}
	m_oldWindow = hWnd;

	NativeWindowFilter::deliver(m_window, this);

	QOperatingSystemVersion currentVersion = QOperatingSystemVersion::current();
	if (currentVersion < QOperatingSystemVersion::Windows8) {
		return;
	}

	LONG oldStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME
		| WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;;
	LONG newStyle = WS_POPUP | WS_THICKFRAME;

	if (QtWin::isCompositionEnabled())
		newStyle |= WS_CAPTION;

	if (m_window->flags() & Qt::CustomizeWindowHint) {
		if (m_window->flags() & Qt::WindowSystemMenuHint)
			newStyle |= WS_SYSMENU;
		if (m_window->flags() & Qt::WindowMinimizeButtonHint)
			newStyle |= WS_MINIMIZEBOX;
		if (m_window->flags() & Qt::WindowMaximizeButtonHint)
			newStyle |= WS_MAXIMIZEBOX;
	}
	else {
		newStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	}

	LONG currentStyle = GetWindowLong(hWnd, GWL_STYLE);
	SetWindowLong(hWnd, GWL_STYLE, (currentStyle & ~oldStyle) | newStyle);

	SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
		SWP_NOOWNERZORDER | SWP_NOZORDER |
		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

	if (QtWin::isCompositionEnabled())
		QtWin::extendFrameIntoClientArea(m_window, 1, 1, 1, 1);
}

bool WindowsFramelessHelper::isMaximized() const
{
	Q_CHECK_PTR(m_window);

	HWND hWnd = reinterpret_cast<HWND>(m_window->winId());
	if (NULL == hWnd)
		return false;

	WINDOWPLACEMENT windowPlacement;
	if (!GetWindowPlacement(hWnd, &windowPlacement))
		return false;

	return (SW_MAXIMIZE == windowPlacement.showCmd);
}

QRect WindowsFramelessHelper::availableGeometry() const
{
	MONITORINFO mi{ 0 };
	mi.cbSize = sizeof(MONITORINFO);

	auto hWnd = reinterpret_cast<HWND>(m_window->winId());
	auto hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	if (!hMonitor || !GetMonitorInfoW(hMonitor, &mi)) {
		Q_ASSERT(NULL != hMonitor);
		return m_window->screen()->availableGeometry();
	}

	return QRect(mi.rcWork.left, mi.rcWork.top, mi.rcWork.right - mi.rcWork.left, mi.rcWork.bottom - mi.rcWork.top);
}

int WindowsFramelessHelper::hitTest(int x, int y) const
{
	Q_CHECK_PTR(m_window);

	x = x / m_window->devicePixelRatio();
	y = y / m_window->devicePixelRatio();

	enum RegionMask {
		Client = 0x0000,
		Top = 0x0001,
		Left = 0x0010,
		Right = 0x0100,
		Bottom = 0x1000,
	};

	auto wfg = m_window->frameGeometry();
	QMargins draggableMargins
		= this->draggableMargins();

	int top = draggableMargins.top();
	int left = draggableMargins.left();
	int right = draggableMargins.right();
	int bottom = draggableMargins.bottom();

	if (top <= 0)
		top = GetSystemMetrics(SM_CYFRAME);
	if (left <= 0)
		left = GetSystemMetrics(SM_CXFRAME);
	if (right <= 0)
		right = GetSystemMetrics(SM_CXFRAME);
	if (bottom <= 0)
		bottom = GetSystemMetrics(SM_CYFRAME);

	auto result =
		(Top *    (y < (wfg.top() + top))) |
		(Left *   (x < (wfg.left() + left))) |
		(Right *  (x > (wfg.right() - right))) |
		(Bottom * (y > (wfg.bottom() - bottom)));

	bool wResizable = m_isResizeEnabled ? m_window->minimumWidth() < m_window->maximumWidth() : false;
	bool hResizable = m_isResizeEnabled ? m_window->minimumHeight() < m_window->maximumHeight() : false;

	switch (result) {
		case Top | Left: return wResizable && hResizable ? HTTOPLEFT : HTCLIENT;
		case Top: return hResizable ? HTTOP : HTCLIENT;
		case Top | Right: return wResizable && hResizable ? HTTOPRIGHT : HTCLIENT;
		case Right: return wResizable ? HTRIGHT : HTCLIENT;
		case Bottom | Right: return wResizable && hResizable ? HTBOTTOMRIGHT : HTCLIENT;
		case Bottom: return hResizable ? HTBOTTOM : HTCLIENT;
		case Bottom | Left: return wResizable && hResizable ? HTBOTTOMLEFT : HTCLIENT;
		case Left: return wResizable ? HTLEFT : HTCLIENT;
	}

	auto pos = m_window->mapFromGlobal(QPoint(x, y));
	return HTCLIENT;
}
