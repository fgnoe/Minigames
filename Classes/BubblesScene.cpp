#include "BubblesScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

    Vector<Sprite*> bubbles;
Scene* Bubbles::createScene()
{
    auto scene = Scene::create();
    auto layer = Bubbles::create();
    scene->addChild(layer);
    return scene;
}
bool Bubbles::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    //schedule spawner
    schedule(schedule_selector(Bubbles::spawnBubble), 0.5f);
    
    //setting touchlistener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Bubbles::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    //Score parameters
    score = new ScoreG(GameMode::Bubbles, this);
    
    //Life parameters
    life = new LifeG(this);
    
    return true;
}
void Bubbles::spawnBubble(float dt)
{
    Sprite* newBubble = Sprite::create("Bubbles/green.png");
    
    //sprite size
    newBubble->setContentSize(Size::ZERO);
    float width = visibleSize.width/10;
    newBubble->runAction(ResizeTo::create(0.1f, Size(width,
                                                     width)));
    //set position in the screen
    newBubble->setPosition
    (
        Vec2(origin.x + random(width, visibleSize.width - width),
             origin.y + random(width, visibleSize.height - width))
    );
    addChild(newBubble);
    bubbles.pushBack(newBubble);
    newBubble->setGlobalZOrder(--bubbleZOrder);
}

bool Bubbles::onTouchBegan(Touch* touch, Event* event)
{
    bool onABubble = false;
    for(Sprite* bubble : bubbles)
    {
        if(bubble->getBoundingBox().containsPoint(touch->getLocation()))
        {
            bubble->runAction(FadeOut::create(0.13f));
            
            //Destruction
            Sequence* sec = Sequence::create(DelayTime::create(0.13f),RemoveSelf::create(true), NULL);
            bubble->runAction(sec);
            bubbles.eraseObject(bubble);
            score->add();
            onABubble = true;
            break;
        }
    }
    if(!onABubble)
    {
        if( score->getScore()>0)
            score->add(-1);
        life->substract();
    }
    return true;
}

