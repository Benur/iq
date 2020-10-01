#include <iostream>
#include <vector>
#include <string>
#include <QApplication>
#include <QPushButton>
#include <dlib/algs.h>

using namespace dlib;
using namespace std;


int main(int argc, char **argv)
{
 QApplication app (argc, argv);

 QPushButton button1 ("test");
 QPushButton button2 ("other", &button1);
 unsigned long x = 9;
 cout << square_root(x) << endl;

 button1.show();

 return app.exec();
}