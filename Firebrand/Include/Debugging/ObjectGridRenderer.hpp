#ifndef __OBJECT_GRID_RENDERER_H__
#define __OBJECT_GRID_RENDERER_H__

#include "../ACECS/GameLevel.hpp"
#include "../ACECS/Panels.hpp"
#include "../Game/World/Objects/Object Grid/ObjectGrid.hpp"
#include "../Game/World/Objects/ObjectRegistry.hpp"
#include "../Game/World/Objects/ObjectTypes.hpp"

struct ObjectGridRenderer {

	sf::Texture texture;

	void gridRender(ObjectGrid& objectGrid, PanelRect viewRect);

};

#endif