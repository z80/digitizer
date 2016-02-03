
#include <QTextBrowser>
class QHelpEngineCore;

class HelpBrowser: public QTextBrowser
{
public:
    HelpBrowser( QWidget * parent = 0 );
    ~HelpBrowser();

    void setHelpEngine( QHelpEngineCore * engine );

    QVariant loadResource( int type,
                           const QUrl & url );
private:
    QHelpEngineCore * m_core;
};
