#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include <QObject>
#include <QVector>

#include<array>
#include<queue>
#include<bitset>

#include<Eigen/Dense>


#define MAX_LEDS 256

#define WIDTH 1280
#define FOCAL_WIDTH (1*WIDTH)
#define SCALE 0.5f


/***********************
 * Datatype for storing absolute position
 * of robot data including position, orientation and ID
 * from the perspective of the World.
 * **************************************/

#include <QPointF>

struct dataRobotLocation{
    QPointF worldPossition;
    int robotId;
    float worldOrientation;
};



class MessageTranslator : public QObject
{
    Q_OBJECT
private:
    static const float pi;
    static const std::array<std::array<int,16>,15> ledSequences;
    struct dataLed{int x,y,size, colour;};
    struct dataEllipse{float x,y,rx,ry,t;};
    QVector<dataRobotLocation> knn_graph_partition(int numLeds);
    dataRobotLocation identify(std::vector<int> good);
    dataEllipse fitEllipse(std::vector<int> &component);
    float ellipse_to_circle(float phi,float rz);
    float min_rounding(std::vector<float> &angles);
    float circle_to_ellipse(float theta,float rz);
    Eigen::Vector3f eig(Eigen::Matrix3f &M);
    std::vector<int> getGoodLEDPoints(std::vector<int> vecLeds);
    QVector<dataLed> MessageTranslator::getLeds(QByteArray message, qint32 numLeds);
    qint32 MessageTranslator::getNumLeds(QByteArray btyarrMessage);
    int MessageTranslator::getBits(int from, int to, char byte);

public:
    explicit MessageTranslator(QObject *parent = 0);

signals:
    void translatedData(QVector<dataRobotLocation>);

public slots:
    void newMsg(QByteArray);

};

#endif // MESSAGETRANSLATOR_H
