#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QGenericMatrix>

#include "ThreadableQObject.h"
#include "dataRobotLocation.h"

#include<array>
#include<queue>
#include<bitset>

#include <math.h>

#include<Eigen/Dense>


#define MAX_LEDS 256

#define WIDTH 1280
#define FOCAL_WIDTH (1*WIDTH)
#define SCALE 0.5f

class MessageTranslator  : public QThread
{
    Q_OBJECT
private:
    static const std::array<std::array<int,16>,15> ledSequences;
    struct dataLed{int x,y,colour,size;};
    struct dataEllipse{float x,y,rx,ry,t;};

    QList<dataLed> getLeds(QByteArray message);
    int getBits(int from, int to, char byte);



//    QList<dataRobotLocation> knn_graph_partition(int numLeds);
//    dataRobotLocation identify(std::vector<int> good);
//    dataEllipse fitEllipse(std::vector<int> &component);
//    float ellipse_to_circle(float phi,float rz);
//    float min_rounding(std::vector<float> &angles);
//    float circle_to_ellipse(float theta,float rz);
//    Eigen::Vector3f eig(Eigen::Matrix3f &M);
//    std::vector<int> getGoodLEDPoints(std::vector<int> vecLeds);
//    QVector<dataLed> MessageTranslator::getLeds(QByteArray message, qint32 numLeds);
//    qint32 MessageTranslator::getNumLeds(QByteArray btyarrMessage);

public:
    explicit MessageTranslator(QObject *parent = 0);
    ~MessageTranslator();
    void translate(QByteArray btyarMessage);

signals:
    void translationDone(QList<dataRobotLocation>);
    void ledsDone(QList<QString>);

public slots:
    void run();

};

#endif // MESSAGETRANSLATOR_H
