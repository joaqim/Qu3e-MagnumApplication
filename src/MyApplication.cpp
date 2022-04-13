#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_EMSCRIPTEN
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/EmscriptenApplication.h>
#endif

#include <Magnum/Timeline.h>

#include "Renderer.cpp"

#include <q3.h>

#include <stdlib.h>
#include <time.h>

using namespace Magnum;
float DELTA = 1.0f / 60.0f;

class MyApplication : public Platform::Application
{
public:
    explicit MyApplication(const Arguments &arguments);
    ~MyApplication();

    void mousePressEvent(MouseEvent &event) override;

private:
    void drawEvent() override;
    void initScene();
    void updateScene();
    void fastestStep();

    q3Scene scene;
    q3Body *_pDice;
    q3Body *_pFloor;

    Renderer renderer;
    Timeline _timeline;
    Int _substeps = 1;
};

q3BodyDef ContainerDefinition() {
    q3BodyDef definition;

    q3Transform tx;
    definition.Set(tx, q3Vec3(8.0f, 1.0f, 8.0f));

    definition.
}

q3BodyDef DiceDefinition()
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
    definition.position.Set(0.0f, 5.0f, 0.0f);
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

q3Body *ThrowDie(q3Scene &scene);
q3Body *ThrowDie(q3Scene &scene)
{
    static const q3BoxDef boxDef = SquareBoxDefinition();
    // Not static since it regenerates initial motion values on each throw
    const q3BodyDef diceDef = DiceDefinition();
    return CreateBox(scene, diceDef, boxDef);
}

void MyApplication::initScene()
{
    q3BodyDef bodyDef;
    q3BoxDef boxDef;

    boxDef.SetRestitution(0);
    q3Transform tx;
    q3Identity(tx);
    boxDef.Set(tx, q3Vec3(50.0f, 1.0f, 50.0f));
    _pFloor = CreateBox(scene, bodyDef, boxDef);

    /*
    bodyDef.bodyType = eDynamicBody;
    bodyDef.position.Set(0, 5.0f, 0);
    for (int i = 0; i < 2; ++i)
    {
        tx.position.Set(q3RandomFloat(-5.0f, 5.0f), q3RandomFloat(-5.0f, 5.0f), q3RandomFloat(10.0f, 20.0f));
        // tx.position.Set(5.f, 0.f, q3RandomFloat(5.0f, 10.0f));

        boxDef.Set(tx, q3Vec3(1.0f, 1.0f, 1.0f));
        CreateBox(scene, bodyDef, boxDef);
    }
    */
}

MyApplication::MyApplication(const Arguments &arguments) : Platform::Application{arguments}, scene(DELTA)
{
    scene.SetIterations(1);
    scene.SetEnableFriction(true);
    scene.SetAllowSleep(true);

    srand(time(0));

    initScene();
    _pDice = ThrowDie(scene);

    /* Start the timer, loop at 60 Hz max */
    setSwapInterval(1);
    setMinimalLoopPeriod(16);

    _timeline.start();
}

MyApplication::~MyApplication()
{
    this->scene.RemoveAllBodies();
}

void MyApplication::drawEvent()
{
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    updateScene();
    // fastestStep();

    // scene.Render(&renderer);
    renderer.DrawFloor(_pFloor);
    renderer.DrawDie(_pDice);

    swapBuffers();
    _timeline.nextFrame();

    redraw();
}

void MyApplication::fastestStep()
{
    /* Adjust the substep number to maximize CPU usage each frame */
    const Float lastAvgStepTime = _timeline.previousFrameDuration() / Float(_substeps);
    const Int newSubsteps = lastAvgStepTime > 0 ? Int(DELTA / lastAvgStepTime) + 1 : 1;
    if (Math::abs(newSubsteps - _substeps) > 1)
        _substeps = newSubsteps;
    // TODO: scene.Step() expects to be called at fixed delta, not at timestepped interval
    for (Int i = 0; i < _substeps; ++i)
        scene.Step();
}

void MyApplication::updateScene()
{
    const Float time = _timeline.previousFrameDuration();
    static float accumulator = 0;
    accumulator += time;

    q3Clamp01(accumulator);
    while (accumulator >= DELTA)
    {
        scene.Step();
        accumulator -= DELTA;
    }
}

void MyApplication::mousePressEvent(MouseEvent &event)
{
    if (event.button() != MouseEvent::Button::Left)
        return;

    scene.RemoveBody(_pDice);
    _pDice = ThrowDie(scene);

    event.setAccepted();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
