#include "SaluLib.h"

//ScoreG class functions

ScoreG::ScoreG(GameMode gameMode, Node* rootNode, bool render)
{
    mode = gameMode;
    UserDefault::getInstance()->setIntegerForKey("lastMode", (int)gameMode);
    this->rootNode = rootNode;
    if(render)
    {
        Size visibleSize = Params::getGameSize();
        //Label parameters
        //label for parameters
        label = Label::createWithTTF("0", "fonts/kenneyB.ttf", (int)visibleSize.height/20);
        label->setString("0");
        rootNode->addChild(label);
        label->setGlobalZOrder(10.f);
        label->setPosition(Vec2(visibleSize.width/2,
                                visibleSize.height - label->getContentSize().height));
        updateRenderedScore();
    }
    
}
void ScoreG::updateRenderedScore()
{
    if(render)
        label->setString(Str::toString(getScore()));
}
GameMode ScoreG::getLastGameMode()
{
    return (GameMode)UserDefault::getInstance()->getIntegerForKey("lastMode",(int)GameMode::Bubbles);
}
int ScoreG::getScore()
{
    if(realScore == -fakeScore)
        return realScore;
    return 0;
}
void ScoreG::add(int n)
{
    realScore += n;
    fakeScore -= n;
    updateRenderedScore();
}
void ScoreG::setScore(int n)
{
    if(realScore == -fakeScore)
    {
        realScore = n;
        fakeScore = -n;
    }
    else
    {
        realScore = 0;
        fakeScore = 0;
    }
    updateRenderedScore();
}
void ScoreG::submitScore()
{
    if(realScore == -fakeScore)
    {
        UserDefault::getInstance()->setIntegerForKey("last", realScore);
        UserDefault::getInstance()->setIntegerForKey("lastFake", fakeScore);
        
        //new highscore
        if(realScore > getMaxScore())
        {
            UserDefault::getInstance()->setIntegerForKey(Str::concat("best", mode).c_str(), realScore);
            UserDefault::getInstance()->setIntegerForKey(Str::concat("bestFake", mode).c_str(), fakeScore);
            UserDefault::getInstance()->setBoolForKey("newBest", true);
        }
        else
            UserDefault::getInstance()->setBoolForKey("newBest", false);
    }
    
    //Invalid score
    else
    {
        UserDefault::getInstance()->setIntegerForKey("last", 0);
        UserDefault::getInstance()->setIntegerForKey("lastFake", 0);
        UserDefault::getInstance()->setBoolForKey("newBest", false);
    }
}
bool ScoreG::isNewHighScore()
{
    return UserDefault::getInstance()->getBoolForKey("newBest",false);
}
int ScoreG::getLastScore()
{
    int last = UserDefault::getInstance()->getIntegerForKey("last",0);
    int lastFake = UserDefault::getInstance()->getIntegerForKey("lastFake",0);
    if(last == -lastFake)
        return last;
    return 0;
}
int ScoreG::getMaxScore()
{
    int best = UserDefault::getInstance()->getIntegerForKey(Str::concat("best", mode).c_str(),0);
    int bestFake = UserDefault::getInstance()->getIntegerForKey(Str::concat("bestFake", mode).c_str(),0);
    
    if(best == -bestFake)
        return best;
    else
    {
        UserDefault::getInstance()->setIntegerForKey(Str::concat("best", mode).c_str(), 0);
        UserDefault::getInstance()->setIntegerForKey(Str::concat("bestFake", mode).c_str(), 0);
        return 0;
    }
}

//LifeG functions

LifeG::LifeG(Node* scene, int lives, bool render)
{
    rootNode = scene;
    realLives = lives;
    fakeLives = -lives;
    renderLives = render;
    originalLives = lives;
    updateRenderedLives();
}
int LifeG::getLives()
{
    if(realLives == -fakeLives)
        return realLives;
    return 0;
}
void LifeG::substract(int n)
{
    if(realLives == -fakeLives)
    {
        realLives -= n;
        fakeLives += n;
    }
    else
    {
        realLives = 0;
        fakeLives = 0;
    }
    if(realLives < 0)
    {
        realLives = 0;
        fakeLives = 0;
    }
    else if(realLives > originalLives)
    {
        realLives = originalLives;
        fakeLives = -originalLives;
    }
    updateRenderedLives();
}
void LifeG::restore()
{
    if(realLives == -fakeLives)
    {
        realLives = originalLives;
        fakeLives = -originalLives;
    }
    updateRenderedLives();
}
void LifeG::updateRenderedLives()
{
    //Set parameters to render
    Size renderSize = Params::getRenderSize();
    Size gameSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    //Detele old hearts
    for(Sprite* heart : spriteLives)
    {
        heart->runAction(RemoveSelf::create(true));
        spriteLives.eraseObject(heart);
    }
    if(renderLives)
    {
        for(int i = 0; i < realLives; i++)
        {
            
            Sprite* heart = Sprite::create("heartFull.png");
            //////////Implement sizing of heart
            float heartScale = renderSize.width / 800.f;
            float heartSize = heartScale * heart->getContentSize().width;
            
            log("%s",(Str::concat(heartScale, "  ")+Str::toString(heartSize)).c_str());
            
            heart->setScale(heartScale);
            Vec2 position = Vec2(origin.x + (gameSize.width/20 + heartSize * i * 1.1),//X
                                 origin.y + (gameSize.height - heartSize));//Y
            heart->setPosition(position);
            rootNode->addChild(heart);
            spriteLives.pushBack(heart);
        }
    }
}

//Parameters helper

void Params::setRenderSize(Size frameSize)
{
    UserDefault::getInstance()->setFloatForKey("height",
                                               frameSize.height);
    UserDefault::getInstance()->setFloatForKey("width",
                                               frameSize.width);
}
Size Params::getRenderSize()
{
    return
    Size(
         UserDefault::getInstance()->getFloatForKey("width",480),
         UserDefault::getInstance()->getFloatForKey("height",360)
         );
}
Size Params::getGameSize()
{
    return Director::getInstance()->getVisibleSize();
}









