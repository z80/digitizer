
#include "help_browser.h"
#include <QHelpEngineCore>
#include <QDebug>

HelpBrowser::HelpBrowser( QWidget * parent )
    : QTextBrowser( parent )
{
    m_core = 0;
}

HelpBrowser::~HelpBrowser()
{
}

void HelpBrowser::setHelpEngine( QHelpEngineCore * engine )
{
    m_core = engine;
}

QVariant HelpBrowser::loadResource( int type,
                                     const QUrl & url )
{
    qDebug() << type << ", " << url;
    if (url.scheme() == "qthelp")
    {
        QByteArray ba = m_core->fileData(url);
        qDebug() << "file size: " << ba.size();

        return QVariant( ba );
    }
    else
        return QTextBrowser::loadResource(type, url);
}
