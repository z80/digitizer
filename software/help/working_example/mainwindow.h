#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QHelpEngine>
#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include <QHelpSearchQueryWidget>
#include <QDebug>
#include <QDir>
#include "helpbrowser.h"
#include <QDockWidget>
#include <QSplitter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QTableWidget>
#include <QKeyEvent>
#include <QApplication>

class TextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void about();
    void insertItem();

private:
    void createActions();
    void createMenus();
    void createConnections();
    void createHelpWindow();

    QGraphicsView* view;
    QGraphicsScene* scene;

    QMenu *itemMenu;
    QAction *insertEllipseAction;
    QAction *insertRectangleAction;

    QMenu *helpMenu;
    QAction *helpAction;
    QAction *aboutAction;
    QDockWidget* helpWindow;
};

#endif
