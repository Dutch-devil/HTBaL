#ifndef AchievementStateListener_H_
#define AchievementStateListener_H_

#include "../StateListener.h"

class AchievementStateListener : public StateListener {
public:

    AchievementStateListener(StateManager* stateManager);
	~AchievementStateListener();
    
	/**
	 * Sets this achievements name
	 */
	void setAchievementName(const char* name);

    /**
     * Function for handling all specific things when this listener is done.
     */
    void handleDone();

	const char* getListenerName();

	unsigned short getAchievementIndex();

	static AchievementStateListener* createFromName(const char* name, StateManager* stateManager);

protected:
	char* achievementName;
	unsigned short achievementIndex;
};

#endif