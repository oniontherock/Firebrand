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
	sf::Sprite panelSprite;
	panelSprite.setTexture(textureUngrayscaled);
	// set panelSprite's position to that of the view rect so things line up properly (if we don't do this, the panelSprite is drawn as if it were an object in the world, which is obviously no good)
	panelSprite.setPosition(viewRect.getPosition());

	// load grayscale shader
	sf::Shader grayscaleShader;
	grayscaleShader.loadFromFile("Include/Shaders/Grayscale.glsl", sf::Shader::Fragment);
	grayscaleShader.setUniform("texture", sf::Shader::CurrentTexture);
	grayscaleShader.setUniform("divider", 1.1f);
	grayscaleShader.setUniform("lerp", 0.9f);
	
	// clear panel texture
	texture.clear();
	// draw panelSprite to the panel with grayscaling applied
	objectDraw(panelSprite, grayscaleShader);
}

void PanelStaticView::backgroundDraw(GameLevel* levelActive) {
	// draw background base color
	sf::RectangleShape backgroundRectangle;
	backgroundRectangle.setSize(viewRect.getSize());
	backgroundRectangle.setFillColor(sf::Color(10, 75, 0, 255));
	backgroundRectangle.setTextureRect(sf::IntRect(viewRect));
	backgroundRectangle.setPosition(viewRect.left, viewRect.top);



	levelActive->backgroundDraw(sf::FloatRect(viewRect.getPosition() - (viewRect.getSize() / 2.f), viewRect.getSize() * 2.f), 256);


	// draw base background color
	objectDraw(backgroundRectangle);
	// draw grass
	levelActive->backgroundTexture.drawRectangleToTexture(viewRect, texture);
	// draw paths
	levelActive->pathsTexture.drawRectangleToTexture(viewRect, texture);
}
void PanelStaticView::charactersDraw(GameLevel* levelActive) {
	for (const std::set<EntityId>& idSetCur : levelActive->entitiesDrawableStaticGet()) {
		for (const EntityId& idCur : idSetCur) {
			Entity& entityCur = EntityManager::entityGet(idCur);

			auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();

			objectDraw(componentSprite->sprite);
		}
	}
}

void PanelDynamicView::panelUpdate() {

	//if (isFirstUpdate) {
		viewMaskCreate();
	//}

	checkModeChange();

	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);

	staticDraw();
	charactersDraw(level);
	viewMaskApply();
	isFirstUpdate = false;
}

std::vector<sf::Vector2f> PanelDynamicView::viewMaskShapeCreate(float radius, float coneSize, uint16_t pointsCount) {

	// get player
	Entity& player = EntityManager::entityGet(GameData::playerId);

	GameLevel* gameLevel = GameLevelGrid::levelGet(player.levelId);

	// list of points used for the vision mask
	std::vector<sf::Vector2f> viewMaskShape;
	if (coneSize < Mathf::TAU) {
		//viewMaskShape.resize(pointsCount + 1);
		viewMaskShape.push_back(sf::Vector2f(0, 0));
	}
	else {
		//viewMaskShape.resize(pointsCount);
	}

	// get player position
	const sf::Vector2f playerPosition = player.entityComponentGet<EntityComponents::ComponentPosition>()->position;
	// get player rotation
	const float playerRotation = player.entityComponentGet<EntityComponents::ComponentRotation>()->rotation;;
	// get cone angle
	const float coneAngle = playerRotation - (coneSize / 2.f);

	// the angular difference (in radians) between two rays.
	const float rayAngleDifference = coneSize / pointsCount;

	for (uint32_t curRayInd = 0; curRayInd < pointsCount; curRayInd++) {

		// the rotation (in radians) of the current ray.
		const float rayRotation = coneAngle + (float(curRayInd) * rayAngleDifference);
		// heading of the ray
		const sf::Vector2f rayHeading = sf::Vector2f(cos(rayRotation), sin(rayRotation));
		// iterate from 0.f to rayMaxDist in steps of 1.f, updating the ray each iteration
		viewMaskShape.push_back(sf::Vector2f(cos(rayRotation), sin(rayRotation)));
		float dist = 0.f;
		for (; dist < radius; dist += 1.f) {
			// calculate ray position from castPosition and heading * dist
			sf::Vector2f rayPositionCur = playerPosition + (rayHeading * dist);

			if (!gameLevel->occlusionGrid.worldPosIsInGridFull(rayPositionCur)) break;

			// if we should occlude, check if the ray's position falls on a cell in the occlusion grid which is marked as occluding, if that's the case, break
			if (gameLevel->occlusionGrid.cellGetFromWorld(rayPositionCur)) break;


		}
		sf::Vector2f pointPosition = sf::Vector2f(cos(rayRotation), sin(rayRotation)) * dist;
		viewMaskShape.push_back(pointPosition);
	}

	return viewMaskShape;
}

void PanelDynamicView::viewMaskCreate() {

	sf::Vector2u viewMaskSize = sf::Vector2u(2000, 2000);

	std::vector<sf::Vector2f> viewConePoints = viewMaskShapeCreate(1280.f, Mathf::PI / 2.f, 512);

	sf::VertexArray coneVertexArray(sf::Lines, viewConePoints.size());

	for (uint32_t i = 0; i < viewConePoints.size(); i++) {
		coneVertexArray[i].position = viewConePoints[i] + sf::Vector2f(viewMaskSize / 2u);
		coneVertexArray[i].color = sf::Color(255, 255, 255, 255);
	}

	std::vector<sf::Vector2f> viewAreaPoints = viewMaskShapeCreate(48.f, Mathf::TAU, 256);

	sf::VertexArray areaVertexArray(sf::Lines, viewAreaPoints.size());

	for (uint32_t i = 0; i < viewAreaPoints.size(); i++) {
		areaVertexArray[i].position = viewAreaPoints[i] + sf::Vector2f(viewMaskSize / 2u);
		areaVertexArray[i].color = sf::Color(255, 255, 255, 255);
	}

	viewMaskTexture.create(viewMaskSize.x, viewMaskSize.y);
	viewMaskTexture.clear(sf::Color::Transparent);

	viewMaskTexture.draw(coneVertexArray);
	viewMaskTexture.draw(areaVertexArray);
	viewMaskTexture.display();
}

void PanelDynamicView::checkModeChange() {
	//if (InputInterface::inputGetActive("Toggle Room Type Drawing")) {
	//	if (mode == Normal) mode = DebugDrawRoomTypes;
	//	else mode = Normal;
	//}
}
void PanelDynamicView::staticDraw() {

	PanelStaticView& panelStaticView = static_cast<PanelStaticView&>(PanelManager::panelGet(PanelName::StaticView));

	sf::Sprite staticSprite;
	staticSprite.setTexture(panelStaticView.textureUngrayscaled);
	staticSprite.setPosition(viewRect.getPosition());

	objectDraw(staticSprite);
}
void PanelDynamicView::charactersDraw(GameLevel* levelActive) {
	for (const std::set<EntityId>& idSetCur : levelActive->entitiesDrawableDynamicGet()) {
		for (const EntityId& idCur : idSetCur) {
			Entity& entityCur = EntityManager::entityGet(idCur);

			auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();

			objectDraw(componentSprite->sprite);
		}
	}
}
void PanelDynamicView::viewMaskApply() {
	// render texture used for drawing a clipped section of the panel that works as a "view"
	sf::RenderTexture viewRenderTexture;
	viewRenderTexture.create(uint16_t(screenRect.getSize().x), uint16_t(screenRect.getSize().y));

	// sprite using the view mask's texture
	sf::Sprite viewMaskSprite;
	viewMaskSprite.setTexture(viewMaskTexture.getTexture());
	// set view mask sprite origin to the center of it's texture
	viewMaskSprite.setOrigin(viewMaskTexture.getTexture().getSize().x / 2.f, viewMaskTexture.getTexture().getSize().y / 2.f);

	// get player
	Entity& player = EntityManager::entityGet(GameData::playerId);
	// get player position
	sf::Vector2f playerPosition = player.entityComponentGet<EntityComponents::ComponentPosition>()->position;

	// set position to the player's position relative to the viewRect
	viewMaskSprite.setPosition(playerPosition - viewRect.getPosition());

	viewRenderTexture.draw(viewMaskSprite);
	viewRenderTexture.display();

	sf::Sprite viewSprite;
	viewSprite.setTexture(viewRenderTexture.getTexture());
	// set viewSprite position to align with the viewRect
	viewSprite.setPosition(viewRect.getPosition());

	texture.draw(viewSprite, sf::BlendMultiply);
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
		objectGridSprite.setPosition(viewRect.getPosition());
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

			if (entity.entityComponentHas<EntityComponents::ComponentCollisionShape>()) {
				auto* componentCollisionShape = entity.entityComponentGet<EntityComponents::ComponentCollisionShape>();
				sf::ConvexShape convexShape;
				convexShape.setPointCount(componentCollisionShape->shape.vertices.size());
				for (uint16_t i = 0; i < componentCollisionShape->shape.vertices.size(); i++) {
					convexShape.setPoint(i, componentCollisionShape->shape.vertices[i]);
				}
				convexShape.setFillColor(sf::Color(255, 0, 0, 127));

				objectDraw(convexShape);
			}
			else if (entity.entityComponentHas<EntityComponents::ComponentCollisionShapeMulti>()) {
				auto* componentCollisionShapeMulti = entity.entityComponentGet<EntityComponents::ComponentCollisionShapeMulti>();

				for (CollisionShape& shape : componentCollisionShapeMulti->shapes) {

					sf::ConvexShape convexShape;
					convexShape.setPointCount(shape.vertices.size());
					for (uint16_t i = 0; i < shape.vertices.size(); i++) {
						convexShape.setPoint(i, shape.vertices[i]);
					}
					convexShape.setFillColor(sf::Color(255, 0, 0, 127));

					objectDraw(convexShape);
				}
			}
		}
		for (uint16_t i = 0; i < level->entitiesNoUpdate.size(); i++) {
			Entity& entity = EntityManager::entityGet(level->entitiesNoUpdate[i]);

			if (entity.entityComponentHas<EntityComponents::ComponentCollisionShape>()) {
				auto* componentCollisionShape = entity.entityComponentGet<EntityComponents::ComponentCollisionShape>();
				sf::ConvexShape convexShape;
				convexShape.setPointCount(componentCollisionShape->shape.vertices.size());
				for (uint16_t i = 0; i < componentCollisionShape->shape.vertices.size(); i++) {
					convexShape.setPoint(i, componentCollisionShape->shape.vertices[i]);
				}
				convexShape.setFillColor(sf::Color(255, 0, 0, 127));

				objectDraw(convexShape);
			}
			else if (entity.entityComponentHas<EntityComponents::ComponentCollisionShapeMulti>()) {
				auto* componentCollisionShapeMulti = entity.entityComponentGet<EntityComponents::ComponentCollisionShapeMulti>();
				
				for (CollisionShape& shape : componentCollisionShapeMulti->shapes) {

					sf::ConvexShape convexShape;
					convexShape.setPointCount(shape.vertices.size());
					for (uint16_t i = 0; i < shape.vertices.size(); i++) {
						convexShape.setPoint(i, shape.vertices[i]);
					}
					convexShape.setFillColor(sf::Color(255, 0, 0, 127));

					objectDraw(convexShape);
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
	sf::Text text;
	sf::Font font;
	font.loadFromFile("Art/Basic bitmap.ttf");
	text.setFont(font);
	text.setString("You Win!");

	text.setPosition(500, 500);
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
	sf::Text text;
	sf::Font font;
	font.loadFromFile("Art/Basic bitmap.ttf");
	text.setFont(font);
	text.setString("You Lose!");

	text.setPosition(500, 500);
	objectDraw(text);
}


