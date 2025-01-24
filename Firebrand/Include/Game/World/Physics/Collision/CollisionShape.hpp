#ifndef __COLLISION_SHAPE_H__
#define __COLLISION_SHAPE_H__

#include "SFML/Graphics.hpp"

typedef std::vector<sf::Vector2f> CollisionPolygon;

struct CollisionShapeBase {
	CollisionShapeBase();

	// returns the support point in a given direction
	virtual sf::Vector2f supportPointGet(sf::Vector2f direction) = 0;

	// sets a new center and updates vertices
	virtual void centerSet(sf::Vector2f centerNew);
	// offsets the center and updates vertices
	virtual void centerAdd(sf::Vector2f centerOffset);
	// sets a new rotation and updates vertices
	virtual void rotationSet(float rotationNew);
	// adds to the rotation and updates vertices
	virtual void rotationAdd(float rotationOffset);
	// sets a new center and rotation and updates vertices,
	// faster than calling centerSet and rotationSet independently, since it only has to update vertices once
	virtual void transformSet(sf::Vector2f centerNew, float rotationNew);

	sf::Vector2f centerGet() const;
	float rotationGet() const;
	float shapeMaxDistGet() const;

protected:
	float rotation = 0.f;

	sf::Vector2f center = sf::Vector2f(0, 0);
	// the distance to the furthest vertex in vertices
	float shapeMaxDist;
};
struct CollisionShapePolygon : CollisionShapeBase {
	CollisionShapePolygon();
	CollisionShapePolygon(CollisionPolygon _vertices);

	// list of points that make up the shape relative to center
	CollisionPolygon vertices;

	// returns the support point in a given direction
	sf::Vector2f supportPointGet(sf::Vector2f direction) final;

	// sets a new center and updates vertices
	void centerSet(sf::Vector2f centerNew) final;
	// offsets the center and updates vertices
	void centerAdd(sf::Vector2f centerOffset) final;
	// sets a new rotation and updates vertices
	void rotationSet(float rotationNew) final;
	// adds to the rotation and updates vertices
	void rotationAdd(float rotationOffset) final;
	// sets a new center and rotation and updates vertices,
	// faster than calling centerSet and rotationSet independently, since it only has to update vertices once
	void transformSet(sf::Vector2f centerNew, float rotationNew) final;
};

#endif