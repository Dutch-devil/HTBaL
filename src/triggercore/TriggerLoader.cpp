
#include "TriggerLoader.h"


list<StateTrigger*> TriggerLoader::getAchievementTriggers() {
    list<StateTrigger*> triggerList;
    triggerList.push_back(new HouseStateTrigger(false));
    return triggerList;
}