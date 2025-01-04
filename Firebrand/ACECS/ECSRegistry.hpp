#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "../ACECS/Panels.hpp"
#include "../Include/Game/RayCasting/Object Vision/ObjectVision.hpp"
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
	struct ComponentSprite final : public Component {

		void system(Entity& entity) final;

		ComponentSprite() {
			hasSystem = true;
		};
		// constructor that takes file name and extension, then loads/gets an image from the imageStore, and loads the texture with that image
		ComponentSprite(std::string _fileName, std::string _fileExtension, bool _isDynamic, sf::Vector2f _origin = sf::Vector2f(-INFINITY, -INFINITY)) :
			ComponentSprite()
		{
			fileName = _fileName;
			fileExtension = _fileExtension;
			isDynamic = _isDynamic;
			origin = _origin;

			textureInitialize();
		};
		ComponentSprite(std::string _fileName, bool _isDynamic, sf::Vector2f _origin = sf::Vector2f(-INFINITY, -INFINITY)) :
			ComponentSprite(_fileName, GraphicsStore::imageStore.extensionDefaultGet(), _isDynamic, _origin)
		{};

		// the name of the file for the texture
		std::string fileName = "Art/Error texture";
		// the name of the extension for the texture
		std::string fileExtension = "png";

		// whether this sprite should be drawn as a dynamic or static sprite, dynamic only draw when in vision, static draw in or out of vision
		bool isDynamic = false;
		// origin of the sprite, note that this is relative to the center of the sprite, note the top left corner.
		sf::Vector2f origin;

		sf::Sprite sprite;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSprite(fileName, fileExtension, isDynamic, origin));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;

	private:
		void textureInitialize() {
			// if texture does exist, get/load image from the file name and extension, then create texture with that image
			if (!GraphicsStore::textureStore.objectExists(fileName)) {

				sf::Image& image = GraphicsStore::imageStore.fileGetOrLoadFromName(fileName, fileExtension);

				sf::Texture texture;
				texture.loadFromImage(image);

				GraphicsStore::textureStore.objectAddFromInstance(fileName, texture);
			}
		}
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
			WorldPosition{};
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
}

#endif
