
#include "StateManagerFactory.h"

StateManager* StateManagerFactory::achievementManager = NULL;

void StateManagerFactory::initialize() {
    achievementManager = StateManagerFactory::createAchievementManager();
}

void StateManagerFactory::finalize() {
    SAFE_DELETE(achievementManager);
}

StateManager* StateManagerFactory::getAchievementManager() {
	if (achievementManager == NULL) {
		throw "Not initalized yet";
	}
	return achievementManager;
}

StateManager* StateManagerFactory::createAchievementManager() {
	list<StateTrigger*> triggers = TriggerLoader::getAchievementTriggers();
	StateManager* achievementManager = new StateManager(triggers);
	StateManagerFactory::fillStateManagerFromXml("res/core/achievements", achievementManager);
	achievementManager->display();
	achievementManager->unregisterTriggers();
	for (StateTrigger * trigger : triggers) {
		SAFE_DELETE(trigger);
	}
    return achievementManager;
}

StateManager* StateManagerFactory::fillStateManagerFromXml(const char* xmlName, StateManager* stateManager) {
	XMLDocument doc;
	doc.LoadFile(xmlName);
	if (doc.ErrorID()) {
		printf("XML parse error with id: %d\n", doc.ErrorID());
        throw "Unsuccessfull parse of xml document";
    }

	// skip violet specific data
	XMLNode* content = doc.FirstChild()->NextSibling()->FirstChild();
	list<XMLNode*> noteConnects;
	list<XMLNode*> labelConnects;

	unordered_map<const char*, StateTriggerEvent> triggerEventsById;
	unordered_map<const char*, StateListener*> listenersById;
	unordered_map<const char*, const char*> listenerConnects;

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
				const char* stateName = nodeType->FirstChild()->FirstChild()->FirstChildElement()->GetText();
				StateListener* listener = AchievementStateListener::createFromName(stateName, stateManager);
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
					XMLNode* fromId = connectType->NextSibling();
					listenerConnects.insert(pair<const char*, const char*>(fromId->ToElement()->Attribute("idref"), fromId->NextSiblingElement()->Attribute("idref")));
				}
			} else if (!strcmp(connectName, "com.horstmann.violet.NoteEdge")) {
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
	unordered_map<const char*, const char*>::iterator itr;
	for (itr = listenerConnects.begin(); itr != listenerConnects.end(); itr++) {
		// lock listener with id itr->second by listener with id itr->first
		StateListener* listenerFrom = NULL, *listenerTo = NULL;
		unordered_map<const char*, StateListener*>::iterator itr2;
		for (itr2 = listenersById.begin(); itr2 != listenersById.end(); itr2++) {
			if (!strcmp(itr2->first, itr->first)) {
				listenerFrom = itr2->second;
			} else if (!strcmp(itr2->first, itr->second)) {
				listenerTo = itr2->second;
			}
			if (listenerFrom != NULL && listenerTo != NULL) {
				break;
			}
		}
		if (listenerFrom == NULL || listenerTo == NULL) {
			throw "Listener lock not found";
		}
		listenerFrom->lock(listenerTo);
	}

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