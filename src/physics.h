#pragma once
#include "common.h"
#include "btBulletDynamicsCommon.h"

struct PhysicsSimulator {
	btDefaultCollisionConfiguration* config;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* cache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* world;
	btAlignedObjectArray<btCollisionShape*> shapes;

	void init();

	btRigidBody* add_body(glm::vec3 bbMin, glm::vec3 bbMax, glm::vec3 pos, glm::vec3 initial_velocity);

	void integrate(float t);

	~PhysicsSimulator();
};