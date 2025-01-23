#include "CollisionProcessor.hpp"
#include <Auxiliary/VectorMath.hpp>
#include <ECS/Entities/EntityManager.hpp>
#include "../include/Game/GameData.hpp"
#include "../ACECS/ECSRegistry.hpp"

std::vector<CollisionProcessor::Collider> CollisionProcessor::colliders{};
std::vector<CollisionProcessor::Collider> CollisionProcessor::collidables{};

void CollisionProcessor::colliderRequestProcess(EntityId entityId, std::vector<CollisionShape*> shapes, float shapesMaxVertexDist) {
	colliders.push_back(Collider(entityId, shapesMaxVertexDist, shapes));
}
void CollisionProcessor::colliderRequestProcess(EntityId entityId, CollisionShape* shape, float shapeMaxVertexDist) {
	colliderRequestProcess(entityId, std::vector<CollisionShape*>{ shape }, shapeMaxVertexDist);
}
void CollisionProcessor::colliderRequestProcess(EntityId entityId, std::vector<CollisionShape*> shapes) {
	float distMax = -INFINITY;

	for (CollisionShape* shape : shapes) {
		float distCur = shape->vertexMaxDistGet();
		if (distCur > distMax) {
			distMax = distCur;
		}
	}

	colliderRequestProcess(entityId, shapes, distMax);
}
void CollisionProcessor::colliderRequestProcess(EntityId entityId, CollisionShape* shape) {
	colliderRequestProcess(entityId, { shape }, shape->vertexMaxDistGet());
}

void CollisionProcessor::entityMarkCollidable(EntityId entityId, std::vector<CollisionShape*> shapes, float shapesMaxVertexDist) {
	collidables.push_back(Collider(entityId, shapesMaxVertexDist, shapes));
}
void CollisionProcessor::entityMarkCollidable(EntityId entityId, CollisionShape* shape, float shapeMaxVertexDist) {
	entityMarkCollidable(entityId, std::vector<CollisionShape*>{ shape }, shapeMaxVertexDist);
}
void CollisionProcessor::entityMarkCollidable(EntityId entityId, std::vector<CollisionShape*> shapes) {
	float distMax = -INFINITY;

	for (CollisionShape* shape : shapes) {
		float distCur = shape->vertexMaxDistGet();
		if (distCur > distMax) {
			distMax = distCur;
		}
	}

	entityMarkCollidable(entityId, shapes, distMax);
}
void CollisionProcessor::entityMarkCollidable(EntityId entityId, CollisionShape* shape) {
	entityMarkCollidable(entityId, { shape }, shape->vertexMaxDistGet());
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

	for (Collider& collider : colliders) {

		std::vector<Collider*> colliderPossibleCollidables;

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

			for (CollisionShape* shapeCur : collider.shapes) {
				for (CollisionShape* shapeOtherCur : collidable.shapes) {

					if (!CollisionHandler::collisionCheck(*shapeCur, *shapeOtherCur)) continue;

					sf::Vector2f collisionAxis = CollisionHandler::collisionVectorGet(*shapeCur, *shapeOtherCur);

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

					auto* eventCollisionI = entityI.entityEventAddAndGet<EntityEvents::EventCollision>();
					eventCollisionI->colliderId = collidable.entityId;
					eventCollisionI->collisionAxis = (-collisionAxis / GameData::physicsTimer.target) * float(TimeHandler::deltaRealGet()) * movementAmountI;

					auto* eventCollisionJ = entityJ.entityEventAddAndGet<EntityEvents::EventCollision>();
					eventCollisionJ->colliderId = collider.entityId;
					eventCollisionJ->collisionAxis = (collisionAxis / GameData::physicsTimer.target) * float(TimeHandler::deltaRealGet()) * movementAmountJ;

					if (entityI.entityComponentHas<EntityComponents::ComponentCollisionResponse>()) entityI.entityComponentGet<EntityComponents::ComponentCollisionResponse>()->system(entityI);
					if (entityJ.entityComponentHas<EntityComponents::ComponentCollisionResponse>()) entityJ.entityComponentGet<EntityComponents::ComponentCollisionResponse>()->system(entityJ);

					entityI.entityComponentGet<EntityComponents::ComponentPosition>()->system(entityI);
					entityJ.entityComponentGet<EntityComponents::ComponentPosition>()->system(entityJ);

					entityI.entityComponentGet<EntityComponents::ComponentCollisionShape>()->system(entityI);
					entityJ.entityComponentGet<EntityComponents::ComponentCollisionShape>()->system(entityJ);
				}
			}
		}
	}

	colliders.clear();
}

