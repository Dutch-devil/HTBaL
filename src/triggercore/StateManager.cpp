
#include "StateManager.h"

StateManager::StateManager() {}

StateManager::StateManager(list<StateTrigger*> triggers) {
    while (!triggers.empty()) {
        StateTrigger* trigger = triggers.back();
        triggers.pop_back();
        
        // register and unregister the trigger
        // this way it's name will be saved
        trigger->setStateManager(this);
    }
}

StateManager::~StateManager() {
    unregisterTriggers();
    for (vector<list<StateListener*>> triggerListeners : listeners) {
        for (list<StateListener*> eventListeners : triggerListeners) {
            for (StateListener * listener : eventListeners) {
                SAFE_DELETE(listener);
            }
        }
    }
}

void StateManager::unregisterTriggers() {
    for (StateTrigger * trigger : triggers) {
        if (trigger) {
            unregisterTrigger(registerTrigger(trigger));
        }
    }
}



unsigned short StateManager::registerTrigger(StateTrigger* trigger) {
    const char* triggerName = trigger->getTriggerName();
    
    // Check if the trigger name was registered before. If so, take that id.
    // Else make a new id for it.
    unsigned short triggerId = 0;
    for (vector<const char*>::iterator itr = triggerNames.begin(); itr != triggerNames.end(); itr++) {
        if (!strcmp(triggerName, *itr)) {
            triggerId = itr - triggerNames.begin();
            triggers[triggerId] = trigger;
            return triggerId;
        }
    }
    triggerNames.push_back(triggerName);
    triggers.push_back(trigger);
    listeners.push_back(vector<list<StateListener*>>(trigger->getStateCount()));
    return triggers.size() - 1;
}

StateTrigger* StateManager::getTrigger(const char* triggerName) {
    for (vector<const char*>::iterator itr = triggerNames.begin(); itr != triggerNames.end(); itr++) {
        if (!strcmp(triggerName, *itr)) {
            return triggers[itr - triggerNames.begin()];
        }
    }
    return NULL;
}

StateListener* StateManager::getDoneListener() {
	if (doneListeners.empty()) {
		return NULL;
	}
	StateListener* first = doneListeners.front();
	doneListeners.pop_front();
	return first;
}

list<StateListener*> StateManager::getAllDoneListeners() {
	list<StateListener*> copy = list<StateListener*>(doneListeners);
	doneListeners.clear();
	return copy;
}

void StateManager::unregisterTrigger(unsigned short triggerId) {
    triggers[triggerId] = NULL;
}

void StateManager::registerListener(StateListener* listener, StateTriggerEvent trigger) {
    listeners[trigger.triggerId][trigger.stateId].push_back(listener);
}

void StateManager::trigger(StateTriggerEvent evt) {
    if (triggers[evt.triggerId]) {
        // check if the value even exists or this is a false positive event.
        Value value = triggers[evt.triggerId]->getStateValue(evt.stateId);
        if (value.getValueType() != Value::ValueType::NONE) {
            // loop over all listeners registered to this event.
            list<StateListener*> listenerList = listeners[evt.triggerId][evt.stateId];
            for (list<StateListener*>::iterator itr = listenerList.begin(); itr != listenerList.end();) {
                // if the condition is met on the listener, unregister it automagically.
                if ((*itr)->conditionMet(evt, value)) {
					list<StateListener*> doneList = (*itr)->checkDone();
					doneListeners.merge(doneList);
					itr = listenerList.erase(itr);
                } else {
                    itr++;
                }
            }
        }
    }
}

void StateManager::display() {
	print("\n\nTriggers:\n");
	for (int i = 0; i < triggers.size(); i++) {
		StateTrigger* trigger = triggers[0];
		if (trigger != 0) {
			print(" - %s with %d states\n", trigger->getTriggerName(), listeners[i].size());
			for (int j = 0; j < listeners[i].size(); i++) {
				print("   - state %s with %d listeners\n", trigger->getStateName(j), listeners[i][j].size());
				for (StateListener* listener : listeners[i][j]) {
					print("     - listener %s\n", listener->getListenerName());
					for (StateListener::Condition condition : listener->getConditions()) {
						print("       Must be ");
						switch (condition.compareType) {
							case StateListener::Condition::SMALLER: print("smaller than "); break;
							case StateListener::Condition::EQUAL: print("equal to "); break;
							case StateListener::Condition::GREATER: print("greater than "); break;
						}
						switch (condition.value.getValueType()) {
							case Value::INTEGER: print("%d\n", condition.value.getIntValue()); break;
							case Value::FLOAT: print("%f\n", condition.value.getIntValue()); break;
							case Value::CHARACTER: print("%c\n", condition.value.getCharValue()); break;
							case Value::STRING: print("%s\n", condition.value.getStringValue()); break;
						}
					}
				}
			}
		}
		print("\n");
	}
}