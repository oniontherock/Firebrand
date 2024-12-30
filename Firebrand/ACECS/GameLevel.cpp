#include "GameLevel.hpp"

GameLevel::GameLevel() :
	levelSize(sf::Vector2u(4096, 4096)),
	aStarGrid(AStarGrid(levelSize.x / 16, levelSize.y / 16, 16.f, 16.f)),
	pathGenerator(PathGenerator())
{
	entities = std::vector<EntityId>();
}

GameLevel::GameLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ) :
	GameLevel()
{
	levelPosition = LevelPosition(_idX, _idY, _idZ);
}

GameLevel::GameLevel(LevelPosition _id) :
	GameLevel(_id.x, _id.y, _id.z)
{
}

void GameLevel::backgroundDraw() {
	grassDraw();
	//pathsDraw();
}

void GameLevel::grassDraw() {
	backgroundTexture.create(levelSize.x, levelSize.y);

	sf::VertexArray lines;
	lines.setPrimitiveType(sf::Lines);

	uint32_t grassBladeCount = (levelSize.x * levelSize.y) / 30;

	for (uint32_t i = 0; i < grassBladeCount; i++) {

		sf::Color color = sf::Color(RNGf::getRange(50, 125), RNGf::getRange(100, 255), RNGf::getRange(0, 25), 85);

		sf::Vertex lineStart;
		lineStart.color = color;
		lineStart.position = sf::Vector2f(RNGf::getRange(0.f, levelSize.x), RNGf::getRange(0, levelSize.y));

		sf::Vertex lineEnd;
		lineEnd.color = color;

		// create vector with random heading
		sf::Vector2f lineEndOffset = sf::Vector2f(cos(RNGf::getFullRange(Mathf::PI)), sin(RNGf::getFullRange(Mathf::PI)));
		// scale length of vector
		lineEndOffset = Vector2fMath::lengthSet(lineEndOffset, RNGf::getRange(3.f, 12.f));

		lineEnd.position = lineStart.position + lineEndOffset;

		lines.append(lineStart);
		lines.append(lineEnd);


		sf::Vector2f axis = lineEnd.position - lineStart.position;
		float angleSide = atan2(axis.y, axis.x) + (Mathf::PI * 0.5f);

		sf::Vector2f offset = Vector2fMath::lengthSet(sf::Vector2f(cos(angleSide), sin(angleSide)), RNGf::getRange(1.f, 2.f));

		sf::Vertex lineStartOffsetted = lineStart;
		lineStartOffsetted.color = lineStart.color;
		lineStartOffsetted.position += offset;

		sf::Vertex lineEndOffsetted = lineEnd;
		lineEndOffsetted.color = lineEnd.color;
		lineEndOffsetted.position += offset;

		lines.append(lineStartOffsetted);
		lines.append(lineEndOffsetted);
	}

	backgroundTexture.draw(lines);
	backgroundTexture.display();

}
void GameLevel::pathsGenerate() {
	pathGenerator.pathGenerate(sf::Vector2f(256, levelSize.y / 2), sf::Vector2f(levelSize.x - 256, levelSize.y / 2));
}
void GameLevel::pathsDraw() {
	pathsTexture.create(levelSize.x, levelSize.y);

	//sf::VertexArray lines;
	//lines.setPrimitiveType(sf::Lines);

	//// we use this quad array to batch draw circles by drawing a quad with the texture of a circle
	//sf::VertexArray quads;
	//quads.setPrimitiveType(sf::Quads);
	//sf::Texture& circleTexture = GraphicsStore::textureStore.objectGet("Circle");

	//const auto& path = pathGenerator.pathGet();
	//const auto& connections = pathGenerator.connectionsGet();

	//constexpr float circleSize = 325.f;

	//std::vector<sf::Vector2f> points;
	//for (uint16_t i = 0; i < connections.size(); i++) {

	//	PointIndex from = connections[i].first;
	//	PointIndex to = connections[i].second;

	//	sf::Vector2f pointFrom = path[from].position;
	//	sf::Vector2f pointTo = path[to].position;

	//	constexpr float desiredDist = 16.f;

	//	points.push_back(pointFrom);
	//	
	//	float dist = Vector2fMath::distSqrd(pointFrom, pointTo);
	//	while (dist > desiredDist * desiredDist) {

	//		dist = Vector2fMath::distSqrd(pointFrom, pointTo);

	//		if (dist < (desiredDist * 2.f) * (desiredDist * 2.f)) {
	//			pointFrom = pointTo;
	//		}
	//		else {
	//			pointFrom += Vector2fMath::lengthSet(Vector2fMath::axis(pointFrom, pointTo), desiredDist);
	//			pointFrom += sf::Vector2f(RNGf::getRange(32.f), RNGf::getRange(32.f));
	//		}
	//		points.push_back(pointFrom);
	//	}
	//}

	//const uint32_t dirtCount = 500 * points.size();

	//for (uint32_t i = 0; i < dirtCount; i++) {

	//	sf::Vector2f pointPosition = points[RNGu16::getUnder(points.size())];

	//	sf::Vector2f averagedOffset;
	//	uint16_t weight = 32;
	//	for (uint16_t weightIteration = 0; weightIteration < weight; weightIteration++) {
	//		averagedOffset += sf::Vector2f(RNGf::getFullRange(circleSize), RNGf::getFullRange(circleSize));
	//	}
	//	averagedOffset /= float(weight);

	//	sf::Vector2f circlePosition = pointPosition + Vector2fMath::lengthLimit(averagedOffset, circleSize);

	//	float distToNearestPointSqrd = 99999999999;
	//	for (uint32_t i = 0; i < points.size(); i++) {

	//		float distSqrd = Vector2fMath::distSqrd(points[i], circlePosition);
	//		if (distSqrd < distToNearestPointSqrd) {
	//			distToNearestPointSqrd = distSqrd;
	//		}
	//	}

	//	float offsetDistFromCenter = 1.f - (sqrt(distToNearestPointSqrd) / circleSize);
	//	float colorMult = offsetDistFromCenter * offsetDistFromCenter * offsetDistFromCenter;

	//	sf::Color color = sf::Color(RNGf::getRange(25, 70) * colorMult, RNGf::getRange(20, 60) * colorMult, RNGf::getRange(0, 6) * colorMult, 85 * colorMult);

	//	sf::FloatRect circleDimensions = sf::FloatRect(circlePosition.x, circlePosition.y, RNGf::getRange(1.f, 16.f), RNGf::getRange(1.f, 16.f));
	//	circleDimensions.left -= (circleDimensions.width / 2.f);
	//	circleDimensions.top -= (circleDimensions.height / 2.f);


	//	// top left corner
	//	sf::Vertex cornerTopLeft;
	//	cornerTopLeft.position = sf::Vector2f(circleDimensions.left, circleDimensions.top);
	//	// top right corner
	//	sf::Vertex cornerTopRight;
	//	cornerTopRight.position = sf::Vector2f(circleDimensions.left + circleDimensions.width, circleDimensions.top);
	//	// bottom right corner
	//	sf::Vertex cornerBottomRight;
	//	cornerBottomRight.position = sf::Vector2f(circleDimensions.left + circleDimensions.width, circleDimensions.top + circleDimensions.height);
	//	// bottom left corner
	//	sf::Vertex cornerBottomLeft;
	//	cornerBottomLeft.position = sf::Vector2f(circleDimensions.left, circleDimensions.top + circleDimensions.height);

	//	cornerTopLeft.color = color;
	//	cornerTopRight.color = color;
	//	cornerBottomRight.color = color;
	//	cornerBottomLeft.color = color;

	//	cornerTopLeft.texCoords = sf::Vector2f(0.f, 0.f);
	//	cornerTopRight.texCoords = sf::Vector2f(circleTexture.getSize().x, 0.f);
	//	cornerBottomRight.texCoords = sf::Vector2f(circleTexture.getSize().x, circleTexture.getSize().y);
	//	cornerBottomLeft.texCoords = sf::Vector2f(0.f, circleTexture.getSize().y);

	//	quads.append(cornerTopLeft);
	//	quads.append(cornerTopRight);
	//	quads.append(cornerBottomRight);
	//	quads.append(cornerBottomLeft);
	//}

	//const uint32_t lineCount = 25 * points.size();

	//for (uint32_t i = 0; i < lineCount; i++) {

	//	sf::Vector2f pointPosition = points[RNGu16::getUnder(points.size())];

	//	sf::Vector2f averagedOffset;
	//	uint16_t weight = 16;
	//	for (uint16_t weightIteration = 0; weightIteration < weight; weightIteration++) {
	//		averagedOffset += sf::Vector2f(RNGf::getFullRange(circleSize), RNGf::getFullRange(circleSize));
	//	}
	//	averagedOffset /= float(weight);

	//	sf::Vector2f linePosition = pointPosition + Vector2fMath::lengthLimit(averagedOffset, circleSize);

	//	float distToNearestPointSqrd = 9999999999999999;
	//	for (uint32_t i = 0; i < points.size(); i++) {

	//		float distSqrd = Vector2fMath::distSqrd(points[i], linePosition);
	//		if (distSqrd < distToNearestPointSqrd) {
	//			distToNearestPointSqrd = distSqrd;
	//		}
	//	}

	//	float offsetDistFromCenter = 1.f - (sqrt(distToNearestPointSqrd) / circleSize);
	//	float colorMult = offsetDistFromCenter * offsetDistFromCenter;

	//	sf::Color color = sf::Color(RNGf::getRange(25, 125) * colorMult, RNGf::getRange(25, 75) * colorMult, RNGf::getRange(0, 0) * colorMult, 85);

	//	sf::Vertex lineStart;
	//	lineStart.color = color;
	//	lineStart.position = linePosition;

	//	sf::Vertex lineEnd;
	//	lineEnd.color = color;

	//	// create vector with random heading
	//	sf::Vector2f lineEndOffset = sf::Vector2f(cos(RNGf::getFullRange(Mathf::PI)), sin(RNGf::getFullRange(Mathf::PI)));
	//	// scale length of vector
	//	lineEndOffset = Vector2fMath::lengthSet(lineEndOffset, RNGf::getRange(3.f, 12.f));

	//	lineEnd.position = lineStart.position + lineEndOffset;

	//	lines.append(lineStart);
	//	lines.append(lineEnd);


	//	sf::Vector2f axis = lineEnd.position - lineStart.position;
	//	float angleSide = atan2(axis.y, axis.x) + (Mathf::PI * 0.5f);

	//	sf::Vector2f offset = Vector2fMath::lengthSet(sf::Vector2f(cos(angleSide), sin(angleSide)), RNGf::getRange(1.f, 2.f));

	//	sf::Vertex lineStartOffsetted = lineStart;
	//	lineStartOffsetted.color = lineStart.color;
	//	lineStartOffsetted.position += offset;

	//	sf::Vertex lineEndOffsetted = lineEnd;
	//	lineEndOffsetted.color = lineEnd.color;
	//	lineEndOffsetted.position += offset;

	//	lines.append(lineStartOffsetted);
	//	lines.append(lineEndOffsetted);
	//}

	//pathsTexture.draw(quads, &circleTexture);
	//pathsTexture.draw(lines);
	pathsTexture.display();
}