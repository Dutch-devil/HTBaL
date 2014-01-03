#ifndef AchievementStateListener_H_
#define AchievementStateListener_H_

#include "../StateListener.h"

class AchievementStateListener : public StateListener {
public:

    AchievementStateListener(StateManager* stateManager);
    
    /**
     * Function for handling all specific things when this listener is done.
     */
    void handleDone();
};

#endif