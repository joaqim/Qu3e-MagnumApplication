#pragma once

#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_EMSCRIPTEN
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/EmscriptenApplication.h>
#endif
#include <Magnum/Timeline.h>


#include "Renderer.cpp"
#include "q3Utils.h"

#include <q3.h>

using namespace Magnum;
float const DELTA = 1.0f / 60.0f;
int const NUM_DICE = 5;

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

    void throwDice();

    q3Scene scene;
    q3Body *_dice[NUM_DICE];
    q3Body *_pDice;
    q3Body *_pFloor;

    Renderer renderer;
    Timeline _timeline;
    Int _substeps = 1;
};