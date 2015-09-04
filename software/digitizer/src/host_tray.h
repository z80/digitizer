
#ifndef __HOST_TRAY_H_
#define __HOST_TRAY_H_

#include <QtGui>
#include <QSystemTrayIcon>

class MainWnd;

class HostTray: public QSystemTrayIcon
{
    Q_OBJECT
public:
    HostTray( QObject * parent = 0 );
    ~HostTray();

public slots:
    void slotActivted( QSystemTrayIcon::ActivationReason reason );
    void slotShowMainWnd();
    void slotQuit();
private:

    QMenu * m_menu;
    QAction * m_actionShow;
    QAction * m_actionQuit;
    MainWnd * m_mainWnd;
};

#endif


