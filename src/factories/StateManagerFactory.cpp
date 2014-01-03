
#include "StateManagerFactory.h"

StateManager* StateManagerFactory::achievementStateManager = NULL;

void StateManagerFactory::initialize() {
    list<StateTrigger*> triggers = TriggerLoader::getAchievementTriggers();
    achievementStateManager = StateManagerFactory::createStateManagerFromXml("res/core/achievements", triggers);
    for (StateTrigger * trigger : triggers) {
        SAFE_DELETE(trigger);
    }
}

void StateManagerFactory::finalize() {
    SAFE_DELETE(achievementStateManager);
}

StateManager* StateManagerFactory::getAchievementStateManager() {
    if (!achievementStateManager) {
        throw "Not initialized yet";
    }
    return achievementStateManager;
}

StateManager* StateManagerFactory::createStateManagerFromXml(const char* xmlName, list<StateTrigger*> triggers) {
    xml_document doc;
    xml_parse_result result = doc.load_file(xmlName);
    if (!result) {
        throw "Unsuccessfull parse of xml document";
    }
    
    StateManager* stateManager = new StateManager(triggers);
    
    // skip violet specific data
    xml_node content = doc.first_child().first_child();
    
    list<xml_node> noteConnects;
    list<xml_node> labelConnects;
    unordered_map<const char_t*, StateTriggerEvent> triggerEventsById;
    unordered_map<const char_t*, StateListener*> listenersById;
    
    // loop all functions
    for (xml_node methodNode = content.first_child(); methodNode; methodNode = methodNode.next_sibling()) {
        const char_t* methodName = methodNode.attribute("method").value();
        if (!strcmp(methodName, "addNode")) {
            // create a new node (statelistener)
            xml_node nodeType = methodNode.first_child();
            const char_t* nodeName = nodeType.attribute("class").value();
            const char_t* nodeId = nodeType.attribute("id").value();
            if (!strcmp(nodeName, "com.horstmann.violet.CircularStateNode")) {
                // trigger node
            } else if (!strcmp(nodeName, "com.horstmann.violet.StateNode")) {
                // listenener node
                // TODO: meerdere soorten listeners en hun data mee kunnen geven
                StateListener* listener = new AchievementStateListener(stateManager);
                listenersById.insert(pair<const char_t*, StateListener*>(nodeId, listener));
            } else if (!strcmp(nodeName, "com.horstmann.violet.NoteNode")) {
                // trigger node name
                // save the name for connecting with a trigger
                const char_t* trigger = nodeType.first_child().first_child().first_child().child_value();
                StateTriggerEvent triggerEvent = parseTriggerEvent(stateManager, trigger);
                triggerEventsById.insert(pair<const char_t*, StateTriggerEvent>(nodeId, triggerEvent));
            }
        } else if (!strcmp(methodName, "connect")) {
            // add a new connection between nodes
            xml_node connectType = methodNode.first_child();
            const char_t* connectName = connectType.attribute("class").value();
            if (!strcmp(connectName, "com.horstmann.violet.StateTransitionEdge")) {
                // connection between listeners or listener and trigger
                xml_node labelProperty = connectType.first_child();
                if (labelProperty) {
                    // connection has label, must be trigger-listener connection
                    // skip them at first to connect notes to triggers first
                    labelConnects.push_back(connectType);
                } else {
                    // connection doesn't have label, must be listener-listener connection
                }
            } else if (!strcmp(connectName, "com.horstmann.violet.NoteEdge")) {
                // connection between trigger and it's name
                xml_node noteId = connectType.next_sibling();
                unordered_map<const char_t*, StateTriggerEvent>::iterator itr;
                const char_t* id = noteId.next_sibling().attribute("idref").value();
                for (itr = triggerEventsById.begin(); itr != triggerEventsById.end(); itr++) {
                    if (!strcmp(itr->first, id)) {
                        break;
                    }
                }
                if (itr == triggerEventsById.end()) {
                    throw "Note id not found";
                }
                triggerEventsById.insert(pair<const char_t*, StateTriggerEvent>(noteId.attribute("idref").value(), itr->second));
                triggerEventsById.erase(itr);
            }
        }
    }
    
    set<StateListener*> listenerList;
    
    // and loop all triggers to connect
    list<xml_node>::iterator connectsItr;
    for (connectsItr = labelConnects.begin(); connectsItr != labelConnects.end(); connectsItr++) {
        xml_node labelConnect = *connectsItr;
        const char_t* label = labelConnect.first_child().first_child().child_value();
        const char_t* id1 = labelConnect.next_sibling().attribute("idref").value();
        const char_t* id2 = labelConnect.next_sibling().next_sibling().attribute("idref").value();
        
        unordered_map<const char_t*, StateTriggerEvent>::iterator itr;
        for (itr = triggerEventsById.begin(); itr != triggerEventsById.end(); itr++) {
            if (!strcmp(itr->first, id1)) {
                break;
            }
        }
        if (itr == triggerEventsById.end()) {
            throw "Listener not found";
        }
        StateTriggerEvent triggerEvent = itr->second;
        
        StateListener* listener;
        unordered_map<const char_t*, StateListener*>::iterator itr2;
        for (itr2 = listenersById.begin(); itr2 != listenersById.end(); itr2++) {
            if (!strcmp(itr2->first, id2)) {
                break;
            }
        }
        if (itr2 == listenersById.end()) {
            throw "Listener not found";
        }
        listener = itr2->second;
        
        // connect trigger at id1 with listener at id2 with value label
        StateListener::Condition::CompareType compareType;
        switch (label[0]) {
            case '<':
                compareType = StateListener::Condition::CompareType::SMALLER;
                break;
            case '=':
                compareType = StateListener::Condition::CompareType::EQUAL;
                break;
            case '>':
                compareType = StateListener::Condition::CompareType::GREATER;
                break;
            default:
                throw "Invalid compare type found while connecting triggers";
        }
        Value value = Value::parseValue(++label);   // save the value while skipping the first char
        print("halfway1\n");
        StateListener::Condition(triggerEvent, compareType, value);
        print("halfway2\n");
        listener->addCondition(StateListener::Condition(triggerEvent, compareType, value));
        listenerList.insert(listener);
        print("halfwayi\n");
    }
    print("halfwayi2\n");
    stateManager->unregisterTriggers();
    
    print("halfwaya\n");
    for (StateListener * listener : listenerList) {
        listener->registerListeners();
    }
    print("halfwayb\n");
    return stateManager;
}

StateTriggerEvent StateManagerFactory::parseTriggerEvent(StateManager* stateManager, const char* triggerString) {
    char* triggerName = new char[100];
    char* stateName = new char[100];
    if (sscanf(triggerString, "%[^:]:%s", triggerName, stateName) != 2) {
        print("%s\n", triggerName);
        throw "Should be formatted as Trigger State";
    }
    strToLower(triggerName);
    strToLower(stateName);
    
    StateTrigger* trigger = stateManager->getTrigger(triggerName);
    if (trigger == NULL) {
        throw "Trigger not found";
    }
    
    StateTriggerEvent triggerEvent = trigger->getEvent(stateName);
    SAFE_DELETE_ARRAY(triggerName);
    SAFE_DELETE_ARRAY(stateName);
    return triggerEvent;
}

void StateManagerFactory::strToLower(char* str) {
    while (*str != 0) {
        *str = tolower(*str);
        str++;
    }
}