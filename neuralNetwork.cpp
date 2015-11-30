#include "neuralnetwork.h"
#include <time.h>
#include <stdlib.h>
#include <fstream>

double tanh(double s){
    double res = 1.0 + s + 0.5*s*s;
    return (res - 1.0/res)/(res + 1.0/res);
}

double theta(double s){
    double res = 1 / ( 1.0 + s + 0.5*s*s);
    return 1/(1 + res);
}

double d_tanh(double s){
    double res = 1 + 2.0*s + 2.0*s*s;
    return 4*res/(1+res)/(1+res);
}

double d_theta(double s){
    double res = 1 / ( 1.0 + s + 0.5*s*s);
    return res/(1+res)/(1+res);
}

int neuralNetwork::setLayer(size_type layNum){
    int curNum = 0;
    if(layNum > 0){
        curNum = score.size();
        w.resize(layNum - 1);
        score.resize(layNum);
        bpdlt.resize(layNum - 1);
        w0.resize(layNum - 1);
    }
    return curNum;
}

void neuralNetwork::setNodeNum(size_type lay, size_type Num){
    if(lay <= score.size() && lay){
        score.at(lay - 1).resize(Num);
        if(lay - 1 != 0){
            bpdlt.at(lay - 2).resize(Num);
            w.at(lay - 2).resize(Num);
            w0.at(lay - 2).resize(Num);
            int size = score.at(lay - 2).size();
            for(size_type i(0); i < Num; i++)
                w.at(lay - 2).at(i).resize(size);
        }
    }
}

void neuralNetwork::imput(const int *picture, int Size){
    picSize = Size;
    score.at(0).resize(Size);
    for(int i = 0; i < Size; i++)
        score.at(0).at(i) = static_cast<double>(picture[i]);
}

double neuralNetwork::get_res(){
    double num(0.0);
    for(int i(0); i < OUTPUTLAYER; i++)
        num += x.at(i) / 2.0 + 0.45;
    return num + 0.5;
}

void neuralNetwork::output(char &number){
    run();
    number = static_cast<int>(get_res()) + '\0';
}


void neuralNetwork::setTransformationFunction(neuralNetwork::transFunctionType type){
    switch (type) {
    case tanh_s:
        transformationFun = tanh;
        d_fun = d_tanh;
        break;
    case theta_s:
        transformationFun = theta;
        d_fun = d_theta;
        break;
    case none_s:
        transformationFun = nullptr;
        d_fun =nullptr;
    default:
        break;
    }
}


void neuralNetwork::run(){
    bool iscopy = false;
    size_type i;
    for(i = 0; i < w.size(); i++){
        iscopy = false;
        for(size_type j = 0; j < w.at(i).size(); j++){
            for(size_type k = 0;k < w.at(i).at(j).size(); k++){
                if(0 == i)
                    score.at(i + 1).at(j) += + score.at(i).at(k) * w.at(i).at(j).at(k);
                else{
                    if(iscopy == false){
                        poolcpy(i);
                        iscopy = true;
                    }
                    score.at(i + 1).at(j) += x.at(k) * w.at(i).at(j).at(k);
                }
                score.at(i + 1).at(j) += w0.at(i).at(j);
            }
        }
    }
    poolcpy(i);
}

double neuralNetwork::getRand(){
    int _rand_1 = rand() % 200;
    int _rand_2 = rand() % 3;
    double res = (100.0 - static_cast<double>(_rand_1) )/ 200.0;
    switch (_rand_2) {
    case 0:
        break;
    case 1:
        res /= 10.0;
        break;
    case 2:
        res /= 100.0;
        break;
    default:
        break;
    }
    return res;

}

void neuralNetwork::initNetwork(){
    if(!w.empty()){
        srand((unsigned)time(0));
        size_type k = 0;
        size_type j = 0;
        for(size_type i = 0; i < w.size(); i++)
            for(j = 0; j < w.at(i).size(); j++){
                for(k = 0; k < w.at(i).at(j).size(); k++){
                    w.at(i).at(j).at(k) = getRand();
                }
            }
        for(size_type i = 0; i < w0.size(); i++)
            for(size_type j = 0; j < w0.at(i).size(); j++){
                w0.at(i).at(j) = getRand();
            }

    }
}

void neuralNetwork::train_importData(const char *fileName, int dataCnt){
    if(learncnt >= dataCnt)
        return;
    std::fstream file;
    file.seekp(learncnt * picSize);
    int trans(0);
    file.open(fileName, std::ios::in);
    if (file.is_open()) {
        for(int j = 0; j < picSize; j++){
            trans = file.get();
            score.at(0).at(j) = static_cast<double>(trans - 48);
        }
        trans = file.get();
        sign = static_cast<double>(trans - 48);

    }
    file.close();
    learncnt++;
}

void neuralNetwork::sigle_train(int dataCnt){
    while(learncnt < dataCnt){
        train_importData("/home/lancelot/my_pros/qtpros/number_identification/data.txt", dataCnt);
        initNetwork();
        run();
        double err = sign - get_res();

        /******** bp algorthim *********/
        double dlt_l = -2.0 * err;
        size_type L = bpdlt.size() - 1;
        int i(0);
        size_type j(0);
        size_type k(0);

        //!compute dlt
        for(i = 0; i < (int)bpdlt.at(L).size(); i++)
            bpdlt.at(L).at(i) = dlt_l * d_fun(score.at(L + 1).at(i));
        for(i = L - 1; i >= 0; i--){
            for(j = 0; j < bpdlt.at(i).size(); j++){
                for(k = 0; k < bpdlt.at(i + 1).size(); k++){
                    if(0 == k)
                        bpdlt.at(i).at(j) = bpdlt.at(i + 1).at(k) *
                                w.at(i + 1).at(k).at(j) *
                                d_fun(score.at(i + 1).at(j));
                    else
                        bpdlt.at(i).at(j) +=bpdlt.at(i + 1).at(k) *
                                w.at(i + 1).at(k).at(j) *
                                d_fun(score.at(i + 1).at(j));

                }
            }
        }

        //! update w
        for(i = 0; i < (int)w.size(); i++){
            for(j = 0; j < w.at(i).size(); j++){
                for(k = 0; k < w.at(i).at(j).size(); k++){
                    if(i == 0)
                        w.at(i).at(j).at(k) -= STUDYRATE *
                                score.at(i).at(j)* bpdlt.at(i).at(j);
                    else
                        w.at(i).at(j).at(k) -= STUDYRATE*
                                transformationFun(score.at(i - 1).at(j)) * bpdlt.at(i).at(j);
                }
            }
        }
    }
}

void neuralNetwork::train(int threadNum){
    sigle_train(10);
}

void neuralNetwork::poolcpy(size_type layer){
    if(0 < layer){
        for(size_type i(0); i < score.at(layer).size(); i++){
            x.at(i) = transformationFun(score.at(layer).at(i));
        }
    }
}

