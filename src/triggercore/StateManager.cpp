
#include "StateManager.h"

StateManager::StateManager() {

}

StateManager::StateManager(const char* path) {
    char* text = FileSystem::readAll(path);
    //xml_document<> doc;
    //doc.parse<0>(text);
    
    SAFE_DELETE_ARRAY(text);
}

StateManager::~StateManager() {}



unsigned short StateManager::registerTrigger(StateTrigger* trigger) {
    const char* triggerName = trigger->getTriggerName();
    
    // Check if the trigger name was registered before. If so, take that id.
    // Else make a new id for it.
    unsigned short triggerId;
    for (vector<const char*>::iterator itr = triggerNames.begin(); itr != triggerNames.end(); itr++) {
        if (!strcmp(triggerName, *itr)) {
            triggerId = itr - triggerNames.begin();
            triggers[triggerId] = trigger;
            listeners[triggerId] = vector<list<StateListener*>>(trigger->getStateCount());
            return triggerId;
        }
    }
    triggerNames.push_back(triggerName);
    triggers.push_back(trigger);
    return triggers.size() - 1;
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
        if (value.getValueType() == Value::ValueType::NONE) {
            // loop over all listeners registered to this event.
            list<StateListener*> listenerList = listeners[evt.triggerId][evt.stateId];
            for (list<StateListener*>::iterator itr = listenerList.begin(); itr != listenerList.end();) {
                // if the condition is met on the listener, unregister it automagically.
                if ((*itr)->conditionMet(evt, value)) {
                    itr = listenerList.erase(itr);
                } else {
                    itr++;
                }
            }
        }
    }
}