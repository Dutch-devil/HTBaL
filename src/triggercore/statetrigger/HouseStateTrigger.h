#ifndef HouseStateTrigger_H_
#define HouseStateTrigger_H_

#include "../Value.h"
#include "../StateTrigger.h"
#include "../../factories/StateManagerFactory.h"
#include <string.h>

using namespace std;

class HouseStateTrigger : public StateTrigger {
public:
    HouseStateTrigger();
    HouseStateTrigger(bool get);
    
    /**
     * Function for getting the actual value of a trigger. Will always be called after
     * a trigger event has been sent. May also be called when a new listener is added
     * to the list for updating it's values
     */
    Value getStateValue(unsigned char stateId);
    
    /**
     * Function for registering ths triggers name. Should return a unique name which
     * can be used in config files. Only called once when reading config.
     */
    const char* getTriggerName();
    
    /**
     * For efficiency purposes. Return the amount of states this will trigger.
     * Essentially it is just the highest stateId this will trigger.
     */
    unsigned char getStateCount();
    
    /**
     * Function for converting string names to their id's. Should return the state
     * enum value by their string for using in config files. Only called once when
     * reading config files.
     */
    unsigned char getStateId(const char*);

	/** 
	 * Get the state name belonging to an id.
	 * The inverse function of getStateId
	 */
	const char* getStateName(unsigned char stateId);
    
protected:
    enum HouseStates {
		ROOM_COUNT,
		CURRENT_FLOOR,
        COUNT
    };
    
	virtual int getRoomCount();
	virtual int getCurrentFloor();
};

#endif