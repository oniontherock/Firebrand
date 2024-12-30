#include "ECSRegistry.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/TimeHandler.hpp>
#include <Auxiliary/VectorMath.hpp>
#include <Graphics/PanelManager.hpp>
#include <Input.hpp>

uint32_t MAX_ENTITIES = 100;
uint16_t MAX_EVENT_TYPES = 3;
uint16_t MAX_COMPONENT_TYPES = 5;

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

	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByInput>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotateToMouse>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();

	// sprites/drawing
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSprite>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentViewFollow>>();
	//ComponentRegistry::typeRegister<ComponentIDs<COMPONENT_GOES_HERE>>();
	//ComponentRegistry::typeRegister<ComponentIDs<COMPONENT_GOES_HERE>>();
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
			createComponentPairFromType<ComponentMoveByInput>(120.f),
			createComponentPairFromType<ComponentPosition>(sf::Vector2f(256.f, 256.f)),
			createComponentPairFromType<ComponentRotateToMouse>(Mathf::TAU * 1.25f),
			createComponentPairFromType<ComponentSprite>("Art/Squad Member"),
			createComponentPairFromType<ComponentViewFollow>(std::vector<PanelName> { PanelName::StaticView, PanelName::DynamicView } ),

		}
	);

}

#pragma endregion Component Templates
#pragma region Systems

using namespace EntityComponents;
using namespace EntityEvents;

// if you need to include a certain file for a system, include it here.
#include <iostream>

// if the system is not using the entity parameter, remove it's name to avoid a C4100 error

void ComponentMoveByInput::system(Entity& entity) {
	sf::Vector2f inputAxis;
	inputAxis.x = float(InputInterface::inputGetActive("Move Right") - InputInterface::inputGetActive("Move Left"));
	inputAxis.y = float(InputInterface::inputGetActive("Move Down") - InputInterface::inputGetActive("Move Up"));

	if (inputAxis.x != 0 || inputAxis.y != 0) {

		inputAxis = Vector2fMath::normalize(inputAxis) * moveSpeed * static_cast<float>(TimeHandler::deltaSimulatedGet());

		auto* moveEvent = entity.entityEventAddAndGet<EventMove>();
		moveEvent->moveAxis = inputAxis;
	}
}
void ComponentRotateToMouse::system(Entity& entity) {
	if (entity.entityComponentHas<ComponentPosition>() && entity.entityComponentHas<ComponentRotation>()) {
		const float delta = float(TimeHandler::deltaSimulatedGet());

		const float turnSpeedDelta = turnSpeed * delta;

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

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	sf::Texture& texture = GraphicsStore::textureStore.objectGet(fileName);

	sprite.setTexture(texture);
	sprite.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
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

#pragma endregion Systems
