#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Panels.hpp"

void PanelGameView::panelUpdate() {

	checkModeChange();
	
	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);

	for (const EntityId& i : level->entities) {
		Entity& entityCur = EntityManager::entityGet(i);

		auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();
		auto* componentPosition = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

		objectDraw(componentSprite->sprite);
	}
}

void PanelGameView::checkModeChange() {
	 
}
