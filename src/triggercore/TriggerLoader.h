#ifndef TriggerLoader_H_
#define TriggerLoader_H_

#include <list>
#include "StateTrigger.h"
#include "statetrigger/HouseStateTrigger.h"

using namespace std;

class TriggerLoader {
public:
    static list<StateTrigger*> getAchievementTriggers();
    
};

#endif