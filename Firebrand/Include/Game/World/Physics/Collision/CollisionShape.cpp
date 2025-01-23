#include "Auxiliary/VectorMath.hpp"
#include "CollisionShape.hpp"
#include "queue"
#include <Auxiliary/ConsoleHandler.hpp>
#include <iostream>

CollisionShape::CollisionShape() {
}

CollisionShape::CollisionShape(CollisionPolygon _vertices) {
	if (_vertices.size() <= 2) {
		ConsoleHandler::consolePrintErr("CollisionShape created with less than 3 vertices!");
		return;
	}

	vertices = _vertices;


	float maxDistSqrd = -INFINITY;
	for (uint16_t i = 0; i < vertices.size(); i++) {

		const sf::Vector2f& vertex = vertices[i];

		float distSqrd = Vector2fMath::lengthSqrd(vertex);

		if (distSqrd > vertexMaxDist) {
			vertexMaxDist = distSqrd;
		}
	}
	vertexMaxDist = sqrt(maxDistSqrd);
}

void CollisionShape::centerSet(sf::Vector2f centerNew) {

	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by moving it by the axis from the old center to the new center
		vertices[i] += centerNew - center;
	}

	center = centerNew;
}
void CollisionShape::centerAdd(sf::Vector2f centerOffset) {
	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by moving it by centerOffset
		vertices[i] += centerOffset;
	}

	center += centerOffset;
}
void CollisionShape::rotationSet(float rotationNew) {

	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by localizing it with the center,
		// rotating it by the difference of the old rotation to the new rotation,
		// then globalizing it to center
		vertices[i] = Vector2fMath::rotate(vertices[i] - center, rotationNew - rotation) + center;
	}

	rotation = rotationNew;
}
void CollisionShape::rotationAdd(float rotationOffset) {

	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by localizing it with the center,
		// rotating it by the rotationOffset,
		// then globalizing it to center
		vertices[i] = Vector2fMath::rotate(vertices[i] - center, rotationOffset) + center;
	}

	rotation += rotationOffset;
}
void CollisionShape::transformSet(sf::Vector2f centerNew, float rotationNew) {

	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by localizing it with the old center,
		// rotating it by the difference of the old rotation to the new rotation,
		// then globalizing it to new center
		vertices[i] = Vector2fMath::rotate(vertices[i] - center, rotationNew - rotation) + centerNew;
	}

	center = centerNew;
	rotation = rotationNew;
}

sf::Vector2f CollisionShape::centerGet() const {
	return center;
}
float CollisionShape::rotationGet() const {
	return rotation;
}

sf::Vector2f CollisionShape::supportPointGet(sf::Vector2f direction) {
	float bestDot = -1234567.f;
	uint16_t bestInd = UINT16_MAX;


	for (uint16_t i = 0; i < vertices.size(); i++) {
		float dot = Vector2fMath::dot(vertices[i], direction);
		if (dot > bestDot || bestInd >= UINT16_MAX) {
			bestDot = dot;
			bestInd = i;
		}
	}

	return vertices[bestInd];
}

float CollisionShape::vertexMaxDistGet() const {
	return vertexMaxDist;
}
