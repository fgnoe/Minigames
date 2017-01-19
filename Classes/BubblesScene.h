#ifndef __BUBBLES_SCENE_H__
#define __BUBBLES_SCENE_H__

#include "cocos2d.h"
#include "SaluLib.h"

USING_NS_CC;

class Bubbles : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void spawnBubble(float dt);
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    Vec2 origin;
    Size visibleSize;
    float bubbleZOrder = 0;
    ScoreG* score;
    LifeG* life;
    CREATE_FUNC(Bubbles);
};

#endif
