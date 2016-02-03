
#include <QtGui>
#include <QMainWindow>

class Ui_MainWindow;
class QHelpEngine;
class QHelpSearchEngine;

class HelpMainWnd: public QMainWindow
{
    Q_OBJECT
public:
    HelpMainWnd( const QString & fileName );
    ~HelpMainWnd();
protected slots:
    void print();
    void about();
    void setupFinished();
    void indexingStarted();
    void indexingFinished();
private:
    Ui_MainWindow * ui;
    
    QHelpEngine             * m_helpEngine;
    QHelpSearchEngine       * m_searchEngine;
};


