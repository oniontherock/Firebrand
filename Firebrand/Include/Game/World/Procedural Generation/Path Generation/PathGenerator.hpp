#ifndef __PATH_GENERATOR_H__
#define __PATH_GENERATOR_H__

#include <cstdint>
#include <set>
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <map>

// index of a point in the PathGenerator's points vector
typedef uint16_t PointIndex;
// pair of two PointIndexes, represents a connection between two points
typedef std::pair<PointIndex, PointIndex> PointConnection;

struct PathPoint {

	PathPoint(sf::Vector2f _position, PointIndex _index);

	// position of this point
	sf::Vector2f position;
	// index of this point
	PointIndex index;
};

// generates the large paths that make up the world
class PathGenerator {

	// vector of points in the path.
	std::vector<PathPoint*> pathPoints;
	// vector of PointConnections between points in the path
	std::vector<PointConnection> connections;

	// returns the index of the point at the specified position, or UINT32_MAX if there is not point.
	// technically this doesn't get the point EXACTLY at the position, it just gets the first point within a 16 pixel radius of the position
	PointIndex pointGetFromPosition(sf::Vector2f position);
	// returns the closest point to the position, more expensive than pointGetFromPosition when the point is within 16 pixels
	PointIndex pointGetClosest(sf::Vector2f position);
	// returns  a vector of any pairs of point indexes and squared distances to position that are closer than threshold
	std::vector<PointIndex> pointsGetByDistance(sf::Vector2f position, float threshold = 999999);

	// connects two points, takes the point's indexes
	void pointsConnect(PointIndex indA, PointIndex indB);
	// connects two points
	void pointsConnect(const PathPoint* a, const PathPoint* b);

	// creates a point, adds it to the points vector, and returns it
	PathPoint* pointCreate(sf::Vector2f position);

	// merges the point with the point closest to position if the angle to that merge point isn't more than angleThreshold, returns whether or not it merged
	bool pointMergeWithClosestToPosition(PointIndex pointInd, sf::Vector2f position, float angle, float angleThreshold);

	// returns whether the given line intersects any of the point connections (I.E. if the line crosses the path.
	bool lineIntersectsPath(const PathPoint* lineStart, const PathPoint* lineEnd);
	// returns the squared distance to the point closest to the given point
	float pointGetDistSqrdToClosest(const PathPoint* point);

	// generates a point, this includes children and connections.
	// this function recursively calls itself with it's children points,
	//
	// @param point: the point to generate.
	// @param pointEnd: the end point of the generation.
	// @param generation: the current generation of points created.
	// @param generationMax: the maximum number of generations, if the generation reaches the max generation, the point is connected to pointEnd.
	void pointGenerate(PathPoint* point, const PathPoint* pointEnd, uint16_t generation, const uint16_t generationMax);

public:
	~PathGenerator();
	void pathGenerate(sf::Vector2f pointStartPosition, sf::Vector2f pointEndPosition);

	sf::FloatRect generationBounds;

	const std::vector<PathPoint*>& pathGet();
	const std::vector<PointConnection>& connectionsGet();
};


#endif