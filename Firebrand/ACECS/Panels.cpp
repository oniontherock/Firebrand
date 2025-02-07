#include "../Include/Debugging/AStarPathDrawer.hpp"
#include "../Include/Debugging/ObjectGridRenderer.hpp"
#include "../Include/Game/GameData.hpp"
#include "ECSRegistry.hpp"
#include "Panels.hpp"
#include "SFML/Graphics.hpp"
#include <Graphics/PanelManager.hpp>
#include <Input/InputInterface.hpp>

void PanelStaticView::panelUpdate() {

	checkModeChange();
	
	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);

	backgroundDraw(level);
	charactersDraw(level);
	panelApplyGrayscale();
}

void PanelStaticView::checkModeChange() {
	 
}

void PanelStaticView::panelApplyGrayscale() {

	// display panel texture since we're about to use it
	texture.display();

	// set textureUngrayscaled to panel texture prior to grayscaling
	textureUngrayscaled = texture.getTexture();

	// create sprite from textureUngrayscaled (we use textureUngrayscaled because using texture.getTexture() returns a reference, and we clear texture prior to drawing so we need a copy)
	sf::Sprite panelSprite(textureUngrayscaled);
	// set panelSprite's position to that of the view rect so things line up properly (if we don't do this, the panelSprite is drawn as if it were an object in the world, which is obviously no good)
	panelSprite.setPosition(viewRect.position);

	// load grayscale shader
	sf::Shader grayscaleShader;
	if (!grayscaleShader.loadFromFile("Include/Shaders/Grayscale.glsl", sf::Shader::Type::Fragment)) {
		ConsoleHandler::consolePrintErr("Grayscale shader doesn't exist or path is incorrect!");
	}
	grayscaleShader.setUniform("texture", sf::Shader::CurrentTexture);
	grayscaleShader.setUniform("divider", 1.4f);
	grayscaleShader.setUniform("lerp", 0.9f);
	//panelSprite.setColor()

	// clear panel texture
	texture.clear();
	// draw panelSprite to the panel with grayscaling applied
	texture.draw(panelSprite, &grayscaleShader);
}

void PanelStaticView::backgroundDraw(GameLevel* levelActive) {
	// draw background base color
	sf::RectangleShape backgroundRectangle;
	backgroundRectangle.setSize(viewRect.size);
	backgroundRectangle.setFillColor(sf::Color(10, 75, 0, 255));
	backgroundRectangle.setTextureRect(sf::IntRect(viewRect));
	backgroundRectangle.setPosition(viewRect.position);

	levelActive->backgroundDraw(sf::FloatRect(viewRect.position - (viewRect.size / 2.f), viewRect.size * 2.f), 256);

	// draw base background color
	texture.draw(backgroundRectangle);
	// draw grass
	levelActive->backgroundTexture.drawRectangleToTexture(viewRect, texture);
	// draw paths
	levelActive->pathsTexture.drawRectangleToTexture(viewRect, texture);
}
void PanelStaticView::charactersDraw(GameLevel* levelActive) {
	

	static std::vector<EntityId> idsToDraw;

	static Cooldown cullCooldown(1.f);

	constexpr float cullDist = 1000.f;

	if (cullCooldown.updateAutoReset(TimeHandler::deltaRealGet())) {

		idsToDraw.clear();
		
		sf::Vector2f playerPosition = EntityManager::entityGet(GameData::playerId).entityComponentGet<EntityComponents::ComponentPosition>()->position;

		for (const std::set<EntityId>& idSetCur : levelActive->entitiesDrawableStaticGet()) {
			for (const EntityId& idCur : idSetCur) {
				if (Vector2fMath::distSqrd(playerPosition, EntityManager::entityGet(idCur).entityComponentGet<EntityComponents::ComponentPosition>()->position) > cullDist * cullDist) continue;

				idsToDraw.push_back(idCur);
			}
		}

	}

	for (uint32_t i = 0; i < idsToDraw.size(); i++) {
		Entity& entityCur = EntityManager::entityGet(idsToDraw[i]);

		auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();

		sf::Texture& textureCur = GraphicsStore::textureStore.objectGet(componentSprite->fileName);

		sf::Sprite sprite(textureCur);

		if (entityCur.entityComponentHas<EntityComponents::ComponentSpriteColor>()) {
			sprite.setColor(entityCur.entityComponentGet<EntityComponents::ComponentSpriteColor>()->color);
		}

		if (!entityCur.entityComponentHas<EntityComponents::ComponentSpriteOrigin>()) {
			sprite.setOrigin(sf::Vector2f(textureCur.getSize()) / 2.f);
		}
		else {
			sprite.setOrigin(entityCur.entityComponentGet<EntityComponents::ComponentSpriteOrigin>()->origin);
		}
		sprite.setPosition(entityCur.entityComponentGet<EntityComponents::ComponentPosition>()->position);
		if (entityCur.entityComponentHas<EntityComponents::ComponentRotation>()) {
			sprite.setRotation(sf::radians(entityCur.entityComponentGet<EntityComponents::ComponentRotation>()->rotation));
		}

		texture.draw(sprite);
	}
}

PanelDynamicView::PanelDynamicView(PanelRect _screenRect, PanelRect _viewRect, sf::Color _clearColor) {
	screenRect = _screenRect;
	viewRect = _viewRect;
	clearColor = _clearColor;
	viewRectSizeOriginal = _viewRect.size;

	texture = PanelScreenTexture(sf::Vector2u(screenRect.size), sf::ContextSettings{ 0, 8 });
	viewCreate();
}

void PanelDynamicView::panelUpdate() {


	checkModeChange();

	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);

	texture.clearStencil(0);
	viewMaskCreate();

	staticDraw();
	charactersDraw(level);
	isFirstUpdate = false;
}

std::vector<sf::Vector2f> PanelDynamicView::viewMaskShapeCreate(sf::Vector2f position, float angle, float radius, float coneSize, uint16_t pointsCount, GameLevel* gameLevel) {

	const float coneAngle = angle - (coneSize / 2.f);

	// list of points used for the vision mask
	std::vector<sf::Vector2f> viewMaskShape;

	viewMaskShape.push_back(position);

	// the angular difference (in radians) between two rays.
	const float rayAngleDifference = coneSize / pointsCount;

	for (uint32_t curRayInd = 0; curRayInd < pointsCount; curRayInd++) {

		// the rotation (in radians) of the current ray.
		const float rayRotation = coneAngle + (float(curRayInd) * rayAngleDifference);
		// heading of the ray
		const sf::Vector2f rayHeading = sf::Vector2f(cos(rayRotation), sin(rayRotation));

		// iterate from 0.f to rayMaxDist in steps of 1.f, updating the ray each iteration
		float dist = 0.f;
		for (; dist < radius; dist += 1.f) {
			// calculate ray position from castPosition and heading * dist
			sf::Vector2f rayPositionCur = position + (rayHeading * dist);

			if (!gameLevel->occlusionGrid.worldPosIsInGridFull(rayPositionCur)) break;

			// if the cell is o
			if (gameLevel->occlusionGrid.cellGetFromWorld(rayPositionCur)) break;
		}
		viewMaskShape.push_back(position + (rayHeading * dist));
	}

	return viewMaskShape;
}

void PanelDynamicView::viewMaskCreate() {

	// get player
	Entity& player = EntityManager::entityGet(GameData::playerId);
	// get the level the player is in
	GameLevel* gameLevel = GameLevelGrid::levelGet(player.levelId);
	// get player position
	const sf::Vector2f playerPosition = player.entityComponentGet<EntityComponents::ComponentPosition>()->position;
	// get player rotation
	const float playerRotation = player.entityComponentGet<EntityComponents::ComponentRotation>()->rotation;

	float coneSize = Mathf::PI / 2.f;

	std::vector<sf::Vector2f> viewConePoints = viewMaskShapeCreate(playerPosition, playerRotation, 1280.f, coneSize, 128, gameLevel);
	std::vector<sf::Vector2f> viewAreaPoints = viewMaskShapeCreate(playerPosition, playerRotation + Mathf::PI, 48.f, Mathf::TAU - (coneSize/1.05f), 128, gameLevel);

	sf::VertexArray coneVertexArray(sf::PrimitiveType::TriangleFan, viewConePoints.size());

	for (uint32_t i = 0; i < viewConePoints.size(); i++) {
		coneVertexArray[i].position = viewConePoints[i];
		coneVertexArray[i].color = sf::Color(255, 255, 255, 255);
	}


	for (uint32_t i = 0; i < viewAreaPoints.size(); i++) {
		sf::Vertex vertex;
		vertex.position = viewAreaPoints[i];
		vertex.color = sf::Color::White;
		coneVertexArray.append(vertex);
	}

	texture.clearStencil(0);
	texture.draw(coneVertexArray, sf::StencilMode{ sf::StencilComparison::Always, sf::StencilUpdateOperation::Replace, 1, 0xFF, true });
}

void PanelDynamicView::checkModeChange() {
	//if (InputInterface::inputGetActive("Toggle Room Type Drawing")) {
	//	if (mode == Normal) mode = DebugDrawRoomTypes;
	//	else mode = Normal;
	//}
}
void PanelDynamicView::staticDraw() {

	PanelStaticView& panelStaticView = static_cast<PanelStaticView&>(PanelManager::panelGet(PanelName::StaticView));

	sf::Sprite staticSprite(panelStaticView.textureUngrayscaled);
	staticSprite.setPosition(viewRect.position);

	texture.draw(staticSprite, sf::StencilMode{ sf::StencilComparison::Equal, sf::StencilUpdateOperation::Keep, 1, 0xFF, false });
}
void PanelDynamicView::charactersDraw(GameLevel* levelActive) {

	static std::vector<EntityId> idsToDraw;

	static Cooldown cullCooldown(1.f);

	constexpr float cullDist = 300.f;

	if (cullCooldown.updateAutoReset(TimeHandler::deltaRealGet())) {

		idsToDraw.clear();

		sf::Vector2f playerPosition = EntityManager::entityGet(GameData::playerId).entityComponentGet<EntityComponents::ComponentPosition>()->position;

		for (const std::set<EntityId>& idSetCur : levelActive->entitiesDrawableDynamicGet()) {
			for (const EntityId& idCur : idSetCur) {
				if (Vector2fMath::distSqrd(playerPosition, EntityManager::entityGet(idCur).entityComponentGet<EntityComponents::ComponentPosition>()->position) > cullDist * cullDist) continue;

				idsToDraw.push_back(idCur);
			}
		}

	}

	for (uint32_t i = 0; i < idsToDraw.size(); i++) {
		Entity& entityCur = EntityManager::entityGet(idsToDraw[i]);

		auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();

		sf::Texture& textureCur = GraphicsStore::textureStore.objectGet(componentSprite->fileName);

		sf::Sprite sprite(textureCur);

		if (entityCur.entityComponentHas<EntityComponents::ComponentSpriteColor>()) {
			sprite.setColor(entityCur.entityComponentGet<EntityComponents::ComponentSpriteColor>()->color);
		}

		if (!entityCur.entityComponentHas<EntityComponents::ComponentSpriteOrigin>()) {
			sprite.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
		}
		else {
			sprite.setOrigin(entityCur.entityComponentGet<EntityComponents::ComponentSpriteOrigin>()->origin);
		}
		if (entityCur.entityComponentHas<EntityComponents::ComponentRotation>()) {
			sprite.setRotation(sf::radians(entityCur.entityComponentGet<EntityComponents::ComponentRotation>()->rotation));
		}

		sprite.setPosition(entityCur.entityComponentGet<EntityComponents::ComponentPosition>()->position);

		texture.draw(sprite, sf::StencilMode{ sf::StencilComparison::Equal, sf::StencilUpdateOperation::Keep, 1, 0xFF, false });
	}
}

void PanelHud::panelUpdate() {

	checkModeChange();

	GameLevel* level = GameLevelGrid::levelGet(EntityManager::entityGet(GameData::playerId).levelId);

	if (mode == Normal || mode == ObjectsRender || mode == PathsRender || mode == CollisionShapesRender) {
	}
	if (mode == ObjectsRender) {
		ObjectGrid& objectGrid = level->objectGrid;

		static ObjectGridRenderer objectGridRenderer = ObjectGridRenderer();

		objectGridRenderer.gridRender(objectGrid, viewRect);

		sf::Sprite objectGridSprite(objectGridRenderer.texture);
		objectGridSprite.setScale(objectGrid.cellsGetSize());
		objectGridSprite.setPosition(viewRect.position);
		objectGridSprite.setColor(sf::Color(255, 255, 255, 200));

		objectDraw(objectGridSprite);
	}
	if (mode == PathsRender) {
		AStarPathDrawer::cellsInvalidDraw(level->aStarGrid);
		AStarPathDrawer::pathsTexture.display();

		sf::Texture pathsTexture = AStarPathDrawer::pathsTexture.getTexture();

		sf::Sprite pathsSprite(pathsTexture);

		objectDraw(pathsSprite);
		AStarPathDrawer::pathsTextureReset();
	}
	if (mode == CollisionShapesRender) {

		for (uint16_t i = 0; i < level->entities.size(); i++) {
			Entity& entity = EntityManager::entityGet(level->entities[i]);

			if (entity.entityComponentHas<EntityComponents::ComponentCollisionPolygons>()) {
				auto* componentCollisionShapeMulti = entity.entityComponentGet<EntityComponents::ComponentCollisionPolygons>();

				for (CollisionShapeBase& shape : componentCollisionShapeMulti->shapes) {

					try {
						CollisionShapePolygon& polygon = dynamic_cast<CollisionShapePolygon&>(shape);

						sf::ConvexShape convexShape;
						convexShape.setPointCount(polygon.vertices.size());
						for (uint16_t i = 0; i < polygon.vertices.size(); i++) {
							convexShape.setPoint(i, polygon.vertices[i]);
						}
						convexShape.setFillColor(sf::Color(255, 0, 0, 127));

						objectDraw(convexShape);
					}
					catch (const std::bad_cast& e) {
						continue;
					}

				}
			}
		}
		for (uint16_t i = 0; i < level->entitiesNoUpdate.size(); i++) {
			Entity& entity = EntityManager::entityGet(level->entitiesNoUpdate[i]);
			
			if (entity.entityComponentHas<EntityComponents::ComponentCollisionPolygons>()) {
				auto* componentCollisionShapeMulti = entity.entityComponentGet<EntityComponents::ComponentCollisionPolygons>();
				
				for (CollisionShapeBase& shape : componentCollisionShapeMulti->shapes) {

					try {
						CollisionShapePolygon& polygon = dynamic_cast<CollisionShapePolygon&>(shape);

						sf::ConvexShape convexShape;
						convexShape.setPointCount(polygon.vertices.size());
						for (uint16_t i = 0; i < polygon.vertices.size(); i++) {
							convexShape.setPoint(i, polygon.vertices[i]);
						}
						convexShape.setFillColor(sf::Color(255, 0, 0, 127));

						objectDraw(convexShape);
					}
					catch (const std::bad_cast&) {
						continue;
					}
				}
			}
		}
	}
}
void PanelHud::checkModeChange() {
	if (InputInterface::inputGetActive("Toggle ObjectGrid Rendering")) {
		if (mode != ObjectsRender) {
			mode = ObjectsRender;
		}
		else {
			mode = Normal;
		}
	}
	if (InputInterface::inputGetActive("Toggle Paths Rendering")) {
		if (mode != PathsRender) {
			mode = PathsRender;
		}
		else {
			mode = Normal;
		}
	}
	if (InputInterface::inputGetActive("Toggle Collision Shapes Draw")) {
		if (mode != CollisionShapesRender) {
			mode = CollisionShapesRender;
		}
		else {
			mode = Normal;
		}
	}
}


void PanelWinScreen::panelUpdate() {

	checkModeChange();

	backgroundDraw();
	textDraw();
}
void PanelWinScreen::checkModeChange() {
}
void PanelWinScreen::backgroundDraw() {
}
void PanelWinScreen::textDraw() {
	sf::Font font;
	if (!font.openFromFile("Art/Basic bitmap.ttf")) {
		ConsoleHandler::consolePrintErr("Font loading failed");
	}

	sf::Text text(font);
	text.setFont(font);
	text.setString("You Win!");

	text.setPosition(sf::Vector2f(500, 500));
	objectDraw(text);
}

void PanelLoseScreen::panelUpdate() {

	checkModeChange();

	backgroundDraw();
	textDraw();
}
void PanelLoseScreen::checkModeChange() {
}
void PanelLoseScreen::backgroundDraw() {
}
void PanelLoseScreen::textDraw() {
	sf::Font font;
	if (!font.openFromFile("Art/Basic bitmap.ttf")) {
		ConsoleHandler::consolePrintErr("Font loading failed");
	}

	sf::Text text(font);
	text.setFont(font);
	text.setString("You Lose!");

	text.setPosition(sf::Vector2f(500, 500));
	objectDraw(text);
}


