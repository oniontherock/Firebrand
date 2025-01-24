#include "ECSRegistry.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/TimeHandler.hpp>
#include <Auxiliary/VectorMath.hpp>
#include <Graphics/PanelManager.hpp>
#include <Input.hpp>

uint32_t MAX_ENTITIES = 10000;
uint16_t MAX_EVENT_TYPES = 5;
uint16_t MAX_COMPONENT_TYPES = 24;

void ECSRegistry::ECSInitialize() {
	EntityManager::entityIdsInitialize();
	EntityComponents::componentIDsInitialize();
	EntityComponents::componentTemplatesInitialize();
	EntityEvents::eventIDsInitialize();
	EventPool::eventPoolInitialize();
}
void ECSRegistry::ECSTerminate() {
	EntityEvents::allEventsTerminate();
	EntityManager::entitiesAllDelete();
}

#pragma region Events
/*
registers all events
registering an event gives it an ID which dictates it's update order, lower ID, sooner update.

the order of registry is very important, as it heavily dictates the behavior of events,
for example:

REGISTER(Event, EventA)
REGISTER(Event, EventB)

in this example, EventA is ALWAYS updated BEFORE EventB,
which is very important, because if EventA sends and event, EventB will always receive it,
but if the order were swapped, EventB would never receive it
*/
void EntityEvents::eventIDsInitialize() {
	using EventRegistry = TypeIDAllocator<Event>;

	EventRegistry::typeRegister<EventIDs<EventInitialize>>();
	EventRegistry::typeRegister<EventIDs<EventMove>>();
	EventRegistry::typeRegister<EventIDs<EventRotate>>();
	EventRegistry::typeRegister<EventIDs<EventObjectSeen>>();
	EventRegistry::typeRegister<EventIDs<EventCollision>>();

	//EventRegistry::typeRegister<EventIDs<EVENT_GOES_HERE>>();
	//EventRegistry::typeRegister<EventIDs<EVENT_GOES_HERE>>();
	//EventRegistry::typeRegister<EventIDs<EVENT_GOES_HERE>>();
}

#pragma endregion Events
#pragma region Components
/*
registers all components,
registering a component gives it an ID which dictates it's update order, lower ID, sooner update.

the order of registry is very important, as it heavily dictates the behavior of components,
for example:

REGISTER(Component, ComponentA)
REGISTER(Component, ComponentB)

in this example, ComponentA is ALWAYS updated BEFORE ComponentB,
which is very important, because if ComponentA sends and event, ComponentB will always receive it,
but if the order were swapped, ComponentB would never receive it
*/


void EntityComponents::componentIDsInitialize() {
	using ComponentRegistry = TypeIDAllocator<Component>;

	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectTypeAssigner>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentCollidable>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentCollider>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByInput>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotateToMouse>>();

	// collision response
	ComponentRegistry::typeRegister<ComponentIDs<ComponentCollisionResponse>>();
	// physics
	ComponentRegistry::typeRegister<ComponentIDs<ComponentHingeOnPoint>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMass>>();
	// transform
	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();
	// collision
	ComponentRegistry::typeRegister<ComponentIDs<ComponentCollisionPolygons>>();

	//ComponentRegistry::typeRegister<ComponentIDs<COMPONENT_GOES_HERE>>();
	//ComponentRegistry::typeRegister<ComponentIDs<COMPONENT_GOES_HERE>>();
	//ComponentRegistry::typeRegister<ComponentIDs<COMPONENT_GOES_HERE>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentOcclusionRectangles>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentOcclusionRadius>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectGridInhabiterRadius>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectGridInhabiterRectangles>>();

	// sprites/drawing
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteOrigin>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteFlip>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteColor>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSprite>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentViewFollow>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectVision>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectVisionDebug>>();

}

#pragma endregion Components
#pragma region Component Templates

void EntityComponents::componentTemplatesInitialize() {
	using namespace EntityComponents;

	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Transform",
		/// list of components in template
		{
			createComponentPairFromType<ComponentPosition>(),
			createComponentPairFromType<ComponentRotation>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Input Controlled",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentMoveByInput>(),
			createComponentPairFromType<ComponentRotateToMouse>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Player",
		{
			"Input Controlled",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Player),
			createComponentPairFromType<ComponentObjectGridInhabiterRadius>(32.f),
			createComponentPairFromType<ComponentMoveByInput>(120.f),
			createComponentPairFromType<ComponentRotateToMouse>(Mathf::TAU * 1.25f),
			createComponentPairFromType<ComponentSprite>("Art/Squad Member", false, 50u),
			createComponentPairFromType<ComponentViewFollow>(std::vector<PanelName> { PanelName::StaticView, PanelName::DynamicView, PanelName::Hud }),
			createComponentPairFromType<ComponentCollider>(),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-12, -24), sf::Vector2f(12, -24), sf::Vector2f(12, 24), sf::Vector2f(-12, 24)
				})),
			createComponentPairFromType<ComponentCollisionResponse>(),
			createComponentPairFromType<ComponentMass>(120.f),
		}
	);
#pragma region Wall Templates
	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Wall Single",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Wall),
			createComponentPairFromType<ComponentSprite>("Art/Structures/Walls/Wall Wooden Single", false, 60),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-12, -12, 48, 24) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-12, -12, 48, 24) }),
			createComponentPairFromType<ComponentCollidable>(),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-12, -12), sf::Vector2f(32, -12), sf::Vector2f(32, 12), sf::Vector2f(-12, 12)
				})),
			createComponentPairFromType<ComponentMass>(10000000000.f),
		}
	);
	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Wall Straight",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Wall),
			createComponentPairFromType<ComponentSprite>("Art/Structures/Walls/Wall Wooden Straight", false, 60),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-32, -6, 64, 12) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-32, -6, 64, 12) }),
			createComponentPairFromType<ComponentCollidable>(),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-32, -6), sf::Vector2f(32, -6), sf::Vector2f(32, 6), sf::Vector2f(-32, 6)
				})),
			createComponentPairFromType<ComponentMass>(10000000000.f),
		}
	);
	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Wall Corner",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Wall),
			createComponentPairFromType<ComponentSprite>("Art/Structures/Walls/Wall Wooden Corner", false, 60),
			createComponentPairFromType<ComponentSpriteOrigin>(12.f, 12.f),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-5, -5, 10, 38), sf::FloatRect(-5, -5, 38, 10) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-5, -5, 10, 38), sf::FloatRect(-5, -5, 38, 10) }),
			createComponentPairFromType<ComponentCollidable>(),
			createComponentPairFromType<ComponentCollisionPolygons>(std::vector{
			CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-6, -6), sf::Vector2f(6, -6), sf::Vector2f(6, 32), sf::Vector2f(-6, 32)
				}),
			CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(6, -6), sf::Vector2f(32, -6), sf::Vector2f(32, 6), sf::Vector2f(6, 6)
				})
			}),
			createComponentPairFromType<ComponentMass>(10000000000.f),
		}
	);
	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Wall Junction T",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Wall),	
			createComponentPairFromType<ComponentSprite>("Art/Structures/Walls/Wall Wooden Junction T", false, 60),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-5, -5, 38, 10), sf::FloatRect(-5, -32, 10, 65) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-5, -5, 38, 10), sf::FloatRect(-5, -32, 10, 65) }),
			createComponentPairFromType<ComponentCollidable>(),
			createComponentPairFromType<ComponentCollisionPolygons>(std::vector{
			CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-6, -32), sf::Vector2f(6, -32), sf::Vector2f(6, 32), sf::Vector2f(-6, 32)
				}),
			CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(6, -6), sf::Vector2f(32, -6), sf::Vector2f(32, 6), sf::Vector2f(6, 6)
				})
			}),
			createComponentPairFromType<ComponentMass>(10000000000.f),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Wall Junction Plus",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Wall),
			createComponentPairFromType<ComponentSprite>("Art/Structures/Walls/Wall Wooden Junction Plus", false, 60),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-32, -5, 64, 10), sf::FloatRect(-5, -32, 10, 64) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-32, -5, 64, 10), sf::FloatRect(-5, -32, 10, 64) }),
			createComponentPairFromType<ComponentCollidable>(),
			createComponentPairFromType<ComponentCollisionPolygons>(std::vector{
			CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-6, -32), sf::Vector2f(6, -32), sf::Vector2f(6, 32), sf::Vector2f(-6, 32)
				}),
			CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-32, -6), sf::Vector2f(32, -6), sf::Vector2f(32, 6), sf::Vector2f(-32, 6)
				})
			}),
			createComponentPairFromType<ComponentMass>(10000000000.f),
		}
		);
#pragma endregion Wall Templates
#pragma region Floor Templates

	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Floor Plank",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentSprite>("Art/Structures/Floors/Floor Plank", false, 0),
		}
		);

	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Floor Tile",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentSprite>("Art/Structures/Floors/Floor Tile", false, 0),
		}
		);
#pragma endregion Floor Templates
	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Door Wooden",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Wall),
			createComponentPairFromType<ComponentSprite>("Art/Structures/Doors/Door Wooden", false, 60),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-32, -2, 64, 4) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(-32, -2, 64, 4) }),
			createComponentPairFromType<ComponentCollidable>(),
			createComponentPairFromType<ComponentCollider>(),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-24, -4), sf::Vector2f(24, -4), sf::Vector2f(24, 4), sf::Vector2f(-24, 4)
				})),
			createComponentPairFromType<ComponentCollisionResponse>(),
			createComponentPairFromType<ComponentMass>(20.f),
			createComponentPairFromType<ComponentHingeOnPoint>(sf::Vector2f(-32, 0)),
		}
		);

	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Test Object",
		{
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Door),
			createComponentPairFromType<ComponentObjectGridInhabiterRadius>(32.f),
			createComponentPairFromType<ComponentPosition>(sf::Vector2f(512.f, 256.f)),
			createComponentPairFromType<ComponentSprite>("Art/Circle", true, 50),

		}
		);
}

#pragma endregion Component Templates
#pragma region Systems

using namespace EntityComponents;
using namespace EntityEvents;

// if you need to include a certain file for a system, include it here.
#include <iostream>
#include "../Include/Game/World/Objects/ObjectRegistry.hpp"
#include "../Include/Game/World/Physics/Collision/Collision Processor/CollisionProcessor.hpp"

// if the system is not using the entity parameter, remove it's name to avoid a C4100 error

void ComponentMoveByInput::system(Entity& entity) {
	sf::Vector2f inputAxis;
	inputAxis.x = float(InputInterface::inputGetActive("Move Right") - InputInterface::inputGetActive("Move Left"));
	inputAxis.y = float(InputInterface::inputGetActive("Move Down") - InputInterface::inputGetActive("Move Up"));

	if (inputAxis.x != 0 || inputAxis.y != 0) {

		inputAxis = Vector2fMath::normalize(inputAxis) * float(double(moveSpeed * TimeHandler::deltaSimulatedGet()));

		auto* moveEvent = entity.entityEventAddAndGet<EventMove>();
		moveEvent->moveAxis = inputAxis;
	}
}
void ComponentRotateToMouse::system(Entity& entity) {
	if (entity.entityComponentHas<ComponentPosition>() && entity.entityComponentHas<ComponentRotation>()) {
		const float turnSpeedDelta = float(double(turnSpeed) * TimeHandler::deltaSimulatedGet());

		auto& gameViewPanel = PanelManager::panelGet(PanelName::StaticView);

		float angle = Vector2fMath::angle(entity.entityComponentGet<ComponentPosition>()->position, gameViewPanel.viewMousePositionGet());

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

		// wrap rotation between -PI and +PI
		if (rotationComponent->rotation >= +Mathf::PI) rotationComponent->rotation -= Mathf::TAU;
		if (rotationComponent->rotation <= -Mathf::PI) rotationComponent->rotation += Mathf::TAU;

		float angleDiff = angle - rotationComponent->rotation;

		// wrap angleDiff between -PI and +PI
		if (angleDiff >= +Mathf::PI) angleDiff -= Mathf::TAU;
		if (angleDiff <= -Mathf::PI) angleDiff += Mathf::TAU;

		auto* rotateEvent = entity.entityEventAddAndGet<EntityEvents::EventRotate>();

		if (angleDiff > turnSpeedDelta) {
			rotateEvent->rotateAmount = turnSpeedDelta;
		}
		else if (angleDiff < -turnSpeedDelta) {
			rotateEvent->rotateAmount = -turnSpeedDelta;
		}
		else {
			rotateEvent->rotateAmount = angleDiff;
		}
	}
}
void ComponentPosition::system(Entity& entity) {
	if (entity.entityEventHas<EventMove>()) {

		auto moveEvents = entity.entityEventGetAllOfType<EventMove>();

		for (uint32_t i = 0; i < moveEvents.size(); i++) {
			position += moveEvents[i]->moveAxis;
		}
	}
}
void ComponentRotation::system(Entity& entity) {
	if (entity.entityEventHas<EventRotate>()) {

		auto rotateEvents = entity.entityEventGetAllOfType<EventRotate>();

		for (uint32_t i = 0; i < rotateEvents.size(); i++) {
			rotation += rotateEvents[i]->rotateAmount;
		}
	}
}
void ComponentSprite::system(Entity& entity) {

	if (!entity.entityComponentHas<ComponentPosition>()) return;

	if (entity.entityEventHas<EventInitialize>()) {
		GameLevelGrid::levelGet(entity.levelId)->entityMarkDrawable(entity.Id, isDynamic, drawOrder);
	}

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	sf::Texture& texture = GraphicsStore::textureStore.objectGet(fileName);

	sprite.setTexture(texture);

	if (entity.entityComponentHas<ComponentSpriteColor>()) {
		sprite.setColor(entity.entityComponentGet<ComponentSpriteColor>()->color);
	}

	if (!entity.entityComponentHas<ComponentSpriteOrigin>()) {
		sprite.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
	}
	else {
		sprite.setOrigin(entity.entityComponentGet<ComponentSpriteOrigin>()->origin);
	}
	sprite.setPosition(positionComponent->position);

	if (entity.entityComponentHas<ComponentRotation>()) {
		sprite.setRotation(entity.entityComponentGet<ComponentRotation>()->rotation * 180.f / Mathf::PI);
	}
}
void ComponentViewFollow::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentPosition>()) {

		for (auto& panelName : panelViewsToFollow) {
			auto& panel = PanelManager::panelGet(panelName);

			// camera position prior to movement
			sf::Vector2f cameraPositionPrev = panel.viewRect.getPosition();
			// camera position prior to movement,
			sf::Vector2f cameraPositionPrevNaturalized = cameraPositionPrev;

			auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

			sf::Vector2f posDiff = positionComponent->position - panel.viewGet().getCenter();

			float lerp = 0.15f;

			panel.viewMove(posDiff * lerp);
			panel.viewUpdate();

			sf::Vector2f mousePos = panel.viewMousePositionGet();

			sf::Vector2f mouseDiff = mousePos - panel.viewGet().getCenter();

			lerp = 0.025f;

			mouseDiff.x *= lerp / (16.f / 9.f);
			mouseDiff.y *= lerp * (16.f / 9.f);

			panel.viewMove(mouseDiff);
			panel.viewUpdate();
		}
	}
}
void ComponentObjectTypeAssigner::system(Entity& entity) {
	if (!entity.entityEventHas<EventInitialize>()) return;

	ObjectRegistry::entityObjectTypeAssign(entity.Id, objectType);
}
void ComponentObjectGridInhabiterRadius::system(Entity& entity) {
	// check that entity has ComponentPosition
	if (!entity.entityComponentHas<ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("ComponentObjectGridInhabiterRadius assigned to an entity without a ComponentPosition!");

		entity.entityComponentTerminate<ComponentObjectGridInhabiterRadius>();
		return;
	}

	// check that entity has ObjectType
	if (ObjectRegistry::entityObjectTypeGet(entity.Id) == ObjectType::Null) {
		ConsoleHandler::consolePrintErr("ComponentObjectGridInhabiterRadius assigned to an entity without an ObjectType!");

		entity.entityComponentTerminate<ComponentObjectGridInhabiterRadius>();
		return;
	}

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	// check if positionPrev has been set yet
	if (positionPrev == sf::Vector2f(0, 0)) {
		positionPrev = positionComponent->position;
	}

	// note that two separate objectGrid references are used because an entity could travel to a different level.

	auto& objectGridDepopulation = GameLevelGrid::levelGet(entity.levelId)->objectGrid;

	for (float offsetX = -radius / 2.f; offsetX <= +radius / 2.f; offsetX += 1.f) {
		for (float offsetY = -radius / 2.f; offsetY <= +radius / 2.f; offsetY += 1.f) {

			// ensure the offset is normalized (I.E. in the circle that the radius represents)
			if (Vector2fMath::lengthSqrd(offsetX, offsetY) > (radius * radius) / (2.f * 2.f)) continue;
			// ensure positionPrev is in the grid, skip if not
			if (!objectGridDepopulation.worldPosIsInGridFull(positionPrev.x + offsetX, positionPrev.y + offsetY)) continue;
			// remove entity's id from grid at the offset
			objectGridDepopulation.cellGetFromWorld(positionPrev.x + offsetX, positionPrev.y + offsetY).idRemove(entity.Id);
		}
	};

	auto& objectGridPopulation = GameLevelGrid::levelGet(entity.levelId)->objectGrid;

	for (float offsetX = -radius / 2.f; offsetX <= +radius / 2.f; offsetX += 1.f) {
		for (float offsetY = -radius / 2.f; offsetY <= +radius / 2.f; offsetY += 1.f) {

			// ensure the offset is normalized (I.E. in the circle that the radius represents)
			if (Vector2fMath::lengthSqrd(offsetX, offsetY) > (radius * radius) / (2.f * 2.f)) continue;
			// ensure position is in the grid, skip if not
			if (!objectGridPopulation.worldPosIsInGridFull(positionComponent->position.x + offsetX, positionComponent->position.y + offsetY)) continue;
			// add entity's id to grid at the offset
			objectGridPopulation.cellGetFromWorld(positionComponent->position.x + offsetX, positionComponent->position.y + offsetY).idAdd(entity.Id);
		}
	};

	positionPrev = positionComponent->position;
}
void ComponentObjectGridInhabiterRectangles::system(Entity& entity) {
	// check that entity has ComponentPosition
	if (!entity.entityComponentHas<ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("ComponentObjectGridInhabiterRectangles assigned to an entity without a ComponentPosition!");

		entity.entityComponentTerminate<ComponentObjectGridInhabiterRectangles>();
		return;
	}


	sf::Vector2f& position = entity.entityComponentGet<ComponentPosition>()->position;
	float& rotation = entity.entityComponentGet<ComponentRotation>()->rotation;

	// check if positionPrev has been set yet
	if (positionPrev == sf::Vector2f(0, 0)) {
		positionPrev = position;
		rotationPrev = rotation;
	}

	// note that two separate objectGrid references are used because an entity could travel to a different level.

	auto& objectGrid = GameLevelGrid::levelGet(entity.levelId)->objectGrid;

	for (uint16_t i = 0; i < rectangles.size(); i++) {
		sf::FloatRect& rectangle = rectangles[i];

		for (uint16_t x = 0; x < rectangle.width; x++) {
			for (uint16_t y = 0; y < rectangle.height; y++) {

				int16_t offsetX = int16_t(rectangle.left + x);
				int16_t offsetY = int16_t(rectangle.top + y);

				sf::Vector2f offset = Vector2fMath::rotate(offsetX, offsetY, rotationPrev);

				// ensure positionPrev is in the grid, skip if not
				if (!objectGrid.worldPosIsInGridFull(positionPrev.x + offset.x, positionPrev.y + offset.y)) continue;
				// remove entity's id from grid at the offset
				objectGrid.cellGetFromWorld(positionPrev.x + offset.x, positionPrev.y + offset.y).idRemove(entity.Id);
			}
		}
	}

	for (uint16_t i = 0; i < rectangles.size(); i++) {
		sf::FloatRect& rectangle = rectangles[i];

		for (uint16_t x = 0; x < rectangle.width; x++) {
			for (uint16_t y = 0; y < rectangle.height; y++) {

				int16_t offsetX = int16_t(rectangle.left + x);
				int16_t offsetY = int16_t(rectangle.top + y);

				sf::Vector2f offset = Vector2fMath::rotate(offsetX, offsetY, rotation);

				// ensure position is in the grid, skip if not
				if (!objectGrid.worldPosIsInGridFull(position.x + offset.x, position.y + offset.y)) continue;
				// add entity's id to grid at the offset
				objectGrid.cellGetFromWorld(position.x + offset.x, position.y + offset.y).idAdd(entity.Id);
			}
		}
	}

	positionPrev = position;
	rotationPrev = rotation;
}
void ComponentObjectVision::system(Entity& entity) {


	try {
		if (!entity.entityComponentHas<ComponentRotation>()) {
			throw "Does not have ComponentRotation";
		}
		if (!entity.entityComponentHas<ComponentPosition>()) {
			throw "Does not have ComponentPosition";
		}
	}
	catch (const char* e) {
		ConsoleHandler::consolePrintErr("ComponentObjectVision system failed: Exception: " + std::string(e));
		return;
	}

	if (cooldownVisionUpdate.updateAutoReset(float(TimeHandler::deltaSimulatedGet()))) {
		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		float coneSize = 90.f * Mathf::PI / 180.f;

		float angle = rotationComponent->rotation;

		// set the objectVision's objectGrid to the entity's level's ObjectGrid
		objectVision.objectGridSet(&GameLevelGrid::levelGet(entity.levelId)->objectGrid);

		objectVision.update(positionComponent->position, angle, coneSize, 640, 64);

		ObjectIdVector& objectsSeenSet = objectVision.objectsSeenGet();

		if (objectsSeenSet.size() <= 0) return;

		auto* eventObjectSeen = entity.entityEventAddAndGet<EventObjectSeen>();
		eventObjectSeen->objectsSeen = &objectsSeenSet;
	}
}
void ComponentObjectVisionDebug::system(Entity& entity) {

	cooldownPrint.update(float(TimeHandler::deltaRealGet()));

	if (cooldownPrint.ready()) {

		constexpr const char* objectTypesNames[] = {
		   "Null",
		   "Player",
		   "SquadMember",
		   "Door",
		   "Skipper",
		   "Wall",
		   "SIZE",
		};

		std::string string;

		if (entity.entityEventHas<EventObjectSeen>()) {
			cooldownPrint.reset();

			auto* eventObjectSeen = entity.entityEventGet<EventObjectSeen>();

			const ObjectIdVector* objectsSeenVector = eventObjectSeen->objectsSeen;

			string += "[";
			for (uint16_t i = 0; i < objectsSeenVector->size(); i++) {

				for (uint16_t j = 0; j < objectsSeenVector->at(i).size(); j++) {
					string += objectTypesNames[i];
					string += ", ";
				}
			}
			string += "]";
			ConsoleHandler::consolePrintColor(string, 6);
		}
	}
}
void ComponentOcclusionRadius::system(Entity& entity) {
	// check that entity has ComponentPosition
	if (!entity.entityComponentHas<ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("ComponentOcclusionRadius assigned to an entity without a ComponentPosition!");

		entity.entityComponentTerminate<ComponentOcclusionRadius>();
		return;
	}


	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	// check if positionPrev has been set yet
	if (positionPrev == sf::Vector2f(0, 0)) {
		positionPrev = positionComponent->position;
	}

	// note that two separate objectGrid references are used because an entity could travel to a different level.

	auto& occlusionGrid = GameLevelGrid::levelGet(entity.levelId)->occlusionGrid;

	for (float offsetX = -radius / 2.f; offsetX <= +radius / 2.f; offsetX += 1.f) {
		for (float offsetY = -radius / 2.f; offsetY <= +radius / 2.f; offsetY += 1.f) {

			// ensure the offset is normalized (I.E. in the circle that the radius represents)
			if (Vector2fMath::lengthSqrd(offsetX, offsetY) > (radius * radius) / (2.f * 2.f)) continue;
			// ensure positionPrev is in the grid, skip if not
			if (!occlusionGrid.worldPosIsInGridFull(positionPrev.x + offsetX, positionPrev.y + offsetY)) continue;
			// remove entity's id from grid at the offset
			occlusionGrid.cellSetFromWorld(positionPrev.x + offsetX, positionPrev.y + offsetY, false);
		}
	}

	for (float offsetX = -radius / 2.f; offsetX <= +radius / 2.f; offsetX += 1.f) {
		for (float offsetY = -radius / 2.f; offsetY <= +radius / 2.f; offsetY += 1.f) {

			// ensure the offset is normalized (I.E. in the circle that the radius represents)
			if (Vector2fMath::lengthSqrd(offsetX, offsetY) > (radius * radius) / (2.f * 2.f)) continue;
			// ensure position is in the grid, skip if not
			if (!occlusionGrid.worldPosIsInGridFull(positionComponent->position.x + offsetX, positionComponent->position.y + offsetY)) continue;
			// add entity's id to grid at the offset
			occlusionGrid.cellSetFromWorld(positionComponent->position.x + offsetX, positionComponent->position.y + offsetY, true);
		}
	}

	positionPrev = positionComponent->position;
}
void ComponentOcclusionRectangles::system(Entity& entity) {
	// check that entity has ComponentPosition
	if (!entity.entityComponentHas<ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("ComponentOcclusionRadius assigned to an entity without a ComponentPosition!");

		entity.entityComponentTerminate<ComponentOcclusionRadius>();
		return;
	}


	sf::Vector2f& position = entity.entityComponentGet<ComponentPosition>()->position;
	float& rotation = entity.entityComponentGet<ComponentRotation>()->rotation;

	// check if positionPrev has been set yet
	if (positionPrev == sf::Vector2f(0, 0)) {
		positionPrev = position;
		rotationPrev = rotation;
	}

	// note that two separate objectGrid references are used because an entity could travel to a different level.

	auto& occlusionGrid = GameLevelGrid::levelGet(entity.levelId)->occlusionGrid;

	for (uint16_t i = 0; i < rectangles.size(); i++) {
		sf::FloatRect& rectangle = rectangles[i];

		for (uint16_t x = 0; x < rectangle.width; x++) {
			for (uint16_t y = 0; y < rectangle.height; y++) {

				int16_t offsetX = int16_t(rectangle.left + x);
				int16_t offsetY = int16_t(rectangle.top + y);

				sf::Vector2f offset = Vector2fMath::rotate(offsetX, offsetY, rotationPrev);

				// ensure positionPrev is in the grid, skip if not
				if (!occlusionGrid.worldPosIsInGridFull(positionPrev.x + offset.x, positionPrev.y + offset.y)) continue;
				// remove entity's id from grid at the offset
				occlusionGrid.cellSetFromWorld(positionPrev.x + offset.x, positionPrev.y + offset.y, false);
			}
		}
	}

	for (uint16_t i = 0; i < rectangles.size(); i++) {
		sf::FloatRect& rectangle = rectangles[i];

		for (uint16_t x = 0; x < rectangle.width; x++) {
			for (uint16_t y = 0; y < rectangle.height; y++) {

				int16_t offsetX = int16_t(rectangle.left + x);
				int16_t offsetY = int16_t(rectangle.top + y);

				sf::Vector2f offset = Vector2fMath::rotate(offsetX, offsetY, rotation);

				// ensure position is in the grid, skip if not
				if (!occlusionGrid.worldPosIsInGridFull(position.x + offset.x, position.y + offset.y)) continue;
				// add entity's id to grid at the offset
				occlusionGrid.cellSetFromWorld(position.x + offset.x, position.y + offset.y, true);
			}
		}
	}

	positionPrev = position;
	rotationPrev = rotation;
}
void ComponentCollisionPolygons::system(Entity& entity) {
	// check that entity has correct components
	try {
		if (!entity.entityComponentHas<ComponentPosition>()) throw std::string("ComponentCollisionShapeMulti assigned to an entity without a ComponentPosition!");
		if (!entity.entityComponentHas<ComponentRotation>()) throw std::string("ComponentCollisionShapeMulti assigned to an entity without a ComponentRotation!");
	}
	catch (std::string e) {
		ConsoleHandler::consolePrintErr(e);

		entity.entityComponentTerminate<ComponentCollisionPolygons>();
	}

	sf::Vector2f& position = entity.entityComponentGet<ComponentPosition>()->position;
	float& rotation = entity.entityComponentGet<ComponentRotation>()->rotation;

	for (CollisionShapePolygon& shape : shapes) {
		// change these two calls to a shape.transformSet(), not doing it right now because i haven't tested if it works, (almost certain it does)
		shape.centerSet(position);
		shape.rotationSet(rotation);
	}

	if (entity.entityEventHas<EventInitialize>()) {
		if (entity.entityComponentHas<ComponentCollidable>()) {
			CollisionProcessor::entityMarkCollidable(entity.Id, shapesPtrs, shapesMaxDist);
		}
	}
	if (entity.entityComponentHas<ComponentCollider>()) {
		CollisionProcessor::colliderRequestProcess(entity.Id, shapesPtrs, shapesMaxDist);
	}
}
void ComponentHingeOnPoint::system(Entity& entity) {
	// check that entity has ComponentPosition
	if (!entity.entityComponentHas<ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("ComponentHingeOnPoint assigned to an entity without a ComponentPosition!");

		entity.entityComponentTerminate<ComponentHingeOnPoint>();
		return;
	}


	const sf::Vector2f& position = entity.entityComponentGet<ComponentPosition>()->position;
	const float& rotation = entity.entityComponentGet<ComponentRotation>()->rotation;

	sf::Vector2f hingeOffsetRotated = Vector2fMath::rotate(hingeOffset, rotation);

	//sf::Vector2f hingePosition = positionComponent + hingeOffset;
	if (hingePoint.x < -999999.f && hingePoint.y < -999999.f) {
		hingePoint = position + hingeOffsetRotated;
	}

	sf::Vector2f hingePointGlobal = position + hingeOffsetRotated;
	sf::Vector2f hingePointGlobalInverted = position - hingeOffsetRotated;

	sf::Vector2f hingeAxis = Vector2fMath::axis(hingePointGlobalInverted, hingePointGlobal);
	sf::Vector2f hingeAxisInverted = Vector2fMath::axis(hingePointGlobalInverted, hingePoint);

	float angleAxis = atan2(hingeAxis.y, hingeAxis.x);
	float angleAxisInverted = atan2(hingeAxisInverted.y, hingeAxisInverted.x);

	entity.entityEventAddAndGet<EventRotate>()->rotateAmount = angleAxisInverted - angleAxis;

	float updatedRotation = rotation + (angleAxisInverted - angleAxis);

	hingeOffsetRotated = Vector2fMath::rotate(hingeOffset, updatedRotation);
	hingePointGlobal = position + hingeOffsetRotated;

	entity.entityEventAddAndGet<EventMove>()->moveAxis = Vector2fMath::axis(hingePointGlobal, hingePoint);

}
void ComponentCollisionResponse::system(Entity& entity) {

	sf::Vector2f collisionAxisTotal;

	if (entity.entityEventHas<EventCollision>()) {

		auto eventCollisionAll = entity.entityEventGetAllOfType<EventCollision>();

		for (uint16_t i = 0; i < eventCollisionAll.size(); i++) {
			collisionAxisTotal += eventCollisionAll[i]->collisionAxis;
		}

		entity.entityEventAddAndGet<EventMove>()->moveAxis = collisionAxisTotal;
	}

}

#pragma endregion Systems
