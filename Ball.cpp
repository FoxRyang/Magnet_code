#include "Ball.h"
#include "Ground.h"
#include "PullPush.h"
using namespace cocos2d;

#define damp 0.17

bool Ball::init(float radius)
{
	sprite = Sprite::create("plus_ball.png");
	float size = sprite->getContentSize().width;
	float scale = radius / size * 2;
	ball_scale = scale;
	sprite->setScale(scale);
	setPosition(Point::ZERO);
	addChild(sprite);

	resetpoint = Point::ZERO;
	velocity = Point::ZERO;
	acceleration = Point::ZERO;
	mass = 0.01f;
	stagecount = 1;

	setContentSize(sprite->getContentSize() * scale);
	setattached(0);

	gravity = Point::ZERO;

	return true;
}

Ball* Ball::create(float Radius)
{
	Ball* ret = new Ball();
	if((ret != nullptr) && ret->init(Radius))
		ret->autorelease();
	else
		CC_SAFE_RELEASE_NULL(ret);
	ret->radius = Radius;
	return ret;
}

void Ball::SetResetPoint(Point a)
{
	resetpoint = a;
}

cocos2d::Point Ball::getResetPoint()
{
	return resetpoint;
}

float Ball::getRadius() const
{
	return radius;
}

int Ball::getattached() const
{
	return attached;
}

int Ball::setattached(int a)
{
	return attached = a;
}

void Ball::initForce()
{
	acceleration = Point::ZERO;
}

void Ball::applyForce(const Point& force)
{
	acceleration += force / mass;
}

void Ball::resetVelocity()
{
	velocity = Point::ZERO;
}

void Ball::XresetVelocity()
{
	velocity.x = 0;
}

void Ball::YresetVelocity()
{
	velocity.y = 0;
}

void Ball::sameVelocity(int direction)
{
	if(attached == 0){
		if (direction == 1)
		{
			if(velocity.x < 200.0f)
				applyForce(Point(10.0f,0));

			//velocity.x = 200.0f;
		}

		if (direction == -1)
		{
			if(velocity.x > -200.0f)
				applyForce(Point(-10.0f,0));
			//velocity.x = 200.0f;
		}
	}else{
		if (direction == 1)
		{
			angle -= 0.1;
		}

		if (direction == -1)
		{
			angle += 0.1;
		}
	}
}

void Ball::updatePhysics(Ball* ball, CCArray* groundarray, CCArray* pullpusharray, float deltaTime)
{


	Point position = getPosition();
	oldposition = getPosition();

	CCObject* obj1 = NULL;
	CCObject* obj2 = NULL;



	if ((ball->getattached()) == 0)
	{            
		ball->applyForce(gravity);
		ball->pushforce(ball,pullpusharray);
	}


	CCARRAY_FOREACH(groundarray, obj2)
	{
		Ground* ground = (Ground*)obj2;
		Point oldvelocity = velocity;
		ColideBarrier(ball, ground, (ball->getPosition()));
	}

	CCARRAY_FOREACH(pullpusharray, obj1)
	{
		PullPush* pullpush = (PullPush*)obj1;

		if(pullpush->able){
			if(getattached() == 0)
				pushbarrier(ball, pullpush, (ball->getPosition()));
			else
			{										// 그냥 붙었을 경우를 아예 따로 처리해버림.
				/*if(acceleration.x > 0)						// 각도 바뀌는 경우
				{
				angle-=0.1;
				acceleration.x = 0;
				}
				else if(acceleration.x < 0)
				{
				angle+=0.1;
				acceleration.x = 0;
				}*/

				position = anch + Point(circle_radius * cos(angle),circle_radius * sin(angle));
				// 포지션을 고정시켜서 돌려버림
				ball->setPosition(position);
			}
		}
	}

	velocity += acceleration * deltaTime;
	ball -> setPosition(ball->getPosition() + velocity * deltaTime);



}

void Ball::ColideBarrier(Ball* ball, const Ground* ground, Point pos)
{
	Point temp_ballposition = getPosition();
	Point temp_groundposition = ground->getPosition();
	float temp_ballsize = getContentSize().height;
	float temp_Xgroundsize = ground->getContentSize().width;
	float temp_Ygroundsize = ground->getContentSize().height;
	bool what = false; // 붙었는지 안붙었는지 (땅이랑)


	if ((0 <= (pos.x - temp_groundposition.x)+temp_ballsize/4) && ((pos.x - temp_groundposition.x) <= (ground->getContentSize().width)+temp_ballsize/4))
	{
		what = false;

		if (((pos.y - temp_groundposition.y) > 0) && ((pos.y - temp_groundposition.y) <= (temp_Ygroundsize + temp_ballsize/2)))		// 위쪽
		{
			/*
			temp_ballposition = Point(pos.x,temp_groundposition.y + temp_Ygroundsize + temp_ballsize/2);
			velocity.y = 0;
			acceleration.y = 0;
			ball->setPosition(temp_ballposition);
			*/

			temp_ballposition = Point(0,temp_groundposition.y + temp_Ygroundsize + temp_ballsize/2 - pos.y);	// 원래 있어야 하는 자리와 지금 자리의 차이만큼 더해주는 방식으로 변경
			//ball->setPosition(ball->getPosition()+temp_ballposition);
			if(velocity.y < 0){
				velocity.y = -velocity.y * damp;
			}
			acceleration.y = 0;
			what = true;
		}
		else if (((pos.y - (temp_groundposition.y + temp_Ygroundsize) <= 0)) && (abs(pos.y - temp_groundposition.y) <= temp_ballsize/2))
		{
			/*
			temp_ballposition = Point(pos.x,temp_groundposition.y - temp_ballsize/2);
			velocity.y = 0;
			acceleration.y = 0;
			ball->setPosition(temp_ballposition);
			*/

			temp_ballposition = Point(0,temp_groundposition.y - temp_ballsize/2 - pos.y);
			//ball->setPosition(ball->getPosition() + temp_ballposition);
			if(velocity.y > 0){
				velocity.y = -velocity.y * damp;
			}
			acceleration.y = 0;
			what = true;

		}        
	}
	if ((-temp_ballsize/3 < (pos.y - temp_groundposition.y)) && ((pos.y - temp_groundposition.y) < (temp_Ygroundsize + temp_ballsize/3)))
	{
		if (((pos.x - temp_groundposition.x - temp_ballsize/2) >= 0) && ((pos.x - temp_groundposition.x) <= (temp_Xgroundsize + temp_ballsize/2)))
		{/*
		 temp_ballposition = Point(temp_groundposition.x + temp_Xgroundsize + temp_ballsize/2 ,pos.y);
		 velocity.x = 0;
		 acceleration.x = ;0
		 ball->setPosition(temp_ballposition);*/
			temp_ballposition = Point(temp_groundposition.x + temp_Xgroundsize + temp_ballsize/2 - pos.x,0);
			//ball->setPosition(ball->getPosition() + temp_ballposition);
			if(velocity.x < 0){
				velocity.x = -velocity.x * damp;
			}
			acceleration.x = 0;
		}
		else if (( ((pos.x - temp_groundposition.x) < temp_ballsize/2) && (pos.x - temp_groundposition.x) > - temp_ballsize/2)
			|| ((abs(pos.x- temp_groundposition.x) <= temp_ballsize/2)) && abs(pos.x - temp_groundposition.x >= - temp_ballsize/2))
		{/*
		 temp_ballposition = Point(temp_groundposition.x - temp_ballsize/2 ,pos.y);
		 velocity.x = 0;
		 acceleration.x = 0;
		 ball->setPosition(temp_ballposition);*/
			temp_ballposition = Point(temp_groundposition.x - temp_ballsize/2 - pos.x,0);
			//ball->setPosition(ball->getPosition() + temp_ballposition);
			if(velocity.x > 0){
				velocity.x = -velocity.x * damp;
			}
			acceleration.x = 0;
		}

	} 

	if(what){
		if(velocity.x > 0){
			if(velocity.x < 8)
				velocity.x = 0;
			else
				velocity.x += -8.0f;
		}else if(velocity.x < 0){
			if(velocity.x > -8)
				velocity.x = 0;
			else
				velocity.x += 8.0f;
		}
	}
}

void Ball::pushforce(Ball* ball, CCArray* pullpusharray)
{
	CCObject* obj1 = NULL;

	CCARRAY_FOREACH(pullpusharray, obj1)
	{
		PullPush* pullpush = (PullPush*)obj1;

		Point ball_position = getPosition();
		Point pullpush_position = pullpush->getPosition();
		float temp_ballsize = this->radius;
		float temp_pullpushsize = pullpush->getrangesize().height;
		float temp_rangesize = pullpush->getrangesize().height;
		float force_x;
		float force_y;

		float force;
		float delta_x;
		float delta_y;
		float distance;

		delta_x = pullpush_position.x-ball_position.x;
		delta_y = pullpush_position.y-ball_position.y;
		distance = ccpDistance(pullpush_position, ball_position);

		if (distance < temp_rangesize/2)
		{
			force = float(pullpush->getState()) * pullpush->getRadius() * 9000 / pow(distance, 2);
			//	force = float(pullpush->getState()) * 1000.0f / distance;
			if(pullpush->getState() == -1)
				force /= 2;

			if(force > pullpush->getRadius() * 40)
				force = pullpush->getRadius() * 40;
			else if(force < -pullpush->getRadius() * 40)
				force = -pullpush->getRadius() * 40;

			applyForce(Point(force * (delta_x/distance), force * (delta_y/distance)));

		}
	}

}

void Ball::pushbarrier(Ball* ball, const PullPush* pullpush, Point pos)
{
	Point temp_ballposition = pos;
	Point temp_pullpushposition = pullpush->getPosition();
	float temp_ballsize = getContentSize().height;
	float temp_pullpushsize = pullpush->getContentSize().height;
	float distance = ccpDistance(temp_ballposition, temp_pullpushposition);
	float ratio;
	float bitbyeon;
	float temp_x;
	float temp_y;

	//닮음비를 이용해서 공을 되돌리려고 했으나 잘 안됨

	if(((temp_ballsize/2) + (temp_pullpushsize/2)) >= distance)
	{
		// 붙을 경우에 attached의 상태만 바꾸어줌. 아 물론 나머지 정보도 긁어옴.
		if (pullpush->getState() == 1)     
			setattached(1);

		velocity = Point::ZERO;
		acceleration = Point::ZERO;
		angle = atan2f(this->getPositionY() - pullpush->getPositionY(), this->getPositionX() - pullpush->getPositionX());
		//angle = CC_RADIANS_TO_DEGREES(angle);
		anch = pullpush->getPosition();
		circle_radius = pullpush->getRadius()*pullpush->getScale() + this->getRadius()*this->getScale();
	}

}