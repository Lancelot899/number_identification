#include "window.h"
#include <fstream>

Window::Window(QWidget *parent):QWidget(parent),number(-1), netWork(MAXNETNODENUM){
    _initwindow();
      QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(show_res()));
}

void Window::show_res(){
    QString str;
    int copyCont = 0;
    for(int i = 5, irow = 0; i < __windowSize::imputwindowWidth; i = i + 12, irow++)
        for(int j = 5, icolumn = 0; j < __windowSize::imputwindowHeight; j = j + 12, icolumn++)
            if(imputWindow->getImage().pixel(i, j) != qRgb(255, 255, 255)){
                copyCont++;
                array[irow* COMPUTMATRIXLEN + icolumn] = 1;
            }

    if(0 == copyCont)
        return;

///*
    netWork.imput(array, COMPUTMATRIXLEN*COMPUTMATRIXLEN);
    netWork.output(number);

    if(number != -1){
        str.append(number + '0');
        outputWindow->setText(str);
        outputWindow->update();
    }

    // */

/*
    std::fstream file;
    file.open("/home/lancelot/my_pros/qtpros/number_identification/data.txt",std::ios::out);
    file.seekg(std::ios::end);
    for(int i = 0; i < 100; i++)
        file << array[i];
    file << 1;
    file.close();

    // */


    clear();
}

void Window::_initwindow(){
    memset(array, 0, sizeof(int)*COMPUTMATRIXLEN*COMPUTMATRIXLEN);
    this->setWindowTitle("Indentify");
    gVlayout = new QVBoxLayout(this);
  //  imputLabel = new QLabel("<h1>Please imput a number</h1>");
    imputWindow = new Paintwidge;
    outputLayout = new QHBoxLayout;
    outputWindow = new QLineEdit;
    pushButton = new QPushButton;
    outputLayout->addWidget(outputWindow, 9);
    outputLayout->addWidget(pushButton, 1);
  //  gVlayout->addWidget(imputLabel, 1);
    gVlayout->addWidget(imputWindow);
    gVlayout->addLayout(outputLayout, 1);
    this->setFixedSize(__windowSize::interfaceWidth, __windowSize::interfaceHeight);
    imputWindow->setImageSize(__windowSize::imputwindowWidth, __windowSize::imputwindowHeight);
    imputWindow->setFixedSize(__windowSize::imputwindowWidth+5, __windowSize::imputwindowHeight + 5);
    imputWindow->setImageColor(Qt::white);
    imputWindow->setPenWidth(10);
    imputWindow->setPenColor(Qt::red);
    imputWindow->setShape(Paintwidge::ShapeType::None);

    pushButton->setText("indentify");
    outputWindow->setReadOnly(true);

    //! initial netWork
    netWork.set_pictureSize(COMPUTMATRIXLEN*COMPUTMATRIXLEN);
    netWork.setLayer(3);
    netWork.setNodeNum(1, 100);
    netWork.setNodeNum(2, 30);
    netWork.setNodeNum(3, OUTPUTLAYER);
    netWork.setTransformationFunction(neuralNetwork::transFunctionType::tanh_s);
    netWork.train(TRAIN_THREAD_NUM);
}

void Window::clear(){
    imputWindow->doClear();
    memset(array, 0, sizeof(int) * COMPUTMATRIXLEN * COMPUTMATRIXLEN);
    number = -1;
}

