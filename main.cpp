#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Repository * repo = new Repository();
    Widget w(repo);
    w.show();
    return a.exec();
}
