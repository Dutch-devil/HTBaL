
#include "StateListener.h"

StateListener::Condition::Condition(CompareType type, Value val): compareType(type), value(val) {}
StateListener::Condition::~Condition() {}

StateTriggerEvent StateListener::Condition::getTrigger() {
    return trigger;
}

bool StateListener::Condition::compare(Value other) {
    switch (compareType) {
        case SMALLER:
            return value < other;
        case EQUAL:
            return value == other;
        case GREATER:
            return value > other;
    }
    return false;
}


StateListener::StateListener(StateManager* stateManager): stateManager(stateManager) {}
StateListener::~StateListener() {}

void StateListener::lock(StateListener* toLock) {
    toLock->setLock(this);
    unlocks.push_back(toLock);
}

bool StateListener::conditionMet(StateTriggerEvent trigger, Value val) {
    // check all conditions
    for (list<Condition>::iterator itr = conditions.begin(); itr != conditions.end(); itr++) {
        if ((*itr).getTrigger() == trigger && (*itr).compare(val)) {
            // condition is met, remove it from the list
            conditions.erase(itr);
            if (conditions.empty()) {
                // all conditions are met, trigger done event
                done();
            }
            return true;
        }
    }
    return false;
}

void StateListener::registerListeners() {
    if (locks.empty()) {
        for (Condition condition : conditions) {
            stateManager->registerListener(this, condition.getTrigger());
        }
    }
}


void StateListener::setLock(StateListener* locker) {
    locks.push_back(locker);
}

void StateListener::removeLock(StateListener* locker) {
    locks.remove(locker);
    registerListeners();
}

void StateListener::done() {
    for (StateListener * unlock : unlocks) {
        unlock->removeLock(this);
    }
}