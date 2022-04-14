#include "MyApplication.h"

#include <stdlib.h> // For rand
#include <time.h>   // for rand seed time(NULL)

void MyApplication::throwDice()
{
    static float const DICE_POS_X = -5.f;
    static float const DICE_POS_Z = -5.f;
    static float const DICE_POS_OFFSET_X = 1.5f;
    static float const DICE_POS_OFFSET_Z = 0.f;
    static float const DICE_POS_HEIGHT = 5.f;

    for (int n = 0; n < NUM_DICE; n += 1)
    {
        auto die = _dice[n];
        if (die != nullptr)
            scene.RemoveBody(die);
        _dice[n] = ThrowDie(scene, q3Vec3(
                                       DICE_POS_X + DICE_POS_OFFSET_X * n,
                                       DICE_POS_HEIGHT,
                                       DICE_POS_Z + DICE_POS_OFFSET_Z * n));
    }
}

void MyApplication::initScene()
{
    q3BodyDef bodyDef;
    q3BoxDef boxDef;

    boxDef.SetRestitution(0);
    q3Transform tx;
    q3Identity(tx);
    boxDef.Set(tx, q3Vec3(20.0f, 1.0f, 20.0f));
    _pFloor = CreateBox(scene, bodyDef, boxDef);
}

MyApplication::MyApplication(const Arguments &arguments) : Platform::Application{arguments}, scene(DELTA)
{
    scene.SetIterations(1);
    scene.SetEnableFriction(true);
    scene.SetAllowSleep(true);

    srand(time(NULL));

    initScene();

    for (int n = 0; n < NUM_DICE; n += 1)
    {
        _dice[n] = nullptr;
    }
    throwDice();

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
    for (auto die : _dice)
    {
        renderer.DrawDie(die);
    }

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

    throwDice();

    event.setAccepted();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
