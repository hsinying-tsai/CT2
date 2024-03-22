#include "widget.h"
#include<my_qlabel.h>
#include<QLayout>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
