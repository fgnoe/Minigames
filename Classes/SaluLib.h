#ifndef SaluLib
#define SaluLib

#include "cocos2d.h"
#include <string>
#include <sstream>

USING_NS_CC;

enum GameMode
{
    Bubbles
};
//String utils
class Str
{
public:
    template <typename T>
    static std::string toString(T value)
    {
        std::ostringstream os ;
        os << value ;
        return os.str() ;
    }
    template <typename T, typename Y>
    static std::string concat(T a, Y b)
    {
        return toString(a)+toString(b);
    }
    
};
//Scoring utils
class ScoreG
{
private:
    int realScore = 0;
    int fakeScore = 0;
    Node* rootNode;
    GameMode mode;
    bool render = true;
    void updateRenderedScore();
    Label* label;
public:
    ScoreG(GameMode gameMode, Node* scene = Node::create(), bool render = true);
    int getScore();
    void add(int n = 1);
    void setScore(int n);
    void submitScore();
    int getMaxScore();
    static bool isNewHighScore();
    static int getLastScore();
    static GameMode getLastGameMode();
};
//Lives utils
class LifeG
{
private:
    bool renderLives;
    int realLives;
    int fakeLives;
    int originalLives;
    Vector<Sprite*> spriteLives;
    Node* rootNode;
public:
    LifeG(Node* scene, int lives = 3, bool render = true);
    int getLives();
    void substract(int n = 1);
    void restore();
    void updateRenderedLives();
};

//Parameters helper
class Params
{
public:
    static void setRenderSize(Size frameSize);
    static Size getRenderSize();
    static Size getGameSize();
};
#endif





















