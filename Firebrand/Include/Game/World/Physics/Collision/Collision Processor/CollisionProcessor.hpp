#ifndef __COLLISION_PROCESSOR_H__
#define __COLLISION_PROCESSOR_H__

//#include "../ACECS/ECSRegistry.hpp"
#include "../CollisionHandler.hpp"
#include <ECS/TypeDefinitions.hpp>
#include <map>


class CollisionProcessor {

	struct Collider {
		EntityId entityId;
		float shapesVertexMaxDist;
		std::vector<CollisionShapeBase*> shapes;

		bool operator< (const Collider& other);
	};

	// vector of Colliders, whenever an entity requests that it's collisions be processed, it goes in this queue
	static std::vector<Collider> colliders;
	// vector of Colliders, colliders that are added to this list are always processed in collisionsProcess, this is used for things like walls, that don't update but need their collisions checked
	static std::vector<Collider> collidables;
public:
	static void colliderRequestProcess(EntityId entityId, std::vector<CollisionShapeBase*> shapes, float shapesMaxVertexDist);
	static void colliderRequestProcess(EntityId entityId, CollisionShapeBase* shape, float shapeMaxVertexDist);

	static void colliderRequestProcess(EntityId entityId, std::vector<CollisionShapeBase*> shapes);
	static void colliderRequestProcess(EntityId entityId, CollisionShapeBase* shape);


	static void entityMarkCollidable(EntityId entityId, std::vector<CollisionShapeBase*> shapes, float shapesMaxVertexDist);
	static void entityMarkCollidable(EntityId entityId, CollisionShapeBase* shape, float shapeMaxVertexDist);

	static void entityMarkCollidable(EntityId entityId, std::vector<CollisionShapeBase*> shapes);
	static void entityMarkCollidable(EntityId entityId, CollisionShapeBase* shape);


	static void collisionsProcess();
};


#endif