#pragma once
#include <q3.h>

q3BodyDef DiceDefinition(q3Vec3 position = q3Vec3(0.f, 0.f, 0.f));
q3BoxDef SquareBoxDefinition();
q3Body *CreateBox(q3Scene &scene, q3BodyDef bodyDef, q3BoxDef boxDef);
q3Body *ThrowDie(q3Scene &scene, q3Vec3 position = q3Vec3(0.f,5.f,0.f));