
#include "StateListener.h"

StateListener::Condition::Condition(StateTriggerEvent trigger, CompareType type, Value val): trigger(trigger), compareType(type), value(val) {}
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

void StateListener::addCondition(Condition condition) {
    conditions.push_back(condition);
}

list<StateListener::Condition> StateListener::getConditions() {
	return conditions;
}

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
            return true;
        }
    }
    return false;
}

list<StateListener*> StateListener::checkDone() {
	list<StateListener*> doneList;
	if (conditions.empty()) {
		// all conditions are met, trigger done event
		doneList.push_back(this);
		doneList.merge(done());
	}
	return doneList;
}

list<StateListener*> StateListener::registerListeners() {
	list<StateListener*> doneList;
    if (locks.empty()) {
		doneList.merge(checkDone());
		if (doneList.empty()) {
			// TODO:
			// normale foreach loop geeft corruption?
			for (list<Condition>::iterator itr = conditions.begin(); itr != conditions.end(); itr++) {
				stateManager->registerListener(this, itr->getTrigger());
			}
		}
	}
	return doneList;
}


void StateListener::setLock(StateListener* locker) {
    locks.push_back(locker);
}

list<StateListener*> StateListener::removeLock(StateListener* locker) {
    locks.remove(locker);
    return registerListeners();
}

list<StateListener*> StateListener::done() {
	list<StateListener*> doneList;
	// TODO: handleDone moet ook listeners kunnen returnen?
	handleDone();
    for (StateListener * unlock : unlocks) {
        doneList.merge(unlock->removeLock(this));
    }
	return doneList;
}

const char* StateListener::getListenerName() {
	return "Unnamed";
}