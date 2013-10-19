
#include "StateTrigger.h"

StateTriggerEvent::StateTriggerEvent(): triggerId(-1), stateId(-1) {}
StateTriggerEvent::StateTriggerEvent(unsigned short triggerId, unsigned char stateId): triggerId(triggerId), stateId(stateId) {}

bool StateTriggerEvent::operator==(StateTriggerEvent& other) {
    return triggerId == other.triggerId && stateId == other.stateId;
}

StateTrigger::StateTrigger(): stateManager(0) {}

StateTrigger::~StateTrigger() {
    if (stateManager) {
        stateManager->unregisterTrigger(triggerId);
    }
}

void StateTrigger::trigger(unsigned char stateId) {
    stateManager->trigger(StateTriggerEvent(triggerId, stateId));
}

void StateTrigger::setStateManager(StateManager* stateManager) {
    if (this->stateManager) {
        this->stateManager->unregisterTrigger(triggerId);
    }
    this->stateManager = stateManager;
    triggerId = stateManager->registerTrigger(this);
}