#include "mainwindow.h"

MainWindow::MainWindow()
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,400,200);
    view = new QGraphicsView(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing);
    setCentralWidget(view);

    createHelpWindow();
    createActions();
    createMenus();
    createConnections();

    setWindowTitle(tr("QGraphicsScene Help Example"));
    resize(600, 400);
}

void MainWindow::createHelpWindow(){
    QHelpEngine* helpEngine = new QHelpEngine(
                QApplication::applicationDirPath() +
                "/documentation/qgraphicshelpexample.qhc");
    helpEngine->setupData();
    QTabWidget* tWidget = new QTabWidget;
    tWidget->setMaximumWidth(200);
    tWidget->addTab(helpEngine->contentWidget(), "Contents");
    tWidget->addTab(helpEngine->indexWidget(), "Index");

    HelpBrowser *textViewer = new HelpBrowser(helpEngine);
    textViewer->setSource(
                QUrl("qthelp://walletfox.qt.helpexample/doc/index.html"));
    connect(helpEngine->contentWidget(),
            SIGNAL(linkActivated(QUrl)),
            textViewer, SLOT(setSource(QUrl)));

    connect(helpEngine->indexWidget(),
            SIGNAL(linkActivated(QUrl, QString)),
            textViewer, SLOT(setSource(QUrl)));

    QSplitter *horizSplitter = new QSplitter(Qt::Horizontal);
    horizSplitter->insertWidget(0, tWidget);
    horizSplitter->insertWidget(1, textViewer);

    helpWindow = new QDockWidget(tr("Help"), this);
    helpWindow->setWidget(horizSplitter);
    helpWindow->hide();
    addDockWidget(Qt::BottomDockWidgetArea, helpWindow);
}

void MainWindow::createActions()
{
    insertEllipseAction = new QAction(tr("Insert &Ellipse"), this);
    insertEllipseAction->setIcon(QIcon(":/icons/ellipse.png"));

    insertRectangleAction = new QAction(tr("Insert &Rectangle"), this);
    insertRectangleAction->setIcon(QIcon(":/icons/rectangle.png"));

    helpAction = new QAction(tr("Help Contents..."), this);
    helpAction->setShortcut(QKeySequence::HelpContents);

    aboutAction = new QAction(tr("&About"), this);
}

void MainWindow::createConnections(){
    connect(insertEllipseAction, SIGNAL(triggered()),
            this, SLOT(insertItem()));
    connect(insertRectangleAction, SIGNAL(triggered()),
            this, SLOT(insertItem()));
    connect(helpAction, SIGNAL(triggered()),
            helpWindow, SLOT(show()));
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));
}

void MainWindow::createMenus()
{
    itemMenu = new QMenu(tr("&Item"), this);
    itemMenu->addAction(insertEllipseAction);
    itemMenu->addAction(insertRectangleAction);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);

    menuBar()->addMenu(itemMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About QGraphicsScene Help Example"),
                       tr("This example demonstrates how to implement\n"
                          "help for a Qt application."));
}

void MainWindow::insertItem(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action){
        QGraphicsItem* itemToAdd = 0;
        QPen mPen(Qt::black, 3, Qt::SolidLine);
        QBrush eBrush(QColor("#FF7F50"));
        QBrush rBrush(QColor("#CC0000"));
        if(action->iconText() == "Insert Ellipse")
            itemToAdd = scene->addEllipse(0,0,150,75, mPen, eBrush);
        else if(action->iconText() == "Insert Rectangle")
            itemToAdd = scene->addRect(0,0,100,100, mPen, rBrush);
        if(itemToAdd)
            itemToAdd->setFlags(QGraphicsItem::ItemIsSelectable |
                               QGraphicsItem::ItemIsMovable);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete){
        foreach(QGraphicsItem* item, scene->selectedItems()){
            scene->removeItem(item);
            delete item;
        }
    }
    else
        QMainWindow::keyPressEvent(event);
}



