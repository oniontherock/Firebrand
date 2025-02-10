#include "BatchDrawHandler.hpp"
#include <Auxiliary/VectorMath.hpp>
#include <Graphics/Stores/GraphicsStore.hpp>
#include <iostream>
#include <Auxiliary/ConsoleHandler.hpp>
#include <Auxiliary/VectorMath.hpp>

std::map<std::string, std::vector<BatchDrawableTransform>> BatchDrawHandler::batchDrawMap{};


std::pair<sf::Texture&, sf::VertexArray> BatchDrawHandler::batchedDrawPopProcess(std::string batchToProcess, sf::Vector2f observerPosition, float observerMaxDistSqrd) {

	std::vector<BatchDrawableTransform> batchedDraws = batchDrawMap[batchToProcess];

	sf::VertexArray vertexArray(sf::PrimitiveType::Triangles, batchedDraws.size() * 6);
	uint32_t vertexArraySize = 0;


	if (!GraphicsStore::textureStore.objectExists(batchToProcess)) {
		ConsoleHandler::consolePrintErr("Batch draw attempted with non-existant texture: " + batchToProcess);
	}

	sf::Texture& texture = GraphicsStore::textureStore.objectGet(batchToProcess);

	const sf::Vector2f textureSize = sf::Vector2f(texture.getSize());

	const sf::Vector2f cornerTopLeft = sf::Vector2f(-textureSize.x / 2.f, -textureSize.y / 2.f);
	const sf::Vector2f cornerTopRight = sf::Vector2f(textureSize.x / 2.f, -textureSize.y / 2.f);
	const sf::Vector2f cornerBottomRight = sf::Vector2f(textureSize.x / 2.f, textureSize.y / 2.f);
	const sf::Vector2f cornerBottomLeft = sf::Vector2f(-textureSize.x / 2.f, textureSize.y / 2.f);

	for (uint16_t i = 0; i < batchedDraws.size(); i++) {

		BatchDrawableTransform& drawableCur = batchedDraws[i];

		if (Vector2fMath::distSqrd(drawableCur.position, observerPosition) > observerMaxDistSqrd) continue;

		//vertexArraySize += 6;
		//vertexArray.resize(vertexArraySize);

		vertexArray[(i * 6) + 0].position = drawableCur.position + Vector2fMath::rotate(cornerTopLeft, drawableCur.rotation);
		vertexArray[(i * 6) + 1].position = drawableCur.position + Vector2fMath::rotate(cornerTopRight, drawableCur.rotation);
		vertexArray[(i * 6) + 2].position = drawableCur.position + Vector2fMath::rotate(cornerBottomLeft, drawableCur.rotation);
		vertexArray[(i * 6) + 3].position = drawableCur.position + Vector2fMath::rotate(cornerBottomRight, drawableCur.rotation);
		vertexArray[(i * 6) + 4].position = drawableCur.position + Vector2fMath::rotate(cornerTopRight, drawableCur.rotation);
		vertexArray[(i * 6) + 5].position = drawableCur.position + Vector2fMath::rotate(cornerBottomLeft, drawableCur.rotation);

		vertexArray[(i * 6) + 0].texCoords = sf::Vector2f(0, 0);
		vertexArray[(i * 6) + 1].texCoords = sf::Vector2f(textureSize.x, 0);
		vertexArray[(i * 6) + 2].texCoords = sf::Vector2f(0, textureSize.y);
		vertexArray[(i * 6) + 3].texCoords = sf::Vector2f(textureSize.x, textureSize.y);
		vertexArray[(i * 6) + 4].texCoords = sf::Vector2f(textureSize.x, 0);
		vertexArray[(i * 6) + 5].texCoords = sf::Vector2f(0, textureSize.y);
	}

	return std::pair<sf::Texture&, sf::VertexArray>(texture, vertexArray);
}
std::vector<std::pair<sf::Texture&, sf::VertexArray>> BatchDrawHandler::batchedDrawsProcess(sf::Vector2f observerPosition, float observerMaxDist) {
	std::vector<std::pair<sf::Texture&, sf::VertexArray>>  processedBatches;

	const float observerMaxDistSqrd = observerMaxDist * observerMaxDist;

	for (auto& pair : batchDrawMap) {
		processedBatches.push_back(batchedDrawPopProcess(pair.first, observerPosition, observerMaxDistSqrd));
	}

	return processedBatches;
}

void BatchDrawHandler::batchDrawRequest(std::string textureName, BatchDrawableTransform batchDrawableTransform) {

	if (!batchDrawMap.contains(textureName)) {
		batchDrawMap.insert({ textureName, std::vector<BatchDrawableTransform>() });
	}

	batchDrawMap[textureName].push_back(batchDrawableTransform);
}
void BatchDrawHandler::batchDrawRequest(std::string textureName, sf::Vector2f position, float rotation) {
	batchDrawRequest(textureName, BatchDrawableTransform(position, rotation));
}

