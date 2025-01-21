#ifndef __COLLISION_SHAPE_H__
#define __COLLISION_SHAPE_H__

#include "SFML/Graphics.hpp"

typedef std::vector<sf::Vector2f> CollisionPolygon;

struct CollisionShape {
	CollisionShape();
	CollisionShape(CollisionPolygon _vertices);

	// list of points that make up the shape relative to center
	CollisionPolygon vertices;

	// returns the support point in a given direction
	sf::Vector2f supportPointGet(sf::Vector2f direction);

	// sets a new center and updates vertices
	void centerSet(sf::Vector2f centerNew);
	// offsets the center and updates vertices
	void centerAdd(sf::Vector2f centerOffset);
	// sets a new rotation and updates vertices
	void rotationSet(float rotationNew);
	// adds to the rotation and updates vertices
	void rotationAdd(float rotationOffset);
	// sets a new center and rotation and updates vertices,
	// faster than calling centerSet and rotationSet independently, since it only has to update vertices once
	void transformSet(sf::Vector2f centerNew, float rotationNew);

	sf::Vector2f centerGet() const;
	float rotationGet() const;


private:
	float rotation = 0.f;

	sf::Vector2f center = sf::Vector2f(0, 0);
};


#endif