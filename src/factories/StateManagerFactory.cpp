
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
	XMLDocument doc;
	doc.LoadFile(xmlName);
	if (doc.ErrorID()) {
		printf("XML parse error with id: %d\n", doc.ErrorID());
        throw "Unsuccessfull parse of xml document";
    }
    
    StateManager* stateManager = new StateManager(triggers);

	// skip violet specific data
	XMLNode* content = doc.FirstChild()->NextSibling()->FirstChild();
	list<XMLNode*> noteConnects;
	list<XMLNode*> labelConnects;

	unordered_map<const char*, StateTriggerEvent> triggerEventsById;
	unordered_map<const char*, StateListener*> listenersById;

	// loop all functions
	for (XMLNode* methodNode = content->FirstChild(); methodNode; methodNode = methodNode->NextSibling()) {
		const char* methodName = methodNode->ToElement()->Attribute("method");
		if (!strcmp(methodName, "addNode")) {
			// create a new node (statelistener)
			XMLNode* nodeType = methodNode->FirstChild();
			const char* nodeName = nodeType->ToElement()->Attribute("class");
			const char* nodeId = nodeType->ToElement()->Attribute("id");
			if (!strcmp(nodeName, "com.horstmann.violet.CircularStateNode")) {
				// trigger node
			}else if (!strcmp(nodeName, "com.horstmann.violet.StateNode")) {
				// listenener node
				// TODO: meerdere soorten listeners en hun data mee kunnen geven
				StateListener* listener = new AchievementStateListener(stateManager);
				listenersById.insert(pair<const char*, StateListener*>(nodeId, listener));
			}else if (!strcmp(nodeName, "com.horstmann.violet.NoteNode")) {
				// trigger node name
				// save the name for connecting with a trigger
				const char* trigger = nodeType->FirstChild()->FirstChild()->FirstChildElement()->GetText();
				StateTriggerEvent triggerEvent = parseTriggerEvent(stateManager, trigger);
				triggerEventsById.insert(pair<const char*, StateTriggerEvent>(nodeId, triggerEvent));
			}
		}
		else if (!strcmp(methodName, "connect")) {
			// add a new connection between nodes
			XMLNode* connectType = methodNode->FirstChild();
			const char* connectName = connectType->ToElement()->Attribute("class");
			if (!strcmp(connectName, "com.horstmann.violet.StateTransitionEdge")) {
				// connection between listeners or listener and trigger
				XMLNode* labelProperty = connectType->FirstChild();
				if (labelProperty) {
					// connection has label, must be trigger-listener connection
					// skip them at first to connect notes to triggers first
					labelConnects.push_back(connectType);
				}
				else {
					// connection doesn't have label, must be listener-listener connection
				}
			}
			else if (!strcmp(connectName, "com.horstmann.violet.NoteEdge")) {
				// connection between trigger and it's name
				XMLNode* noteId = connectType->NextSibling();
				unordered_map<const char*, StateTriggerEvent>::iterator itr;
				const char* id = noteId->NextSiblingElement()->Attribute("idref");
				for (itr = triggerEventsById.begin(); itr != triggerEventsById.end(); itr++) {
					if (!strcmp(itr->first, id)) {
						break;
					}
				}
				if (itr == triggerEventsById.end()) {
					throw "Note id not found";
				}
				triggerEventsById.insert(pair<const char*, StateTriggerEvent>(noteId->ToElement()->Attribute("idref"), itr->second));
				triggerEventsById.erase(itr);
			}
		}
	}

	set<StateListener*> listenerList;
	// and loop all triggers to connect
	list<XMLNode*>::iterator connectsItr;
	for (connectsItr = labelConnects.begin(); connectsItr != labelConnects.end(); connectsItr++) {
		XMLNode* labelConnect = *connectsItr;
		const char* label = labelConnect->FirstChild()->FirstChildElement()->GetText();
		const char* id1 = labelConnect->NextSiblingElement()->Attribute("idref");
		const char* id2 = labelConnect->NextSibling()->NextSiblingElement()->Attribute("idref");

		unordered_map<const char*, StateTriggerEvent>::iterator itr;
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
		unordered_map<const char*, StateListener*>::iterator itr2;
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
		StateListener::Condition(triggerEvent, compareType, value);
		listener->addCondition(StateListener::Condition(triggerEvent, compareType, value));
		listenerList.insert(listener);
	}
	stateManager->unregisterTriggers();

	for (StateListener * listener : listenerList) {
		listener->registerListeners();
	}

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