#ifndef StateTrigger_H_
#define StateTrigger_H_

class StateTrigger;
struct StateTriggerEvent {
public:
    StateTriggerEvent();
    StateTriggerEvent(unsigned short triggerId, unsigned char stateId);
    bool operator==(StateTriggerEvent& other);
    
    unsigned short triggerId;
    unsigned char stateId;
};

#include "Value.h"
#include "StateManager.h"

class StateTrigger {
public:

    StateTrigger();
    ~StateTrigger();
    
    /**
     * Set the stateManager to trigger events on. This trigger will be automatically
     * registered to the specified stateManager for keeping track of the values.
     */
    void setStateManager(StateManager* stateManager);
    
    /**
     * Trigger the specified stateId. Usually the stateId should be an enum value.
     * stateId is local to the overriding class and will be combined with the triggerId
     * to specify a state.
     */
    void trigger(unsigned char stateId);
    
    /**
     * Get an event for this trigger by the event state name. Just converts the
     * string and creates the event from it.
     */
    StateTriggerEvent getEvent(const char* stateName);
    
    /**
     * Function for getting the actual value of a trigger. Will always be called after
     * a trigger event has been sent. May also be called when a new listener is added
     * to the list for updating it's values
     */
    virtual Value getStateValue(unsigned char stateId) = 0;
    
    /**
     * Function for registering ths triggers name. Should return a unique name which
     * can be used in config files. Only called once when reading config.
     */
    virtual const char* getTriggerName() = 0;
    
    /**
     * For efficiency purposes. Return the amount of states this will trigger.
     * Essentially it is just the highest stateId this will trigger.
     */
    virtual unsigned char getStateCount() = 0;
    
    /**
     * Function for converting string names to their id's. Should return the state
     * enum value by their string for using in config files. Only called once when
     * reading config files.
     */
    virtual unsigned char getStateId(const char*) = 0;
    
private:
    /**
     * The manager for registering this state on. Used for triggering state events.
     */
    StateManager* stateManager;
    /**
     * The unique id of this trigger. For the stateManager to index it in an array.
     */
    unsigned short triggerId;
    
};

#endif