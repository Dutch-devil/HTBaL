
#include "HouseStateTrigger.h"

HouseStateTrigger::HouseStateTrigger() {
    setStateManager(StateManagerFactory::getAchievementManager());
}

HouseStateTrigger::HouseStateTrigger(bool get) {}


Value HouseStateTrigger::getStateValue(unsigned char stateId) {
    switch (stateId) {
        case HouseStates::ROOM_COUNT:
            return Value(getRoomCount());
		case HouseStates::CURRENT_FLOOR:
			return Value(getCurrentFloor());
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
	} else if (!strcmp("currentfloor", stateName)) {
		return HouseStates::CURRENT_FLOOR;
	}
    throw "state not found";
}

const char* HouseStateTrigger::getStateName(unsigned char stateId) {
	switch (stateId) {
		case HouseStates::ROOM_COUNT:
			return "roomcount";
		case HouseStates::CURRENT_FLOOR:
			return "currentfloor";
		default:
			return "unknown";
	}
}

int HouseStateTrigger::getRoomCount() {
	return 0;
}

int HouseStateTrigger::getCurrentFloor() {
	return 0;
}