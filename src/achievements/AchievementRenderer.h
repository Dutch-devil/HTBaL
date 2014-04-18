#pragma once

#include "../MaterialManager.h"
#include "../triggercore/StateManager.h"
#include "../triggercore/statelistener/AchievementStateListener.h"
#include "../factories/StateManagerFactory.h"

#define ANIMATION_START 0.0f
#define ANIMATION_PAUSE 1000.0f
#define ANIMATION_RESUME 3000.0f
#define ANIMATION_END 4000.0f

#define ACHIEVEMENT_X_COUNT 2
#define ACHIEVEMENT_Y_COUNT 1

class AchievementRenderer {
public:
	AchievementRenderer();
	AchievementRenderer(StateManager* achievementManager);
	~AchievementRenderer();

	void render(float elapsedTime);

protected:
	StateManager* achievementManager;
	SpriteBatch* spriteBatch;
	AchievementStateListener* visibleAchievement;
	float totalDisplayTime;
};

