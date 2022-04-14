#include "q3Utils.h"

q3BodyDef DiceDefinition(q3Vec3 position)
{
    static const float INITIAL_ANGULAR_MIN = 2.f;
    static const float INITIAL_ANGULAR_MAX = 4.f;

    static const float INITIAL_LINEAR_X_MIN = 1.f;
    static const float INITIAL_LINEAR_X_MAX = 3.f;
    static const float INITIAL_LINEAR_Y_MIN = 1.f;
    static const float INITIAL_LINEAR_Y_MAX = 1.5f;
    static const float INITIAL_LINEAR_Z_MIN = 3.f;
    static const float INITIAL_LINEAR_Z_MAX = 6.f;

    q3BodyDef definition;
    definition.position = position;
    /*
    definition.axis.Set(
        q3RandomFloat(-1.0f, 1.0f),
        q3RandomFloat(-1.0f, 1.0f),
        q3RandomFloat(-1.0f, 1.0f));
        */
    // definition.angle = q3PI * q3RandomFloat(-1.0f, 1.0f);
    // definition.angle = 0;

    definition.bodyType = eDynamicBody;
    definition.angularVelocity.Set(
        q3RandomFloat(INITIAL_ANGULAR_MIN, INITIAL_ANGULAR_MAX),
        q3RandomFloat(INITIAL_ANGULAR_MIN, INITIAL_ANGULAR_MAX),
        q3RandomFloat(INITIAL_ANGULAR_MIN, INITIAL_ANGULAR_MAX));

    definition.angularVelocity *= q3Sign(q3RandomFloat(-1.0f, 1.0f));

    definition.linearVelocity.Set(
        q3RandomFloat(INITIAL_LINEAR_X_MIN, INITIAL_LINEAR_X_MAX),
        q3RandomFloat(INITIAL_LINEAR_Y_MIN, INITIAL_LINEAR_Y_MAX),
        q3RandomFloat(INITIAL_LINEAR_Z_MIN, INITIAL_LINEAR_Z_MAX));
    //definition.linearVelocity *= q3Sign(q3RandomFloat(-1.0f, 1.0f));
    definition.linearVelocity *= q3Sign(1.0f);
    //  definition.gravityScale = 1.f;
    return definition;
}

q3BoxDef SquareBoxDefinition()
{
    q3BoxDef definition;
    definition.SetRestitution(0);
    q3Transform tx;
    q3Identity(tx);
    definition.Set(tx, q3Vec3(1.0f, 1.0f, 1.0f));
    return definition;
}

q3Body *CreateBox(q3Scene &scene, q3BodyDef bodyDef, q3BoxDef boxDef)
{
    q3Body *body = scene.CreateBody(bodyDef);
    body->AddBox(boxDef);
    return body;
}

q3Body *ThrowDie(q3Scene &scene, q3Vec3 position)
{
    static const q3BoxDef boxDef = SquareBoxDefinition();
    // Not static since it regenerates initial motion values on each throw
    const q3BodyDef diceDef = DiceDefinition(position);

    return CreateBox(scene, diceDef, boxDef);
}