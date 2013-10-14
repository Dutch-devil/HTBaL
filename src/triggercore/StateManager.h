#ifndef StateManager_H_
#define StateManager_H_

class StateManager;

#include "StateListener.h"
#include "StateTrigger.h"

#include <vector>
#include "gameplay.h"
//#include "rapidxml.hpp"

using namespace std;
using namespace gameplay;
//using namespace rapidxml;

class StateManager {
    // so stateTrigger can register itself
    friend class StateTrigger;
    // so stateListener can register itself
    friend class StateListener;
public:
    /**
     * Create a new stateManager from scratch.
     * You manually have to add triggers and listeners.
     */
    StateManager();
    
    /**
     * Create a new stateManager from a config file at path.
     * Triggers should still be added manually.
     */
    StateManager(const char* path);
    ~StateManager();
    
    /**
     * Add a new listener to this stateManager.
     */
    void addListener(StateListener* listener);
    
private:
    /**
     * All (previously) registered triggers names.
     */
    vector<const char*> triggerNames;
    
    /**
     * And their triggers (if still registered).
     */
    vector<StateTrigger*> triggers;
    
    /**
     * All listeners ordered by the triggerEvent indices.
     */
    vector<vector<list<StateListener*>>> listeners;
    
    /**
     * Registers a new trigger to this stateManager. Will
     * be called by the trigger itself on adding stateManager.
     */
    unsigned short registerTrigger(StateTrigger* trigger);
    
    /**
     * Unregisters a trigger from this stateManager.
     * Called on triggers destructor. Listeners will still wait
     * indefinately on this trigger. Trigger is remembered, so
     * when the trigger is registered again all listeners will be connected
     */
    void unregisterTrigger(unsigned short triggerId);
    
    /**
     * Registers a stateListener to listen for a specific trigger event.
     * All events triggered by the trigger will be forwarded to listener.
     * unregistering will be handled automagically
     */
    void registerListener(StateListener* listener, StateTriggerEvent trigger);
    
    /**
     * Trigger an event from the specified event type. The value will be
     * collected if needed and passed through to all listeners registered
     * for this specific event.
     */
    void trigger(StateTriggerEvent evt);
};

#endif