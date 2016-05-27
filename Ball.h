#pragma once

#include "cocos2d.h"

class Ground;
class PullPush;

class Ball : public cocos2d::Node
{
public:
	cocos2d::Point gravity;
    virtual bool init(float radius); 
    static Ball* create(float radius);

    void SetResetPoint(cocos2d::Point a);
    cocos2d::Point getResetPoint();
    float getRadius() const;
    int getattached() const;
    int setattached(int a);

    void initForce();
    void applyForce(const cocos2d::Point& force);
    void resetVelocity();
    void XresetVelocity();
    void YresetVelocity();
    void sameVelocity(int direction);
    void updatePhysics(Ball* ball, cocos2d::CCArray* groundarray, cocos2d::CCArray* pullpusharray, float deltaTime);
    void ColideBarrier(Ball* ball, const Ground* ground, cocos2d::Point pos);
    void pushforce(Ball* ball, cocos2d::CCArray* pullpusharray);
    void pushbarrier(Ball* ball, const PullPush* pullpush, cocos2d::Point pos);

private:
    float radius;
    float mass;
    int stagecount;
    cocos2d::Point resetpoint;
    cocos2d::Point velocity;
    cocos2d::Point acceleration;
    cocos2d::Sprite* sprite;
    cocos2d::Point oldposition;


	//// �� �ֺ� �����ϴµ� �ʿ��� ����. �߰��Ѱ�����.
	cocos2d::Point anch;// �߽���
    int attached;
	float angle;		// ����
	float ball_scale;	// �� ����� �� ������
	float circle_radius;// �� �ֺ� �� ������
	
};
