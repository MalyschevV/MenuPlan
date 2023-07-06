#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QtSql>
#include <QPushButton>
#include <QTextDocumentWriter>
#include <stdlib.h>
#include <time.h>
#include "help.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:
    void closeWidget();
    void ShowHelp();
    void ShowMenu();
    void readBase();
    void SaveMenu();
    void Save();
    void SaveAll();
    void Clear();
    void hint();
};

#endif // MAINWINDOW_H
