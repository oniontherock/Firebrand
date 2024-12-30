#include "PathGenerator.hpp"
#include <Auxiliary/VectorMath.hpp>
#include <Auxiliary/Math.hpp>
#include <iostream>
#include <cmath>
#include <Auxiliary/NumberGenerator.hpp>

PathPoint::PathPoint(sf::Vector2f _position, PointIndex _index) {
	position = _position;
	index = _index;
}

PointIndex PathGenerator::pointGetFromPosition(sf::Vector2f position) {
	for (PointIndex i = 0; i < points.size(); i++) {
		if (Vector2fMath::distSqrd(points[i].position, position) < 16.f * 16.f) {
			return i;
		}
	}
}
PointIndex PathGenerator::pointGetClosest(sf::Vector2f position) {

	PointIndex lowestInd = 0;
	float lowestDistSqrd = 9999999999999999999;

	for (PointIndex i = 0; i < points.size(); i++) {
		
		float distSqrd = Vector2fMath::distSqrd(points[i].position, position);

		if (distSqrd < lowestDistSqrd) {
			lowestInd = i;
			lowestDistSqrd = distSqrd;
		}
	}

	return lowestInd;
}

void PathGenerator::pointsConnect(PointIndex indA, PointIndex indB) {
	connections.push_back(PointConnection(indA, indB));
}
void PathGenerator::pointsConnect(const PathPoint& a, const PathPoint& b) {
	pointsConnect(a.index, b.index);
}

PointIndex PathGenerator::pointCreate(sf::Vector2f position) {

	PointIndex index = points.size();

	points.push_back(PathPoint(position, index));

	return index;
}
PathPoint& PathGenerator::pointGet(PointIndex index) {
	return points[index];
}

bool PathGenerator::lineIntersectsPath(const PathPoint& lineStart, const PathPoint& lineEnd) {
	for (uint32_t i = 0; i < connections.size(); i++) {

		std::cout << points.size() << " " << connections[i].first << " " << connections[i].second << "\n";
		const PathPoint& lineOtherStart = points[connections[i].first];
		const PathPoint& lineOtherEnd = points[connections[i].second];

		// make sure other line isn't the connected to the main line directly (I.E. share a point)
		if (lineStart.index == lineOtherStart.index) continue;
		if (lineStart.index == lineOtherEnd.index) continue;
		if (lineEnd.index == lineOtherStart.index) continue;
		if (lineEnd.index == lineOtherEnd.index) continue;

		if (Vector2fMath::linesIntersect(lineStart.position, lineEnd.position, lineOtherStart.position, lineOtherEnd.position)) {
			return true;
		}
	}

	return false;
}
float PathGenerator::pointGetDistSqrdToClosest(const PathPoint& point) {

	float closestDistSqrd = 999999999999999;

	for (uint16_t i = 0; i < points.size(); i++) {
		if (i == point.index) continue;

		float distSqrd = Vector2fMath::distSqrd(point.position, points[i].position);

		if (distSqrd < closestDistSqrd) {
			closestDistSqrd = distSqrd;
		}
	}

	return closestDistSqrd;
}

void PathGenerator::pointGenerate(PathPoint& point, const PathPoint& pointEnd, uint16_t generation, const uint16_t generationMax) {

	// generation from 0.0 to 1.0, where a generation equal to generationMax is equal to 1.0.
	// this is used for making things more or less likely depending on the generation
	float generationValue = (1.f / float(generationMax)) * float(generation + 1);

	sf::Vector2f axisToEnd = Vector2fMath::axis(point.position, pointEnd.position);

	if (generation < generationMax) {

		// the minimum distance a child can be from any neighbors
		constexpr float childrenMinDist = 256.f;
		// distance that children can offset themselves from the "optimal" distance
		constexpr float childrenDistOffsetMin = -256.f;
		constexpr float childrenDistOffsetMax = 128.f;

		uint16_t childrenCount = RNGf::probability(generationValue) ? 1 : (RNGf::probability(generationValue) ? 2 : 3);

		float angleToTarget = atan2(axisToEnd.y, axisToEnd.x);
		float distFromParent = Vector2fMath::length(axisToEnd) / float((generationMax - generation) + 1.f);

		// the "best" position for the child, which is a step directly towards the target
		sf::Vector2f childBestPosition = point.position + (axisToEnd / float((generationMax - generation) + 1.f));

		for (uint16_t i = 0; i < childrenCount; i++) {

			// random chance to merge with neighbor, higher generations are more likely to merge
			if (RNGf::probability(generationValue)) {
				PointIndex pointInd = pointGetClosest(childBestPosition);
				if (point.index != pointInd) {
					std::cout << "merged: " << point.index << " " << pointInd << std::endl;
					pointsConnect(point.index, pointInd);
					continue;
				}
			}

			PathPoint& childPoint = pointGet(pointCreate(childBestPosition));

			uint16_t breaker = 10000;
			while ((lineIntersectsPath(point, childPoint) || (pointGetDistSqrdToClosest(childPoint) < childrenMinDist * childrenMinDist)) && ((--breaker) > 0)) {

				float childAngleOffset = RNGf::getRange(Mathf::PI);
				float childDistanceOffset = RNGf::getRange(childrenDistOffsetMin, childrenDistOffsetMax);

				sf::Vector2f childOffset = sf::Vector2f(cos(angleToTarget + childAngleOffset), sin(angleToTarget + childAngleOffset)) * (distFromParent + childDistanceOffset);

				childPoint.position = childBestPosition + childOffset;
			}
			if (breaker <= 0) {
				// erase if a point that didn't intersect the path could not be found
				points.erase(points.begin() + childPoint.index);

				// connect parent point to closest point to avoid dead ends
				PointIndex pointInd = pointGetClosest(childBestPosition);
				if (point.index != pointInd) {
					std::cout << "dead end avoided: " << point.index << " " << pointInd << std::endl;
					pointsConnect(point, points[pointInd]);
					continue;
				}
			}
			else {
				std::cout << "parent connected: " << point.index << " " << childPoint.index << std::endl;
				pointsConnect(point, childPoint);

				pointGenerate(childPoint, pointEnd, generation + 1, generationMax);

			}
		}
	}
	else {
		std::cout << "end connected: " << point.index << " " << pointEnd.index << std::endl;
		pointsConnect(point, pointEnd);

	}
}
void PathGenerator::pathGenerate(sf::Vector2f pointStartPosition, sf::Vector2f pointEndPosition) {

	PathPoint& pointStart = pointGet(pointCreate(pointStartPosition));
	std::cout << "generation started: " << pointStart.index << std::endl;

	PathPoint& pointEnd = pointGet(pointCreate(pointEndPosition));
	std::cout << "generation started: " << pointStart.index << " " << pointEnd.index << std::endl;

	pointGenerate(pointStart, pointEnd, 0, 10);
}

const std::vector<PathPoint>& PathGenerator::pathGet() {
	return points;
}
const std::vector<PointConnection>& PathGenerator::connectionsGet() {
	return connections;
}
