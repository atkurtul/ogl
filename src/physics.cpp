#include "physics.h"

void PhysicsSimulator::init()
{
	config = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(config);
	cache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	world = new btDiscreteDynamicsWorld(dispatcher, cache, solver, config);
	world->setGravity(btVector3(0, -10, 0));

}

btRigidBody* PhysicsSimulator::add_body(glm::vec3 bbMin, glm::vec3 bbMax, glm::vec3 pos, glm::vec3 initial_velocity)
{
	auto half = (bbMax - bbMin) * 0.5f;
	float mass = half.x * half.y * half.z;
	bool isDynamic = (mass != 0.f);
	btCollisionShape* colShape = new btBoxShape(btVector3(half.x, half.y, half.z));
	shapes.push_back(colShape);
	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);
	startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setLinearVelocity(btVector3(initial_velocity.x, initial_velocity.y, initial_velocity.z));
	world->addRigidBody(body);
	return body;
}

void PhysicsSimulator::integrate(float t)
{
	world->stepSimulation(t);
}

PhysicsSimulator::~PhysicsSimulator()
{
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
			delete body->getMotionState();

		world->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < shapes.size(); j++)
	{
		delete shapes[j];
	}

	delete world;
	delete solver;
	delete cache;
	delete dispatcher;
	delete config;
}