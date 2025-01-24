#include "Auxiliary/VectorMath.hpp"
#include "CollisionShape.hpp"
#include "queue"
#include <Auxiliary/ConsoleHandler.hpp>
#include <iostream>

CollisionShapeBase::CollisionShapeBase() {
}

void CollisionShapeBase::centerSet(sf::Vector2f centerNew) {
	center = centerNew;
}
void CollisionShapeBase::centerAdd(sf::Vector2f centerOffset) {
	center += centerOffset;
}
void CollisionShapeBase::rotationSet(float rotationNew) {
	rotation = rotationNew;
}
void CollisionShapeBase::rotationAdd(float rotationOffset) {
	rotation += rotationOffset;
}
void CollisionShapeBase::transformSet(sf::Vector2f centerNew, float rotationNew) {
	center = centerNew;
	rotation = rotationNew;
}

sf::Vector2f CollisionShapeBase::centerGet() const {
	return center;
}

float CollisionShapeBase::shapeMaxDistGet() const {
	return shapeMaxDist;
}

float CollisionShapeBase::rotationGet() const {
	return rotation;
}

sf::Vector2f CollisionShapePolygon::supportPointGet(sf::Vector2f direction) {
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

CollisionShapePolygon::CollisionShapePolygon() {
}
CollisionShapePolygon::CollisionShapePolygon(CollisionPolygon _vertices) {
	if (_vertices.size() <= 2) {
		ConsoleHandler::consolePrintErr("CollisionShape created with less than 3 vertices!");
		return;
	}

	vertices = _vertices;


	float maxDistSqrd = -INFINITY;
	for (uint16_t i = 0; i < vertices.size(); i++) {

		const sf::Vector2f& vertex = vertices[i];

		float distSqrd = Vector2fMath::lengthSqrd(vertex);

		if (distSqrd > maxDistSqrd) {
			maxDistSqrd = distSqrd;
		}
	}
	shapeMaxDist = sqrt(maxDistSqrd);
}

void CollisionShapePolygon::centerSet(sf::Vector2f centerNew) {

	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by moving it by the axis from the old center to the new center
		vertices[i] += centerNew - center;
	}

	center = centerNew;
}
void CollisionShapePolygon::centerAdd(sf::Vector2f centerOffset) {
	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by moving it by centerOffset
		vertices[i] += centerOffset;
	}

	center += centerOffset;
}
void CollisionShapePolygon::rotationSet(float rotationNew) {

	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by localizing it with the center,
		// rotating it by the difference of the old rotation to the new rotation,
		// then globalizing it to center
		vertices[i] = Vector2fMath::rotate(vertices[i] - center, rotationNew - rotation) + center;
	}

	rotation = rotationNew;
}
void CollisionShapePolygon::rotationAdd(float rotationOffset) {

	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by localizing it with the center,
		// rotating it by the rotationOffset,
		// then globalizing it to center
		vertices[i] = Vector2fMath::rotate(vertices[i] - center, rotationOffset) + center;
	}

	rotation += rotationOffset;
}
void CollisionShapePolygon::transformSet(sf::Vector2f centerNew, float rotationNew) {

	for (uint16_t i = 0; i < vertices.size(); i++) {
		// update the current vertex by localizing it with the old center,
		// rotating it by the difference of the old rotation to the new rotation,
		// then globalizing it to new center
		vertices[i] = Vector2fMath::rotate(vertices[i] - center, rotationNew - rotation) + centerNew;
	}

	center = centerNew;
	rotation = rotationNew;
}
