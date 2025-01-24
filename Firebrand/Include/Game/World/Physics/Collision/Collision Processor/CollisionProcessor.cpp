#include "CollisionProcessor.hpp"
#include <Auxiliary/VectorMath.hpp>
#include <ECS/Entities/EntityManager.hpp>
#include "../include/Game/GameData.hpp"
#include <unordered_map>
#include "../ACECS/ECSRegistry.hpp"

std::vector<CollisionProcessor::Collider> CollisionProcessor::colliders{};
std::vector<CollisionProcessor::Collider> CollisionProcessor::collidables{};

void CollisionProcessor::colliderRequestProcess(EntityId entityId, std::vector<CollisionShapeBase*> shapes, float shapesMaxVertexDist) {
	colliders.push_back(Collider(entityId, shapesMaxVertexDist, shapes));
}
void CollisionProcessor::colliderRequestProcess(EntityId entityId, CollisionShapeBase* shape, float shapeMaxVertexDist) {
	colliderRequestProcess(entityId, std::vector<CollisionShapeBase*>{ shape }, shapeMaxVertexDist);
}
void CollisionProcessor::colliderRequestProcess(EntityId entityId, std::vector<CollisionShapeBase*> shapes) {
	float distMax = -INFINITY;

	for (CollisionShapeBase* shape : shapes) {
		float distCur = shape->shapeMaxDistGet();
		if (distCur > distMax) {
			distMax = distCur;
		}
	}

	colliderRequestProcess(entityId, shapes, distMax);
}
void CollisionProcessor::colliderRequestProcess(EntityId entityId, CollisionShapeBase* shape) {
	colliderRequestProcess(entityId, { shape }, shape->shapeMaxDistGet());
}

void CollisionProcessor::entityMarkCollidable(EntityId entityId, std::vector<CollisionShapeBase*> shapes, float shapesMaxVertexDist) {
	collidables.push_back(Collider(entityId, shapesMaxVertexDist, shapes));
}
void CollisionProcessor::entityMarkCollidable(EntityId entityId, CollisionShapeBase* shape, float shapeMaxVertexDist) {
	entityMarkCollidable(entityId, std::vector<CollisionShapeBase*>{ shape }, shapeMaxVertexDist);
}
void CollisionProcessor::entityMarkCollidable(EntityId entityId, std::vector<CollisionShapeBase*> shapes) {
	float distMax = -INFINITY;

	for (CollisionShapeBase* shape : shapes) {
		float distCur = shape->shapeMaxDistGet();
		if (distCur > distMax) {
			distMax = distCur;
		}
	}

	entityMarkCollidable(entityId, shapes, distMax);
}
void CollisionProcessor::entityMarkCollidable(EntityId entityId, CollisionShapeBase* shape) {
	entityMarkCollidable(entityId, { shape }, shape->shapeMaxDistGet());
}

struct PairComparator {
	bool operator()(const std::pair<EntityId, EntityId>& p1, const std::pair<EntityId, EntityId>& p2) const {
		return p1.second - p2.second;
	}
};
bool CollisionProcessor::Collider::operator< (const Collider& other) {
	return entityId < other.entityId;
}

void CollisionProcessor::collisionsProcess() {

	std::unordered_map<EntityId, sf::Vector2f> entityCollisionAxes;

#pragma region Collision Detection
	for (Collider& collider : colliders) {
		for (Collider& collidable : collidables) {
			// skip if i equals j
			if (collider.entityId == collidable.entityId) continue;

			if (collider.shapes.size() <= 0 || collidable.shapes.size() <= 0) continue;

			// this isn't an amazing way of doing this, because it just assumes that every shape in a Collider has the same center,
			// but it's really quick this way, and i think it'll be fine so i'm leaving it.
			float centersDistSqrd = Vector2fMath::distSqrd(collider.shapes[0]->centerGet(), collidable.shapes[0]->centerGet());
			// get the maximum distance that the two colliders can be away from each other while still colliding
			float shapesVertexMaxDist = collider.shapesVertexMaxDist + collidable.shapesVertexMaxDist;

			// skip if the shapes are farther than the farthest possible distance they can be while still colliding
			if (centersDistSqrd > (shapesVertexMaxDist * shapesVertexMaxDist)) continue;

			for (CollisionShapeBase* shapeCur : collider.shapes) {
				for (CollisionShapeBase* shapeOtherCur : collidable.shapes) {

					if (!CollisionHandler::collisionCheck(shapeCur, shapeOtherCur)) continue;

					sf::Vector2f collisionAxis = CollisionHandler::collisionVectorGet(shapeCur, shapeOtherCur);

					//float massRatio = 
					Entity& entityI = EntityManager::entityGet(collider.entityId);
					Entity& entityJ = EntityManager::entityGet(collidable.entityId);

					float massI = 1000000.f;
					float massJ = 1000000.f;

					if (entityI.entityComponentHas<EntityComponents::ComponentMass>()) {
						massI = entityI.entityComponentGet<EntityComponents::ComponentMass>()->mass;
					}
					if (entityJ.entityComponentHas<EntityComponents::ComponentMass>()) {
						massJ = entityJ.entityComponentGet<EntityComponents::ComponentMass>()->mass;
					}

					bool iHigherMass = massI > massJ;

					float massRatio = iHigherMass ? (massI / massJ) : (massJ / massI);

					float movementRatio = 0.5f / massRatio;

					float movementAmountI = iHigherMass ? (movementRatio) : (1.f - movementRatio);
					float movementAmountJ = iHigherMass ? (1.f - movementRatio) : (movementRatio);

					entityI.entityEventAddAndGet<EntityEvents::EventCollision>()->collisionAxis = (-collisionAxis * movementAmountI);
					entityJ.entityEventAddAndGet<EntityEvents::EventCollision>()->collisionAxis = (collisionAxis * movementAmountJ);
				}
			}
		}
	}
#pragma endregion Collision Detection

	colliders.clear();
}

