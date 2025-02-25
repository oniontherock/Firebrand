#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "../ACECS/Panels.hpp"
#include "../Include/Common/DataCache.hpp"
#include "../Include/Game/AI/Teams/TeamHolder.hpp"
#include "../Include/Game/AI/Teams/TeamRegistry.hpp"
#include "../Include/Game/AI/Teams/TeamRelationHolder.hpp"
#include "../Include/Game/AI/Teams/TeamRelationHolder.hpp"
#include "../Include/Game/RayCasting/Object Vision/ObjectVision.hpp"
#include "../Include/Game/World/Physics/Collision/CollisionHandler.hpp"
#include "../Include/Game/World/Physics/Collision/CollisionShape.hpp"
#include "ECS.hpp"
#include "SFML/Graphics.hpp"
#include <Audio/AudioStore.hpp>
#include <Auxiliary/Math.hpp>
#include <functional>
#include <Graphics/Stores/GraphicsStore.hpp>

namespace ECSRegistry {
	void ECSInitialize();
	void ECSTerminate();
}

// avoid having undefined constructor arguments for events or components,
// as it's more convenient to not have to define every event/component all the time.
// whenever you create a new type, ensure you register it in the implementation file of this header

namespace EntityEvents {
	struct EventMove final : public Event {

		EventMove() {};

		sf::Vector2f moveAxis{ 0,0 };

		void clear() final {
			moveAxis *= 0.f;
		}
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventMove());
		};
	};
	struct EventRotate final : public Event {

		EventRotate() {};

		float rotateAmount = 0.f;

		void clear() final {
			rotateAmount = 0.f;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventRotate());
		};
	};
	// contains a list of EntityIds and ObjectTypes that were seen this update.
	struct EventObjectSeen final : public Event {

		EventObjectSeen() {
			clear();
		};

		ObjectIdVector* objectsSeen;

		void clear() final {
			objectsSeen = nullptr;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventObjectSeen());
		};
	};
	struct EventCollision final : public Event {

		EventCollision() {
			clear();
		};

		// axis of collision
		sf::Vector2f collisionAxis;

		void clear() final {
			collisionAxis = sf::Vector2f(0, 0);
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventCollision());
		};
	};
	struct EventSensesAbstracted final : public Event {

		EventSensesAbstracted() {
			clear();
		};

		DataCache abstractedSenses;

		void clear() final {
			abstractedSenses.dataClear();
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventSensesAbstracted());
		};
	};
	struct EventTeamSwitch final : public Event {

		EventTeamSwitch() {
			clear();
		};

		Teams::TeamId teamIdNew;

		void clear() final {
			teamIdNew = UINT32_MAX;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventTeamSwitch());
		};
	};
}
namespace EntityComponents {
	struct ComponentMoveByInput final : public Component {

		void system(Entity& entity) final;

		ComponentMoveByInput() {
			hasSystem = true;
		};
		ComponentMoveByInput(float _moveSpeed) :
			ComponentMoveByInput()
		{
			moveSpeed = _moveSpeed;
		};

		float moveSpeed = 1.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentMoveByInput(moveSpeed));
		};
		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentSpriteOrigin final : public Component {

		ComponentSpriteOrigin() {
			hasSystem = false;
		};
		ComponentSpriteOrigin(sf::Vector2f _origin) :
			ComponentSpriteOrigin()
		{
			origin = _origin;
		};
		ComponentSpriteOrigin(float originX, float originY) :
			ComponentSpriteOrigin(sf::Vector2f(originX, originY))
		{
		};

		// origin of the sprite component on this entity
		sf::Vector2f origin;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSpriteOrigin(origin));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentSpriteFlip final : public Component {

		ComponentSpriteFlip() {
			hasSystem = false;
		};
		ComponentSpriteFlip(bool _flipX, bool _flipY) :
			ComponentSpriteFlip()
		{
			flipX = _flipX;
			flipY = _flipY;
		};

		// whether or not the X scale of the sprite should be inverted
		bool flipX = false;
		// whether or not the Y scale of the sprite should be inverted
		bool flipY = false;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSpriteFlip(flipX, flipY));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentSpriteColor final : public Component {

		ComponentSpriteColor() {
			hasSystem = false;
		};
		ComponentSpriteColor(sf::Color _color) :
			ComponentSpriteColor()
		{
			color = _color;
		};
		ComponentSpriteColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) :
			ComponentSpriteColor(sf::Color(r, g, b, a))
		{
		};

		// color of the sprite component on this entity
		sf::Color color;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSpriteColor(color));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentSprite final : public Component {

		void system(Entity& entity) final;

		ComponentSprite() {
			hasSystem = true;
		};
		// constructor that takes file name and extension, then loads/gets an image from the imageStore, and loads the texture with that image
		ComponentSprite(std::string _fileName, std::string _fileExtension, bool _isDynamic, uint16_t _drawOrder) :
			ComponentSprite()
		{
			fileName = _fileName;
			fileExtension = _fileExtension;
			isDynamic = _isDynamic;
			drawOrder = _drawOrder;

			textureInitialize();
		};
		ComponentSprite(std::string _fileName, bool _isDynamic, uint16_t _drawOrder) :
			ComponentSprite(_fileName, GraphicsStore::imageStore.extensionDefaultGet(), _isDynamic, _drawOrder)
		{
		};

		// the name of the file for the texture
		std::string fileName = "Art/Error texture";
		// the name of the extension for the texture
		std::string fileExtension = "png";

		// whether this sprite should be drawn as a dynamic or static sprite, dynamic only draw when in vision, static draw in or out of vision
		bool isDynamic = false;

		// the draw order of the sprite
		uint16_t drawOrder = 50;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSprite(fileName, fileExtension, isDynamic, drawOrder));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;

	private:
		void textureInitialize() {

			if (fileName == "Art/Error texture") return;

			// if texture does exist, get/load image from the file name and extension, then create texture with that image
			if (!GraphicsStore::textureStore.objectExists(fileName)) {

				sf::Image& image = GraphicsStore::imageStore.fileGetOrLoadFromName(fileName, fileExtension);

				sf::Texture texture(image);

				GraphicsStore::textureStore.objectAddFromInstance(fileName, texture);
			}
		}
	};
	// similar to ComponentSprite, but instead tells the BatchDrawHandler to draw it.
	struct ComponentBatchSprite final : public Component {

		void system(Entity& entity) final;

		ComponentBatchSprite() {
			hasSystem = true;
		};
		// constructor that takes file name and extension, then loads/gets an image from the imageStore, and loads the texture with that image
		ComponentBatchSprite(std::string _fileName, std::string _fileExtension) :
			ComponentBatchSprite()
		{
			fileName = _fileName;
			fileExtension = _fileExtension;
		};
		ComponentBatchSprite(std::string _fileName) :
			ComponentBatchSprite(_fileName, GraphicsStore::imageStore.extensionDefaultGet())
		{
		};

		// the name of the file for the texture
		std::string fileName = "Art/Error texture";
		// the name of the extension for the texture
		std::string fileExtension = "png";

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentBatchSprite(fileName, fileExtension));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	// creates an EventRotate every frame that linearly interpolates towards the mouse
	struct ComponentRotateToMouse final : public Component {

		void system(Entity& entity) final;

		ComponentRotateToMouse() {
			hasSystem = true;
		};
		ComponentRotateToMouse(float _turnSpeed) :
			ComponentRotateToMouse()
		{
			turnSpeed = _turnSpeed;
		};

		// turn speed in radians, note that this is speed per-second
		float turnSpeed = Mathf::PI;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentRotateToMouse(turnSpeed));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentPosition final : public Component {

		void system(Entity& entity) final;

		ComponentPosition() {
			hasSystem = true;
		};
		ComponentPosition(sf::Vector2f _position) :
			ComponentPosition()
		{
			position = _position;
		};

		sf::Vector2f position;
		float& x = position.x;
		float& y = position.y;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentPosition(position));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;

	};
	struct ComponentRotation final : public Component {

		void system(Entity& entity) final;

		ComponentRotation() {
			hasSystem = true;
		};
		ComponentRotation(float _rotation) :
			ComponentRotation()
		{
			rotation = _rotation;
		};

		float rotation = 0.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentRotation(rotation));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentViewFollow final : public Component {

		void system(Entity& entity) final;

		ComponentViewFollow() {
			hasSystem = true;
			panelViewsToFollow = std::vector<PanelName>();
		};
		ComponentViewFollow(std::vector<PanelName> _panelViewsToFollow) :
			ComponentViewFollow()
		{
			panelViewsToFollow = _panelViewsToFollow;
		};

		std::vector<PanelName> panelViewsToFollow;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentViewFollow(panelViewsToFollow));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	// assigns the ObjectType in the ObjectRegistry for the entity to be the specified objectType
	struct ComponentObjectTypeAssigner final : public Component {

		void system(Entity& entity) final;

		ComponentObjectTypeAssigner() {
			hasSystem = true;
			objectType = ObjectType::Null;
		};
		ComponentObjectTypeAssigner(ObjectType _objectType) :
			ComponentObjectTypeAssigner()
		{
			objectType = _objectType;
		};
		// ObjectType to assign to the entity
		ObjectType objectType;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectTypeAssigner(objectType));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	// populates the ObjectGrid with the specified type every update,
	// NOTE: this component should be ordered AFTER any movement components.
	struct ComponentObjectGridInhabiterRadius final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridInhabiterRadius() {
			hasSystem = true;
			radius = 0;
		};
		ComponentObjectGridInhabiterRadius(float _radius) :
			ComponentObjectGridInhabiterRadius()
		{
			radius = _radius;
		};

		float radius;
		// previous position of population, used for depopulation.
		sf::Vector2f positionPrev;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectGridInhabiterRadius(radius));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	// fills the ObjectGrid with some rectangles
	struct ComponentObjectGridInhabiterRectangles final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridInhabiterRectangles() {
			hasSystem = true;
		};
		ComponentObjectGridInhabiterRectangles(std::vector<sf::FloatRect> _rectangles) :
			ComponentObjectGridInhabiterRectangles()
		{
			rectangles = _rectangles;
		};

		std::vector<sf::FloatRect> rectangles;
		// previous position of population, used for depopulation.
		sf::Vector2f positionPrev;
		// previous rotation
		float rotationPrev = 0.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectGridInhabiterRectangles(rectangles));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	// gets a list of seen objects using an ObjectVision and creates an EventObjectSeen with them.
	struct ComponentObjectVision final : public Component {

		void system(Entity& entity) final;

		ComponentObjectVision() {
			hasSystem = true;
			objectVision = ObjectVision();
		};
		ComponentObjectVision(Cooldown _updateCooldown) :
			ComponentObjectVision()
		{
			cooldownVisionUpdate = _updateCooldown;
		};

		ObjectVision objectVision;

		// cooldown for when the finish updates
		Cooldown cooldownVisionUpdate;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectVision(cooldownVisionUpdate));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentObjectVisionDebug final : public Component {

		void system(Entity& entity) final;

		ComponentObjectVisionDebug() {
			hasSystem = true;
		};

		Cooldown cooldownPrint = Cooldown(0.1f);

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectVisionDebug());
		};
	};
	// fills the OcclusionGrid in a radius around the entity
	struct ComponentOcclusionRadius final : public Component {

		void system(Entity& entity) final;

		ComponentOcclusionRadius() {
			hasSystem = true;
			radius = 0;
		};
		ComponentOcclusionRadius(float _radius) :
			ComponentOcclusionRadius()
		{
			radius = _radius;
		};

		float radius;
		// previous position of population, used for depopulation.
		sf::Vector2f positionPrev;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentOcclusionRadius(radius));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	// fills the OcclusionGrid with some rectangles
	struct ComponentOcclusionRectangles final : public Component {

		void system(Entity& entity) final;

		ComponentOcclusionRectangles() {
			hasSystem = true;
		};
		ComponentOcclusionRectangles(std::vector<sf::FloatRect> _rectangles) :
			ComponentOcclusionRectangles()
		{
			rectangles = _rectangles;
		};

		std::vector<sf::FloatRect> rectangles;
		// previous position of population, used for depopulation.
		sf::Vector2f positionPrev;
		// previous rotation
		float rotationPrev = 0;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentOcclusionRectangles(rectangles));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	// gives an entity a mass, mass affects which object gets pushed more in a physics context
	struct ComponentMass final : public Component {

		ComponentMass() {
			hasSystem = false;
		};
		ComponentMass(float _mass) :
			ComponentMass()
		{
			mass = _mass;
		};

		// mass for this object, affects the 
		float mass = 10.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentMass(mass));
		};
		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	// marks an entity as collidable in the GameLevel
	struct ComponentCollidable final : public Component {

		ComponentCollidable() {
			hasSystem = false;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentCollidable());
		};
	};
	// marks an entity as collidable in the GameLevel
	struct ComponentCollider final : public Component {

		ComponentCollider() {
			hasSystem = false;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentCollider());
		};
	};
	struct ComponentCollisionPolygons final : public Component {

		void system(Entity& entity) final;

		ComponentCollisionPolygons() {
			hasSystem = true;
		};
		ComponentCollisionPolygons(std::vector<CollisionShapePolygon> _shapes) :
			ComponentCollisionPolygons()
		{
			shapes = _shapes;
			shapesPtrs.resize(shapes.size());

			for (uint16_t i = 0; i < shapes.size(); i++) {
				if (shapes[i].shapeMaxDistGet() > shapesMaxDist) {
					shapesMaxDist = shapes[i].shapeMaxDistGet();
				}

				shapesPtrs[i] = &shapes[i];
			}
		};
		ComponentCollisionPolygons(CollisionShapePolygon _shape) :
			ComponentCollisionPolygons(std::vector{ _shape })
		{
		};

		std::vector<CollisionShapePolygon> shapes;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentCollisionPolygons(shapes));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	private:
		// maximum distance to a vertex local to the origin in the given shapes
		float shapesMaxDist = 0.f;
		// pointers to every shape in shapes, we have this because the CollisionProcessor needs pointers instead of instances
		std::vector<CollisionShapeBase*> shapesPtrs;
	};
	// if this entity collides with another collidable object, it moves itself 
	struct ComponentCollisionResponse final : public Component {

		void system(Entity& entity) final;

		ComponentCollisionResponse() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentCollisionResponse());
		};
	};
	struct ComponentHingeOnPoint final : public Component {

		void system(Entity& entity) final;

		ComponentHingeOnPoint() {
			hasSystem = true;
		};
		ComponentHingeOnPoint(sf::Vector2f _hingeOffset) :
			ComponentHingeOnPoint()
		{
			hingeOffset = _hingeOffset;
		};

		// point local to the entity to hinge around
		sf::Vector2f hingeOffset;
		// global position of the hinge point in the world,
		// determined when the component first runs based off of the hingeOffset and the entity's position
		sf::Vector2f hingePoint = sf::Vector2f(-INFINITY, -INFINITY);


		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentHingeOnPoint(hingeOffset));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentObserver final : public Component {

		void system(Entity& entity) final;

		ComponentObserver() {
			hasSystem = true;
		};
		ComponentObserver(float _observationDistance) :
			ComponentObserver()
		{
			observationDistance = _observationDistance;
		};

		// the distance at which this observer will mark an observable entity as observed
		float observationDistance = 0.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObserver(observationDistance));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct ComponentActorStateHolder final : public Component {

		void system(Entity& entity) final;

		ComponentActorStateHolder() {
			hasSystem = true;
		};

		DataCache actorStateHolder;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentActorStateHolder());
		};
	};
	struct ComponentSenseAbstractor final : public Component {

		void system(Entity& entity) final;

		ComponentSenseAbstractor() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSenseAbstractor());
		};
	};
	struct ComponentSenseAbstractorDebugger final : public Component {

		void system(Entity& entity) final;

		ComponentSenseAbstractorDebugger() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSenseAbstractorDebugger());
		};
	};
	struct ComponentBlackboard final : public Component {

		void system(Entity& entity) final;

		ComponentBlackboard() {
			hasSystem = true;
		};

		DataCache data;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentBlackboard());
		};
	};
	struct ComponentTeam final : public Component {

		void system(Entity& entity) final;

		ComponentTeam() {
			hasSystem = true;
		};
		ComponentTeam(Teams::TeamId _teamId) :
			ComponentTeam()
		{
			teamId = _teamId;
		};
		ComponentTeam(Teams::TeamType _teamType) :
			ComponentTeam(Teams::TeamId(_teamType))
		{
		};

		Teams::TeamId teamId = UINT32_MAX;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentTeam(teamId));
		};
	};}

#endif
