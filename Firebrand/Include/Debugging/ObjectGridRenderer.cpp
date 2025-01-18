#include "ObjectGridRenderer.hpp"
#include <algorithm>
#include <cmath>

sf::Color encodedColorGet(ObjectType objectType) {
	
	int32_t objectTypeAsInt = int32_t(objectType);
	
		//sf::Color(((i * 1000) % 127) * 2, (((i * 9999) % 255) * 72) % 255, (i * 99999) % 255, 255)
	return sf::Color(
		sf::Uint8(((objectTypeAsInt * 1000) % 127) * 2),
		sf::Uint8((((objectTypeAsInt * 9999) % 255) * 72) % 255),
		sf::Uint8((objectTypeAsInt * 99999) % 255),
		255
	);
}

void ObjectGridRenderer::gridRender(ObjectGrid& objectGrid, PanelRect viewRect) {

	sf::Image image;
	image.create(objectGrid.gridGetSizeX(), objectGrid.gridGetSizeY());

	for (uint16_t x = 0; x < viewRect.width / objectGrid.cellsGetSizeX(); x++) {
		for (uint16_t y = 0; y < viewRect.height / objectGrid.cellsGetSizeY(); y++) {

			sf::Vector2f worldPos = viewRect.getPosition() + sf::Vector2f(x * objectGrid.cellsGetSizeX(), y * objectGrid.cellsGetSizeY());


			const std::set<EntityId>& ids = objectGrid.cellGetFromWorld(worldPos).idsGet();
			
			if (ids.size() <= 0) continue;

			uint16_t r = 0;
			uint16_t g = 0;
			uint16_t b = 0;

			for (auto itr = ids.begin(); itr != ids.end(); itr++) {

				sf::Color color = encodedColorGet(ObjectRegistry::entityObjectTypeGet(*itr));

				r += color.r;
				g += color.g;
				b += color.b;
			}

			sf::Uint8 rFinal = sf::Uint8(r / ids.size());
			sf::Uint8 gFinal = sf::Uint8(g / ids.size());
			sf::Uint8 bFinal = sf::Uint8(b / ids.size());

			image.setPixel(x, y, sf::Color(rFinal, gFinal, bFinal, 255));
		}
	}

	image.createMaskFromColor(sf::Color::Black);

	texture.loadFromImage(image);
}

