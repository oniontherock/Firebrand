#include "ObjectGridRenderer.hpp"
#include <algorithm>
#include <cmath>

sf::Color encodedColorGet(ObjectType objectType) {
	
	int32_t objectTypeAsInt = int32_t(objectType);
	
		//sf::Color(((i * 1000) % 127) * 2, (((i * 9999) % 255) * 72) % 255, (i * 99999) % 255, 255)
	return sf::Color(
		uint8_t(((objectTypeAsInt * 1000) % 127) * 2),
		uint8_t((((objectTypeAsInt * 9999) % 255) * 72) % 255),
		uint8_t((objectTypeAsInt * 99999) % 255),
		255
	);
}

void ObjectGridRenderer::gridRender(ObjectGrid& objectGrid, PanelRect viewRect) {

	sf::Image image(sf::Vector2u(objectGrid.gridGetSizeX(), objectGrid.gridGetSizeY()));

	for (uint16_t x = 0; x < viewRect.size.x / objectGrid.cellsGetSizeX(); x++) {
		for (uint16_t y = 0; y < viewRect.size.y / objectGrid.cellsGetSizeY(); y++) {

			sf::Vector2f worldPos = viewRect.position + sf::Vector2f(x * objectGrid.cellsGetSizeX(), y * objectGrid.cellsGetSizeY());


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

			uint8_t rFinal = uint8_t(r / ids.size());
			uint8_t gFinal = uint8_t(g / ids.size());
			uint8_t bFinal = uint8_t(b / ids.size());

			image.setPixel(sf::Vector2u(x, y), sf::Color(rFinal, gFinal, bFinal, 255));
		}
	}

	image.createMaskFromColor(sf::Color::Black);

	if (texture.loadFromImage(image)) {
		ConsoleHandler::consolePrintErr("Image loading failed in ObjectGridRenderer");
	}
}

