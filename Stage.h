#pragma once

#include "cocos2d.h"

class ball;
class ground;
class PullPush;

class Stage : public cocos2d::Scene
{
public:
    virtual bool init();
    CREATE_FUNC(Stage);
};