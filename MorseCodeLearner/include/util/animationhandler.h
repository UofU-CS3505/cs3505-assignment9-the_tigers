#ifndef ANIMATIONHANDLER_H
#define ANIMATIONHANDLER_H

#include <Box2D/Box2D.h>
#include <QTimer>

class animationhandler
{
public:
    animationhandler(b2World world);

signals:
    void objectShake(b2Vec2 position);
    void objectJump(int height);

private:
    b2World world;
    b2Body* shakeBody;
    b2Body* jumpBody;
    b2Body* shakeAnchor;
    b2DistanceJoint* distanceJoint;
    QTimer timer;

    int shakeObjectX;
    int shakeObjectY;

    int jumpObjectX;
    int jumpObjectY;

private slots:
    void updateWorld();
    void shakeObject();
    void jumpObject();
};

#endif // ANIMATIONHANDLER_H
