#include "../Include/Game/AI/Sensory Abstraction/ObjectAbstractor.hpp"
#include "../Include/Game/Drawing/Batch Draw Handler/BatchDrawHandler.hpp"
#include "../Include/Game/World/Objects/ObjectRegistry.hpp"
#include "../Include/Game/World/Physics/Collision/Collision Processor/CollisionProcessor.hpp"
#include "ECSRegistry.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/TimeHandler.hpp>
#include <Auxiliary/VectorMath.hpp>
#include <Graphics/PanelManager.hpp>
#include <Input.hpp>

uint32_t MAX_ENTITIES = 100000;
uint16_t MAX_EVENT_TYPES = 8;
uint16_t MAX_COMPONENT_TYPES = 32;

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
	EventRegistry::typeRegister<EventIDs<EventMoveDirection>>();
	EventRegistry::typeRegister<EventIDs<EventRotate>>();
	EventRegistry::typeRegister<EventIDs<EventObjectSeen>>();
	EventRegistry::typeRegister<EventIDs<EventCollision>>();
	EventRegistry::typeRegister<EventIDs<EventBlackboardUpdated>>();
	EventRegistry::typeRegister<EventIDs<EventTeamSwitch>>();

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

	ComponentRegistry::typeRegister<ComponentIDs<ComponentObserver>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentTeam>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectTypeAssigner>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentCollidable>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentCollider>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByInput>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotateToMouse>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentGoapPlanExecuter>>();

	// collision response
	ComponentRegistry::typeRegister<ComponentIDs<ComponentCollisionResponse>>();
	// physics
	ComponentRegistry::typeRegister<ComponentIDs<ComponentHingeOnPoint>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMass>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveSpeed>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMovementHandler>>();
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
	// camera/view
	ComponentRegistry::typeRegister<ComponentIDs<ComponentViewFollow>>();
	// senses
	// vision
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectVision>>();
	// hearing

	// AI
	ComponentRegistry::typeRegister<ComponentIDs<ComponentIsAnimate>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorStateHolder>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSenseAbstractor>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentGoapActor>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentGoapPlanner>>();

	// sprites/drawing
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteOrigin>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteFlip>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteColor>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentBatchSprite>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSprite>>();

	// debug
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
		"Actor",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentSenseAbstractor>(),
			createComponentPairFromType<ComponentActorStateHolder>(),
			createComponentPairFromType<ComponentGoapActor>(),
			createComponentPairFromType<ComponentGoapPlanExecuter>(),
		}
	);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Creature",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Null),
			createComponentPairFromType<ComponentObjectGridInhabiterRadius>(32.f),
			createComponentPairFromType<ComponentCollider>(),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-32, -32), sf::Vector2f(32, -32), sf::Vector2f(32, 32), sf::Vector2f(-32, 32)
				})),
			createComponentPairFromType<ComponentCollisionResponse>(),
			createComponentPairFromType<ComponentMass>(100.f),
			createComponentPairFromType<ComponentObserver>(500.f),
			createComponentPairFromType<ComponentTeam>(),
			createComponentPairFromType<ComponentIsAnimate>(),
			createComponentPairFromType<ComponentMovementHandler>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Player",
		{
			"Input Controlled",
			"Creature",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Human),
			createComponentPairFromType<ComponentObjectGridInhabiterRadius>(32.f),
			createComponentPairFromType<ComponentMoveSpeed>(120.f),
			createComponentPairFromType<ComponentMoveByInput>(),
			createComponentPairFromType<ComponentRotateToMouse>(Mathf::TAU * 1.25f),
			createComponentPairFromType<ComponentSprite>("Art/Squad Member", false, uint16_t(50u)),
			createComponentPairFromType<ComponentViewFollow>(std::vector<PanelName> { PanelName::StaticView, PanelName::DynamicView, PanelName::Hud }),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-12, -24), sf::Vector2f(12, -24), sf::Vector2f(12, 24), sf::Vector2f(-12, 24)
				})),
			createComponentPairFromType<ComponentMass>(120.f),
			createComponentPairFromType<ComponentObserver>(1280.f),
			createComponentPairFromType<ComponentTeam>(Teams::TeamType::Player),
		}
	);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Test Creature",
		{
			"Transform",
			"Creature",
			"Actor",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Human),
			createComponentPairFromType<ComponentObjectGridInhabiterRadius>(32.f),
			createComponentPairFromType<ComponentSpriteColor>(sf::Color(255, 0, 0, 255)),
			createComponentPairFromType<ComponentSprite>("Art/Squad Member", false, uint16_t(50u)),
			createComponentPairFromType<ComponentCollider>(),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-12, -24), sf::Vector2f(12, -24), sf::Vector2f(12, 24), sf::Vector2f(-12, 24)
				})),
			createComponentPairFromType<ComponentCollisionResponse>(),
			createComponentPairFromType<ComponentMoveSpeed>(60.f),
			createComponentPairFromType<ComponentMass>(120.f),
			createComponentPairFromType<ComponentObserver>(1280.f),
			createComponentPairFromType<ComponentTeam>(),
			createComponentPairFromType<ComponentObjectVision>(),
			createComponentPairFromType<ComponentGoapActor>(std::vector<Goap::GoalName>{ "KeepSafe" }, std::vector<Goap::ActionName>{ "Flee" }),
			createComponentPairFromType<ComponentGoapPlanner>(),
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
			createComponentPairFromType<ComponentBatchSprite>("Wall Wooden Single"),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-12, -12), sf::Vector2f(48, 24)) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-12, -12), sf::Vector2f(48, 24)) }),
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
			createComponentPairFromType<ComponentBatchSprite>("Wall Wooden Straight"),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-32, -6), sf::Vector2f(64, 12)) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-32, -6), sf::Vector2f(64, 12)) }),
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
			createComponentPairFromType<ComponentBatchSprite>("Wall Wooden Corner"),
			createComponentPairFromType<ComponentSpriteOrigin>(12.f, 12.f),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-5, -5), sf::Vector2f(10, 38)), sf::FloatRect(sf::Vector2f(-5, -5), sf::Vector2f(38, 10)) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-5, -5), sf::Vector2f(10, 38)), sf::FloatRect(sf::Vector2f(-5, -5), sf::Vector2f(38, 10)) }),
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
			createComponentPairFromType<ComponentBatchSprite>("Wall Wooden Junction T"),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-5, -5), sf::Vector2f(38, 10)), sf::FloatRect(sf::Vector2f(-5, -32), sf::Vector2f(10, 65)) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-5, -5), sf::Vector2f(38, 10)), sf::FloatRect(sf::Vector2f(-5, -32), sf::Vector2f(10, 65)) }),
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
			createComponentPairFromType<ComponentBatchSprite>("Wall Wooden Junction Plus"),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-32, -5), sf::Vector2f(64, 10)), sf::FloatRect(sf::Vector2f(-5, -32), sf::Vector2f(10, 64)) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-32, -5), sf::Vector2f(64, 10)), sf::FloatRect(sf::Vector2f(-5, -32), sf::Vector2f(10, 64)) }),
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
	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Door Wooden",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Door),
			createComponentPairFromType<ComponentSprite>("Art/Structures/Doors/Door Wooden", false, uint16_t(59u)),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-32, -2), sf::Vector2f(64, 4)) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-32, -2), sf::Vector2f(64, 4)) }),
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
		"Prop Dresser",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Dresser),
			createComponentPairFromType<ComponentSprite>("Art/Structures/Props/Dresser", false, uint16_t(50u)),
			createComponentPairFromType<ComponentOcclusionRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-16, -32), sf::Vector2f(32, 64)) }),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-16, -32), sf::Vector2f(32, 64)) }),
			createComponentPairFromType<ComponentCollidable>(),
			createComponentPairFromType<ComponentCollider>(),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-16, -32), sf::Vector2f(16, -32), sf::Vector2f(16, 32), sf::Vector2f(-16, 32)
				})),
			createComponentPairFromType<ComponentCollisionResponse>(),
			createComponentPairFromType<ComponentMass>(200.f),
		}
	);
	ComponentTemplateManager::componentTemplateAdd(
		/// template name
		"Prop Table",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Table),
			createComponentPairFromType<ComponentSprite>("Art/Structures/Props/Table Wooden", false, uint16_t(50u)),
			createComponentPairFromType<ComponentObjectGridInhabiterRectangles>(std::vector<sf::FloatRect>{ sf::FloatRect(sf::Vector2f(-16, -32), sf::Vector2f(32, 64)) }),
			createComponentPairFromType<ComponentCollidable>(),
			createComponentPairFromType<ComponentCollider>(),
			createComponentPairFromType<ComponentCollisionPolygons>(CollisionShapePolygon(CollisionPolygon{
			sf::Vector2f(-16, -32), sf::Vector2f(16, -32), sf::Vector2f(16, 32), sf::Vector2f(-16, 32)
				})),
			createComponentPairFromType<ComponentCollisionResponse>(),
			createComponentPairFromType<ComponentMass>(200.f),
		}
	);
}

#pragma endregion Component Templates
#pragma region Systems

using namespace EntityComponents;
using namespace EntityEvents;

// if the system is not using the entity parameter, remove it's name to avoid a C4100 error

void ComponentMoveByInput::system(Entity& entity) {
	sf::Vector2f inputAxis;
	inputAxis.x = float(InputInterface::inputGetActive("Move Right") - InputInterface::inputGetActive("Move Left"));
	inputAxis.y = float(InputInterface::inputGetActive("Move Down") - InputInterface::inputGetActive("Move Up"));

	if (inputAxis.x != 0 || inputAxis.y != 0) {
		entity.entityEventAddAndGet<EventMoveDirection>()->moveDirection = Vector2fMath::normalize(inputAxis);
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
	if (entity.entityEventHas<EventInitialize>()) {
		GameLevelGrid::levelGet(entity.levelId)->entityMarkDrawable(entity.Id, isDynamic, drawOrder);
	}
}
void ComponentBatchSprite::system(Entity& entity) {
	if (entity.entityEventHas<EventInitialize>()) {

		sf::Vector2f position;
		float rotation = 0.f;
		sf::Vector2f origin = sf::Vector2f(-INFINITY, -INFINITY);

		if (entity.entityComponentHas<ComponentPosition>()) {
			position = entity.entityComponentGet<ComponentPosition>()->position;
		}
		if (entity.entityComponentHas<ComponentRotation>()) {
			rotation = entity.entityComponentGet<ComponentRotation>()->rotation;
		}
		if (entity.entityComponentHas<ComponentSpriteOrigin>()) {
			origin = entity.entityComponentGet<ComponentSpriteOrigin>()->origin;
		}

		BatchDrawableTransform transform(position, rotation, origin);

		BatchDrawHandler::batchDrawRequest(fileName, transform);
	}
}
void ComponentViewFollow::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentPosition>()) {

		for (auto& panelName : panelViewsToFollow) {
			auto& panel = PanelManager::panelGet(panelName);

			// camera position prior to movement
			sf::Vector2f cameraPositionPrev = panel.viewRect.position;
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

	if (objectType == ObjectType::Null) {
		ConsoleHandler::consolePrintErr("Entity (Id=" + std::to_string(entity.Id) + ") has ComponentObjectTypeAssigner with ObjectType::Null. Please Assign a proper ObjectType");
	}

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

		for (uint16_t x = 0; x < rectangle.size.x; x++) {
			for (uint16_t y = 0; y < rectangle.size.y; y++) {

				int16_t offsetX = int16_t(rectangle.position.x + x);
				int16_t offsetY = int16_t(rectangle.position.y + y);

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

		for (uint16_t x = 0; x < rectangle.size.x; x++) {
			for (uint16_t y = 0; y < rectangle.size.y; y++) {

				int16_t offsetX = int16_t(rectangle.position.x + x);
				int16_t offsetY = int16_t(rectangle.position.y + y);

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

		objectVision.occlusionGrid = &GameLevelGrid::levelGet(entity.levelId)->occlusionGrid;
		// set the objectVision's objectGrid to the entity's level's ObjectGrid
		objectVision.objectGridSet(&GameLevelGrid::levelGet(entity.levelId)->objectGrid);

		objectVision.update(positionComponent->position, angle, coneSize, 640, 64);

		ObjectIdVector& objectIdVector = objectVision.objectsSeenGet();

		// remove our id from the objectIdVector
		if (entity.entityComponentHas<ComponentObjectTypeAssigner>()) {
			// get entity's object type
			ObjectType objType = entity.entityComponentGet<ComponentObjectTypeAssigner>()->objectType;
			// get sub vector that this entity would be in
			std::vector<EntityId>& objectIdSubVector = objectIdVector[uint16_t(objType)];
			// attempt to find the entity in the objectIdSubVector
			auto itr = std::find(objectIdSubVector.begin(), objectIdSubVector.end(), entity.Id);
			// erase the entity from the sub vector if they are in it
			if (itr != objectIdSubVector.end()) objectIdSubVector.erase(itr);
		}

		if (objectIdVector.size() <= 0) return;

		auto* eventObjectSeen = entity.entityEventAddAndGet<EventObjectSeen>();
		eventObjectSeen->objectsSeen = &objectIdVector;
	}
}
void ComponentObjectVisionDebug::system(Entity& entity) {

	cooldownPrint.update(float(TimeHandler::deltaRealGet()));

	if (cooldownPrint.ready()) {

		std::string string;

		if (entity.entityEventHas<EventObjectSeen>()) {
			cooldownPrint.reset();

			auto* eventObjectSeen = entity.entityEventGet<EventObjectSeen>();

			const ObjectIdVector* objectsSeenVector = eventObjectSeen->objectsSeen;

			string += "[";
			for (uint16_t i = 0; i < objectsSeenVector->size(); i++) {

				for (uint16_t j = 0; j < objectsSeenVector->at(i).size(); j++) {
					string += objectTypeToString(ObjectType(i));
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

		for (uint16_t x = 0; x < rectangle.size.x; x++) {
			for (uint16_t y = 0; y < rectangle.size.y; y++) {

				int16_t offsetX = int16_t(rectangle.position.x + x);
				int16_t offsetY = int16_t(rectangle.position.y + y);

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

		for (uint16_t x = 0; x < rectangle.size.x; x++) {
			for (uint16_t y = 0; y < rectangle.size.y; y++) {

				int16_t offsetX = int16_t(rectangle.position.x + x);
				int16_t offsetY = int16_t(rectangle.position.y + y);

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
		if (!entity.entityComponentHas<ComponentPosition>()) throw std::string("ComponentCollisionPolygons assigned to an entity without a ComponentPosition!");
		if (!entity.entityComponentHas<ComponentRotation>()) throw std::string("ComponentCollisionPolygons assigned to an entity without a ComponentRotation!");
	}
	catch (std::string e) {
		ConsoleHandler::consolePrintErr(e);

		entity.entityComponentTerminate<ComponentCollisionPolygons>();
	}

	const sf::Vector2f& position = entity.entityComponentGet<ComponentPosition>()->position;
	const float& rotation = entity.entityComponentGet<ComponentRotation>()->rotation;

	for (CollisionShapePolygon& shape : shapes) {
		shape.transformSet(position, rotation);
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
void ComponentObserver::system(Entity& entity) {
	if (entity.entityEventHas<EventInitialize>()) {
		GameLevelGrid::levelGet(entity.levelId)->entitiesObservers.push_back(entity.Id);
	}
}
void ComponentActorStateHolder::system(Entity&) {
	// currently there aren't really any states to hold, since there are no movement states or an inventory, so this component does nothing for now.
}
void ComponentSenseAbstractor::system(Entity& entity) {
	// get whether the senses have updated
	bool hasSightUpdated = entity.entityEventHas<EventObjectSeen>();
	bool hasHearingUpdated = false; // not yet implemented, later replace with check for an actual hearing event

	// if none of the senses have updated, just return
	if (!hasSightUpdated && !hasHearingUpdated) return;

	// if any sense has updated, created an event for the new about to be abstracted data
	auto& blackboardNew = entity.entityEventAddAndGet<EventBlackboardUpdated>()->blackboardNew;

	// check if the sight has been updated
	if (hasSightUpdated) {
		// if it has been updated, abstract the sight data and add it to the abstractedSenses

		auto* eventObjectSeen = entity.entityEventGet<EventObjectSeen>();

		ObjectIdVector* objectIdVector = eventObjectSeen->objectsSeen;

		for (uint16_t objTypeCur = 0; objTypeCur < objectIdVector->size(); objTypeCur++) {

			for (uint16_t objCur = 0; objCur < objectIdVector->at(objTypeCur).size(); objCur++) {

				EntityId objectId = objectIdVector->at(objTypeCur)[objCur];

				ObjectAbstractor::blackboardAddObjectData(blackboardNew, entity, EntityManager::entityGet(objectId), ObjectType(objTypeCur));
			}
		}
	}
	// check if the hearing has been updated
	if (hasHearingUpdated) {
		// not yet implemented, later put hearing stuff here
	}
}
void ComponentGoapActor::system(Entity& entity) {
	if (!entity.entityEventHas<EventBlackboardUpdated>()) return;

	actor.blackboard = entity.entityEventGet<EventBlackboardUpdated>()->blackboardNew;
}
void ComponentTeam::system(Entity& entity) {
	if (entity.entityEventHas<EventInitialize>()) {
		// if the teamId hasn't been specified, set it to a new team
		if (teamId >= UINT32_MAX) {
			teamId = Teams::TeamHolder::teamCreate();
		}
		// add the entity's id to the specified team
		Teams::TeamHolder::teamAddEntity(teamId, entity.Id);
	}
	if (entity.entityEventHas<EventTeamSwitch>()) {
		Teams::TeamId teamIdNew = entity.entityEventGet<EventTeamSwitch>()->teamIdNew;

		Teams::TeamHolder::entityTeamSwitch(teamIdNew, entity.Id);
	}
}
void ComponentGoapPlanner::system(Entity& entity) {

	if (!entity.entityComponentHas<ComponentGoapActor>()) {
		ConsoleHandler::consolePrintErr("ComponentGoapPlanner assigned to an entity (Id=" + std::to_string(entity.Id) + ") without a ComponentGoapActor");
		return;
	}

	Goap::Actor& actor = entity.entityComponentGet<ComponentGoapActor>()->actor;

	goal = Goap::Planner::actorGoalGet(actor);

	plan = Goap::Planner::actorPlanGet(actor, goal);
}
void ComponentGoapPlanExecuter::system(Entity& entity) {

	if (!entity.entityComponentGet<ComponentGoapPlanner>()) {
		ConsoleHandler::consolePrintErr("Entity (Id=" + std::to_string(entity.Id) + ") has ComponentGoapPlanExecuter without ComponentGoapPlanner");
		return;
	}

	auto& actor = entity.entityComponentGet<ComponentGoapActor>()->actor;
	auto& plan = entity.entityComponentGet<ComponentGoapPlanner>()->plan;

	if (plan.empty()) return;

	plan[0].execute(entity, actor);
}
void ComponentMovementHandler::system(Entity& entity) {

	const float movespeed = (entity.entityComponentHas<ComponentMoveSpeed>() ? entity.entityComponentGet<ComponentMoveSpeed>()->moveSpeed : 120.f);

	if (entity.entityEventHas<EventMoveDirection>()) {

		auto eventMoveDirectionAll = entity.entityEventGetAllOfType<EventMoveDirection>();

		sf::Vector2f moveDirectionTotal;

		const uint32_t eventMoveDirectionAmount = uint32_t(eventMoveDirectionAll.size());

		for (uint32_t i = 0; i < eventMoveDirectionAmount; i++) {
			moveDirectionTotal += eventMoveDirectionAll[i]->moveDirection;
		}

		if (eventMoveDirectionAmount > 0) {
			moveDirectionTotal /= float(eventMoveDirectionAmount);
		}

		entity.entityEventAddAndGet<EventMove>()->moveAxis = moveDirectionTotal * float(double(movespeed) * TimeHandler::deltaSimulatedGet());

	}
}

#pragma endregion Systems
