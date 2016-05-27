#include "Stage.h"
#include "Ball.h"
#include "PullPush.h"
#include "ground.h"
#include "Star.h"
using namespace cocos2d;

class StageLayer : public Layer
{

public:
    CREATE_FUNC(StageLayer);

    Ball* ball;
	Star* star;
    PullPush* pullpush;
    PullPush* pullpush2;
    Ground* ground1;
    Ground* ground2;
	Ground* ground3;
    CCArray* pullpusharray;
    CCArray* groundarray;
    boolean rightpress;
    boolean leftpress;
    Size winSize;
    CCLabelTTF* pLabel;
    int lifecount;

    virtual bool init()
    {
        if(!Layer::init())
            return false;

        pullpusharray = CCArray::create();
        groundarray = CCArray::create();
        pullpusharray -> retain();
        groundarray -> retain();
        lifecount = 5;

        std::string stringlife = std::to_string(lifecount);
        pLabel = CCLabelTTF::create("Life : " + stringlife, "Thonburi", 30);
		pLabel->setPosition(100, this->getContentSize().height - 40);
        pLabel->setColor(ccc3(255,255,255));
        this->addChild(pLabel, 1);

        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = CC_CALLBACK_2(StageLayer::onKeyPressed, this);
        listener->onKeyReleased = CC_CALLBACK_2(StageLayer::onKeyReleased, this);
    
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        Stagesender(1);
    }

    void Stagesender(int stagecount)
    {
        switch(stagecount)
        {
        case (1):
            ball = Ball::create(12.0f);
			ball->gravity = Point(0.0f,-6.0f);

            ball->setPosition(Point(150, 15));
            addChild(ball);

            pullpush = PullPush::create(12.0f, 120.0f, 2);
            pullpush -> setPosition (Point(350, 120));
            addChild(pullpush);
            pullpusharray -> addObject(pullpush);

            pullpush = PullPush::create(12.0f, 120.0f, 1);
            pullpush -> setPosition (Point(650, 120));
            addChild(pullpush);
            pullpusharray -> addObject(pullpush);

            addChild(ground1 = Ground::create(Rect(0, 0, 300, 15)));
            addChild(ground2 = Ground::create(Rect(1050, 0, 300, 15)));
            groundarray -> addObject(ground1);
            groundarray -> addObject(ground2);

			addChild(ground3 = Ground::create(Rect(900, 100, 500, 15)));
            groundarray -> addObject(ground3);
        
            ball->SetResetPoint(Point(20,50));
			ball->setPosition(ball->getResetPoint());

			star = new Star(Point(1200, 50));
			addChild(star);



            scheduleUpdate();
            break;
			//////////////////////////////////////////
		case (2):
            ball = Ball::create(12.0f);
			ball->gravity = Point(0.0f,-6.0f);

            ball->setPosition(Point(150, 15));
            addChild(ball);

            pullpush = PullPush::create(12.0f, 120.0f, 2);
            pullpush -> setPosition (Point(350, 120));
            addChild(pullpush);
            pullpusharray -> addObject(pullpush);

            pullpush = PullPush::create(12.0f, 120.0f, 1);
            pullpush -> setPosition (Point(650, 120));
            addChild(pullpush);
            pullpusharray -> addObject(pullpush);

            addChild(ground1 = Ground::create(Rect(0, 0, 300, 15)));
            addChild(ground2 = Ground::create(Rect(900, 0, 300, 15)));
            groundarray -> addObject(ground1);
            groundarray -> addObject(ground2);
        
            ball->SetResetPoint(Point(20,50));
			ball->setPosition(ball->getResetPoint());

            scheduleUpdate();
            break;
			///////////////////////////////////////////
        default:
            break;
            
        }
    }

    virtual void update(float deltaTime) override
    {
        //attached라는 변수로 붙었나 안붙었나 체크 만약에 pullpush에 붙었으면 힘을 작용안하는 방식으로 간다.

		ball->initForce();

        if (rightpress == true)
        {
            if ((ball->getattached()) == 0)
            {
                ball->sameVelocity(1);
            }
            if ((ball->getattached()) == 1)
            {
                ball->sameVelocity(1);
            }
        }
        
        if (leftpress == true)
        {
            if ((ball->getattached()) == 0)
            {
                ball->sameVelocity(-1);
            }
            if ((ball->getattached()) == 1)
            {
				ball->sameVelocity(-1);
            }
        }

        ball->updatePhysics(ball, groundarray, pullpusharray, deltaTime);

        if ((ball->getPositionY()) < -(200 + (ball->getContentSize().height)/2))
        {/*
            Point resetpoint = ball->getResetPoint();            
            ball->setPosition(resetpoint.x, resetpoint.y);*/
			
			lifecount -= 1;
			
			ball->setPosition(Point(20,50));		// Key_R 부분을 복붙했음. (속도가 그대로라서)
            ball->resetVelocity();
			ball->setattached(0);
			
			
			pLabel->setString("Life : " + std::to_string(lifecount));

			/*if(lifecount == 0){
				CCScene* pScene = PrototypeScene::create();
				CCDirector::sharedDirector()->pushScene(pScene);
			}*/
			
        }
        Layer::update(deltaTime);
    }

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
    {
	    if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	    {
		    rightpress = true;
	    }

	    if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	    {
		    leftpress = true;
	    }


	    if (keyCode == EventKeyboard::KeyCode::KEY_R)
	    {
		    ball->setPosition(Point(20,50));
            ball->resetVelocity();
			ball->setattached(0);
	    }

        if (keyCode == EventKeyboard::KeyCode::KEY_Z)
        {
			
			if(ball->getattached() == 1)
		    ball->initForce();
			ball->setattached(0);
			change(pullpusharray);
        }

    }

    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
    {
	    if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	    {
		    rightpress = false;
            //ball->XresetVelocity();
	    }

	    if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	    {
		    leftpress = false;
            //ball->XresetVelocity();
	    }
    }

	void change(CCArray* pparray)		// State 변경
	{
		CCObject* obj = NULL;

		CCARRAY_FOREACH(pparray, obj)
		{
			PullPush* pp = (PullPush*)obj;
			if(pp->able)
			{
				pp->push->setVisible(true);
				pp->pull->setVisible(false);
				pp->able = false;
			} 
			else
			{
				pp->push->setVisible(false);
				pp->pull->setVisible(true);
				pp->able = true;
			}
		}
    }

};

bool Stage::init()
{
    if(!Scene::init()) return false;
    addChild(StageLayer::create());
    return true;
}