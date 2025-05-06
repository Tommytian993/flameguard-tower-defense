#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H

#include "manager.h"

// class manages the whole game
class GameManager : public Manager<GameManager> {
    friend class Manager<GameManager>;  // allow Manager to create GameManagers

public:
    // will later add like startGame()、loadLevel() and etc...

protected:
    GameManager() {}     // only Manager could access as friend
    ~GameManager() {}
};

#endif // !_GAME_MANAGER_H
