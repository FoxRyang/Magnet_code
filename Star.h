#pragma once

#include "cocos2d.h"

class Star : public cocos2d::Node
{
public:
	virtual bool init(cocos2d::Point point); 

	Star::Star(cocos2d::Point point);

	int getState() const;

	cocos2d::Sprite* star;

	cocos2d::Point getposition() const;
    
    // state 찾기 위해서.

private:
    float radius;
    float range_radius;
    float range_scale;

};

