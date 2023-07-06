#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTime>

void LoadModules(QSplashScreen* psplash){
    QTime time;
    time.start();
    for(int i=0; i<100 ; ){
        if(time.elapsed() > 40){
            time.start();
            ++i;
        }
        psplash->showMessage("Loading modules: " + QString::number(i)+"%", Qt::AlignCenter | Qt::AlignBottom, Qt::cyan);
        qApp->processEvents();
    }
}

int main(int argc, char *argv[])
{
    QString path = QDir::currentPath();
    QApplication a(argc, argv);

    QSplashScreen splash(QPixmap(path + "/assets/food.jpg"));

    MainWindow w;

    splash.show();
    LoadModules(&splash);
    splash.finish(&w);

    w.show();

    return a.exec();
}
