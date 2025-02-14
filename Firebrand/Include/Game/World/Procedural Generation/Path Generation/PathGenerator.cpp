#include "PathGenerator.hpp"
#include <Auxiliary/VectorMath.hpp>
#include <Auxiliary/Math.hpp>
#include <iostream>
#include <cmath>
#include <Auxiliary/NumberGenerator.hpp>

PathGenerator::~PathGenerator() {
	for (uint16_t i = 0; i < pathPoints.size(); i++) {
		delete pathPoints[i];
	}
}
PointIndex PathGenerator::pointGetFromPosition(sf::Vector2f position) {
	for (PointIndex i = 0; i < pathPoints.size(); i++) {
		if (Vector2fMath::distSqrd(pathPoints[i]->position, position) < 16.f * 16.f) {
			return i;
		}
	}
	// failsafe to ensure something is returned
	return 0;
}
PointIndex PathGenerator::pointGetClosest(sf::Vector2f position) {

	PointIndex lowestInd = 0;
	float lowestDistSqrd = 9999999999999999999.f;

	for (PointIndex i = 0; i < pathPoints.size(); i++) {

		float distSqrd = Vector2fMath::distSqrd(pathPoints[i]->position, position);

		if (distSqrd < lowestDistSqrd) {
			lowestInd = i;
			lowestDistSqrd = distSqrd;
		}
	}

	return lowestInd;
}

std::vector<PointIndex> PathGenerator::pointsGetByDistance(sf::Vector2f position, float threshold) {
	
	// multimap of distances and indexes of points, we have the distance as the key so that the map is sorted based off distance,
	// later we just extract the indexes and put them in a vector, we never actually use the key for anything other than having it sorted
	std::multimap<float, PointIndex> pointsInThresholdMultiMap;
	
	// get all points within threshold
	for (PointIndex i = 0; i < pathPoints.size(); i++) {
		float distSqrd = Vector2fMath::distSqrd(pathPoints[i]->position, position);

		if (distSqrd < threshold * threshold) {
			pointsInThresholdMultiMap.insert({ distSqrd, i });
		}
	}

	// convert pointsInThresholdMap to a vector where
	std::vector<PointIndex> pointsInThresholdVector;
	for (const std::pair<float, PointIndex>& pair : pointsInThresholdMultiMap) {
		pointsInThresholdVector.push_back(pair.second);
	}

	return pointsInThresholdVector;
}

void PathGenerator::pointsConnect(PointIndex indA, PointIndex indB) {
	connections.push_back(PointConnection(indA, indB));
}
void PathGenerator::pointsConnect(const PathPoint* a, const PathPoint* b) {
	pointsConnect(a->index, b->index);
}

PathPoint* PathGenerator::pointCreate(sf::Vector2f position) {

	PointIndex index = PointIndex(pathPoints.size());

	pathPoints.push_back(new PathPoint(position, index));

	return pathPoints[index];
}

bool PathGenerator::lineIntersectsPath(const PathPoint* lineStart, const PathPoint* lineEnd) {
	for (uint32_t i = 0; i < connections.size(); i++) {

		const PathPoint* lineOtherStart = pathPoints[connections[i].first];
		const PathPoint* lineOtherEnd = pathPoints[connections[i].second];

		// make sure other line isn't the connected to the main line directly (I.E. share a point)
		if (lineStart->index == lineOtherStart->index) continue;
		if (lineStart->index == lineOtherEnd->index) continue;
		if (lineEnd->index == lineOtherStart->index) continue;
		if (lineEnd->index == lineOtherEnd->index) continue;

		if (Vector2fMath::linesIntersect(lineStart->position, lineEnd->position, lineOtherStart->position, lineOtherEnd->position)) {
			return true;
		}
	}

	return false;
}
float PathGenerator::pointGetDistSqrdToClosest(const PathPoint* point) {

	float closestDistSqrd = 999999999999999.f;

	for (uint16_t i = 0; i < pathPoints.size(); i++) {
		if (i == point->index) continue;

		float distSqrd = Vector2fMath::distSqrd(point->position, pathPoints[i]->position);

		if (distSqrd < closestDistSqrd) {
			closestDistSqrd = distSqrd;
		}
	}

	return closestDistSqrd;
}

bool PathGenerator::pointMergeWithClosestToPosition(PointIndex pointInd, sf::Vector2f position, float angle, float angleThreshold) {

	PathPoint* point = pathPoints[pointInd];

	std::vector<PointIndex> points = pointsGetByDistance(position);

	for (uint16_t i = 0; i < points.size(); i++) {
		if (pointInd != points[i]) {

			float angleToMergePoint = Vector2fMath::angle(point->position, pathPoints[points[i]]->position);

			// merge if the angle to the merge point isn't too off course to the end target
			if (abs(angleToMergePoint - angle) < angleThreshold) {
				pointsConnect(pointInd, points[i]);
				return true;
			}
		}
	}

	return false;
}

void PathGenerator::pointGenerate(PathPoint* point, const PathPoint* pointEnd, uint16_t generation, const uint16_t generationMax) {

	// generation from 0.0 to 1.0, where a generation equal to generationMax is equal to 1.0.
	// this is used for making things more or less likely depending on the generation
	float generationValue = (1.f / float(generationMax)) * float(generation + 1);

	sf::Vector2f axisToEnd = Vector2fMath::axis(point->position, pointEnd->position);

	if (generation < generationMax) {

		// the minimum distance a child can be from any neighbors
		constexpr float childrenMinDist = 4096.f;
		// distance that children can offset themselves from the "optimal" distance
		constexpr float childrenDistOffsetMin = -4096.f;
		constexpr float childrenDistOffsetMax = 4096.f;

		uint16_t childrenCount = RNGf::probability(generationValue) ? 1 : (RNGf::probability(generationValue) ? 2 : 3);

		float angleToTarget = atan2(axisToEnd.y, axisToEnd.x);
		float distFromParent = Vector2fMath::length(axisToEnd) / float((generationMax - generation) + 1.f);

		float childAngleOffset = RNGf::getRange(Mathf::PI);
		float childDistanceOffset = RNGf::getRange(childrenDistOffsetMin, childrenDistOffsetMax);

		sf::Vector2f childOffset = sf::Vector2f(cos(angleToTarget + childAngleOffset), sin(angleToTarget + childAngleOffset)) * (distFromParent + childDistanceOffset);

		// the "best" position for the child, which is a step directly towards the target
		sf::Vector2f childBestPosition = point->position + (axisToEnd / float((generationMax - generation) + 1.f));

		for (uint16_t i = 0; i < childrenCount; i++) {

			bool didMerge = false;

			// random chance to merge with neighbor, higher generations are more likely to merge
			if (RNGf::probability(generationValue)) {
				didMerge = pointMergeWithClosestToPosition(point->index, childBestPosition + childOffset, angleToTarget, Mathf::PI / 1.25f);
			}

			if (didMerge) continue;

			PathPoint* childPoint = pointCreate(childBestPosition + childOffset);

			uint32_t breaker = uint32_t(100000u);
			while ((lineIntersectsPath(point, childPoint) || (pointGetDistSqrdToClosest(childPoint) < childrenMinDist * childrenMinDist)) && ((--breaker) > 0)) {

				childAngleOffset = RNGf::getRange(Mathf::PI);
				childDistanceOffset = RNGf::getRange(childrenDistOffsetMin, childrenDistOffsetMax);

				childOffset = sf::Vector2f(cos(angleToTarget + childAngleOffset), sin(angleToTarget + childAngleOffset)) * (distFromParent + childDistanceOffset);

				if (generationBounds.contains(childBestPosition + childOffset)) {
					childPoint->position = childBestPosition + childOffset;
				}
			}

			if (breaker > 0) {
	/*			if (pointGetDistSqrdToClosest(childPoint) < (childrenMinDist * 1.25f) * (childrenMinDist * 1.25f)) {
					pointMergeWithClosestToPosition(childPoint->index, childBestPosition + childOffset, angleToTarget, Mathf::TAU);
				}*/
				pointsConnect(point, childPoint);

				pointGenerate(childPoint, pointEnd, generation + 1, generationMax);

			}
			else {
				// erase child if a point that didn't intersect the path could not be found
				pathPoints.erase(pathPoints.begin() + childPoint->index);

				pointMergeWithClosestToPosition(point->index, childBestPosition + childOffset, angleToTarget, Mathf::PI);
			}
		}
	}
	else {
		pointsConnect(point, pointEnd);
	}
}
void PathGenerator::pathGenerate(sf::Vector2f pointStartPosition, sf::Vector2f pointEndPosition) {
	pathPoints.clear();
	connections.clear();

	PathPoint* pointStart = pointCreate(pointStartPosition);
	PathPoint* pointEnd = pointCreate(pointEndPosition);

	pointGenerate(pointStart, pointEnd, 0, 12);
}

const std::vector<PathPoint*>& PathGenerator::pathGet() const {
	return pathPoints;
}
const std::vector<PointConnection>& PathGenerator::connectionsGet() const {
	return connections;
}
