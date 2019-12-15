#include "MyEntity.h"
using namespace Simplex;
std::map<String, MyEntity*> MyEntity::m_IDMap;
//  Accessors
matrix4 Simplex::MyEntity::GetModelMatrix(void){ return m_m4ToWorld; }
void Simplex::MyEntity::SetModelMatrix(matrix4 a_m4ToWorld)
{
	if (!m_bInMemory)
		return;

	m_m4ToWorld = a_m4ToWorld;
	m_pModel->SetModelMatrix(m_m4ToWorld);
	m_pRigidBody->SetModelMatrix(m_m4ToWorld);
}
Model* Simplex::MyEntity::GetModel(void){return m_pModel;}
MyRigidBody* Simplex::MyEntity::GetRigidBody(void){	return m_pRigidBody; }
bool Simplex::MyEntity::IsInitialized(void){ return m_bInMemory; }
String Simplex::MyEntity::GetUniqueID(void) { return m_sUniqueID; }
void Simplex::MyEntity::SetAxisVisible(bool a_bSetAxis) { m_bSetAxis = a_bSetAxis; }
//  MyEntity
void Simplex::MyEntity::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bInMemory = false;
	m_bSetAxis = false;
	m_pModel = nullptr;
	m_pRigidBody = nullptr;
	m_DimensionArray = nullptr;
	m_m4ToWorld = IDENTITY_M4;
	m_sUniqueID = "";
	m_nDimensionCount = 0;
}
void Simplex::MyEntity::Swap(MyEntity& other)
{
	m_bInMemory = false;
	std::swap(m_pModel, other.m_pModel);
	std::swap(m_pRigidBody, other.m_pRigidBody);
	std::swap(m_m4ToWorld, other.m_m4ToWorld);
	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_bInMemory, other.m_bInMemory);
	std::swap(m_sUniqueID, other.m_sUniqueID);
	std::swap(m_bSetAxis, other.m_bSetAxis);
	std::swap(m_nDimensionCount, other.m_nDimensionCount);
	std::swap(m_DimensionArray, other.m_DimensionArray);
}
void Simplex::MyEntity::Release(void)
{
	m_pMeshMngr = nullptr;
	//it is not the job of the entity to release the model, 
	//it is for the mesh manager to do so.
	m_pModel = nullptr;
	if (m_DimensionArray)
	{
		delete[] m_DimensionArray;
		m_DimensionArray = nullptr;
	}
	SafeDelete(m_pRigidBody);
	m_IDMap.erase(m_sUniqueID);
}
//The big 3
Simplex::MyEntity::MyEntity(String a_sFileName, String a_sUniqueID)
{
	debug = false;
	Init();
	m_pModel = new Model();
	m_pModel->Load(a_sFileName);
	//if the model is loaded
	if (m_pModel->GetName() != "")
	{
		GenUniqueID(a_sUniqueID);
		m_sUniqueID = a_sUniqueID;
		m_IDMap[a_sUniqueID] = this;
		m_pRigidBody = new MyRigidBody(m_pModel->GetVertexList()); //generate a rigid body
		m_bInMemory = true; //mark this entity as viable
	}
}
Simplex::MyEntity::MyEntity(MyEntity const& other)
{
	m_bInMemory = other.m_bInMemory;
	m_pModel = other.m_pModel;
	//generate a new rigid body we do not share the same rigid body as we do the model
	m_pRigidBody = new MyRigidBody(m_pModel->GetVertexList()); 
	m_m4ToWorld = other.m_m4ToWorld;
	m_pMeshMngr = other.m_pMeshMngr;
	m_sUniqueID = other.m_sUniqueID;
	m_bSetAxis = other.m_bSetAxis;
	m_nDimensionCount = other.m_nDimensionCount;
	m_DimensionArray = other.m_DimensionArray;

}
MyEntity& Simplex::MyEntity::operator=(MyEntity const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyEntity temp(other);
		Swap(temp);
	}
	return *this;
}
MyEntity::~MyEntity(){Release();}
//--- Methods
void Simplex::MyEntity::AddToRenderList(bool a_bDrawRigidBody)
{
	//if not in memory return
	if (!m_bInMemory)
		return;

	//draw model
	m_pModel->AddToRenderList();
	
	//draw rigid body
	if(a_bDrawRigidBody)
		m_pRigidBody->AddToRenderList();

	if (m_bSetAxis)
		m_pMeshMngr->AddAxisToRenderList(m_m4ToWorld);
}
MyEntity* Simplex::MyEntity::GetEntity(String a_sUniqueID)
{
	//look the entity based on the unique id
	auto entity = m_IDMap.find(a_sUniqueID);
	//if not found return nullptr, if found return it
	return entity == m_IDMap.end() ? nullptr : entity->second;
}
void Simplex::MyEntity::GenUniqueID(String& a_sUniqueID)
{
	static uint index = 0;
	String sName = a_sUniqueID;
	MyEntity* pEntity = GetEntity(a_sUniqueID);
	//while MyEntity exists keep changing name
	while (pEntity)
	{
		a_sUniqueID = sName + "_" + std::to_string(index);
		index++;
		pEntity = GetEntity(a_sUniqueID);
	}
	return;
}
void Simplex::MyEntity::AddDimension(uint a_uDimension)
{
	//we need to check that this dimension is not already allocated in the list
	if (IsInDimension(a_uDimension))
		return;//it is, so there is no need to add

	//insert the entry
	uint* pTemp;
	pTemp = new uint[m_nDimensionCount + 1];
	if(m_DimensionArray)
	{
		memcpy(pTemp, m_DimensionArray, sizeof(uint) * m_nDimensionCount);
		delete[] m_DimensionArray;
		m_DimensionArray = nullptr;
	}
	pTemp[m_nDimensionCount] = a_uDimension;
	m_DimensionArray = pTemp;

	++m_nDimensionCount;

	SortDimensions();
}
void Simplex::MyEntity::RemoveDimension(uint a_uDimension)
{
	//if there are no dimensions return
	if (m_nDimensionCount == 0)
		return;

	//we look one by one if its the one wanted
	for (uint i = 0; i < m_nDimensionCount; i++)
	{
		if (m_DimensionArray[i] == a_uDimension)
		{
			//if it is, then we swap it with the last one and then we pop
			std::swap(m_DimensionArray[i], m_DimensionArray[m_nDimensionCount - 1]);
			uint* pTemp;
			pTemp = new uint[m_nDimensionCount - 1];
			if (m_DimensionArray)
			{
				memcpy(pTemp, m_DimensionArray, sizeof(uint) * (m_nDimensionCount-1));
				delete[] m_DimensionArray;
				m_DimensionArray = nullptr;
			}
			m_DimensionArray = pTemp;
			
			--m_nDimensionCount;
			SortDimensions();
			return;
		}
	}
}
void Simplex::MyEntity::ClearDimensionSet(void)
{
	if (m_DimensionArray)
	{
		delete[] m_DimensionArray;
		m_DimensionArray = nullptr;
	}
	m_nDimensionCount = 0;
}
bool Simplex::MyEntity::IsInDimension(uint a_uDimension)
{
	//see if the entry is in the set
	for (uint i = 0; i < m_nDimensionCount; i++)
	{
		if (m_DimensionArray[i] == a_uDimension)
			return true;
	}
	return false;
}
bool Simplex::MyEntity::SharesDimension(MyEntity* const a_pOther)
{
	
	//special case: if there are no dimensions on either MyEntity
	//then they live in the special global dimension
	if (0 == m_nDimensionCount)
	{
		//if no spatial optimization all cases should fall here as every 
		//entity is by default, under the special global dimension only
		if(0 == a_pOther->m_nDimensionCount)
			return true;
	}

	//for each dimension on both Entities we check if there is a common dimension
	for (uint i = 0; i < m_nDimensionCount; ++i)
	{
		for (uint j = 0; j < a_pOther->m_nDimensionCount; j++)
		{
			if (m_DimensionArray[i] == a_pOther->m_DimensionArray[j])
				return true; //as soon as we find one we know they share dimensionality
		}
	}

	//could not find a common dimension
	return false;
}
bool Simplex::MyEntity::IsColliding(MyEntity* const other)
{
	//if not in memory return
	if (!m_bInMemory || !other->m_bInMemory)
		return true;

	//if the entities are not living in the same dimension
	//they are not colliding
	if (!SharesDimension(other))
		return false;

	if ((tag == Wall || tag == Goal) && (other->tag == Wall || other->tag == Goal))
	{
		return false;
	}
	else if (tag == Wall || tag == Goal)
	{
		return other->GetRigidBody()->CheckRectSphereCollision(m_pRigidBody);
	}
	else if (other->tag == Wall || other->tag == Goal)
	{
		return m_pRigidBody->CheckRectSphereCollision(other->GetRigidBody());
	}

	return m_pRigidBody->IsColliding(other->GetRigidBody());
}
void Simplex::MyEntity::ClearCollisionList(void)
{
	m_pRigidBody->ClearCollidingList();
}
void Simplex::MyEntity::SortDimensions(void)
{
	std::sort(m_DimensionArray, m_DimensionArray + m_nDimensionCount);
}


void Simplex::MyEntity::PushingCollisionResolution(MyEntity* other)
{

	// Get centers to push apart
	vector3 v3Distance = GetRigidBody()->GetCenterGlobal() - other->GetRigidBody()->GetCenterGlobal();
	float length = sqrt(v3Distance.x * v3Distance.x + v3Distance.y * v3Distance.y);
	vector3 normalized = v3Distance / length;

	// Calculate velocity based on centers
	// Then update position (if it isn't a wall or goal)
	if (tag == Puck || tag == Player)
	{
		velocity += (-v3Distance * .5f + normalized * other->GetRigidBody()->GetRadius());
		velocity.z = 0;
		currentPosition += velocity;
	}
	if (other->tag == Puck || other->tag == Player)
	{
		other->velocity += (v3Distance * .5f - normalized * GetRigidBody()->GetRadius());
		other->velocity.z = 0;
		other->currentPosition += other->velocity;
	}

	//std::cout << "push" << std::endl;
}



void Simplex::MyEntity::CollisionResolution(MyEntity* other)
{

#pragma region Hit Definition

	bool spheres = tag != Wall && other->tag != Wall;

	// Define the hits and if the two objects are moving together there is no reason to reflect
	if (spheres)
	{
		GetRigidBody()->DefineHits(other->GetRigidBody(), spheres);

		// Check for if the objects are moving together
		if ((velocity.x > 0 && other->velocity.x > 0) || (velocity.x < 0 && other->velocity.x < 0))
		{
			GetRigidBody()->xHit = 0;
			other->GetRigidBody()->xHit = 0;
		}
		if ((velocity.y > 0 && other->velocity.y > 0) || (velocity.y < 0 && other->velocity.y < 0))
		{
			GetRigidBody()->yHit = 0;
			other->GetRigidBody()->yHit = 0;
		}
	}

	// If there is a rectangular prism than that prism needs to be the "other" parameter
	else
	{
		if (tag == Wall || tag == Goal)
		{
			other->GetRigidBody()->DefineHits(GetRigidBody(), spheres);
		}
		else
		{
			GetRigidBody()->DefineHits(other->GetRigidBody(), spheres);
		}
	}
#pragma endregion

	
#pragma region Debug Info

	// Print information out for bug testing
	
	/*
	if (tag == Player || other->tag == Player)
	{
		std::cout << "object 1:" << std::endl;
		std::cout << "velocity: x: " << velocity.x << ", y: " << velocity.y << ", z: " << velocity.z << std::endl;
		std::cout << "current position: x: " << currentPosition.x << ", y: " << currentPosition.y << ", z: " << currentPosition.z << std::endl;
		std::cout << "previous position: x: " << previousPosition.x << ", y: " << previousPosition.y << ", z: " << previousPosition.z << std::endl;
		std::cout << "hits: x: " << GetRigidBody()->xHit << ", y: " << GetRigidBody()->yHit << std::endl;
		std::cout << std::endl << std::endl;

		std::cout << "object 2:" << std::endl;
		std::cout << "velocity: x: " << other->velocity.x << ", y: " << other->velocity.y << ", z: " << other->velocity.z << std::endl;
		std::cout << "current position: x: " << other->currentPosition.x << ", y: " << other->currentPosition.y << ", z: " << other->currentPosition.z << std::endl;
		std::cout << "previous position: x: " << other->previousPosition.x << ", y: " << other->previousPosition.y << ", z: " << other->previousPosition.z << std::endl;
		std::cout << "hits: x: " << other->GetRigidBody()->xHit << ", y: " << other->GetRigidBody()->yHit << std::endl;
		std::cout << std::endl << std::endl;
	}
	*/
#pragma endregion

#pragma region Puck Resolutions

	// Puck hits a goal
	if (tag == Puck && other->tag == Goal)
	{
		if (other->currentPosition.x < 0) // *** could be set to a variable rather than specific value
			HitGoal(1);
		else
			HitGoal(2);
	}
	else if (other->tag == Puck && tag == Goal)
	{
		if (currentPosition.x < 0) // *** same here
			other->HitGoal(1);
		else
			other->HitGoal(2);
	}

	
	// Puck reflects off of something 
	// Define reflections
	vector3 velocity1 = velocity;
	vector3 velocity2 = other->velocity;
	vector3 reflection1 = vector3((1 + -2 * GetRigidBody()->xHit) * velocity1.x, (1 + -2 * GetRigidBody()->yHit) * velocity1.y, velocity1.z);
	vector3 reflection2 = vector3((1 + -2 * other->GetRigidBody()->xHit) * velocity2.x, (1 + -2 * other->GetRigidBody()->yHit) * velocity2.y, velocity2.z);

	// Use reflections
	if (other->tag == Puck)
	{
		if (tag == Puck)
		{
			other->velocity = 0.5f * (velocity1 * 1.1f + reflection2);
			velocity = 0.5f * (velocity2 * 1.1f + reflection1);
		}
		else
		{
			other->velocity = velocity1 * 1.1f + reflection2;
			velocity = vector3(0);
		}
	}
	else if (tag == Puck)
	{
		velocity = velocity2 * 1.1f + reflection1;
		other->velocity = vector3(0);
	}
	velocity.z = 0;
	other->velocity.z = 0;
#pragma endregion


#pragma region Player Resolutions

	// Cancel momentum on a player to player collision
	if (tag == Player && other->tag == Player)
	{
		velocity = vector3(0);
		other->velocity = vector3(0);
	}

	// Player hits a wall and needs to slide along it
	if (tag == Player && (other->tag == Wall || other->tag == Goal))
	{
		float x = (GetRigidBody()->xHit) * previousPosition.x + (1 - GetRigidBody()->xHit) * currentPosition.x;
		float y = (GetRigidBody()->yHit) * previousPosition.y + (1 - GetRigidBody()->yHit) * currentPosition.y;
		currentPosition = vector3(x, y, previousPosition.z);
		velocity = vector3(0);
	}
	// Same if but switching which is other and which is this
	else if ((tag == Wall || tag == Goal) && other->tag == Player)
	{
		float x = (other->GetRigidBody()->xHit) * other->previousPosition.x + (1 - other->GetRigidBody()->xHit) * other->currentPosition.x;
		float y = (other->GetRigidBody()->yHit) * other->previousPosition.y + (1 - other->GetRigidBody()->yHit) * other->currentPosition.y;
		other->currentPosition = vector3(x, y, previousPosition.z);
		other->velocity = vector3(0);
	}
	else
	{
		currentPosition = previousPosition;
		other->currentPosition = other->previousPosition;
	}

#pragma endregion



	// Reset information for reflections and sliding
	GetRigidBody()->xHit = GetRigidBody()->yHit = GetRigidBody()->zHit = false;
	other->GetRigidBody()->xHit = other->GetRigidBody()->yHit = other->GetRigidBody()->zHit = false;

}

void Simplex::MyEntity::HitGoal(int goalScoredOn)
{
	// reset the position of this puck
	vector3 v3position = vector3(glm::sphericalRand(34.0f));
	v3position.z = 0;
	previousPosition = v3position;
	currentPosition = v3position;
	velocity = vector3(0);

	// Set GoalScored to true for use in MyEntityManager.cpp
	m_iGoalScored = goalScoredOn;
}

// Method for when model matrix was being overriden incorrectly
void Simplex::MyEntity::RotateModel(float a_fAngle, vector3 a_v3axis)
{
	m_m4ToWorld = glm::rotate(m_m4ToWorld, glm::radians(a_fAngle), a_v3axis);
	m_pModel->SetModelMatrix(m_m4ToWorld);
	m_pRigidBody->SetModelMatrix(m_m4ToWorld);
}
