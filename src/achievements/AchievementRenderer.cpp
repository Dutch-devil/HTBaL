#include "AchievementRenderer.h"


AchievementRenderer::AchievementRenderer() {
	achievementManager = StateManagerFactory::getAchievementManager();
	spriteBatch = SpriteBatch::create("res/achievements.png");
	visibleAchievement = NULL;
}

AchievementRenderer::AchievementRenderer(StateManager* achievementManager) : achievementManager(achievementManager) {
	spriteBatch = SpriteBatch::create("res/achievements.png");
	visibleAchievement = NULL;
}

AchievementRenderer::~AchievementRenderer() {
	SAFE_DELETE(spriteBatch);
}

void AchievementRenderer::render(float elapsedTime) {
	if (visibleAchievement == NULL) {
		visibleAchievement = (AchievementStateListener*)achievementManager->getDoneListener();
		totalDisplayTime = 0;
	} else {
		float x = 500;
		float y = 0;
		float width = 400;
		float height = 400;
		Rectangle clip(x, y, width, height);

		totalDisplayTime += elapsedTime;
		if (totalDisplayTime >= ANIMATION_END) {
			visibleAchievement = NULL;
			return;
		} else if (totalDisplayTime >= ANIMATION_RESUME) {
			// reduced Y coordinate
			y -= (totalDisplayTime - ANIMATION_RESUME) / (ANIMATION_END - ANIMATION_RESUME) * height;
		} else if (totalDisplayTime >= ANIMATION_PAUSE) {
			// static Y coordinate at top of screen
		} else if (totalDisplayTime >= ANIMATION_START) {
			// increase Y coordinate
			y -= height - (totalDisplayTime - ANIMATION_START) / (ANIMATION_PAUSE - ANIMATION_START) * height;
		} else {
			return;
		}
		spriteBatch->start();
		int index = visibleAchievement->getAchievementIndex();
		float xIndex = index % ACHIEVEMENT_X_COUNT;
		float yIndex = index / ACHIEVEMENT_X_COUNT;				// calculate x and y coordinates within atlas map
		float u1 = xIndex / ACHIEVEMENT_X_COUNT;				// u coordinate is a factor between 0 and 1 in atlas map
		float u2 = (xIndex + 1) / ACHIEVEMENT_X_COUNT;			// to the right of the picture within atlas
		float v1 = 1 - yIndex / ACHIEVEMENT_Y_COUNT;			// v is indexed from 1 to 0
		float v2 = 1 - (yIndex + 1) / ACHIEVEMENT_Y_COUNT;		//
		spriteBatch->draw(x, y, width, height, u1, v1, u2, v2, Vector4::one());
		spriteBatch->finish();
	}
}
