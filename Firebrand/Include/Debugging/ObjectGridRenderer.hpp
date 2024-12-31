#ifndef __OBJECT_GRID_RENDERER_H__
#define __OBJECT_GRID_RENDERER_H__

#include "../ACECS/GameLevel.hpp"
#include "../ACECS/Panels.hpp"
#include "../Game/World/Objects/Object Grid/ObjectGrid.hpp"
#include "../Game/World/Objects/ObjectRegistry.hpp"
#include "../Game/World/Objects/ObjectTypes.hpp"

class ObjectGridRenderer {
public:
	ObjectGridRenderer(sf::Vector2f size);

	void objectGridRender(ObjectGrid& objectGrid, sf::FloatRect viewRect);

	const sf::Texture& objectGridTextureGet() const;

private:
	sf::Image objectGridImage;
	sf::Texture objectGridTexture;

	void renderCycleUpdate();

	std::vector<sf::Color> objectTypeColors;
};


#endif