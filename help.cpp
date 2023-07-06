#include "help.h"
#include "ui_help.h"

help::help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);

    this->setWindowTitle("Справка");

    connect(ui->pushButtonAbout, &QPushButton::clicked, this, &help::updateCaption);
    connect(ui->pushButtonHelp, &QPushButton::clicked, this, &help::updateCaption);
    connect(ui->pushButtonHotKeys, &QPushButton::clicked, this, &help::updateCaption);

    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(closeW()));

    updateCaption();
}

help::~help()
{
    delete ui;
}

void help::closeW(){
    QWidget::close();
}

void help::updateCaption()
{
    QString text;

    auto sender = this->sender();

    ui->pushButtonAbout->setChecked(false);
    ui->pushButtonHelp->setChecked(false);
    ui->pushButtonHotKeys->setChecked(false);

    if (sender == ui->pushButtonHelp){
        text="help.txt";
        ui->pushButtonHelp->setChecked(true);
    }else
    if (sender == ui->pushButtonHotKeys){
        text="hotkeys.txt";
        ui->pushButtonHotKeys->setChecked(true);
    } else{
        text="about.txt";
        ui->pushButtonAbout->setChecked(true);
    }


    QString path = QDir::currentPath();

    QFile temp(path +"/assets/" + text);


    if(!temp.open(QIODevice::ReadWrite))
    {
        qDebug() << "Ошибка открытия";
    }
    QTextStream in(&temp);
    in.setCodec("UTF-8");
    QString fileText = in.readAll();


    ui->textBrowser->setHtml(fileText);

}
