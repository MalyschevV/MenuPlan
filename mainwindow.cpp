#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("План питания");

    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(closeWidget()));

    connect(ui->pushButtonMo, &QPushButton::clicked, this, &MainWindow::ShowMenu);
    connect(ui->pushButtonTu, &QPushButton::clicked, this, &MainWindow::ShowMenu);
    connect(ui->pushButtonWe, &QPushButton::clicked, this, &MainWindow::ShowMenu);
    connect(ui->pushButtonTh, &QPushButton::clicked, this, &MainWindow::ShowMenu);
    connect(ui->pushButtonFr, &QPushButton::clicked, this, &MainWindow::ShowMenu);
    connect(ui->pushButtonSa, &QPushButton::clicked, this, &MainWindow::ShowMenu);
    connect(ui->pushButtonSu, &QPushButton::clicked, this, &MainWindow::ShowMenu);

    connect(ui->pushButtonMo, &QPushButton::clicked, this, &MainWindow::readBase);
    connect(ui->pushButtonTu, &QPushButton::clicked, this, &MainWindow::readBase);
    connect(ui->pushButtonWe, &QPushButton::clicked, this, &MainWindow::readBase);
    connect(ui->pushButtonTh, &QPushButton::clicked, this, &MainWindow::readBase);
    connect(ui->pushButtonFr, &QPushButton::clicked, this, &MainWindow::readBase);
    connect(ui->pushButtonSa, &QPushButton::clicked, this, &MainWindow::readBase);
    connect(ui->pushButtonSu, &QPushButton::clicked, this, &MainWindow::readBase);

    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(SaveMenu()));

    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(ShowHelp()));
    connect(ui->actionSaveDay, SIGNAL(triggered()), this, SLOT(Save()));
    connect(ui->actionSaveAll, SIGNAL(triggered()), this, SLOT(SaveAll()));

    connect(ui->pushButtonClearAll, SIGNAL(clicked()), this, SLOT(Clear()));
    connect(ui->pushButtonClearB, SIGNAL(clicked()), this, SLOT(Clear()));
    connect(ui->pushButtonClearD, SIGNAL(clicked()), this, SLOT(Clear()));
    connect(ui->pushButtonClearL, SIGNAL(clicked()), this, SLOT(Clear()));

    QShortcut *keyCtrlS = new QShortcut(this);
    keyCtrlS->setKey(Qt::CTRL + Qt::Key_S);
        connect(keyCtrlS, SIGNAL(activated()), this, SLOT(SaveMenu()));

    QShortcut *keyCtrlShiftS = new QShortcut(this);
    keyCtrlShiftS->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
        connect(keyCtrlShiftS, SIGNAL(activated()), this, SLOT(Save()));

    QShortcut *keyCtrlShiftA = new QShortcut(this);
    keyCtrlShiftA->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_A);
        connect(keyCtrlShiftA, SIGNAL(activated()), this, SLOT(SaveAll()));

    QShortcut *keyCtrlH = new QShortcut(this);
    keyCtrlH->setKey(Qt::CTRL+ Qt::Key_H);
        connect(keyCtrlH, SIGNAL(activated()), this, SLOT(ShowHelp()));

    hint();
    readBase();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hint(){
    srand(time(NULL));
    int num = rand() % 5 + 1;

    QString path = QDir::currentPath();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(path + "/assets/food.db");

    QSqlQuery query;
    QString s = QString::number(num);
    QString qry = "SELECT TEXT FROM HINT WHERE ID = " + s;

    qDebug() << qry;

    if (!db.open()){
        qDebug() << "base couldnt open";
    }

    if (!query.exec(qry)){
         qDebug() << "bad request for finding";
    }

    if (query.next()){
        ui->plainTextEdit->setPlainText(query.value(0).toString());
    }



}

void MainWindow::Clear(){

     auto sender = this->sender();

     if (sender == ui->pushButtonClearB){
        ui->plainTextEditBreakfast->clear();
     } else

         if (sender == ui->pushButtonClearL){
             ui->plainTextEditLunch->clear();

         } else
             if (sender == ui->pushButtonClearD){
                 ui->plainTextEditDinner->clear();

             } else
                 {
                     ui->plainTextEditBreakfast->clear();
                     ui->plainTextEditLunch->clear();
                     ui->plainTextEditDinner->clear();

                 }
}

void MainWindow::Save()
{
    QString filename = QFileDialog::getSaveFileName(0, "Сохранить файл",  QDir::currentPath(), "*.txt");

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << "Меню на " + ui->labelDay->text() + ":\n" +
                              "Завтрак:\n\t-" + ui->plainTextEditBreakfast->toPlainText() + "\n" +
                              "Обед:\n\t-" + ui->plainTextEditLunch->toPlainText() + "\n" +
                              "Ужин:\n\t-" + ui->plainTextEditDinner->toPlainText();
        file.close();

    }
}

void MainWindow::SaveAll()
{
    QString filename = QFileDialog::getSaveFileName(0, "Сохранить файл",  QDir::currentPath(), "*.txt");
    QString str;

    str="Меню на неделю:\n";
    QString day[7] = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
    QString dayEn[7] = {"MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY"};

    QString path = QDir::currentPath();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(path + "/assets/food.db");

    QSqlQuery query[7];

    if (!db.open()){
        qDebug() << "base couldnt open";
    }


    for (int i = 0; i < 7; i++){
        QString qry = "SELECT BREAKFAST, LUNCH, DINNER FROM " + dayEn[i];

        if (!query[i].exec(qry)){
             qDebug() << "bad request for finding";
        }

        qDebug() << qry;

        while (query[i].next()) {

            str+="\n" + day[i] + ":\n";
            str+="\tЗавтрак:\n\t\t-";
            str+=query[i].value(0).toString();
            str+="\n\tОбед:\n\t\t-";
            str+=query[i].value(1).toString();
            str+="\n\tУжин:\n\t\t-";
            str+=query[i].value(2).toString();

        }
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << str;
        file.close();

    }
}

void MainWindow::ShowHelp(){

    help *st = new help();
    st->show();
}

void MainWindow::SaveMenu(){

    QString path = QDir::currentPath();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(path + "/assets/food.db");

    QSqlQuery query;

    QString day;

    if (ui->labelDay->text()=="понедельник"){
        day = "MONDAY";
    } else
        if (ui->labelDay->text()=="вторник"){
            day = "TUESDAY";
        } else
            if (ui->labelDay->text()=="среда"){
                day = "WEDNESDAY";
            } else
                if (ui->labelDay->text()=="четверг"){
                    day = "THURSDAY";
                } else
                    if (ui->labelDay->text()=="пятница"){
                        day = "FRIDAY";
                    } else
                        if (ui->labelDay->text()=="суббота"){
                            day = "SATURDAY";
                        } else
                            {
                                day = "SUNDAY";
                            }


    QString qry = "REPLACE INTO " + day + " (ID, BREAKFAST, LUNCH, DINNER) VALUES (1, '" + ui->plainTextEditBreakfast->toPlainText() +
            "', '" +ui->plainTextEditLunch->toPlainText() + "', '" +ui->plainTextEditDinner->toPlainText() + "') ";

    if (!db.open()){
        qDebug() << "base couldnt open";
    }


    if (!query.exec(qry)){
         qDebug() << "bad request for finding";
    }

}

void MainWindow::readBase(){

    auto sender = this->sender();

    QString day;


         if (sender == ui->pushButtonTu){
             day = "TUESDAY";
         } else
             if (sender == ui->pushButtonWe){
                 day = "WEDNESDAY";
             } else
                 if (sender == ui->pushButtonTh){
                     day = "THURSDAY";
                 } else
                     if (sender == ui->pushButtonFr){
                         day = "FRIDAY";
                     } else
                         if (sender == ui->pushButtonSa){
                             day = "SATURDAY";
                         } else
                             if (sender == ui->pushButtonSu){

                                 day = "SUNDAY";
                                 } else{
                                      day = "MONDAY";
                                 }


    QString path = QDir::currentPath();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(path + "/assets/food.db");

    QSqlQuery query[3];

    if (ui->labelDay->text()=="понедельник"){
        day = "MONDAY";
    } else
        if (ui->labelDay->text()=="вторник"){
            day = "TUESDAY";
        } else
            if (ui->labelDay->text()=="среда"){
                day = "WEDNESDAY";
            } else
                if (ui->labelDay->text()=="четверг"){
                    day = "THURSDAY";
                } else
                    if (ui->labelDay->text()=="пятница"){
                        day = "FRIDAY";
                    } else
                        if (ui->labelDay->text()=="суббота"){
                            day = "SATURDAY";
                        } else
                            {
                                day = "SUNDAY";
                            }


    QString qry = "SELECT BREAKFAST FROM " + day;

    if (!db.open()){
        qDebug() << "base couldnt open";
    }


    if (!query[0].exec(qry)){
         qDebug() << "bad request for finding";
    }

    while (query[0].next()){
        ui->plainTextEditBreakfast->setPlainText(query[0].value(0).toString());
    }

    qry = "SELECT LUNCH FROM " + day;

    if (!query[1].exec(qry)){
         qDebug() << "bad request for finding";
    }

    while (query[1].next()){
        ui->plainTextEditLunch->setPlainText(query[1].value(0).toString());
    }

    qry = "SELECT DINNER FROM " + day;

    if (!query[2].exec(qry)){
         qDebug() << "bad request for finding";
    }

    while (query[2].next()){
        ui->plainTextEditDinner->setPlainText(query[2].value(0).toString());
    }

    this->update();

}


void MainWindow::closeWidget(){
    QWidget::close();
}

void MainWindow::ShowMenu(){

    auto sender = this->sender();

    if (sender == ui->pushButtonMo){
        ui->pushButtonTu->setChecked(false);
        ui->pushButtonWe->setChecked(false);
        ui->pushButtonTh->setChecked(false);
        ui->pushButtonFr->setChecked(false);
        ui->pushButtonSa->setChecked(false);
        ui->pushButtonSu->setChecked(false);

        ui->labelDay->setText("понедельник");
    } else
        if (sender == ui->pushButtonTu){
            ui->pushButtonMo->setChecked(false);
            ui->pushButtonWe->setChecked(false);
            ui->pushButtonTh->setChecked(false);
            ui->pushButtonFr->setChecked(false);
            ui->pushButtonSa->setChecked(false);
            ui->pushButtonSu->setChecked(false);

            ui->labelDay->setText("вторник");

        } else
            if (sender == ui->pushButtonWe){
                ui->pushButtonTu->setChecked(false);
                ui->pushButtonMo->setChecked(false);
                ui->pushButtonTh->setChecked(false);
                ui->pushButtonFr->setChecked(false);
                ui->pushButtonSa->setChecked(false);
                ui->pushButtonSu->setChecked(false);

                ui->labelDay->setText("среду");

            } else
                if (sender == ui->pushButtonTh){
                    ui->pushButtonTu->setChecked(false);
                    ui->pushButtonWe->setChecked(false);
                    ui->pushButtonMo->setChecked(false);
                    ui->pushButtonFr->setChecked(false);
                    ui->pushButtonSa->setChecked(false);
                    ui->pushButtonSu->setChecked(false);

                    ui->labelDay->setText("четверг");

                } else
                    if (sender == ui->pushButtonFr){
                        ui->pushButtonTu->setChecked(false);
                        ui->pushButtonWe->setChecked(false);
                        ui->pushButtonTh->setChecked(false);
                        ui->pushButtonMo->setChecked(false);
                        ui->pushButtonSa->setChecked(false);
                        ui->pushButtonSu->setChecked(false);

                        ui->labelDay->setText("пятницу");

                    } else
                        if (sender == ui->pushButtonSa){
                            ui->pushButtonTu->setChecked(false);
                            ui->pushButtonWe->setChecked(false);
                            ui->pushButtonTh->setChecked(false);
                            ui->pushButtonFr->setChecked(false);
                            ui->pushButtonMo->setChecked(false);
                            ui->pushButtonSu->setChecked(false);

                            ui->labelDay->setText("субботу");

                        } else
                        {

                                ui->pushButtonTu->setChecked(false);
                                ui->pushButtonWe->setChecked(false);
                                ui->pushButtonTh->setChecked(false);
                                ui->pushButtonFr->setChecked(false);
                                ui->pushButtonSa->setChecked(false);
                                ui->pushButtonMo->setChecked(false);

                                ui->labelDay->setText("воскресенье");

                            }

}
