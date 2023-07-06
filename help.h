#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include <QTextBrowser>
#include <qtextbrowser.h>
#include <QAbstractButton>
#include <QDebug>
#include <QDir>

namespace Ui {
class help;
}

class help : public QWidget
{
    Q_OBJECT

public:
    explicit help(QWidget *parent = 0);
    ~help();

private:
    Ui::help *ui;

private slots:
      void updateCaption();
      void closeW();
};

#endif // HELP_H
