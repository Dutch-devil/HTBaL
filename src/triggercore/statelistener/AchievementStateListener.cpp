
#include "AchievementStateListener.h"

AchievementStateListener::AchievementStateListener(StateManager* stateManager): StateListener(stateManager) {}

void AchievementStateListener::handleDone() {
    print("Got achievement!\n");
    return;
}