
#include "HouseStateTrigger.h"

HouseStateTrigger::HouseStateTrigger() : roomCount(0) {
    setStateManager(StateManagerFactory::getAchievementStateManager());
}

HouseStateTrigger::HouseStateTrigger(bool get) : roomCount(0) {}


Value HouseStateTrigger::getStateValue(unsigned char stateId) {
    switch (stateId) {
        case HouseStates::ROOM_COUNT:
            return Value(roomCount);
    }
    throw "Invalid state id";
}


const char* HouseStateTrigger::getTriggerName() {
    return "house";
}


unsigned char HouseStateTrigger::getStateCount() {
    return HouseStates::COUNT;
}


unsigned char HouseStateTrigger::getStateId(const char* stateName) {
    if (!strcmp("roomcount", stateName)) {
        return HouseStates::ROOM_COUNT;
    }
    throw "state not found";
}