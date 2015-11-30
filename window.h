#ifndef WINDOW_H
#define WINDOW_H

#include<QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLineEdit>
#include<QPushButton>
#include<QLabel>

#include"paintwidge.h"
#include "neuralnetwork.h"

#define COMPUTMATRIXLEN     10
#define MAXNETNODENUM       30
#define TRAIN_THREAD_NUM    4


enum __windowSize{
    interfaceWidth =  140,
    interfaceHeight = 180,
    imputwindowWidth= 120,
    imputwindowHeight = 120
};


class Window : public QWidget{
    Q_OBJECT
public:
    explicit Window(QWidget* parent = 0 );
    void clear();

signals:
    
public slots:
    void show_res();
   // void print();
   // void Clicked_change();
    
private:
    void _initwindow();
    
private:
  //  bool isClicked;
    char number;
    QVBoxLayout *gVlayout;
   // QLabel* imputLabel;
    Paintwidge* imputWindow;
    QHBoxLayout* outputLayout;
    QLineEdit* outputWindow;
    QPushButton* pushButton;
    int array[COMPUTMATRIXLEN * COMPUTMATRIXLEN];
    neuralNetwork netWork;
};

#endif // WINDOW_H
