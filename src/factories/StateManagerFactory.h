#ifndef StateManagerFactory_H_
#define StateManagerFactory_H_

#include "../triggercore/StateManager.h"
#include "../triggercore/statelistener/AchievementStateListener.h"
#include "../triggercore/TriggerLoader.h"
#include "pugiconfig.hpp"
#include <unordered_map>

using namespace pugi;
using namespace std;

class StateManagerFactory {
public:
    static void StateManagerFactory::initialize();
    static void StateManagerFactory::finalize();
    static StateManager* StateManagerFactory::getAchievementStateManager();
    
private:
    static StateManager* achievementStateManager;
    
    static StateManager* createStateManagerFromXml(const char* xmlName, list<StateTrigger*> triggers);
    static StateTriggerEvent parseTriggerEvent(StateManager* stateManager, const char* trigger);
    
    static void strToLower(char* str);
};

#endif