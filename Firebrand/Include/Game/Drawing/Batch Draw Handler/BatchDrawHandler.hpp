#ifndef __BATCH_DRAW_HANDLER_H__
#define __BATCH_DRAW_HANDLER_H__

#include "Graphics/Panel.hpp"
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

// transform used for batch drawing something, has a rotation and a position
struct BatchDrawableTransform {
	sf::Vector2f position;
	float rotation;
	sf::Vector2f origin = sf::Vector2f(-INFINITY, -INFINITY);
};

class BatchDrawHandler {
	// map of vectors of BatchDrawableTransforms,
	// the key is the name of the texture that we will load from the GraphicsStore, and the value is a list of transforms for where to draw the texture
	static std::map<std::string, std::vector<BatchDrawableTransform>> batchDrawMap;
	// processes the first next vector in the batchDrawMap and return a vertex array
	static std::pair<sf::Texture&, sf::VertexArray> batchedDrawPopProcess(std::string batchToProcess, sf::Vector2f observerPosition, float observerMaxDistSqrd);
public:
	// runs batchedDrawPopProcess until there are no more draws to process, and returns the result
	static std::vector<std::pair<sf::Texture&, sf::VertexArray>> batchedDrawsProcess(sf::Vector2f observerPosition, float observerMaxDist);

	static void batchDrawRequest(std::string textureName, BatchDrawableTransform batchDrawableTransform);
	static void batchDrawRequest(std::string textureName, sf::Vector2f position, float rotation);
};


#endif