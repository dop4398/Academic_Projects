#include "MyOctant.h"
using namespace Simplex;
using namespace std;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;

MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	// base initialization
	Init();

	// set octant properties
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	// update ID
	m_uID = m_uOctantCount;

	// reset child list
	m_lChild.clear();

	// set as root octant
	m_pRoot = this;

	// min/max vectors of bounding object
	std::vector<vector3> v3MinMax;

	// create bounding object
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		v3MinMax.push_back(m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal());
		v3MinMax.push_back(m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal());
	}
	MyRigidBody* myRigidBody = new MyRigidBody(v3MinMax);

	// find max of half width
	vector3 v3HalfWidth = myRigidBody->GetHalfWidth();
	float fMax = v3HalfWidth.x;
	for (uint i = 0; i < 3; i++)
	{
		if (fMax < v3HalfWidth[i])
			fMax = v3HalfWidth[i];
	}
	
	// set dimensions
	m_fSize = fMax * 2.0f;
	m_v3Center = myRigidBody->GetCenterLocal();
	m_v3Min = m_v3Center - (vector3(fMax));
	m_v3Max = m_v3Center + (vector3(fMax));

	// update octant count
	m_uOctantCount++;

	// construct rest of octree
	ConstructTree(m_uMaxLevel);

	// clean up
	v3MinMax.clear();
	SafeDelete(myRigidBody);
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize) 
{
	// base initialization
	Init();

	// set properties
	m_fSize = a_fSize;

	m_v3Center = a_v3Center;
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	// update octant count     
	m_uOctantCount++;
}

MyOctant::MyOctant(MyOctant const& other)
{
	// copy properties from parameter
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;
	
	m_fSize = other.m_fSize;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_pParent = other.m_pParent;
	m_lChild, other.m_lChild;

	m_pRoot, other.m_pRoot;

	for (uint i = 0; i < 8; i++)
		m_pChild[i] = other.m_pChild[i];
}

MyOctant& MyOctant::operator=(MyOctant const& other) 
{
	if (this != &other)
	{
		// format octant
		Release();
		Init();

		// copy and swap
		MyOctant temp(other);
		Swap(temp);
	}

	return *this;
}

MyOctant::~MyOctant() 
{
	Release();
}

void MyOctant::Swap(MyOctant& other) 
{
	// swap octant properties
	swap(m_uID, other.m_uID);
	swap(m_uLevel, other.m_uLevel);
	swap(m_uChildren, other.m_uChildren);

	swap(m_fSize, other.m_fSize);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	swap(m_v3Center, other.m_v3Center);
	swap(m_v3Min, other.m_v3Min);
	swap(m_v3Max, other.m_v3Max);

	swap(m_lChild, other.m_lChild);
	swap(m_pParent, other.m_pParent);

	swap(m_pRoot, other.m_pRoot);

	for (int n = 0; n < 8; n++)
		swap(m_pChild[n], other.m_pChild[n]);
}

float MyOctant::GetSize(void) 
{
	return m_fSize;
}

vector3 MyOctant::GetCenterGlobal(void) 
{
	return m_v3Center;
}

vector3 MyOctant::GetMinGlobal(void) 
{
	return m_v3Min;
}

vector3 MyOctant::GetMaxGlobal(void)  
{
	return m_v3Max;
}

bool MyOctant::IsColliding(uint a_uRBIndex) 
{
	// not colliding if index not in range
	if (a_uRBIndex >= m_pEntityMngr->GetEntityCount())
		return false;

	// get global min and max
	MyRigidBody* myRigidBody = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();
	vector3 v3MinG = myRigidBody->GetMinGlobal();
	vector3 v3MaxG = myRigidBody->GetMaxGlobal();

	// check axis-aligned bounding boxes
	// x axis
	if (m_v3Max.x < v3MinG.x)
		return false;
	if (m_v3Min.x > v3MaxG.x)
		return false;

	// y axis
	if (m_v3Max.y < v3MinG.y)
		return false;
	if (m_v3Min.y > v3MaxG.y)
		return false;

	// z axis
	if (m_v3Max.z < v3MinG.z)
		return false;
	if (m_v3Min.z > v3MaxG.z)
		return false;

	return true;
}

void MyOctant::Display(uint a_nIndex, vector3 a_v3Color) 
{
	// if ID is current octant
	if (m_uID == a_nIndex)
	{
		// render cube
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
		return;
	}

	// else search children for ID
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_nIndex);
	}
}

void MyOctant::Display(vector3 a_v3Color)  
{
	// display children
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->Display(a_v3Color);

	// render cube
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void MyOctant::DisplayLeafs(vector3 a_v3Color) 
{
	// display children
	for (int i = 0; i < m_lChild.size(); i++)
		m_lChild[i]->DisplayLeafs(a_v3Color);

	// render cube
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void MyOctant::ClearEntityList(void)
{
	// recursively clear entities from children
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->ClearEntityList();

	// clear list
	m_EntityList.clear();
}

void MyOctant::Subdivide(void)  
{
	// base case: return if hit max level
	if (m_uLevel >= m_uMaxLevel)
		return;

	// edge case: return if octant already subdivided
	if (m_uChildren != 0)
		return;

	// set children count
	m_uChildren = 8;

	// half size of octant
	float fSize = m_fSize / 2.0f;

	// center of children
	vector3 v3Center;

	// left bottom back
	v3Center = m_v3Center;
	v3Center.x -= m_fSize / 4.0f;
	v3Center.y -= m_fSize / 4.0f;
	v3Center.z -= m_fSize / 4.0f;
	m_pChild[0] = new MyOctant(v3Center, fSize);

	// right bottom back
	v3Center.x += fSize;
	m_pChild[1] = new MyOctant(v3Center, fSize);

	// right bottom front
	v3Center.z += fSize;
	m_pChild[2] = new MyOctant(v3Center, fSize);

	// left bottom front
	v3Center.x -= fSize;
	m_pChild[3] = new MyOctant(v3Center, fSize);

	// left top front
	v3Center.y += fSize;
	m_pChild[4] = new MyOctant(v3Center, fSize);

	// left top back
	v3Center.z -= fSize;
	m_pChild[5] = new MyOctant(v3Center, fSize);

	// right top back
	v3Center.x += fSize;
	m_pChild[6] = new MyOctant(v3Center, fSize);

	// right top front
	v3Center.z += fSize;
	m_pChild[7] = new MyOctant(v3Center, fSize);

	// set children properties
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;

		// subdivide child
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
			m_pChild[i]->Subdivide();
	}
}

MyOctant* MyOctant::GetChild(uint a_nChild)  
{
	// edge case: invalid child
	if (a_nChild > 7)
		return nullptr;

	return m_pChild[a_nChild];
}

MyOctant* MyOctant::GetParent(void)  
{
	return m_pParent;
}

bool MyOctant::IsLeaf(void)
{
	return (m_uChildren == 0);
}

bool MyOctant::ContainsMoreThan(uint a_nEntities)  
{
	// count
	int n = 0;

	// count if there are more entities in octant than parameter
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		if (IsColliding(i))
			n++;
		if (n > a_nEntities)
			return true;
	}

	return false;
}

void MyOctant::KillBranches(void)  
{
	// kill children and children's children
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->KillBranches(); // recursively destroys the lineage
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}

	// reset children count
	m_uChildren = 0;
}

void MyOctant::ConstructTree(uint a_nMaxLevel) 
{
	// do not construct tree if octant is not root
	if (m_uLevel != 0)
		return;

	// set octant properties
	m_uOctantCount = 1;
	m_uMaxLevel = a_nMaxLevel;
	
	// reset entity list
	ClearEntityList();

	// reset children
	KillBranches();
	m_lChild.clear();

	// create children
	if (ContainsMoreThan(m_uIdealEntityCount))
		Subdivide();

	// assign ids
	AssignIDtoEntity();

	// create list of children
	ConstructList();
}

void MyOctant::AssignIDtoEntity(void)  
{
	// recursively assign ids to children
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->AssignIDtoEntity();

	// base case: if this octant has no children, assign id
	if (m_uChildren == 0)
	{
		for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			if (IsColliding(i))
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}

uint MyOctant::GetOctantCount(void) 
{
	return m_uOctantCount;
}

void Simplex::MyOctant::Update(void)
{
	ConstructTree(m_uMaxLevel);
}

void MyOctant::Release(void)
{
	// if this is the base octant kill branches
	if (m_uLevel == 0)
		KillBranches();

	// reset values
	m_uChildren = 0;
	m_fSize = 0.0f;

	// reset lists
	ClearEntityList();
	m_lChild.clear();
}

void MyOctant::Init(void)
{
	// initialize variables
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pParent = nullptr;
	for (int i = 0; i < 8; i++)
		m_pChild[i] = nullptr;

	m_pRoot = nullptr;
}

void MyOctant::ConstructList(void)
{
	// construct children lists
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->ConstructList();

	// add child to list
	if (m_EntityList.size() > 0)
		m_pRoot->m_lChild.push_back(this);
}