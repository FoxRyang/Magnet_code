#include "Star.h"

using namespace cocos2d;

bool Star::init(Point point)
{

    star = Sprite::create("Star.png");


    star->setPosition(point);

	addChild(star);

    return true;
}

Star::Star(Point point)
{
    init(point);
}

cocos2d::Point Star::getposition() const
{
	return star->getPosition();
}