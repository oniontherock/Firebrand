#include "../Include/Game/GameData.hpp"
#include "ECSRegistry.hpp"
#include "Panels.hpp"
#include <Graphics/PanelManager.hpp>

void PanelStaticView::panelUpdate() {

	checkModeChange();
	
	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);

	backgroundDraw(level);
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

	// draw background texture
	sf::Sprite backgroundSprite(levelActive->backgroundTexture.getTexture());
	backgroundSprite.setTextureRect(sf::IntRect(viewRect));
	backgroundSprite.setPosition(viewRect.left, viewRect.top);

	//// draw paths
	//sf::Sprite pathsSprite(levelActive->pathsTexture.getTexture());
	//pathsSprite.setTextureRect(sf::IntRect(viewRect));
	//pathsSprite.setPosition(viewRect.left, viewRect.top);

	objectDraw(backgroundRectangle);
	objectDraw(backgroundSprite);
	//objectDraw(pathsSprite);
}

void PanelDynamicView::panelUpdate() {

	if (isFirstUpdate) {
		viewMaskCreate();
		isFirstUpdate = false;
	}

	checkModeChange();

	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);

	staticDraw();
	charactersDraw(level);
	viewMaskApply();
}

sf::ConvexShape PanelDynamicView::viewMaskShapeCreate(float radius, float coneSize, uint16_t pointsCount) {

	// convex shape used for the vision mask
	sf::ConvexShape viewMaskShape;
	if (coneSize < Mathf::TAU) {
		viewMaskShape.setPointCount(pointsCount + 1);
		viewMaskShape.setPoint(pointsCount, sf::Vector2f(0, 0));
	}
	else {
		viewMaskShape.setPointCount(pointsCount);
	}

	// the angular distance between two points
	const float pointsAngleDistanceBetween = coneSize / pointsCount;

	// bool for whether the previous angle that was iterated was in the view cone
	bool previousWasInCone = false;
	for (uint16_t i = 0; i < pointsCount; i++) {

		// get the angle of this point in the view shape
		const float pointAngle = pointsAngleDistanceBetween * float(i - (pointsCount / 2));

		sf::Vector2f pointPosition = sf::Vector2f(cos(pointAngle), sin(pointAngle)) * radius;

		viewMaskShape.setPoint(i, pointPosition);
	}  
	return viewMaskShape;
}

void PanelDynamicView::viewMaskCreate() {

	sf::Vector2u viewMaskSize = sf::Vector2u(2000, 2000);

	sf::ConvexShape viewConeShape = viewMaskShapeCreate(1280.f, Mathf::PI / 2.f, 64);
	// set view mask position to center of the screen rect
	viewConeShape.setPosition(float(viewMaskSize.x / 2), float(viewMaskSize.y / 2));
	viewConeShape.setFillColor(sf::Color::White);

	sf::ConvexShape viewSurroundingsShape = viewMaskShapeCreate(48.f, Mathf::TAU, 64);
	// set view mask position to center of the screen rect
	viewSurroundingsShape.setPosition(float(viewMaskSize.x / 2), float(viewMaskSize.y / 2));
	viewSurroundingsShape.setFillColor(sf::Color::White);


	viewMaskTexture.create(viewMaskSize.x, viewMaskSize.y);
	viewMaskTexture.clear(sf::Color::Transparent);

	viewMaskTexture.draw(viewConeShape);
	viewMaskTexture.draw(viewSurroundingsShape);
	viewMaskTexture.display();
}

void PanelDynamicView::checkModeChange() {

}
void PanelDynamicView::staticDraw() {

	PanelStaticView& panelStaticView = static_cast<PanelStaticView&>(PanelManager::panelGet(PanelName::StaticView));

	sf::Sprite staticSprite;
	staticSprite.setTexture(panelStaticView.textureUngrayscaled);
	staticSprite.setPosition(viewRect.getPosition());

	objectDraw(staticSprite);
}
void PanelDynamicView::charactersDraw(GameLevel* levelActive) {

	for (const EntityId& i : levelActive->entities) {
		Entity& entityCur = EntityManager::entityGet(i);

		auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();
		auto* componentPosition = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

		objectDraw(componentSprite->sprite);
	}
}
void PanelDynamicView::viewMaskApply() {
	// render texture used for drawing a clipped section of the panel that works as a "view"
	sf::RenderTexture viewRenderTexture;
	viewRenderTexture.create(screenRect.getSize().x, screenRect.getSize().y);

	// sprite using the view mask's texture
	sf::Sprite viewMaskSprite;
	viewMaskSprite.setTexture(viewMaskTexture.getTexture());
	// set view mask sprite origin to the center of it's texture
	viewMaskSprite.setOrigin(viewMaskTexture.getTexture().getSize().x / 2.f, viewMaskTexture.getTexture().getSize().y / 2.f);

	// get player
	Entity& player = EntityManager::entityGet(GameData::playerId);
	// get player position
	sf::Vector2f playerPosition = player.entityComponentGet<EntityComponents::ComponentPosition>()->position;
	// get player rotation
	float playerRotation = player.entityComponentGet<EntityComponents::ComponentRotation>()->rotation;

	// set position to the player's position relative to the viewRect
	viewMaskSprite.setPosition(playerPosition - viewRect.getPosition());
	// set rotation to that of the player
	viewMaskSprite.setRotation(playerRotation * 180.f / Mathf::PI);

	viewRenderTexture.draw(viewMaskSprite);
	viewRenderTexture.display();

	sf::Sprite viewSprite;
	viewSprite.setTexture(viewRenderTexture.getTexture());
	// set viewSprite position to align with the viewRect
	viewSprite.setPosition(viewRect.getPosition());


	texture.draw(viewSprite, sf::BlendMultiply);
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


