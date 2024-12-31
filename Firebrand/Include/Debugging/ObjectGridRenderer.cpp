#include "ObjectGridRenderer.hpp"
#include <algorithm>
#include <cmath>

ObjectGridRenderer::ObjectGridRenderer(sf::Vector2f size) {
	for (uint16_t i = 0; i < uint16_t(ObjectType::SIZE); i++) {
		objectTypeColors.push_back(sf::Color(((i * 1000) % 127) * 2, (((i * 9999) % 255) * 72) % 255, (i * 99999) % 255, 255));
	}
}

void ObjectGridRenderer::objectGridRender(ObjectGrid& objectGrid, sf::FloatRect viewRect) {


	objectGridImage.create(viewRect.getSize().x, viewRect.getSize().y);
	objectGridImage.createMaskFromColor(sf::Color::Black);


	int16_t xStart =	int16_t(viewRect.left);
	int16_t xEnd =	int16_t(viewRect.left + viewRect.width);

	int16_t yStart =	int16_t(viewRect.top);
	int16_t yEnd =	int16_t(viewRect.top + viewRect.height);

	std::cout << xStart << " " << xEnd << " " << yStart << " " << yEnd << std::endl;

	for (int16_t x = xStart; x < xEnd; x++) {
		for (int16_t y = yStart; y < yEnd; y++) {

			// ensure coordinates are valid and fit inside the objectGridImage
			if (x < 0 || x >= objectGridImage.getSize().x || y < 0 || y >= objectGridImage.getSize().x) continue;

			auto& idsSet = objectGrid.cellTypesGetFromWorld(x, y);

			if (idsSet.size() <= 0) continue;

			std::vector<ObjectType> idsVector(idsSet.begin(), idsSet.end());

			uint16_t r = 0;
			uint16_t g = 0;
			uint16_t b = 0;

			for (uint16_t i = 0; i < idsVector.size(); i++) {
				sf::Color objectTypeColor = objectTypeColors[uint16_t(idsVector[i])];

				r += objectTypeColor.r;
				g += objectTypeColor.g;
				b += objectTypeColor.b;
			}

			uint16_t totalColors = uint16_t(idsVector.size());

			sf::Color pixelColor = sf::Color(sf::Uint8(r / totalColors), sf::Uint8(g / totalColors), sf::Uint8(b / totalColors), 255);

			objectGridImage.setPixel(x, y, pixelColor);
		}
	}

	objectGridTexture.loadFromImage(objectGridImage);
}
const sf::Texture& ObjectGridRenderer::objectGridTextureGet() const {
	return objectGridTexture;
}

