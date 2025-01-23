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



void CollisionProcessor::collisionsProcess() {

	std::unordered_map<EntityId, std::set<EntityId>> collisionExclusions;

	for (Collider& collider : colliders) {

		std::vector<Collider*> colliderPossibleCollidables;

		for (Collider& collidable : collidables) {
			// skip if i equals j
			if (collider.entityId == collidable.entityId) continue;

			if (collisionExclusions[collider.entityId].contains(collidable.entityId)) continue;

			// this isn't an amazing way of doing this, because it just assumes that every shape in a Collider has the same center,
			// but it's really quick this way, and i think it'll be fine so i'm leaving it.
			float centersDistSqrd = Vector2fMath::distSqrd(collider.shapes[0]->centerGet(), collider.shapes[0]->centerGet());
			// get the maximum distance that the two colliders can be away from each other while still colliding
			float shapesVertexMaxDist = abs(collider.shapesVertexMaxDist - collidable.shapesVertexMaxDist);

			// skip if the shapes are farther than the farthest possible distance they can be while still colliding
			if (centersDistSqrd > (shapesVertexMaxDist * shapesVertexMaxDist)) continue;

			colliderPossibleCollidables.push_back(&collidable);
		}

		sf::Vector2f greatestCollisionAxis;
		float greatestCollisionAxisLenSqrd = -999999.f;
		EntityId greatestId = 0;

		for (CollisionShape* shapeCur : collider.shapes) {
			for (Collider* possibleCollidable : colliderPossibleCollidables) {
				for (CollisionShape* shapeOtherCur : possibleCollidable->shapes) {

					// skip if the current two shapes are not colliding
					if (!CollisionHandler::collisionCheck(*shapeCur, *shapeOtherCur)) continue;

					sf::Vector2f collisionAxis = CollisionHandler::collisionVectorGet(*shapeCur, *shapeOtherCur);

					float collisionAxisLenSqrd = Vector2fMath::lengthSqrd(collisionAxis);

					if (collisionAxisLenSqrd > greatestCollisionAxisLenSqrd) {
						greatestCollisionAxis = collisionAxis;
						greatestCollisionAxisLenSqrd = collisionAxisLenSqrd;
						greatestId = possibleCollidable->entityId;
					}
				}
			}
		}
		
		//float massRatio = 
		Entity& entityI = EntityManager::entityGet(collider.entityId);
		Entity& entityJ = EntityManager::entityGet(greatestId);

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
		eventCollisionI->colliderId = greatestId;
		eventCollisionI->collisionAxis = (-greatestCollisionAxis / GameData::physicsTimer.target) * float(TimeHandler::deltaRealGet()) * movementAmountI;

		auto* eventCollisionJ = entityJ.entityEventAddAndGet<EntityEvents::EventCollision>();
		eventCollisionJ->colliderId = collider.entityId;
		eventCollisionJ->collisionAxis = (greatestCollisionAxis / GameData::physicsTimer.target) * float(TimeHandler::deltaRealGet()) * movementAmountJ;
		
		collisionExclusions[greatestId].insert(collider.entityId);
	}

	colliders.clear();
}

