#include "../Include/Game/World/Procedural Generation/Structure Generation/Instantiation/StructureInstantiator.hpp"
#include "../Include/Game/World/Procedural Generation/Structure Generation/StructureGenerator.hpp"
#include "GameLevel.hpp"
#include <Graphics/Stores/GraphicsStore.hpp>

GameLevel::GameLevel() :
	levelSize(sf::Vector2u(4096 * 1, 4096 * 1)),
	aStarGrid(AStarGrid(levelSize.x / 64, levelSize.y / 64, 64.f, 64.f)),
	objectGrid(levelSize.x / 16, levelSize.y / 16, 16.f, 16.f),
	pathGenerator(PathGenerator()),
	backgroundTexture(TextureGrid(uint16_t(ceil(float(levelSize.x) / 1280.f)), uint16_t(ceil(float(levelSize.y) / 720.f)), 1280, 720)),
	pathsTexture(TextureGrid(uint16_t(ceil(float(levelSize.x) / 1280.f)), uint16_t(ceil(float(levelSize.y) / 720.f)), 1280, 720))
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

void GameLevel::backgroundDraw(sf::FloatRect rect, uint32_t drawIterationsMax) {

	textureGridsUpdateValidity(rect);

	//grassDraw(rect, drawIterationsMax);
	pathsDraw(rect, drawIterationsMax);
}

void GameLevel::textureUpdateValidity(sf::FloatRect rect, TextureGrid& texture) const {
	std::vector<sf::Vector2u> texturesInBackgroundRect = texture.texturePositionsGetInRectangle(backgroundRect);

	// iterate over texturesInRect and initialize any invalid textures
	for (uint32_t i = 0; i < texturesInBackgroundRect.size(); i++) {
		if (!texture.cellValidate(texturesInBackgroundRect[i])) {
			texture.cellInitialize(texturesInBackgroundRect[i]);
		}
	}

	std::vector<sf::Vector2u> validTexturesPrev = texture.texturePositionsGetInRectangle(backgroundRectPrev, true);

	// iterate over previous rect's valid textures and terminate them if they are no longer in rect
	for (uint32_t i = 0; i < validTexturesPrev.size(); i++) {
		if (std::find(texturesInBackgroundRect.begin(), texturesInBackgroundRect.end(), validTexturesPrev[i]) == texturesInBackgroundRect.end()) {
			texture.cellTerminate(validTexturesPrev[i]);
		}
	}
}

void GameLevel::textureGridsUpdateValidity(sf::FloatRect rect) {

	backgroundRect = sf::FloatRect(rect.getPosition() - (rect.getSize() / 2.f), rect.getSize() * 2.f);

	textureUpdateValidity(rect, backgroundTexture);
	textureUpdateValidity(rect, pathsTexture);

	backgroundRectPrev = backgroundRect;
}

void GameLevel::pathsGenerate() {

	ConsoleHandler::consolePrintLoadingGame("Path Generation Started");

	constexpr float boundOffset = 64.f;
	pathGenerator.generationBounds = sf::FloatRect(boundOffset, boundOffset, levelSize.x - boundOffset, levelSize.y - boundOffset);

	constexpr float levelSizeDivider = 1.2f;

	sf::Vector2f startPoint;
	sf::Vector2f endPoint;

	// squared distance between startPoint and endPoint
	float pointDistSqrd = 0;

	uint32_t printer = 0;

	// loop if the distance between the start and end points is less than the length of the level size divided by levelSizeDivider
	do {
		startPoint = sf::Vector2f(RNGf::getRange(0, levelSize.x), RNGf::getRange(0, levelSize.y));
		endPoint = sf::Vector2f(RNGf::getRange(0, levelSize.x), RNGf::getRange(0, levelSize.y));

		pointDistSqrd = Vector2fMath::distSqrd(startPoint, endPoint);
		
		if (((printer++) % 10000) == 0) ConsoleHandler::consolePrintLoadingGame("Path Start And End Points Generating...");

	} while (pointDistSqrd < (Vector2fMath::lengthSqrd(float(levelSize.x), float(levelSize.y)) / levelSizeDivider));

	ConsoleHandler::consolePrintLoadingGame("Path Start And End Points Generated");

	pathGenerator.pathGenerate(startPoint, endPoint);
	ConsoleHandler::consolePrintLoadingGame("Path Generation Finished");

	ConsoleHandler::consolePrintLoadingGame("Path Point Plotting Started");
	const auto& path = pathGenerator.pathGet();
	const auto& connections = pathGenerator.connectionsGet();

	// get the points that form the path, used for drawing the path
	for (uint16_t i = 0; i < connections.size(); i++) {

		PointIndex from = connections[i].first;
		PointIndex to = connections[i].second;

		sf::Vector2f pointFrom = path[from]->position;
		sf::Vector2f pointTo = path[to]->position;

		constexpr float desiredDist = 16.f;

		pathPoints.push_back(pointFrom);

		float dist = Vector2fMath::distSqrd(pointFrom, pointTo);
		while (dist > desiredDist * desiredDist) {

			dist = Vector2fMath::distSqrd(pointFrom, pointTo);

			if (dist < (desiredDist * 2.f) * (desiredDist * 2.f)) {
				pointFrom = pointTo;
			}
			else {
				pointFrom += Vector2fMath::lengthSet(Vector2fMath::axis(pointFrom, pointTo), desiredDist);
				pointFrom += sf::Vector2f(RNGf::getRange(32.f), RNGf::getRange(32.f));
			}

			pathPoints.push_back(pointFrom);
		}
	}

	ConsoleHandler::consolePrintLoadingGame("Path Point Plotting Completed");
}
void GameLevel::structuresGenerate() {
	StructureGrid structure = StructureGenerator::structureGenerate(StructureType::Home, sf::Vector2f(2048, 2048), 0, sf::Vector2u(24, 24));
	StructureInstantiator::structureInstantiate(levelPosition, structure);
}

void GameLevel::grassDraw(sf::FloatRect rect, uint32_t drawIterationsMax) {

	const uint32_t grassBladePerTextureCount = 250000 / drawIterationsMax;

	// offset for the random placement of grass, this is needed to avoid obvious seams between cells
	constexpr float posOffset = 32.f;
	constexpr float grassLenMin = 3.f;
	constexpr float grassLenMax = 12.f;

	std::vector<GridTexture*> texturesInRect = backgroundTexture.texturesGetInRectangle(rect);

	for (uint16_t i = 0; i < texturesInRect.size(); i++) {
		
		GridTexture* texture = texturesInRect[i];

		if (texture->displayCountGet() >= drawIterationsMax) continue;

		sf::VertexArray lines;
		lines.setPrimitiveType(sf::Lines);

		for (uint32_t grassBladeCur = 0; grassBladeCur < grassBladePerTextureCount; grassBladeCur++) {
			sf::Color color = sf::Color(RNGu8::getRange(50, 125), RNGu8::getRange(100, 255), RNGu8::getRange(0, 25), 85);

			sf::Vertex lineStart;
			lineStart.color = color;
			lineStart.position = sf::Vector2f(
				RNGf::getRange(-posOffset, backgroundTexture.cellsGetSizeX() + posOffset),
				RNGf::getRange(-posOffset, backgroundTexture.cellsGetSizeY() + posOffset)
			);

			sf::Vertex lineEnd;
			lineEnd.color = color;

			// create vector with random heading
			sf::Vector2f lineEndOffset = sf::Vector2f(cos(RNGf::getFullRange(Mathf::PI)), sin(RNGf::getFullRange(Mathf::PI)));
			// scale length of vector
			lineEndOffset = Vector2fMath::lengthSet(lineEndOffset, RNGf::getRange(grassLenMin, grassLenMax));

			lineEnd.position = lineStart.position + lineEndOffset;

			lines.append(lineStart);
			lines.append(lineEnd);


			sf::Vector2f axis = lineEnd.position - lineStart.position;
			float angleSide = atan2(axis.y, axis.x) + (Mathf::PI * 0.5f);

			sf::Vector2f offset = Vector2fMath::lengthSet(sf::Vector2f(cos(angleSide), sin(angleSide)), RNGf::getRange(-1.f, 1.f));

			sf::Vertex lineStartOffsetted = lineStart;
			lineStartOffsetted.color = lineStart.color;
			lineStartOffsetted.position += offset;

			sf::Vertex lineEndOffsetted = lineEnd;
			lineEndOffsetted.color = lineEnd.color;
			lineEndOffsetted.position += offset;

			lines.append(lineStartOffsetted);
			lines.append(lineEndOffsetted);
		}

		sf::View textureView = texture->getView();
		texture->setView(texture->getDefaultView());

		texture->draw(lines);
		texture->display();

		texture->setView(textureView);
	}
}
void GameLevel::pathsDraw(sf::FloatRect rect, uint32_t drawIterationsMax) {

	const sf::Texture& circleTexture = GraphicsStore::textureStore.objectGet("Circle");
	
	constexpr float circleSize = 325.f;

	std::vector<GridTexture*> texturesInRect = pathsTexture.texturesGetInRectangle(rect);

	for (uint16_t i = 0; i < texturesInRect.size(); i++) {

		GridTexture* texture = texturesInRect[i];

		uint16_t textureDisplayCount = texture->displayCountGet();

		if (textureDisplayCount >= drawIterationsMax) continue;
		
		// rectangle that is twice the size of the texture's rectangle
		sf::FloatRect textureRect = sf::FloatRect(texture->positionGet() - (sf::Vector2f(texture->getSize()) / 2.f), sf::Vector2f(texture->getSize()) * 2.f);

		sf::VertexArray lines;
		lines.setPrimitiveType(sf::Lines);

		// we use this quad array to batch draw circles by drawing a quad with the texture of a circle
		sf::VertexArray quads;
		quads.setPrimitiveType(sf::Quads);

		// get the points in pathPoints that are near the texture
		std::vector<sf::Vector2f> points;
		for (uint16_t i = 0; i < pathPoints.size(); i++) {
			if (textureRect.contains(pathPoints[i])) {
				points.push_back(pathPoints[i]);
			}
		}

		if (points.size() <= 0) continue;

		constexpr uint16_t distributionWeight = 16;

		if (textureDisplayCount < drawIterationsMax / 2) {
			const uint32_t dirtPerPointCount = (500 * points.size()) / drawIterationsMax;

			for (uint32_t i = 0; i < dirtPerPointCount; i++) {

				sf::Vector2f pointPosition = points[RNGu32::getUnder(points.size())];

				sf::Vector2f averagedOffset;
				for (uint16_t weightIteration = 0; weightIteration < distributionWeight; weightIteration++) {
					averagedOffset += sf::Vector2f(RNGf::getFullRange(circleSize), RNGf::getFullRange(circleSize));
				}
				averagedOffset /= float(distributionWeight);

				sf::Vector2f circlePosition = pointPosition + Vector2fMath::lengthLimit(averagedOffset, circleSize);

				float distToNearestPointSqrd = 99999999999;
				for (uint32_t pointIndCur = 0; pointIndCur < points.size(); pointIndCur++) {

					float distSqrd = Vector2fMath::distSqrd(points[pointIndCur], circlePosition);
					if (distSqrd < distToNearestPointSqrd) {
						distToNearestPointSqrd = distSqrd;
					}
				}

				float offsetDistFromCenter = 1.f - (sqrt(distToNearestPointSqrd) / circleSize);
				float colorMult = offsetDistFromCenter * offsetDistFromCenter * offsetDistFromCenter;

				sf::Color color = sf::Color(RNGf::getRange(25, 70) * colorMult, RNGf::getRange(20, 60) * colorMult, RNGf::getRange(0, 6) * colorMult, 85 * colorMult);

				sf::FloatRect circleDimensions = sf::FloatRect(circlePosition.x, circlePosition.y, RNGf::getRange(1.f, 16.f), RNGf::getRange(1.f, 16.f));
				circleDimensions.left -= (circleDimensions.width / 2.f);
				circleDimensions.top -= (circleDimensions.height / 2.f);


				// top left corner
				sf::Vertex cornerTopLeft;
				cornerTopLeft.position = sf::Vector2f(circleDimensions.left, circleDimensions.top);
				// top right corner
				sf::Vertex cornerTopRight;
				cornerTopRight.position = sf::Vector2f(circleDimensions.left + circleDimensions.width, circleDimensions.top);
				// bottom right corner
				sf::Vertex cornerBottomRight;
				cornerBottomRight.position = sf::Vector2f(circleDimensions.left + circleDimensions.width, circleDimensions.top + circleDimensions.height);
				// bottom left corner
				sf::Vertex cornerBottomLeft;
				cornerBottomLeft.position = sf::Vector2f(circleDimensions.left, circleDimensions.top + circleDimensions.height);

				cornerTopLeft.color = color;
				cornerTopRight.color = color;
				cornerBottomRight.color = color;
				cornerBottomLeft.color = color;

				cornerTopLeft.texCoords = sf::Vector2f(0.f, 0.f);
				cornerTopRight.texCoords = sf::Vector2f(circleTexture.getSize().x, 0.f);
				cornerBottomRight.texCoords = sf::Vector2f(circleTexture.getSize().x, circleTexture.getSize().y);
				cornerBottomLeft.texCoords = sf::Vector2f(0.f, circleTexture.getSize().y);

				quads.append(cornerTopLeft);
				quads.append(cornerTopRight);
				quads.append(cornerBottomRight);
				quads.append(cornerBottomLeft);
			}
		}
		else {
			const uint32_t linePerPointCount = (25 * points.size()) / drawIterationsMax;

			for (uint32_t i = 0; i < linePerPointCount; i++) {

				sf::Vector2f pointPosition = points[RNGu16::getUnder(points.size())];

				sf::Vector2f averagedOffset;
				for (uint16_t weightIteration = 0; weightIteration < distributionWeight; weightIteration++) {
					averagedOffset += sf::Vector2f(RNGf::getFullRange(circleSize), RNGf::getFullRange(circleSize));
				}
				averagedOffset /= float(distributionWeight);

				sf::Vector2f linePosition = pointPosition + Vector2fMath::lengthLimit(averagedOffset, circleSize);

				float distToNearestPointSqrd = 9999999999999999;
				for (uint32_t pointIndCur = 0; pointIndCur < points.size(); pointIndCur++) {

					float distSqrd = Vector2fMath::distSqrd(points[pointIndCur], linePosition);
					if (distSqrd < distToNearestPointSqrd) {
						distToNearestPointSqrd = distSqrd;
					}
				}

				float offsetDistFromCenter = 1.f - (sqrt(distToNearestPointSqrd) / circleSize);
				float colorMult = offsetDistFromCenter * offsetDistFromCenter;

				sf::Color color = sf::Color(RNGf::getRange(25, 125) * colorMult, RNGf::getRange(25, 75) * colorMult, RNGf::getRange(0, 0) * colorMult, 85);

				sf::Vertex lineStart;
				lineStart.color = color;
				lineStart.position = linePosition;

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

				sf::Vector2f offset = Vector2fMath::lengthSet(sf::Vector2f(cos(angleSide), sin(angleSide)), RNGf::getRange(-1.f, 1.f));

				sf::Vertex lineStartOffsetted = lineStart;
				lineStartOffsetted.color = lineStart.color;
				lineStartOffsetted.position += offset;

				sf::Vertex lineEndOffsetted = lineEnd;
				lineEndOffsetted.color = lineEnd.color;
				lineEndOffsetted.position += offset;

				lines.append(lineStartOffsetted);
				lines.append(lineEndOffsetted);
			}
		}

		// draw on all surrounding cells so textures connected seamlessly
		for (int16_t xOffset = -1; xOffset <= 1; xOffset++) {
			for (int16_t yOffset = -1; yOffset <= 1; yOffset++) {
				
				sf::Vector2u textureGridPosition = texture->gridPositionGet();
				
				sf::Vector2i posOffsetted = sf::Vector2i(textureGridPosition) + sf::Vector2i(xOffset, yOffset);

				// skip if position is not in grid
				if (!pathsTexture.cellPosIsInGrid(posOffsetted.x, posOffsetted.y)) continue;

				// skip if cell not valid
				if (!pathsTexture.cellValidate(posOffsetted.x, posOffsetted.y)) continue;

				GridTextureUniquePtr& gridTexture = pathsTexture.cellGet(posOffsetted.x, posOffsetted.y);

				gridTexture->draw(quads, &circleTexture);
				gridTexture->draw(lines);
				gridTexture->displayInvisible();
			
			}
		}
		texture->display();
	}
}