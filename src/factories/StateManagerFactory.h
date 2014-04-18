#ifndef StateManagerFactory_H_
#define StateManagerFactory_H_

#include "../triggercore/StateManager.h"
#include "../triggercore/statelistener/AchievementStateListener.h"
#include "../triggercore/TriggerLoader.h"

#include "tinyxml2.h"
#include <unordered_map>

using namespace tinyxml2;
using namespace std;

class StateManagerFactory {
public:
    static void initialize();
    static void finalize();

	static StateManager* getAchievementManager();
    
private:
    static StateManager* achievementManager;

	static StateManager* StateManagerFactory::createAchievementManager();

    static StateManager* fillStateManagerFromXml(const char* xmlName, StateManager* stateManager);
    static StateTriggerEvent parseTriggerEvent(StateManager* stateManager, const char* trigger);
    
    static void strToLower(char* str);
};

#endif