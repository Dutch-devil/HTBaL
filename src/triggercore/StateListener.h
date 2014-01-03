#ifndef StateListener_H_
#define StateListener_H_

class StateListener;

#include "StateTrigger.h"
#include "StateManager.h"
#include "Value.h"

#include <vector>
#include <list>

using namespace std;

class StateListener {
public:
    struct Condition {
        enum CompareType {
            SMALLER, EQUAL, GREATER
        };
        StateTriggerEvent trigger;
        CompareType compareType;
        Value value;
        
        Condition(StateTriggerEvent trigger, CompareType type, Value val);
        ~Condition();
        
        StateTriggerEvent getTrigger();
        bool compare(Value other);
    };
    
    StateListener(StateManager* stateManager);
    ~StateListener();
    
    /**
     * Add a new condition to the list to check for before unlocking
     */
    void addCondition(Condition condition);
    
    /**
     * Locks the stateListener toLock. toLock won't ever be triggered until this
     * listener is triggered.
     */
    void lock(StateListener* toLock);
    
    /**
     * Check if the given condition specified by trigger/value pair is met.
     * If true is returned, this condition won't be fired again.
     */
    bool conditionMet(StateTriggerEvent trigger, Value val);
    
    /**
     * Set all listeners for conditions on stateManager. Function will only
     * work when this listener has no locks. Should be called once after
     * creating all stateListeners.
     */
    void registerListeners();
    
    
    /**
     * Function for handling all specific things when this listener is done.
     */
    virtual void handleDone() = 0;
    
private:
    /**
     * The manager for registering this state on. Used for triggering state events.
     */
    StateManager* stateManager;
    
    /**
     * An array of stateListeners which will be unlocked after this listener is
     * triggered. stateListeners might still be locked by other listeners.
     */
    vector<StateListener*> unlocks;
    
    /**
     * A list of listeners that lock this listener. This listener can't be triggered
     * before all locks are released.
     */
    list<StateListener*> locks;
    
    /**
     * All the conditions that have to be met before this listener will trigger
     */
    list<Condition> conditions;
    
    
    void setLock(StateListener* locker);
    
    /**
     * Unlock the lock set by lock.
     */
    void removeLock(StateListener* locker);
    
    /**
     * triggered when this listener is done. All it's locks will be released
     */
    void done();
};

#endif