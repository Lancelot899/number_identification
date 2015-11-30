#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include<vector>
#include <math.h>

#define OUTPUTLAYER 10
#define STUDYRATE   0.5

enum _transFunctionType{
        tanh_s    =   0x0000,
        theta_s   =   0x0001,
        none_s   =    0xffff
};

//! transformation functions
double tanh(double s);
double theta(double);

//! differential of transformation functions
double d_tanh(double);
double d_theta(double);

typedef double (*_d2dfunc)(double);

class neuralNetwork{
public:
    typedef _transFunctionType                                   transFunctionType;
    typedef std::vector<std::vector<std::vector<double>>>        wight_type;
    typedef std::vector<std::vector<double>>                     score_type;
    typedef unsigned int                                         size_type;
    typedef const double*                                        const_pointer;
    typedef double*                                              pointer;
    typedef _d2dfunc                                             function_pointer;
    typedef std::vector<double>                                  resType;
    typedef score_type                                           bpDltType;
    typedef score_type                                           w0Type;

public:
    neuralNetwork(size_type Len):picSize(0),transformationFun(nullptr),
        d_fun(nullptr), sign(0.0), learncnt(0){
        x.assign(Len, 0);
    }

    ~neuralNetwork(){
    }

public:
    int setLayer(size_type layNum);
    void setNodeNum(size_type lay, size_type Num);
    void train(int threadNum);
    void imput(const int* picture, int Size);
    void output(char&number /*out*/);
    void setTransformationFunction(transFunctionType type);
    void set_pictureSize(int size){
        picSize = size;
    }

private:
    void initNetwork();
    void run();
    void train_importData(const char *fileName, int dataCnt);
    double get_res();
    void sigle_train(int dataCnt);
    void poolcpy(size_type layer);
    double getRand();

private:
    wight_type                      w;                      //! wight
    int                             picSize;
    score_type                      score;
    function_pointer                transformationFun;
    function_pointer                d_fun;
    resType                         x;
    double                          sign;
    bpDltType                       bpdlt;
    w0Type                          w0;
    int                             learncnt;
};



#endif // NEURALNETWORK_H
