#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    client w("localhost", 45454);
    w.show();

    return a.exec();
}
