#include "animationhandler.h"

animationhandler::animationhandler(b2World world)
    : world(b2Vec2(0, -10.00f))
    {
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2BodyDef shakeAnchorDefinition;
    shakeAnchorDefinition.type = b2_staticBody;
    shakeAnchorDefinition.position.Set(0.0f, 3.0f);
    shakeAnchor = world.CreateBody(&shakeAnchorDefinition);

    b2BodyDef shakeBodyDefinition;
    shakeBodyDefinition.type = b2_dynamicBody;
    shakeBodyDefinition.position.Set(0.0f, 1.0f);
    shakeBody = world.CreateBody(&shakeBodyDefinition);

    b2BodyDef jumpDefinition;
    jumpDefinition.type = b2_dynamicBody;
    jumpDefinition.position.Set(8.0f, 0.0f);
    jumpBody = world.CreateBody(&jumpDefinition);

    b2DistanceJointDef distanceJointDefinition;
    distanceJointDefinition.bodyA = shakeAnchor;
    distanceJointDefinition.bodyB = shakeBody;
    distanceJointDefinition.collideConnected = false;
    distanceJoint = (b2DistanceJoint*) world.CreateJoint(&distanceJointDefinition);

    b2FixtureDef shakeFixtureDefinition;
    shakeFixtureDefinition.shape = &dynamicBox;
    shakeFixtureDefinition.density = 1.0f;
    shakeFixtureDefinition.friction = 0.3f;

    b2FixtureDef jumpFixtureDefinition;
    jumpFixtureDefinition.shape = &dynamicBox;
    jumpFixtureDefinition.density = 10.0f;
    jumpFixtureDefinition.friction = 0.3f;

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;

    fixtureDef.restitution = 0.9f;

    // Add the shapes to the body.
    shakeBody->CreateFixture(&shakeFixtureDefinition);
    jumpBody->CreateFixture(&jumpFixtureDefinition);

    //timer.singleShot(10, this, &animationhandler::updateWorld);
}

    void animationhandler::updateWorld(){
        float32 timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;

        // Instruct the world to perform a single step of simulation.
        // It is generally best to keep the time step and iterations fixed.
        world.Step(timeStep, velocityIterations, positionIterations);

        // Now print the position and angle of the body.
        b2Vec2 shakePosition = shakeBody->GetPosition();

        //ui->shakeButton->move(shakeObjectX + shakeBody->GetPosition().x, shakeObjectY + shakeBody->GetPosition().y);
        //ui->jumpButton->move(jumpObjectX, jumpObjectY - jumpBody->GetPosition().y);
        //timer.singleShot(10, this, &animationhandler::updateWorld);
    }

    void animationhandler::shakeObject()
    {
        shakeBody->ApplyForceToCenter(b2Vec2(50000, 0), true);
    }

    void animationhandler::jumpObject()
    {
        jumpBody->SetTransform(b2Vec2(0.0f, 8.0f), 0);
        jumpBody->SetAwake(true);
    }
