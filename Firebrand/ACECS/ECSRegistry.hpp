#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "../ACECS/Panels.hpp"
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
		ComponentSprite(std::string _fileName, std::string _fileExtension) :
			ComponentSprite()
		{
			fileName = _fileName;
			fileExtension = _fileExtension;

			textureInitialize();
		};
		ComponentSprite(std::string _fileName) :
			ComponentSprite(_fileName, GraphicsStore::imageStore.extensionDefaultGet())
		{};

		// the name of the file for the texture
		std::string fileName = "Art/Error texture";
		// the name of the extension for the texture
		std::string fileExtension = "png";

		sf::Sprite sprite;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSprite(fileName, fileExtension));
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
}

#endif
