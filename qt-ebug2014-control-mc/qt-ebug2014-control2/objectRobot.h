#include <QObject>

class ObjectRobot : public QObject
{
    Q_OBJECT

    public:
        ObjectRobot(QObject *parent = 0);
        ~ObjectRobot();

    signals:

    public slots:

    private:
        QString internalKnowledge

};
