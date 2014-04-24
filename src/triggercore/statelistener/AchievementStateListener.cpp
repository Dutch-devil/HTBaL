
#include "AchievementStateListener.h"

AchievementStateListener::AchievementStateListener(StateManager* stateManager): StateListener(stateManager), achievementIndex(-1) {
	achievementName = NULL;
}

AchievementStateListener::~AchievementStateListener() {
	SAFE_DELETE_ARRAY(achievementName);
}

void AchievementStateListener::setAchievementName(const char* name) {
	SAFE_DELETE_ARRAY(achievementName);
	rsize_t len = strlen(name) + 1;
	achievementName = new char[len];
	strcpy_s(achievementName, len, name);

	if (!strcmpnocase("big spender", name)) {
		achievementIndex = 0;
	} else if (!strcmpnocase("mamas floor", name)) {
		achievementIndex = 1;
	}
}

void AchievementStateListener::handleDone() {
    print("Got achievement %s!\n", achievementName);
    return;
}

const char* AchievementStateListener::getListenerName() {
	//char* listenerName = new char[100];
	//sprintf(listenerName, "Achievement State Listener %s", achievementName);
	//return listenerName;
	return "Achievement State Listener";
}

unsigned short AchievementStateListener::getAchievementIndex() {
	return achievementIndex;
}

AchievementStateListener* AchievementStateListener::createFromName(const char* name, StateManager* stateManager) {
	AchievementStateListener* achievement = new AchievementStateListener(stateManager);
	achievement->setAchievementName(name);
	return achievement;
}