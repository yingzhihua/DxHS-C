#include "datahandler.h"
#include "sqlitemgr.h"
#include <QDir>
#include <QDebug>
#include <QSqlRecord>
#include <QFile>
#include <QPdfWriter>
#include <QFont>
#include <QDateTime>

#define DH_VERSION "V1.0"
#define eps 1e-10
const static int BaseLineAccuracy = 10;
const static int BaseLineLeft = 8;
const static int BaseLineRight = 13;

static size_t baseDataIndexStart = 5;
static size_t baseDataCount = 10;
static size_t ReferenceValue = 1;

static QMap<int,int> PosId;
static QHash<int,float> PosIntercept;

static QHash<int,Vec2f> baseLine;
static QHash<int,vector<double>> posBG;
static QHash<int,double> bgMean;
static QHash<int,int> ItemCT;
static QHash<int,int> CurrItemCT;
static QHash<int,int> CT;
static double BGMax;
static QHash<int,vector<Vec6d>> rawData;

static QMap<int,QString> AssayItem;
static QMap<QString, QString> ItemName;
static QStringList Name;

static QHash<int, Vec2i> BL;
void DataHandler::setAssayItem(QMap<int, QString> assayItem){
    AssayItem = assayItem;
}

void DataHandler::setItemCT(QHash<int, int> ct){
    ItemCT = ct;
}

void DataHandler::setItemName(QMap<QString, QString> itemName){
    qDebug()<<"setItemName"<<itemName;
    ItemName = itemName;
    Name.clear();
    for(QMap<QString,QString>::iterator iter = ItemName.begin(); iter != ItemName.end(); iter++){
        if (!Name.contains(iter.value())){
            Name.push_back(iter.value());
            qDebug()<<"setItemName"<<iter.value();
        }
    }
}
/**********************************
 *      smooth
 **********************************/
#define FirstPos 2
static void RemoveNoise(QHash<int, vector<Point2d> > &posArr){
    foreach(int posIndex,posArr.keys())
    {
        QMap<int,int> Eval;
        int EvalMax = FirstPos;
        int EvalMean = 0;

        for(size_t i = FirstPos; i < posArr[posIndex].size()-1; i++)
        {
            Eval[i] = abs(2*posArr[posIndex][i].y - posArr[posIndex][i+1].y - posArr[posIndex][i-1].y);
            if (Eval[i] > Eval[EvalMax])
                EvalMax = i;
            EvalMean += Eval[i];
        }
        EvalMean /= Eval.size();
        EvalMean++;
        //qDebug()<<"RemoveNoise"<<posIndex<<"Max="<<EvalMax<<"Mean="<<EvalMean<<Eval;

        while((EvalMax<15&&Eval[EvalMax]>EvalMean)||Eval[EvalMax] > 2*EvalMean){
            posArr[posIndex][EvalMax].y = (posArr[posIndex][EvalMax-1].y+posArr[posIndex][EvalMax+1].y)/2;
            if (EvalMax == FirstPos)
                posArr[posIndex][FirstPos-1].y = posArr[posIndex][FirstPos].y*2-posArr[posIndex][FirstPos+1].y;
            if (EvalMax == posArr[posIndex].size()-2)
                posArr[posIndex][EvalMax+1].y = posArr[posIndex][EvalMax].y*2-posArr[posIndex][EvalMax-1].y;

            EvalMax=FirstPos;
            for(int i = FirstPos; i < posArr[posIndex].size()-1; i++)
            {
                Eval[i] = abs(2*posArr[posIndex][i].y - posArr[posIndex][i+1].y - posArr[posIndex][i-1].y);
                if (Eval[i] > Eval[EvalMax])
                    EvalMax = i;
            }
            //qDebug()<<"RemoveNoise"<<posIndex<<"Max="<<EvalMax<<"Mean="<<EvalMean<<Eval;
        }
    }
}

static void MoveMeanSmooth(QHash<int, vector<Point2d> > &posArr){
    foreach(int posIndex,posArr.keys())
    {
        unsigned long v1,v2,v3,v4,numm; //zdh add
        for (v1=1;v1<4;v1++)
        {
            for(int i = FirstPos+1; i < posArr[posIndex].size()-2; i++)
            {
                if ((posArr[posIndex][i-2].y<posArr[posIndex][i-1].y)&&
                        (posArr[posIndex][i-1].y<posArr[posIndex][i].y)&&
                        (posArr[posIndex][i].y<posArr[posIndex][i+1].y)&&
                        (posArr[posIndex][i+1].y<posArr[posIndex][i+2].y))
                    continue;
                if ((posArr[posIndex][i-2].y>posArr[posIndex][i-1].y)&&
                        (posArr[posIndex][i-1].y>posArr[posIndex][i].y)&&
                        (posArr[posIndex][i].y>posArr[posIndex][i+1].y)&&
                        (posArr[posIndex][i+1].y>posArr[posIndex][i+2].y))
                    continue;
                numm = abs(4*posArr[posIndex][i].y - posArr[posIndex][i+1].y - posArr[posIndex][i-1].y - posArr[posIndex][i+2].y - posArr[posIndex][i-2].y);
                if (((numm>2)&&(posArr[posIndex][i].y>=posArr[posIndex][i+1].y)&&(posArr[posIndex][i].y>posArr[posIndex][i-1].y))
                        ||((numm>2)&&(posArr[posIndex][i].y<=posArr[posIndex][i+1].y)&&(posArr[posIndex][i].y<posArr[posIndex][i-1].y)))
                {
                    posArr[posIndex][i].y=(posArr[posIndex][i+1].y + posArr[posIndex][i-1].y + posArr[posIndex][i+2].y + posArr[posIndex][i-2].y)/4;
                }
            }
        }
    }
}

static void MoveMeanSmoothEx(QHash<int, vector<Point2d> > &posArr){
    foreach(int posIndex,posArr.keys())
    {
        unsigned long v1,numm;
        /*
        //  GaussianBlur();
        //去噪点
        posArr[posIndex][1].y=posArr[posIndex][3].y;
        posArr[posIndex][2].y=posArr[posIndex][3].y;
        for (v1=1;v1<6;v1++)
        {
            for(int i = FirstPos+1; i < posArr[posIndex].size()-2; i++)
            {

                if ((posArr[posIndex][i-2].y<posArr[posIndex][i-1].y)&&
                        (posArr[posIndex][i-1].y<posArr[posIndex][i].y)&&
                        (posArr[posIndex][i].y<posArr[posIndex][i+1].y)&&
                        (posArr[posIndex][i+1].y<posArr[posIndex][i+2].y))
                    continue;
                if ((posArr[posIndex][i-2].y>posArr[posIndex][i-1].y)&&
                        (posArr[posIndex][i-1].y>posArr[posIndex][i].y)&&
                        (posArr[posIndex][i].y>posArr[posIndex][i+1].y)&&
                        (posArr[posIndex][i+1].y>posArr[posIndex][i+2].y))
                    continue;
                numm = abs(4*posArr[posIndex][i].y - posArr[posIndex][i+1].y - posArr[posIndex][i-1].y - posArr[posIndex][i+2].y - posArr[posIndex][i-2].y);
                if (((numm>3)&&(posArr[posIndex][i].y>=posArr[posIndex][i+1].y)&&(posArr[posIndex][i].y>posArr[posIndex][i-1].y))
                        ||((numm>3)&&(posArr[posIndex][i].y<=posArr[posIndex][i+1].y)&&(posArr[posIndex][i].y<posArr[posIndex][i-1].y)))
                {
                    posArr[posIndex][i].y=(posArr[posIndex][i+1].y + posArr[posIndex][i-1].y)/2;// + posArr[posIndex][i+2].y + posArr[posIndex][i-2].y)/4;
                }
            }

        }
*/

#if 0
        //5点3次线性平滑
        for (v1=1;v1<2;v1++)
        {

            if (posArr[posIndex].size() > 5)
            {
                posArr[posIndex][0].y = (69.0 * posArr[posIndex][0].y + 4.0 * posArr[posIndex][1].y - 6.0 * posArr[posIndex][2].y + 4.0 * posArr[posIndex][3].y - posArr[posIndex][4].y) / 70.0;
                posArr[posIndex][1].y = (2.0 * posArr[posIndex][0].y + 27.0 * posArr[posIndex][1].y + 12.0 * posArr[posIndex][2].y - 8.0 * posArr[posIndex][3].y + 2.0 * posArr[posIndex][4].y) / 35.0;
                for (int i = 2; i <= posArr[posIndex].size() - 3; i++)
                {
                    posArr[posIndex][i].y = (-3.0 * (posArr[posIndex][i-2].y + posArr[posIndex][i+2].y) + 12.0 * (posArr[posIndex][i-1].y + posArr[posIndex][i+1].y) + 17.0 * posArr[posIndex][i].y) / 35.0;
                }
            }
        }
#endif

        //7点2次线性平滑
#if  1
        for (v1=1;v1<2;v1++){
            int size_a=posArr[posIndex].size();
            if (posArr[posIndex].size() >= 7)
            {
                posArr[posIndex][0].y = (32.0 * posArr[posIndex][0].y + 15.0 * posArr[posIndex][1].y + 3.0 * posArr[posIndex][2].y - 4.0 * posArr[posIndex][3].y -
                        6.0 * posArr[posIndex][4].y - 3.0 * posArr[posIndex][5].y + 5.0 * posArr[posIndex][6].y) / 42.0;
                posArr[posIndex][1].y = (5.0 * posArr[posIndex][0].y + 4.0 * posArr[posIndex][1].y + 3.0 * posArr[posIndex][2].y + 2.0 * posArr[posIndex][3].y +
                        posArr[posIndex][4].y - posArr[posIndex][6].y) / 14.0;
                posArr[posIndex][2].y = (1.0 * posArr[posIndex][0].y + 3.0 * posArr[posIndex][1].y + 4.0 * posArr[posIndex][2].y + 4.0 * posArr[posIndex][3].y +
                        3.0 * posArr[posIndex][4].y + 1.0 * posArr[posIndex][5].y - 2.0 * posArr[posIndex][6].y) / 14.0;
                for (int i = 3; i <= size_a - 4; i++)
                {
                    posArr[posIndex][i].y = (-2.0 * ( posArr[posIndex][i-3].y +  posArr[posIndex][i+3].y) +
                            3.0 * ( posArr[posIndex][i-2].y +  posArr[posIndex][i+2].y) +
                            6.0 * ( posArr[posIndex][i-1].y +  posArr[posIndex][i+1].y) + 7.0 *  posArr[posIndex][i].y) / 21.0;
                }
                posArr[posIndex][size_a - 3].y = (1.0 * posArr[posIndex][size_a - 1].y + 3.0 * posArr[posIndex][size_a - 2].y + 4.0 * posArr[posIndex][size_a - 3].y +
                        4.0 * posArr[posIndex][size_a - 4].y + 3.0 * posArr[posIndex][size_a - 5].y + 1.0 * posArr[posIndex][size_a - 6].y - 2.0 * posArr[posIndex][size_a - 7].y) / 14.0;
                posArr[posIndex][size_a - 2].y = (5.0 * posArr[posIndex][size_a - 1].y + 4.0 * posArr[posIndex][size_a - 2].y + 3.0 * posArr[posIndex][size_a - 3].y +
                        2.0 * posArr[posIndex][size_a - 4].y + posArr[posIndex][size_a - 5].y - posArr[posIndex][size_a - 7].y) / 14.0;
                posArr[posIndex][size_a - 1].y = (32.0 * posArr[posIndex][size_a - 1].y + 15.0 * posArr[posIndex][size_a - 2].y + 3.0 * posArr[posIndex][size_a - 3].y -
                        4.0 * posArr[posIndex][size_a - 4].y - 6.0 * posArr[posIndex][size_a - 5].y - 3.0 * posArr[posIndex][size_a - 6].y + 5.0 * posArr[posIndex][size_a - 7].y) / 42.0;
            }
        }
#endif
    }
}

static double tricube(double x){
    double tmp = abs(x);
    tmp = 1 - (tmp*tmp*tmp);
    return tmp*tmp*tmp;
}

static vector<size_t> KNearestNeighbours(size_t k, size_t i, vector<double> &weight){
    vector<size_t> result;
    size_t endindex = weight.size() - 1;
    size_t left = i;
    size_t right = i;
    if (weight[i] > 0)
    {
        result.push_back(i);
        k--;
    }

    while(k>0){
        if (left > 0){
            left--;
            if (weight[left] > 0){
                result.push_back(left);
                k--;
            }
        }
        if (k > 0 && right < endindex){
            right++;
            if (weight[right] > 0){
                result.push_back(right);
                k--;
            }
        }
    }
    sort(result.begin(),result.end());
    return result;
}

static vector<double> BisquareWeights(vector<double> &r){
    vector<double> weight(r);
    sort(weight.begin(),weight.end());
    double mad = weight[weight.size()>>1];
    if (weight.size()%2 == 0)
    {
        mad += weight[(weight.size()>>1)-1];
        mad /= 2;
    }
    mad = mad*6;
    if (abs(mad) < eps)
        mad = eps;
    for (size_t i = 0; i < r.size(); i++){
        if (r[i] < mad){
            weight[i] = 1 - (r[i]*r[i]/mad/mad);
        }
        else {
            weight[i] = 0;
        }
    }
    return weight;
}

static vector<double> TricubeWeights(vector<size_t> &idx, size_t i){
    vector<double> weight;
    vector<size_t> d;
    size_t maxd = 0;
    for (vector<size_t>::iterator it = idx.begin(); it != idx.end();it++)
    {
        size_t ad = *it > i ? (*it - i) : (i - *it);
        d.push_back(ad);
        if (ad > maxd)
            maxd = ad;
    }
    if (maxd > 0){
        for (vector<size_t>::iterator it = d.begin(); it != d.end();it++){
            double dd = static_cast<double>(*it)/maxd;
            dd = dd*dd*dd;
            weight.push_back(pow(1-dd,1.5));
        }
    }
    return weight;
}

static void findLine(vector<Point2d> &pos, double &k, double &intercept){
    double mxy = 0;
    double mx = 0;
    double my = 0;
    double mx2 = 0;
    double n = static_cast<double>(pos.size());
    for (size_t i = 0; i < pos.size(); i++){
        mx += pos[i].x;
        my += pos[i].y;
        mxy += pos[i].x*pos[i].y;
        mx2 += pos[i].x*pos[i].x;
    }
    k = n*mx2 - mx*mx;
    if (abs(k) < eps){
        intercept = 0;
        return;
    }
    k = (n*mxy - mx*my)/k;
    intercept = (my-mx*k)/n;
}

static void LoessSmooth(double bandwidth, vector<double> xval, vector<double> yval, vector<double> &res){
    size_t n = xval.size();
    size_t bandwidthInPoints = static_cast<size_t>(bandwidth * n + 0.5);
    vector<double> residuals(n);
    vector<double> sortedResiduals(n);
    vector<double> robustnessWeights(n,1);
    int robustnessIters = 2;

    for (int iter = 0; iter <= robustnessIters; ++iter){
        size_t bandwidthInterval[2] = { 0, bandwidthInPoints - 1 };
        for (size_t i = 0; i < n; ++i){
            double x = xval[i];
            if (i > 0){
                if (bandwidthInterval[0] != 0 && xval[i] - xval[bandwidthInterval[0] - 1] < xval[bandwidthInterval[1]] - xval[i]){
                    bandwidthInterval[0]++;
                    bandwidthInterval[1]++;
                }
                else if(bandwidthInterval[1] < xval.size() - 1 && xval[bandwidthInterval[1] + 1] - xval[i] < xval[i] - xval[bandwidthInterval[0]]){
                    bandwidthInterval[0]++;
                    bandwidthInterval[1]++;
                }
            }

            size_t ileft = bandwidthInterval[0];
            size_t iright = bandwidthInterval[1];
            size_t edge;
            if (xval[i] - xval[ileft] > xval[iright]-xval[i])
                edge = ileft;
            else
                edge = iright;

            double sumWeights = 0;
            double sumX = 0, sumXSquared = 0, sumY = 0, sumXY = 0;
            double denom = abs(1.0/(xval[static_cast<size_t>(edge)] - x));
            for (size_t k = ileft; k <= iright; k++){
                double xk = xval[k];
                double yk = yval[k];
                double dist;
                if (k < i){
                    dist = (x - xk);
                }
                else {
                    dist = (xk - x);
                }

                double w = tricube(dist*denom)*robustnessWeights[k];
                double xkw = xk * w;
                sumWeights += w;
                sumX += xkw;
                sumXSquared += xk * xkw;
                sumY += yk * w;
                sumXY += yk * xkw;
            }

            double meanX = sumX / sumWeights;
            double meanY = sumY / sumWeights;
            double meanXY = sumXY /sumWeights;
            double meanXSquared = sumXSquared / sumWeights;

            double beta;
            if (meanXSquared == meanX * meanX){
                beta = 0;
            }
            else {
                beta = (meanXY - meanX * meanY) / (meanXSquared - meanX * meanX);
            }
            double alpha = meanY - beta * meanX;
            res[i] = beta * x + alpha;
            residuals[i] = abs(yval[i] - res[i]);
        }

        if (iter == robustnessIters)
            break;
        copy(residuals.begin(),residuals.end(),sortedResiduals.begin());
        sort(sortedResiduals.begin(),sortedResiduals.end());
        double medianResidual = sortedResiduals[n/2];
        if (medianResidual == 0)
            break;
        for (size_t i = 0; i < n; i++){
            double arg = residuals[i]/(6*medianResidual);
            robustnessWeights[i] = (arg >= 1) ? 0 : pow(1-arg*arg, 2);
        }
    }
}

static bool LoessSmoothEx(double bandwidth, vector<Point2d> yval, vector<double> &res){
    vector<double> c(res.size());
    if (bandwidth < 1)
        bandwidth = bandwidth*yval.size();
    size_t span = static_cast<size_t>(ceil(bandwidth));
    if (span == 6 || span == 4)
        span++;
    size_t n = yval.size();
    if (span != 7 && span != 5)
        return false;
    size_t halfw = (span-1)/2;
    vector<double> alpha(5);
    vector<vector<double>> alp(halfw);
    for (size_t i = 0; i < halfw; i++)
        alp[i].resize(span-1);

    if (span == 7){
        alpha = {-0.077344045897451,0.309376183589803,0.535935724615295,0.309376183589803,-0.077344045897451};
        alp[0] = {0.875175120669361,0.261789841046388,-0.071471937794908,-0.108074439587212,0.007529725026794,0.035051690639578};
        alp[1] = {0.278430202795154,0.356301218808744,0.311686021743879,0.142832593964365,-0.038079142371110,-0.051170894941032};
        alp[2] = {-0.033168270965143,0.281867055048599,0.432189913010369,0.343910640328730,0.053982114942752,-0.078781452365307};
    }
    else if (span == 5){
        alpha.resize(2);
        alpha = {0,1};
        alp[0] = {0.965570788513003,0.10328763446099,-0.10328763446099,0.034429211486997};
        alp[1] = {0.130047726028145,0.609856821915563,0.390143178084437,-0.130047726028146};
    }
    res[0] = yval[0].y*alpha[0];
    for (size_t i = 1; i < yval.size(); i++){
        res[i] = yval[i].y*alpha[0];
        for (size_t j = 1; j < min(i,alpha.size()); j++){
            res[i] = res[i]+yval[i-j].y*alpha[j];
        }
    }
    for (size_t i = halfw; i < n-halfw; i++)
        res[i] = res[i+span-halfw-2];



    for (size_t i = 0; i < halfw; i++){
        res[i] = 0;
        res[n-i-1] = 0;
        for (size_t j = 0; j < span-1; j++)
        {
            res[i] += alp[i][j]*yval[j].y;
            res[n-1-i] += alp[i][j]*yval[n-1-j].y;
        }
    }

    //robust
    for (size_t iter = 0; iter < 5; iter++){        
        for (size_t i = 0; i < n; i++)
            c[i] = abs(yval[i].y - res[i]);

        vector<double> rweight = BisquareWeights(c);

        Mat Y = Mat::zeros(static_cast<int>(span),1,CV_64FC1);
        Mat X = Mat::zeros(static_cast<int>(span),3,CV_64FC1);
        Mat A = Mat::zeros(3,1,CV_64FC1);
        for (size_t i = 0; i < n; i++){
            vector<size_t> idx = KNearestNeighbours(span,i,rweight);
            vector<double> weight = TricubeWeights(idx, i);
            vector<int> x1;
            for (size_t j = 0; j < span; j++){
                x1.push_back(static_cast<int>(idx[j]) - static_cast<int>(i));
            }
            for (size_t j = 0; j < span; j++){
                double w = weight[j]*rweight[idx[j]];
                Y.at<double>(static_cast<int>(j)) = w*yval[idx[j]].y;
                X.at<double>(static_cast<int>(j),0) = w;
                X.at<double>(static_cast<int>(j),1) = w*x1[j];
                X.at<double>(static_cast<int>(j),2) = w*x1[j]*x1[j];
            }
            //cout<<X<<endl;
            //cout<<Y<<endl;
            solve(X,Y,A,DECOMP_QR);
            //cout<<A<<endl;
            res[i] = A.at<double>(0,0);
        }
    }
    return true;
}

static void smoothPoint(QHash<int, vector<Point2d> > &posArr){
    vector<double> yfit;
    foreach(int posIndex,posArr.keys())
    {
        yfit.clear();
        yfit.resize(posArr[posIndex].size());
        LoessSmoothEx(0.15,posArr[posIndex],yfit);
        for (size_t i = 0; i < posArr[posIndex].size(); i++)
            posArr[posIndex][i].y = yfit[i];
    }
}

/**********************************
 *      通过拐点判断
 **********************************/
static void CalInflect(QHash<int, vector<Point2d> > &posArr){
    size_t halfw = 3;
    Mat X = Mat::zeros(static_cast<int>(2*halfw+1),3,CV_64FC1);
    Mat Y = Mat::zeros(static_cast<int>(2*halfw+1),1,CV_64FC1);
    Mat A = Mat::zeros(3,1,CV_64FC1);
    for (int i = 0; i < static_cast<int>(2*halfw+1); i++)
    {
        X.at<double>(i,0) = 1;
        X.at<double>(i,1) = i-static_cast<int>(halfw);
        X.at<double>(i,2) = X.at<double>(i,1)*X.at<double>(i,1);
    }
    foreach(int posIndex,posArr.keys())
    {
        vector<double> inflect(posArr[posIndex].size());
        for (size_t i = halfw; i < posArr[posIndex].size() - halfw; i++){
            for (size_t j = 0; j < 2*halfw+1; j++)
                Y.at<double>(static_cast<int>(j)) = posArr[posIndex][i+j-halfw].y;
            solve(X,Y,A,DECOMP_QR);
            inflect[i] = 2*A.at<double>(0,2)/pow(1+A.at<double>(0,1)*A.at<double>(0,1),3/2);
        }
        double infmean = 0;
        int n = 0;
        double max1 = eps, max2 = eps, max3 = eps, max4 = eps, temp, temp1;
        for (size_t i = 0; i < inflect.size(); i++){
            if (inflect[i] > eps){
                n++;
                infmean += inflect[i];
                temp = inflect[i];
                if (temp > max1){
                    temp1 = temp;
                    temp = max1;
                    max1 = temp1;
                }
                if (temp > max2){
                    temp1 = temp;
                    temp = max2;
                    max2 = temp1;
                }
                if (temp > max3){
                    temp1 = temp;
                    temp = max3;
                    max3 = temp1;
                }
                if (temp > max4){
                    temp1 = temp;
                    temp = max4;
                    max4 = temp1;
                }
            }
        }
        if (n > 15)
        {
            if (max1 > eps){
                n--;
                infmean -= max1;
            }
            if (max2 > eps){
                n--;
                infmean -= max2;
            }
            if (max3 > eps){
                n--;
                infmean -= max3;
            }
            if (max4 > eps){
                n--;
                infmean -= max4;
            }
        }
        qDebug()<<"infmean="<<infmean<<n<<infmean/n;
        infmean = infmean*3/n;
        n = 0;
        size_t i;
        for (i = 0; i < inflect.size(); i++){
            if (inflect[i] > infmean)
                n++;
            else
                n = 0;
            if (n > 2)
                break;
        }
        if (n > 2)
            qDebug()<<"有扩展";
        qDebug()<<posIndex<<infmean<<i<<inflect;
    }
}

/**********************************
 *      参考点处理
 **********************************/
static void RefPosFit(QHash<int, vector<Point2d> > &posArr){
    QHash<int,double> posBaseValue;        //各点基准数据

    //计算各点基准数据，cycle1~cycle10中，去掉两个最大值、两个最小值，剩余六个数的均值。
    foreach(int posIndex,posArr.keys()){        
        if (posArr[posIndex].size()<30)
            return;
        for (size_t i = 0; i < 10; i++)
            posBaseValue[posIndex] += posArr[posIndex][i].y;
        posBaseValue[posIndex] = posBaseValue[posIndex]/10;
    }
    foreach(int posIndex,posArr.keys()){
        if (posBaseValue[posIndex] != 0)
        for (vector<Point2d>::iterator it = posArr[posIndex].begin(); it != posArr[posIndex].end();it++)
            it->y /= posBaseValue[posIndex];
    }

    QMap<int,int>::iterator ref = PosId.begin();    
    //去除最大最小斜率参考点
    int MaxSlopePos = 0;
    int MinSlopePos = 0;
    vector<Point2d> tempPoint;
    Vec4f line_para;
    QHash<int,float> karr;
    while(ref != PosId.end()){
        if (ref.value() == static_cast<int>(ReferenceValue)){
            tempPoint.assign(posArr[ref.key()].begin()+static_cast<int64>(baseDataIndexStart),posArr[ref.key()].end());
            fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
            karr[ref.key()] = line_para[1]/line_para[0];
        }
        ref++;
    }
    QHash<int,float>::iterator ktor = karr.begin();
    bool initSlopeArr = false;
    while(ktor != karr.end()){
        if (initSlopeArr == false){
            MaxSlopePos = ktor.key();
            MinSlopePos = ktor.key();
            initSlopeArr = true;
        }
        else {
            if (ktor.value() > karr[MaxSlopePos])
                MaxSlopePos = ktor.key();
            if (ktor.value() < karr[MinSlopePos])
                MinSlopePos = ktor.key();
        }
        ktor++;
    }

    //计算参考点信号变化率
    ref = PosId.begin();
    QMap<size_t,double> refDataMean;
    int refSize = 0;
    while(ref != PosId.end()){
        if (ref.value() == static_cast<int>(ReferenceValue) && ref.key() != MaxSlopePos && ref.key() != MinSlopePos){
            for(size_t i = 0; i < posArr[ref.key()].size(); i++){
                refDataMean[i] += posArr[ref.key()][i].y;
            }
            refSize++;
        }
        ref++;
    }
    //计算参考点信号变化率平均值
    foreach(size_t posIndex,refDataMean.keys()){
        refDataMean[posIndex] /= refSize;
    }

    //qDebug()<<"posBaseValue"<<posBaseValue;
    //qDebug()<<"refDataMean"<<refDataMean;

    //计算检测点衰减信号    
    QMap<int,int>::iterator pos = PosId.begin();
    while(pos != PosId.end()){
        //if (pos.value() != ReferenceValue)
        {
            for (size_t i = 0; i < posArr[pos.key()].size(); i++){
                //double atten = posBaseValue[pos.key()]*refDataMean[i];
                posArr[pos.key()][i].y -= refDataMean[i];
                posArr[pos.key()][i].y = posArr[pos.key()][i].y*100;
            }
        }
        pos++;
    }
}

/*
static void NormalizeFit(QHash<int, vector<Point2d> > &posArr){
    foreach(int posIndex, posArr.keys()){
        for (int i = 0; i < posArr[posIndex].size(); i++){
            posArr[posIndex][i].y = posArr[posIndex][i].y*BGMax/bgMean[posIndex];
        }
    }
}
*/
#define REF_START 0
#define REF_END 15
#define REF_LENGTH (REF_END-REF_START)
static void RefPosFitEx(QHash<int, vector<Point2d> > &posArr){    
    foreach(int posIndex,posArr.keys()){
        if (PosId[posIndex] != static_cast<int>(ReferenceValue)){
            int RefPos = -1;
            double Maxk = 0,Maxa = 0,Maxd = 0, MaxMeanD = 0;
            for (QMap<int,int>::iterator it = PosId.begin(); it != PosId.end(); it++){
                if (it.value() == static_cast<int>(ReferenceValue)){                    
                    double k,a,d;
                    double MYa = 0, MYb = 0, MYa2 = 0, MYaYb = 0, MeanD = 0;
                    for (size_t i = REF_START; i < REF_END; i++){
                        MYa += posArr[posIndex][i].y;
                        MYb += posArr[it.key()][i].y;
                        MYa2 += posArr[posIndex][i].y*posArr[posIndex][i].y;
                        MYaYb += posArr[posIndex][i].y*posArr[it.key()][i].y;
                        MeanD += posArr[it.key()][i].y - posArr[posIndex][i].y;
                    }
                    k = MYa*MYa - REF_LENGTH*MYa2;
                    if (abs(k) > eps)
                        k = (MYa*MYb - REF_LENGTH*MYaYb)/k;
                    a = MYb-k*MYa;
                    if (abs(a) > eps)
                        a = a/REF_LENGTH;
                    MeanD = MeanD/REF_LENGTH;
                    d = 0;
                    for (size_t i = REF_START; i < REF_END; i++){
                        //double temp = (k*posArr[posIndex][i].y+a) - posArr[it.key()][i].y;
                        double temp = posArr[posIndex][i].y - posArr[it.key()][i].y + MeanD;
                        d += temp*temp;
                    }
                    if (RefPos == -1 || d < Maxd){
                        RefPos = it.key();
                        Maxd = d;
                        Maxk = k;
                        Maxa = a;
                        MaxMeanD = MeanD;
                    }
                }
            }
            if (RefPos != -1)
            for (size_t i = 0; i < posArr[posIndex].size(); i++)
                //posArr[posIndex][i].y = posArr[posIndex][i].y*Maxk + Maxa - posArr[RefPos][i].y;
                posArr[posIndex][i].y = posArr[posIndex][i].y + MaxMeanD;
            qDebug()<<"posIndex="<<posIndex<<"RefPos="<<RefPos<<"k="<<Maxk<<"a="<<Maxa;
        }
    }
}

/**********************************
 *      基线拟合
 **********************************/
//第一种方法
static void BaseLineFit(QHash<int, vector<Point2d>> &posArr){
    size_t cycle = 0;

    foreach(int posIndex, posArr.keys()){
        cycle = posArr[posIndex].size();
        if (cycle > 30){
            vector<Point2d> points = posArr[posIndex];
            vector<Point2d> tempPoint;
            int linebaseStart = 3;
            int linebaseEnd = points.size() - 1;
            Vec4f line_para;
            bool doLine = true;
#if 0
            {
                tempPoint.assign(points.begin()+5,points.begin()+15);
                fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);                
                doLine = false;
            }
#endif
            while(doLine){
                doLine = false;
                tempPoint.assign(points.begin()+linebaseStart,points.begin()+linebaseEnd+1);
                fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
                if (linebaseEnd>BaseLineRight){
                    double dv = line_para[1]/line_para[0]*(points[linebaseEnd].x-line_para[2])+line_para[3];
                    if (fabs(points[linebaseEnd].y-dv)>BaseLineAccuracy){
                        linebaseEnd--;
                        doLine = true;
                    }
                    //qDebug()<<"BaseLineRight,linebaseStart="<<linebaseStart<<"linebaseEnd="<<linebaseEnd<<"dv="<<dv<<"points[linebaseEnd].y="<<points[linebaseEnd].y<<" fabs="<<fabs(points[linebaseEnd].y-dv);
                }
                if (doLine == false && linebaseStart<BaseLineLeft){
                    double dv = line_para[1]/line_para[0]*(points[linebaseStart].x-line_para[2])+line_para[3];
                    if (fabs(points[linebaseStart].y-dv)>BaseLineAccuracy){
                        linebaseStart++;
                        doLine = true;
                    }
                    //qDebug()<<"BaseLineLeft,linebaseStart="<<linebaseStart<<"linebaseEnd="<<linebaseEnd<<"dv="<<dv<<"points[linebaseStart].y="<<points[linebaseStart].y<<" fabs="<<fabs(points[linebaseStart].y-dv);
                }
            }

            float k = line_para[1]/line_para[0];
            float intercept = k*(0-line_para[2]) + line_para[3];
            baseLine[posIndex][0] = k;
            baseLine[posIndex][1] = intercept;

            for (int i = 0; i < posArr[posIndex].size();i++){
                posArr[posIndex][i].y = posArr[posIndex][i].y - (posArr[posIndex][i].x*k + intercept);
            }
        }
    }
}

//第二种方法，找基线效果比较好
static void BaseLineFitEx(QHash<int, vector<Point2d>> &posArr){
    size_t cycle = 0;
    int maxleftpos;
    foreach(int posIndex, posArr.keys()){
        cycle = posArr[posIndex].size();        
        if (cycle > 30){
            vector<Point2d> points = posArr[posIndex];
            vector<Point2d> tempPoint;
            Vec4f line_para;            
            vector<double> dyArr;
            QString posStr = QString("\nposIndex=%1,posArr:%2").arg(posIndex).arg(points[0].y);

            for (int i = 1; i < points.size(); i++)
                posStr.append(","+QString::number(points[i].y));
            //qDebug().noquote()<<posStr;

            maxleftpos = (cycle-baseDataCount)>25?25:(cycle-baseDataCount);
            for (int i = baseDataIndexStart; i < maxleftpos; i++){
                tempPoint.assign(points.begin()+i,points.begin()+i+baseDataCount);
                fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
                double dy = 0;
                for (int j = 0; j < tempPoint.size(); j++)
                {
                    double dv = line_para[1]/line_para[0]*(tempPoint[j].x-line_para[2])+line_para[3];
                    dy += fabs(tempPoint[j].y-dv);
                }
                dyArr.push_back(dy);
            }
            int minIndex = distance(dyArr.begin(),min_element(dyArr.begin(),dyArr.end()));
            double minValue = dyArr.at(minIndex);
            //qDebug()<<"dyArr"<<dyArr<<minIndex<<minValue;
            tempPoint.assign(points.begin()+minIndex+baseDataIndexStart,points.begin()+minIndex+baseDataIndexStart+baseDataCount);
            fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
            dyArr.clear();
            for (int j = 0; j < points.size(); j++)
            {
                double dv = line_para[1]/line_para[0]*(points[j].x-line_para[2])+line_para[3];
                dyArr.push_back(fabs(points[j].y-dv));
            }
            //qDebug()<<"points="<<dyArr;
            int startIndex,endIndex;
            for (startIndex = minIndex+baseDataIndexStart; startIndex > baseDataIndexStart; startIndex--)
            {
                if(dyArr.at(startIndex) > minValue)
                {
                    startIndex++;
                    break;
                }
            }
            for (endIndex = minIndex+baseDataIndexStart+baseDataCount; endIndex < points.size(); endIndex++)
            {
                if(dyArr.at(endIndex) > minValue*3)
                    break;
            }
            qDebug()<<"startIndex="<<startIndex<<"endIndex="<<endIndex;
            tempPoint.assign(points.begin()+startIndex,points.begin()+endIndex);
            fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);

            float k = line_para[1]/line_para[0];
            float intercept = k*(0-line_para[2]) + line_para[3];
            qDebug()<<"k="<<k<<"intercept="<<intercept;
            for (int i = 0; i < posArr[posIndex].size();i++){
                posArr[posIndex][i].y = posArr[posIndex][i].y - (posArr[posIndex][i].x*k + intercept);
            }
            PosIntercept[posIndex] = intercept;
        }
    }
}

//第三种方法，单点注册检使用
static void CheckBaseLinePos(vector<Point2d> &points, int &startpos, int &endpos, int &maxleft){
    vector<Point2d> tempPoint;
    vector<double> dyArr;
    Vec4f line_para;
    startpos = 0;
    endpos = points.size()-1;
    if (endpos<baseDataCount)
        return;
    int maxleftpos = maxleft < (points.size()-baseDataCount) ? maxleft:(points.size()-baseDataCount);
    for (int i = 0; i <= maxleftpos;i++){
        tempPoint.assign(points.begin()+i,points.begin()+i+baseDataCount);
        fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
        double dy = 0;
        for (int j = 0; j < tempPoint.size(); j++)
        {
            double dv = line_para[1]/line_para[0]*(tempPoint[j].x-line_para[2])+line_para[3];
            dy += fabs(tempPoint[j].y-dv);
        }
        dyArr.push_back(dy);
    }
    size_t minIndex = distance(dyArr.begin(),min_element(dyArr.begin(),dyArr.end()));
    qDebug()<<"dyArr"<<dyArr<<minIndex;
    double minValue = dyArr.at(minIndex);
    tempPoint.assign(points.begin()+minIndex,points.begin()+minIndex+baseDataCount);
    fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
    dyArr.clear();
    for (int j = 0; j < points.size(); j++)
    {
        double dv = line_para[1]/line_para[0]*(points[j].x-line_para[2])+line_para[3];
        dyArr.push_back(fabs(points[j].y-dv));
    }
    qDebug()<<"points="<<dyArr;
    for (startpos = minIndex; startpos >= 0; startpos--)
    {
        if(dyArr.at(startpos) > minValue)
        {
            startpos++;
            break;
        }
    }
    if (startpos<0)
        startpos = 0;
    for (endpos = minIndex+baseDataCount; endpos < points.size(); endpos++)
    {
        if(dyArr.at(endpos) > minValue*4)
            break;
    }

    tempPoint.assign(points.begin()+startpos,points.begin()+endpos);
    fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
    float k = line_para[1]/line_para[0];
    float intercept = k*(0-line_para[2]) + line_para[3];
    int maxy = 0;
    for (size_t i = 0; i < points.size();i++){
        points[i].y = points[i].y - (points[i].x*k + intercept);
        if (points[i].y > maxy)
            maxy = points[i].y;
    }
    maxy = maxy/15;
    maxleft = 0;
    for (int i = 1; i < points.size(); i++)
    {
        if (points[i-1].y <= maxy && points[i].y >= maxy){
            if (abs(i-20)<abs(maxleft-20))
            {
                maxleft = i;
            }
        }
    }
}

static double calcvar(vector<Point2d> &points, int start, int end){
    double result = 0;
    double sum = 0;
    if (start >= end)
        return 0;

    for (int i = start; i <= end; i++){
        sum += points[i].y;
    }
    sum = sum/(end-start+1);
    for (int i = start; i <= end; i++)
    {
        result += (points[i].y-sum)*(points[i].y-sum);
    }
    return result/(end-start);
}

static void linebasefit(vector<Point2d> &points, Vec4f &line_para){
    float k = line_para[1]/line_para[0];
    float intercept = k*(0-line_para[2]) + line_para[3];
    for (size_t i = 0; i < points.size();i++){
        points[i].y = points[i].y - (points[i].x*k + intercept);
    }
}

static void CheckBaseLinePosEx(vector<Point2d> points, int &startpos, int &endpos, int &maxleft){
    vector<Point2d> tempPoint;
    vector<double> dyArr;
    Vec4f line_para;
    bool looking = true;
    startpos = 0;
    endpos = points.size()-1;
    if (endpos<baseDataCount)
        return;
    double rvar0 = calcvar(points,startpos,endpos);
    double rvar;
    while(looking && endpos-startpos > 10){
        endpos--;
        rvar = calcvar(points,startpos,endpos);
        if (rvar > rvar0){
            looking = false;
            endpos++;
        }
        else {
            rvar0 = rvar;
        }
    }

    int minIndex = startpos;
    double minValue = calcvar(points,startpos,startpos+10);
    if (looking){
        for (int i = startpos+1; i < points.size()-10; i++){
            double tempValue = calcvar(points,i,i+10);
            if (tempValue < minValue){
                minValue = tempValue;
                minIndex = i;
            }
        }
        startpos = minIndex;
        endpos = startpos+10;
        while(looking && endpos < points.size()-1){
            endpos++;
            tempPoint.assign(points.begin()+startpos,points.begin()+endpos+1);
            fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
            linebasefit(tempPoint,line_para);
            rvar = calcvar(tempPoint,0,tempPoint.size()-1);
            if (rvar > rvar0){
                looking = false;
                endpos--;
            }
            else {
                rvar0 = rvar;
            }
        }
    }
    else{
        endpos = endpos-2;
        rvar0 = rvar;
        looking = true;

        while(looking && endpos-startpos>7){
            startpos++;
            tempPoint.assign(points.begin()+startpos,points.begin()+endpos+1);
            fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
            linebasefit(tempPoint,line_para);
            rvar = calcvar(tempPoint,0,tempPoint.size()-1);
            if (rvar > rvar0){
                looking = false;
                endpos--;
            }
            else {
                rvar0 = rvar;
            }            
        }
        if (looking)
            startpos = 5;
    }

    tempPoint.assign(points.begin()+startpos,points.begin()+endpos+1);
    fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
    float k = line_para[1]/line_para[0];
    float intercept = k*(0-line_para[2]) + line_para[3];
    int maxy = 0;
    for (size_t i = 0; i < points.size();i++){
        points[i].y = points[i].y - (points[i].x*k + intercept);
        if (points[i].y > maxy)
            maxy = points[i].y;
    }
    maxy = maxy/15;
    maxleft = 0;
    for (int i = 1; i < points.size(); i++)
    {
        if (points[i-1].y <= maxy && points[i].y >= maxy){
            if (abs(i-20)<abs(maxleft-20))
            {
                maxleft = i;
            }
        }
    }
}

static void normalBaseLine(vector<Point2d> &points){

    int maxvalue = points[0].y;
    int minvalue = points[0].y;

    for (int i = 0; i < points.size(); i++)
    {
        if (points[i].y > maxvalue)
            maxvalue = points[i].y;
        if (points[i].y < minvalue)
            minvalue = points[i].y;
    }
    maxvalue -= minvalue;
    for (int i = 0; i < points.size(); i++)
        points[i].y = (points[i].y - minvalue)*10000/maxvalue;

}

static void BaseLineFitExx(QHash<int, vector<Point2d>> &posArr){
    size_t cycle = 0;
    foreach(int posIndex, posArr.keys()){
        cycle = posArr[posIndex].size();
        if (cycle > 30){
            normalBaseLine(posArr[posIndex]);
            vector<Point2d> points = posArr[posIndex];
            vector<Point2d> tempPoint;
            int startpos,endpos,maxleft;
            tempPoint.assign(points.begin()+baseDataIndexStart,points.end());
            maxleft = 15;
            CheckBaseLinePosEx(tempPoint,startpos,endpos,maxleft);
            startpos+=baseDataIndexStart;
            endpos+=baseDataIndexStart;
            maxleft+=baseDataIndexStart;
            qDebug()<<"BaseLineFitExx,startpos="<<startpos<<",endpos="<<endpos<<",maxleft="<<maxleft;
            int leftpos = maxleft - 15;
            if (leftpos < 0)
                leftpos = 0;
            tempPoint.assign(points.begin()+leftpos,points.end());
            CheckBaseLinePosEx(tempPoint,startpos,endpos,maxleft);
            startpos+=leftpos;
            endpos+=leftpos;
            maxleft+=leftpos;
            qDebug()<<"BaseLineFitExx,startpos="<<startpos<<",endpos="<<endpos<<",maxleft="<<maxleft;

            Vec4f line_para;
            tempPoint.assign(points.begin()+startpos,points.begin()+endpos);
            fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-2,1e-2);
            float k = line_para[1]/line_para[0];
            float intercept = k*(0-line_para[2]) + line_para[3];

            for (int i = 0; i < posArr[posIndex].size();i++){
                posArr[posIndex][i].y = posArr[posIndex][i].y - (posArr[posIndex][i].x*k + intercept);                
            }            
        }
    }
}

//第四种方法，现matlab中使用

// std function in matlab
static double calcstd(vector<double> &dz, size_t start, size_t end){
    double result = 0;
    double sum = 0;
    if (start >= end)
        return 0;

    for (size_t i = start; i <= end; i++){
        sum += dz[i];
    }
    sum = sum/(end-start+1);
    for (size_t i = start; i <= end; i++)
    {
        result += (dz[i]-sum)*(dz[i]-sum);
    }
    return sqrt(result/(end-start));
}

// CallCt_curv in matlab
static void BaseLineFitF(QHash<int, vector<Point2d>> &posArr){
    size_t cycle = 0;
    BL.clear();
    foreach(int posIndex, posArr.keys()){
        cycle = posArr[posIndex].size();
        if (cycle > 30){
            vector<double> z0(cycle);
            bool looking = true;            
            int initpos = baseDataIndexStart;
            int startpos = baseDataIndexStart;
            int endpos = static_cast<int>(cycle) - 1;
            vector<double> dz;
            vector<double> ddz;
            vector<double> rk;
            int r1 = 0;
            int r2;
            LoessSmoothEx(0.15,posArr[posIndex],z0);
            vector<double> z;
            z.insert(z.begin(),z0.begin()+5,z0.end());
            while(looking){
                dz.clear();
                ddz.clear();
                rk.clear();

                dz.push_back(z[1] - z[0]);
                for (size_t i = 1; i < z.size()-1; i++){
                    dz.push_back((z[i+1] - z[i-1])/2);
                }
                dz.push_back(z[z.size()-1]-z[z.size()-2]);

                int cmax = 0;
                double maxdz = dz[0];
                for (size_t i = 1; i < dz.size(); i++)
                {
                    if (dz[i] > maxdz)
                    {
                        maxdz = dz[i];
                        cmax = static_cast<int>(i);
                    }
                }
                size_t rpos = 0;
                for (size_t i = 1; i < cmax; i++){
                    if (dz[static_cast<size_t>(i)] < 0)
                        rpos = i;
                }

                ddz.push_back(dz[1] - dz[0]);
                for (size_t i = 1; i < dz.size()-1; i++){
                    ddz.push_back((dz[i+1] - dz[i-1])/2);
                }
                ddz.push_back(dz[dz.size()-1]-dz[dz.size()-2]);

                for (size_t i = 0; i < dz.size(); i++){
                    double d = dz[i]*dz[i]+1;
                    d = d*d*d;
                    d = sqrt(d);                    
                    rk.push_back(ddz[i]/d);
                }

                double maxrk = rk[0];
                r2 = 0;
                for (size_t i = rpos; i <= cmax; i++){
                    if (rk[i] > maxrk){
                        maxrk = rk[i];
                        r2 = static_cast<int>(i);
                    }
                }

                if (cmax - r2 > 12)
                    r2 = cmax - 12;
                else if(cmax - r2 < 5)
                    r2 = max(r1, cmax-5);

                if (r2 - r1 >= 10){
                    while(calcstd(dz,static_cast<size_t>(r1),static_cast<size_t>(r2)) > 0.35 && r2 - r1 > 15){
                        r1++;
                    }
                    if (maxdz/calcstd(dz,static_cast<size_t>(r1),static_cast<size_t>(r2)) < 2.5){
                        if (cmax+initpos < 39){
                            startpos = r1 + initpos;
                            looking = false;
                        }
                        else if(r2+initpos >= 34 && calcstd(dz,static_cast<size_t>(r1),static_cast<size_t>(r2)) > 0.35){
                            endpos = r2 + initpos - 1;
                            startpos = endpos - 25;
                            looking = false;
                        }
                        else{
                            endpos = r2 + initpos - 1;
                            startpos = r1 + initpos;
                            looking = false;
                        }
                    }
                    else{
                        endpos = r2 + initpos;
                        startpos = r1 + initpos;
                        looking = false;
                    }
                }
                else if(r2 + initpos <= 15)
                {
                    double mindz = maxdz;
                    if (cmax > 0 && dz[static_cast<size_t>(cmax)-1] < mindz)
                        mindz = dz[static_cast<size_t>(cmax)-1];
                    if (static_cast<size_t>(cmax) < dz.size() - 1 && dz[static_cast<size_t>(cmax)+1] < mindz)
                        mindz = dz[static_cast<size_t>(cmax)+1];
                    if (mindz > 10){
                        endpos = r2 + initpos;
                        startpos = r1 + initpos;
                        looking = false;
                    }
                    else if(cycle - cmax - initpos > 15){
                        initpos = cmax + initpos + 1;
                        vector<Point2d> tempPoint;
                        tempPoint.insert(tempPoint.begin(),posArr[posIndex].begin()+initpos,posArr[posIndex].end());
                        z.clear();
                        z.resize(tempPoint.size());
                        LoessSmoothEx(0.15,tempPoint,z);
                    }
                    else
                        looking = false;

                }
                else{
                    endpos = static_cast<int>(cycle) - 1;
                    startpos = 5;
                    looking = false;
                }
            }

            vector<Point2d> tempPoint;
            Vec4f line_para;
            BL[posIndex][0] = startpos+1;
            BL[posIndex][1] = endpos+1;
            for (int i = startpos; i <= endpos; i++)
                tempPoint.push_back(Point2d(i+1,z0[static_cast<size_t>(i)]));
            //fitLine(tempPoint,line_para,cv::DIST_L2,0,1e-3,1e-3);
            //float k = line_para[1]/line_para[0];
            //float intercept = k*(0-line_para[2]) + line_para[3];
            double k,intercept;
            findLine(tempPoint,k,intercept);
            for (size_t i = 0; i < posArr[posIndex].size();i++){
                posArr[posIndex][i].y = z0[i] - (posArr[posIndex][i].x*k + intercept);
            }
        }
    }
}

/**********************************
 *      通过多项式进行曲线拟合
 **********************************/
static bool PloyCurveFit(vector<Point2d> &pos, int n, Mat &A){
    int N = pos.size();
    Mat X = Mat::zeros(n+1,n+1,CV_64FC1);
    for (int i = 0; i < n+1; i++)
    {
        for(int j = 0; j < n+1; j++)
        {
            for(int k = 5; k < N; k++)
            {
                X.at<double>(i,j) = X.at<double>(i,j)+pow(pos[k].x,i+j);
            }
        }
    }

    Mat Y = Mat::zeros(n+1,1,CV_64FC1);
    for (int i = 0; i < n+1; i++){
        for (int k = 5; k < N; k++){
            Y.at<double>(i,0) = Y.at<double>(i,0) + pow(pos[k].x, i)*pos[k].y;
        }
    }

    A = Mat::zeros(n+1,1,CV_64FC1);
    solve(X,Y,A,DECOMP_LU);
    return true;
}

static void CurveFit(QHash<int, vector<Point2d> > &posArr){
    foreach(int posIndex, posArr.keys()){
        Mat A;
        PloyCurveFit(posArr[posIndex],3,A);
        for (int i = 0; i < posArr[posIndex].size(); i++)
        {
            int x = posArr[posIndex][i].x;
            posArr[posIndex][i].y = A.at<double>(0,0)+A.at<double>(1,0)*x+
                    A.at<double>(2,0)*pow(x,2)+A.at<double>(3,0)*pow(x,3)+0.5;
        }

    }
}

/**********************************
 *      加载数据
 **********************************/

static void FillTestData(int Testid, QHash<int, vector<Point2d> > &posArr){
    if (SqliteMgr::isOpen()){
        QSqlQuery query = SqliteMgr::select(QString("select * from TestResult where Testid=%1").arg(Testid));
        int PosIndexNu = query.record().indexOf("PosIndex");
        int ItemidNu = query.record().indexOf("Itemid");
        int cycleNu = query.record().indexOf("cycle");
        int PosValueNu = query.record().indexOf("PosValue");
        int PosNumNu = query.record().indexOf("PosNum");
        int BgValueNu = query.record().indexOf("BgValue");
        int BgNumNu = query.record().indexOf("BgNum");

        posArr.clear();
        PosId.clear();
        PosIntercept.clear();

        rawData.clear();
        posBG.clear();
        Name.clear();
        BGMax = 0;
        while(query.next()){
            double PosValue = query.value(PosValueNu).toReal();
            int PosNum = query.value(PosNumNu).toInt();
            double BgValue = query.value(BgValueNu).toReal();
            int BgNum = query.value(BgNumNu).toInt();
            double bss = (PosValue/PosNum) - (BgValue/BgNum);

            int itemid = query.value(ItemidNu).toInt();
            posArr[query.value(PosIndexNu).toInt()].push_back(Point2d(query.value(cycleNu).toInt(),bss));
            PosId[query.value(PosIndexNu).toInt()] = itemid;
            posBG[query.value(PosIndexNu).toInt()].push_back(BgValue/BgNum);
            rawData[query.value(PosIndexNu).toInt()].push_back(Vec6d(PosValue,PosNum,BgValue,BgNum,bss));
            if (itemid > 1 && !Name.contains(ItemName[AssayItem[itemid]]))
                Name.push_back(ItemName[AssayItem[itemid]]);
        }

        foreach(int posIndex, posBG.keys()){
            double sum = 0;
            for(size_t i = 0; i < posBG[posIndex].size(); i++){
                sum += posBG[posIndex][i];
                if (posBG[posIndex][i] > BGMax)
                    BGMax = posBG[posIndex][i];
            }
            bgMean[posIndex] = sum/posBG[posIndex].size();
        }
    }
}

void DataHandler::FillTestResult(int Testid, QHash<int, vector<Point2d> > &posArr){
    if (SqliteMgr::isOpen()){
        QSqlQuery query = SqliteMgr::select(QString("select * from TestResult where Testid=%1").arg(Testid));
        int PosIndexNu = query.record().indexOf("PosIndex");
        int ItemidNu = query.record().indexOf("Itemid");
        int cycleNu = query.record().indexOf("cycle");
        int PosValueNu = query.record().indexOf("PosValue");
        int PosNumNu = query.record().indexOf("PosNum");
        int BgValueNu = query.record().indexOf("BgValue");
        int BgNumNu = query.record().indexOf("BgNum");
        int TestResultNu = query.record().indexOf("TestValue");
        posArr.clear();
        PosId.clear();
        PosIntercept.clear();

        rawData.clear();
        posBG.clear();
        Name.clear();
        BGMax = 0;
        while(query.next()){
            double PosValue = query.value(PosValueNu).toReal();
            int PosNum = query.value(PosNumNu).toInt();
            double BgValue = query.value(BgValueNu).toReal();
            int BgNum = query.value(BgNumNu).toInt();
            double bss = query.value(TestResultNu).toReal()/100;

            int itemid = query.value(ItemidNu).toInt();
            posArr[query.value(PosIndexNu).toInt()].push_back(Point2d(query.value(cycleNu).toInt(),bss));
            PosId[query.value(PosIndexNu).toInt()] = itemid;
            posBG[query.value(PosIndexNu).toInt()].push_back(BgValue/BgNum);
            rawData[query.value(PosIndexNu).toInt()].push_back(Vec6d(PosValue,PosNum,BgValue,BgNum,bss));
            if (itemid > 1 && !Name.contains(ItemName[AssayItem[itemid]]))
                Name.push_back(ItemName[AssayItem[itemid]]);
        }

        foreach(int posIndex, posBG.keys()){
            double sum = 0;
            for(size_t i = 0; i < posBG[posIndex].size(); i++){
                sum += posBG[posIndex][i];
                if (posBG[posIndex][i] > BGMax)
                    BGMax = posBG[posIndex][i];
            }
            bgMean[posIndex] = sum/posBG[posIndex].size();
        }

        //读取上一次计算结果和CT阀值
        CurrItemCT.clear();
        CT.clear();
        QSqlQuery ResQuery = SqliteMgr::select(QString("select * from AnalysisResult where Testid=%1").arg(Testid));
        PosIndexNu = ResQuery.record().indexOf("PosIndex");
        ItemidNu = ResQuery.record().indexOf("Itemid");
        cycleNu = ResQuery.record().indexOf("cycle");
        PosValueNu = ResQuery.record().indexOf("MaxValue");
        while(ResQuery.next()){
            CurrItemCT[ResQuery.value(ItemidNu).toInt()] = ResQuery.value(PosValueNu).toInt();
            CT[ResQuery.value(PosIndexNu).toInt()] = ResQuery.value(cycleNu).toInt();
            qDebug()<<"CurrItemCT,"<<ResQuery.value(PosIndexNu).toInt()<<ResQuery.value(ItemidNu).toInt()<<ResQuery.value(PosValueNu).toInt()<<ResQuery.value(cycleNu).toInt();
        }
    }
}

static void FillResultData(int Testid, QHash<int, vector<Point2d> > &posArr){
    if (SqliteMgr::isOpen()){
        SqliteMgr::StartTransations();
        foreach(int posIndex, posArr.keys()){
            if (PosId[posIndex] < 2)
                continue;
            for (size_t c = 0; c < posArr[posIndex].size(); c++)
            {
                double value = posArr[posIndex][c].y*100;
                SqliteMgr::execute(QString("update TestResult set TestValue=%1 where Testid=%2 and PosIndex=%3 and cycle=%4")
                                   .arg(static_cast<int>(value)).arg(Testid).arg(posIndex).arg(c+1));
            }
            SqliteMgr::execute(QString("insert into AnalysisResult(Testid,PosIndex,cycle,Itemid,MaxValue) values(%1,%2,%3,%4,%5)")
                               .arg(Testid).arg(posIndex).arg(CT[posIndex]).arg(PosId[posIndex]).arg(ItemCT[PosId[posIndex]]));
        }
        SqliteMgr::execute(QString("update PanelTest set ResultType=3 where Testid=%1").arg(Testid));
        SqliteMgr::EndTransations();
    }
}

static void FillDirectData(int Testid, QHash<int, vector<Point2d> > &posArr){
    QSqlQuery query = SqliteMgr::select(QString("select * from TestResult where Testid=%1").arg(Testid));
    int PosIndexNu = query.record().indexOf("PosIndex");
    int ItemidNu = query.record().indexOf("Itemid");
    int cycleNu = query.record().indexOf("cycle");
    int TestValueNu = query.record().indexOf("TestValue");

    posArr.clear();
    PosId.clear();
    PosIntercept.clear();

    while(query.next()){
        int TestValue = query.value(TestValueNu).toInt();
        posArr[query.value(PosIndexNu).toInt()].push_back(Point2d(query.value(cycleNu).toInt(),TestValue));
        PosId[query.value(PosIndexNu).toInt()] = query.value(ItemidNu).toInt();
    }
}

static bool LoadTestData(QString filename, QHash<int, vector<Point2d> > &posArr){
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        QString line = in.readLine();
        QStringList lineArr = line.split(',');
        QHash<int,int> indexArr;

        posArr.clear();
        PosId.clear();
        rawData.clear();
        posBG.clear();
        Name.clear();
        BGMax = 0;

        for (int i = 1; i < lineArr.size(); i+=5){
            QStringList recordArr = lineArr[i].split('/');
            int posIndex = recordArr[1].toInt();
            PosId[recordArr[2].toInt()] = posIndex;
            qDebug()<<"LoadTestData"<<posIndex<<recordArr[2].toInt()<<AssayItem[posIndex]<<ItemName[AssayItem[posIndex]];
            indexArr[i] = recordArr[2].toInt();
            if (posIndex > 1 && !Name.contains(ItemName[AssayItem[posIndex]]))
            {
                qDebug()<<"LoadTestData"<<posIndex<<AssayItem[posIndex]<<ItemName[AssayItem[posIndex]];
                Name.push_back(ItemName[AssayItem[posIndex]]);
            }
        }        
        line = in.readLine();
        int cycleNu = 1;
        while(!line.isEmpty()){
            lineArr = line.split(',');            
            for (int i = 1; i < lineArr.size(); i+=5){
                double PosValue = lineArr[i].toDouble();
                int PosNum = lineArr[i+1].toInt();
                double BgValue = lineArr[i+2].toDouble();
                int BgNum = lineArr[i+3].toInt();
                double bss = (PosValue/PosNum) - (BgValue/BgNum);

                posArr[indexArr[i]].push_back(Point2d(cycleNu,bss));
                posBG[indexArr[i]].push_back(BgValue/BgNum);
                rawData[indexArr[i]].push_back(Vec6d(PosValue,PosNum,BgValue,BgNum,bss));
            }
            line = in.readLine();
            cycleNu++;
        }
        foreach(int posIndex, posBG.keys()){
            double sum = 0;
            for(size_t i = 0; i < posBG[posIndex].size(); i++){
                sum += posBG[posIndex][i];
                if (posBG[posIndex][i] > BGMax)
                    BGMax = posBG[posIndex][i];
            }
            bgMean[posIndex] = sum/posBG[posIndex].size();
        }
        return true;
    }
    return false;
}

static bool LoadDebugData(QString filename, QHash<int, vector<Point2d> > &posArr){
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)){
        int cycleNu = 1;
        QTextStream in(&file);
        QString line = in.readLine();

        while(!line.isEmpty()){
            QStringList lineArr = line.split(',');
            foreach(int posIndex, PosId.keys()){
                posArr[posIndex].push_back(Point2d(cycleNu,lineArr[posIndex-1].toDouble()));
            }
            cycleNu++;
            line = in.readLine();
        }
    }
    return true;
}

static int CalCtResult(int ct, vector<Point2d> data){
    int result = 0;
    double dct = static_cast<double>(ct);
    qDebug()<<"CalCtResult"<<data.size()<<ct<<data[data.size()-1].y;
    if(data.size()<10 || data[data.size()-1].y < ct)
        return 0;
    for (size_t i = data.size()-1; i > 10; i--){
        if (data[i].y < dct){
            if (i == data.size()-1)
                result = static_cast<int>((i+1)*10);
            else{
                dct = (dct - data[i].y)*10/(data[i+1].y-data[i].y);
                result = static_cast<int>(i+1)*10 + static_cast<int>(dct);
            }
            break;
        }
    }
    return result;
}

static void getCT(QHash<int, vector<Point2d>> &posArr){
    foreach(int posIndex, posArr.keys()){
        CT[posIndex] = CalCtResult(ItemCT[PosId[posIndex]],posArr[posIndex]);
    }
}
/**********************************
 *      输出数据
 **********************************/
static void outputPos(QString filename, QHash<int, vector<Point2d> > posArr){
    //QFile file(saveDir+"/"+filename+".csv");
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream textStream(&file);
        QString saveStr;
        int cyclecount = 0;
        saveStr = "cycle";
        foreach(int posIndex, PosId.keys()){
            saveStr += ",";
            //saveStr += AssayItem[PosId[posIndex]]+"(R"+QString::number(posIndex/11+1);
            //saveStr += "C"+QString::number(posIndex%11+1)+")";
            saveStr += QString("%1(%2)").arg(AssayItem[PosId[posIndex]]).arg(posIndex);
            cyclecount = posArr[posIndex].size();
        }
        textStream<<saveStr<<endl;

        for(int i = 0; i < cyclecount; i++){
            saveStr = QString::number(i+1);            
            foreach(int posIndex, PosId.keys()){
                saveStr += ",";
                saveStr += QString::number(posArr[posIndex][i].y);
            }
            textStream<<saveStr<<endl;
        }

        textStream.flush();
        file.close();
    }
}

static void outputPos(QString filename, QHash<int, vector<Vec6d> > posArr){
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream textStream(&file);
        QString saveStr;
        size_t cyclecount = 0;
        saveStr = "cycle";
        foreach(int posIndex, PosId.keys()){            
            if (PosId[posIndex] == 1)
                saveStr += QString(",%1/%2/%3,,,,").arg("pos").arg(PosId[posIndex]).arg(posIndex);
            else
                saveStr += QString(",%1/%2/%3,,,,").arg(AssayItem[PosId[posIndex]]).arg(PosId[posIndex]).arg(posIndex);
            cyclecount = posArr[posIndex].size();
        }
        textStream<<saveStr<<endl;

        for(size_t i = 0; i < cyclecount; i++){
            saveStr = QString::number(i+1);
            foreach(int posIndex, PosId.keys()){
                saveStr += QString(",%1,%2,%3,%4,%5").arg(posArr[posIndex][i][0]).arg(posArr[posIndex][i][1]).arg(posArr[posIndex][i][2]).arg(posArr[posIndex][i][3]).arg(posArr[posIndex][i][4]);
            }
            textStream<<saveStr<<endl;
        }

        textStream.flush();
        file.close();
    }
}

static void outputPos(QString filename, QHash<int, vector<Point2d> > posArr, QHash<int, Vec2f> baseLineParam){
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream textStream(&file);
        QString saveStr,saveStr1;
        size_t cyclecount = 0;
        saveStr = "cycle";
        foreach(int posIndex, PosId.keys()){
            saveStr += ",";
            //saveStr += AssayItem[PosId[posIndex]]+"(R"+QString::number(posIndex/11+1);
            //saveStr += "C"+QString::number(posIndex%11+1)+")";
            saveStr += QString("%1(%2)").arg(AssayItem[PosId[posIndex]]).arg(posIndex);
            cyclecount = posArr[posIndex].size();
        }
        textStream<<saveStr<<endl;

        saveStr = "";
        saveStr1 = "";
        foreach(int posIndex, PosId.keys()){
            saveStr += ",";
            saveStr += QString::number(baseLineParam[posIndex][0]);
            saveStr1 += ",";
            saveStr1 += QString::number(baseLineParam[posIndex][1]);
        }
        textStream<<saveStr<<endl;
        textStream<<saveStr1<<endl;

        for(int i = 0; i < cyclecount; i++){
            saveStr = QString::number(i+1);
            foreach(int posIndex, PosId.keys()){
                saveStr += ",";
                saveStr += QString::number(posArr[posIndex][i].y);
            }
            textStream<<saveStr<<endl;
        }

        textStream.flush();
        file.close();
    }
}

static void outputPosForMatlab(QString filename, QHash<int, vector<Point2d> > posArr){
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream textStream(&file);
        QString saveStr;
        bool hasPos[122] = {false};
        size_t cyclecount = 50;
        foreach(int posIndex, PosId.keys()){
            if (posArr[posIndex].size() < cyclecount)
                cyclecount = posArr[posIndex].size();
            hasPos[121-posIndex] = true;
        }

        for (size_t i = 0; i < cyclecount; i++){
            if (hasPos[1])
                saveStr = QString::number(posArr[120][0].y,'f',12);
            else
                saveStr = "0";
            for (int j = 2; j < 122; j++){
                if (hasPos[j])
                    saveStr += "," + QString::number(posArr[121-j][i].y,'f',12);
                else
                    saveStr += ",0";
            }
            textStream<<saveStr<<endl;
        }

        textStream.flush();
        file.close();
    }
}

#define LINEHEIGHT 20
void DataHandler::printCT(QPainter &painter){
    painter.drawText(20,20,QString("software version: %1    time: %2").arg(DH_VERSION).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    int posY = 50;
    QRect ItemIndexRect(150,posY,50,LINEHEIGHT);
    QRect ItemCodeRect(200,posY,100,LINEHEIGHT);
    QRect ItemPosRect(300,posY,100,LINEHEIGHT);
    QRect ItemCTRect(400,posY,100,LINEHEIGHT);
    QRect CTRect(500,posY,100,LINEHEIGHT);

    int itemIndex = 1;
    //qDebug()<<"printCT,Name="<<Name;
    //qDebug()<<"printCT,AssayItem="<<AssayItem;
    //qDebug()<<"printCT,ItemName="<<ItemName;
    //qDebug()<<"printCT,PosId="<<PosId;
    painter.drawText(ItemIndexRect,Qt::AlignCenter,tr("序号"));
    painter.drawText(ItemCodeRect,Qt::AlignCenter,tr("代号"));
    painter.drawText(ItemCTRect,Qt::AlignCenter,tr("CT阀值"));
    painter.drawText(CTRect,Qt::AlignCenter,tr("CT平均值"));

    QMap<int,vector<int>> idArr;
    foreach(QString name, Name){
        for(QMap<int,QString>::iterator iter = AssayItem.begin(); iter != AssayItem.end(); iter++){
            if (ItemName[iter.value()] == name){
                vector<int> pos;
                int ctSum = 0;
                int ctNum = 0;
                foreach(int posIndex,PosId.keys()){
                    if (PosId[posIndex] == iter.key()){
                        if (CT[posIndex] > 0){
                            ctSum += CT[posIndex];
                            ctNum++;
                        }
                        pos.push_back(posIndex);
                    }
                }
                sort(pos.begin(),pos.end());
                idArr[iter.key()] = pos;

                posY += LINEHEIGHT;
                ItemIndexRect.moveTop(posY);
                ItemCodeRect.moveTop(posY);
                ItemCTRect.moveTop(posY);
                CTRect.moveTop(posY);
                painter.drawText(ItemIndexRect,Qt::AlignCenter,QString::number(itemIndex++));
                painter.drawText(ItemCodeRect,Qt::AlignCenter,iter.value());
                painter.drawText(ItemCTRect,Qt::AlignCenter,QString::number(ItemCT[iter.key()]));

                if (ctNum > 0)
                    painter.drawText(CTRect,Qt::AlignCenter,QString::number(static_cast<double>(ctSum)/10/ctNum,'f',2));
                else
                    painter.drawText(CTRect,Qt::AlignCenter,"0");
            }
        }
    }

    posY += LINEHEIGHT*3;
    ItemIndexRect.moveTop(posY);
    ItemCodeRect.moveTop(posY);
    ItemPosRect.moveTop(posY);
    ItemCTRect.moveTop(posY);
    CTRect.moveTop(posY);
    itemIndex = 1;
    painter.drawText(ItemIndexRect,Qt::AlignCenter,tr("序号"));
    painter.drawText(ItemCodeRect,Qt::AlignCenter,tr("代号"));
    painter.drawText(ItemPosRect,Qt::AlignCenter,tr("位置索引"));
    painter.drawText(ItemCTRect,Qt::AlignCenter,tr("CT阀值"));
    painter.drawText(CTRect,Qt::AlignCenter,tr("CT值"));
    for(QMap<int,vector<int>>::iterator iter = idArr.begin(); iter != idArr.end(); iter++)
    {
        for (size_t i = 0; i < iter.value().size(); i++)
        {
            int posIndex = iter.value()[i];
            posY += LINEHEIGHT;
            ItemIndexRect.moveTop(posY);
            ItemCodeRect.moveTop(posY);
            ItemPosRect.moveTop(posY);
            ItemCTRect.moveTop(posY);
            CTRect.moveTop(posY);

            painter.drawText(ItemIndexRect,Qt::AlignCenter,QString::number(itemIndex));
            painter.drawText(ItemCodeRect,Qt::AlignCenter,AssayItem[PosId[posIndex]]);
            painter.drawText(ItemPosRect,Qt::AlignCenter,QString::number(posIndex));
            painter.drawText(ItemCTRect,Qt::AlignCenter,QString::number(ItemCT[PosId[posIndex]]));
            painter.drawText(CTRect,Qt::AlignCenter,QString::number(static_cast<double>(CT[posIndex])/10));
            itemIndex++;
        }
    }
}

void DataHandler::printImage(QString dir,QPainter &painter){
    int x = 10;
    int y = 10;
    foreach(QString name, Name){
        if (QFile::exists(dir+"/"+name+".png")){
            painter.drawImage(x,y,QImage(dir+"/"+name+".png"));
            y += 700;
        }
    }
}

int DataHandler::NameSize(){
    return Name.length();
}

#define X0 130
#define Y0 600
#define XWidth 630
#define YHeight 550

#define LEGENDX 200
#define LEGENDY 70
#define LEGENDHEIGHT 20

static const QColor ColorIndex[] ={
    QColor(0,114,189)
    , QColor(217, 83, 25)
    , QColor(237, 177, 32)
    , QColor(126,47,142)
    , QColor(119,172,48)
    , QColor(77,190,238)
    , QColor(162, 20, 47)
    , QColor(0,114,189)
    , QColor(217, 83, 25)
    , QColor(237, 177, 32)
    , QColor(126,47,142)
    , QColor(119,172,48)
    , QColor(77,190,238)
    , QColor(162, 20, 47)
};

static const Qt::PenStyle PenStyleIndex[] = {
    Qt::SolidLine,
    Qt::DashLine,
    Qt::DashDotLine,
    Qt::DotLine
};

static void outputPng(QString dirname, QHash<int,vector<Point2d>> posArr, bool bRaw){
    qDebug()<<"outputPng"<<dirname;
    foreach(QString name, Name){
        double ruleValue = 3;
        QImage image(880,660,QImage::Format_ARGB32);
        QPainter painter(&image);
        QFont font;
        font.setPointSize(10);
        painter.setFont(font);
        image.fill("white");
        painter.setPen(QPen(QColor(0,0,0),1));
        QVector<QLine> lines;
        lines.append(QLine(QPoint(X0,Y0),QPoint(X0,Y0-YHeight)));
        lines.append(QLine(QPoint(X0,Y0),QPoint(X0+XWidth,Y0)));
        for (int i = 0; i < 10; i++){
            lines.append(QLine(QPoint(X0+i*70,Y0-5),QPoint(X0+i*70,Y0)));
            painter.drawText(QRect(X0+i*70-10,Y0+2,20,20),Qt::AlignCenter | Qt::AlignBottom,QString::number(i*5));
        }

        QMap<int,vector<int>> idArr;        
        QString meanStr = "";
        bool firstItem = true;
        for(QMap<int,QString>::iterator iter = AssayItem.begin(); iter != AssayItem.end(); iter++){
            if (ItemName[iter.value()] == name){
                vector<int> pos;
                int ctSum = 0;
                int ctNum = 0;
                foreach(int posIndex,posArr.keys()){
                    if (PosId[posIndex] == iter.key()){
                        if (CT[posIndex] > 0){
                            ctSum += CT[posIndex];
                            ctNum++;
                        }
                        pos.push_back(posIndex);
                    }
                }
                sort(pos.begin(),pos.end());
                idArr[iter.key()] = pos;

                if (!firstItem)
                    meanStr += ";  ";
                else
                    firstItem = false;
                meanStr += "mean-"+iter.value()+" = ";
                if (ctNum > 0)
                    meanStr += QString::number(static_cast<double>(ctSum)/10/ctNum,'f',2);
                else
                    meanStr += "0";
            }
        }

        int n = 0;
        for(QMap<int,vector<int>>::iterator iter = idArr.begin(); iter != idArr.end(); iter++){
            for (size_t i = 0; i < iter.value().size(); i++)
            {
                int posIndex = iter.value()[i];
                QString legend;
                if (bRaw)
                    legend = QString("%1 (%2,%3)").arg(AssayItem[PosId[posIndex]]).arg(posIndex%11).arg(posIndex/11+1);
                else
                    legend = QString("%1 (%2,%3) Ct = %4, BL = [%5 %6]").arg(AssayItem[PosId[posIndex]]).arg(posIndex%11).arg(posIndex/11+1).arg(static_cast<double>(CT[posIndex])/10).arg(BL[posIndex][0]).arg(BL[posIndex][1]);
                painter.drawText(QRect(LEGENDX+50,LEGENDY+n*LEGENDHEIGHT,YHeight,LEGENDHEIGHT),Qt::AlignVCenter | Qt::AlignLeft, legend);
                n++;
                for (size_t j = 0; j < posArr[posIndex].size(); j++){
                    if (posArr[posIndex][j].y > ruleValue*10)
                        ruleValue = ceil(posArr[posIndex][j].y/10);
                }
            }
        }
        for (int i = 0; i < 12; i++){
            lines.append(QLine(QPoint(X0,Y0-i*50),QPoint(X0+5,Y0-i*50)));
            painter.drawText(QRect(X0-32,Y0-i*50-10,30,20),Qt::AlignCenter | Qt::AlignBottom,QString::number(i*ruleValue-ruleValue));
        }
        painter.drawLines(lines);
        painter.save();
        qDebug()<<"png"<<name<<"rule="<<ruleValue;

        font.setPointSize(18);
        painter.setFont(font);
        if (bRaw)
            painter.drawText(QRect(0,10,880,30),Qt::AlignCenter, "z"+name);
        else
        {
            painter.drawText(QRect(0,10,880,30),Qt::AlignCenter, name);
            font.setPointSize(12);
            painter.setFont(font);
            painter.drawText(QRect(0,40,880,30),Qt::AlignCenter, meanStr);
        }

        font.setPointSize(16);
        painter.setFont(font);
        painter.drawText(QRect(X0,Y0+20,XWidth,30),Qt::AlignCenter | Qt::AlignBottom, "Cycle");

        painter.rotate(-90);
        painter.drawText(QRect(-Y0,X0-100,YHeight,60),Qt::AlignCenter, "delta Rn");
        painter.restore();

        //画线
        painter.setViewport(X0,Y0-YHeight,XWidth,YHeight-50);
        painter.setWindow(0,YHeight-50,XWidth,-YHeight+50);
        int panType = 0;
        n = 0;
        for(QMap<int,vector<int>>::iterator iter = idArr.begin(); iter != idArr.end(); iter++){
            for (size_t i = 0; i < iter.value().size(); i++)
            {
                int posIndex = iter.value()[i];
                if (!bRaw){
                    painter.setPen(QPen(QColor(0,0,0),2,Qt::SolidLine));
                    painter.drawLine(0,ItemCT[PosId[posIndex]]*50/ruleValue,XWidth,ItemCT[PosId[posIndex]]*50/ruleValue);
                }
                if (n < sizeof(ColorIndex)/sizeof(ColorIndex[0]))
                    painter.setPen(QPen(ColorIndex[n],2,PenStyleIndex[panType]));
                painter.drawLine(30,YHeight-80-n*LEGENDHEIGHT,110,YHeight-80-n*LEGENDHEIGHT);
                n++;
                QVector<QPointF> v;
                for (size_t j = 0; j < posArr[posIndex].size(); j++){
                    v.push_back(QPointF(posArr[posIndex][j].x*XWidth/45,posArr[posIndex][j].y*50/ruleValue));
                }
                painter.drawPolyline(v);
            }
            panType++;
        }

        if (bRaw)
            image.save(dirname+"/z"+name+".png");
        else
            image.save(dirname+"/"+name+".png");
    }
}

DataHandler::DataHandler()
{
}

QMap<int,int> DataHandler::getPosItemid(){
    return PosId;
}

QHash<int,float> DataHandler::getPosIntercept(){
    return PosIntercept;
}

/**********************************
 *      处理数据
 **********************************/
bool DataHandler::HandleData(int testId, QHash<int, vector<Point2d>> &posArr){    
    FillTestData(testId,posArr);
    //outputPosForMatlab("/home/lx/BSSignal.csv",posArr);
    RefPosFit(posArr);
    //RemoveNoise(posArr);
    //MoveMeanSmooth(posArr);
    //MoveMeanSmoothEx(posArr);
    //smoothPoint(posArr);
    //BaseLineFitEx(posArr);
    BaseLineFitF(posArr);    
    return true;
}

bool DataHandler::HandleDataWithStep(int testId, QHash<int, vector<Point2d> > &posArr, bool bRef, bool bSmooth, bool bBaseLine){    
    FillTestData(testId,posArr);
    if (bRef)
        RefPosFit(posArr);    
    if (bSmooth)
        smoothPoint(posArr);        
    if (bBaseLine)
        BaseLineFitF(posArr);
    return true;
}
bool DataHandler::HandleOnePointData(int testId, QHash<int, vector<Point2d> > &posArr){
    FillDirectData(testId,posArr);
    return true;
}

bool DataHandler::HandleOnePointDataEx(int testId, QHash<int, vector<Point2d> > &posArr){
    FillDirectData(testId,posArr);
    foreach(int posIndex,posArr.keys()){
        QString posStr;
        for (int i = 0; i < posArr[posIndex].size(); i++)
            posStr.append(","+QString::number(posArr[posIndex][i].y));
        qDebug()<<"posArr"<<posStr;
    }

    //RemoveNoise(posArr);
    BaseLineFitExx(posArr);
    //BaseLineFitEx(posArr);
    return true;
}

bool DataHandler::LoadData(QString filename, QHash<int, vector<Point2d> > &posArr){
    if (LoadTestData(filename,posArr))
    {
        RefPosFit(posArr);
        BaseLineFitF(posArr);
        return true;
    }
    return false;
}

bool DataHandler::SaveData(int testId,QString strdir){
    QHash<int, vector<Point2d> > posArr;
    QDir dir(strdir+"/LinuxData");
    if (!dir.exists())
        dir.mkpath(strdir+"/LinuxData");
    strdir += "/LinuxData";
    FillTestData(testId,posArr);
    outputPos(strdir+"/RawData.csv",rawData);
    outputPosForMatlab(strdir+"/BSSignal.csv",posArr);

    RefPosFit(posArr);
    outputPng(strdir,posArr,true);
    BaseLineFitF(posArr);
    getCT(posArr);
    outputPng(strdir,posArr,false);
    outputPosForMatlab(strdir+"/LinuxResult.csv",posArr);
    FillResultData(testId,posArr);
    //outputPdf(strdir+"/result.pdf",posArr);
    return true;
}

bool DataHandler::SaveOnePointData(int testId,QString strdir){
    QHash<int, vector<Point2d> > posArr;
    FillDirectData(testId,posArr);
    outputPos(strdir+"/Raw.csv",posArr);
    return true;
}

bool DataHandler::HandleFileData(QString filename, QString strdir){
    QHash<int, vector<Point2d> > posArr;
    if (LoadTestData(filename,posArr)){
        QDir dir(strdir+"/LinuxData");
        if (!dir.exists())
            dir.mkpath(strdir+"/LinuxData");
        strdir += "/LinuxData";
        outputPos(strdir+"/RawData.csv",rawData);
        outputPosForMatlab(strdir+"/BSSignal.csv",posArr);

        RefPosFit(posArr);
        outputPng(strdir,posArr,true);
        BaseLineFitF(posArr);
        getCT(posArr);
        outputPng(strdir,posArr,false);
        outputPosForMatlab(strdir+"/LinuxResult.csv",posArr);
        //outputPdf(strdir+"/result.pdf",posArr);
    }
    return true;
}

bool DataHandler::debugData(QString dirname, QHash<int, vector<Point2d> > &posArr){    
    LoadDebugData(dirname+"/BSSignal.csv",posArr);
    dirname += "/LinuxData";
    QDir dir(dirname);
    if (!dir.exists())
        dir.mkdir(dirname);
    outputPosForMatlab(dirname+"/BSSignal.csv",posArr);
    RefPosFit(posArr);
    outputPng(dirname,posArr,true);
    BaseLineFitF(posArr);
    getCT(posArr);
    outputPng(dirname,posArr,false);
    outputPosForMatlab(dirname+"/LinuxResult.csv",posArr);
    return true;
}

void DataHandler::setPosItemid(uchar *data){
    PosId.clear();
    for (int i = 0; i < 121; i++){
        if (data[i] != 0)
            PosId[121-i] = data[i];
    }
    //qDebug()<<"setPosItemid"<<PosId;
}

bool DataHandler::RawdataToMatlab(QString dirname){
    QHash<int, vector<Point2d> > posArr;
    if (LoadTestData(dirname+"/RawData.csv",posArr)){
        outputPosForMatlab(dirname+"/BSSignal.csv",posArr);
    }
    return true;
}

QHash<int,int> DataHandler::getCurrCT(){
    return CT;
}

QHash<int,int> DataHandler::getCurrItemCT(){
    return CurrItemCT;
}
