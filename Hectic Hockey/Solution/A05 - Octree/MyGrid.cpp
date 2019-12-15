#include "MyGrid.h"

using namespace Simplex;

/// goal is to implement this: http://www.cs.ucf.edu/~jmesit/publications/scsc%202005.pdf

MyGrid::MyGrid(uint a_nCellSize, float a_fMin, float a_fMax)
{
	// initialize values
	Init();

	// set properties
	m_uCellSize = a_nCellSize;
	m_fMin = a_fMin;
	m_fMax = a_fMax;

	// calculate properties
	m_uWidth = (m_fMax - m_fMin) / m_uCellSize;
	m_uBins = m_uWidth * m_uWidth;
	m_fCF = 1.0f / m_uCellSize;

	// hash entities
	HashEntities();
}

MyGrid::MyGrid(MyGrid const& other)
{
	// copy properties
	m_dBins = other.m_dBins;

	m_uCellSize = other.m_uCellSize;
	m_fMin = other.m_fMin;
	m_fMax = other.m_fMax;

	m_uWidth = other.m_uWidth;
	m_uBins = other.m_uBins;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

MyGrid& MyGrid::operator=(MyGrid const& other)
{
	if (this != &other)
	{
		// format grid
		Release();
		Init();

		// copy and swap
		MyGrid temp(other);
		Swap(temp);
	}

	return *this;
}

MyGrid::~MyGrid(void)
{
	Release();
}

void MyGrid::Swap(MyGrid& other)
{
	// swap contents
	swap(m_dBins, other.m_dBins);

	swap(m_uCellSize, other.m_uCellSize);
	swap(m_fMin, other.m_fMin);
	swap(m_fMax, other.m_fMax);

	swap(m_uWidth, other.m_uWidth);
	swap(m_uBins, other.m_uBins);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

void MyGrid::HashEntities(void)
{
	// reset bins
	EmptyBins();

	// hash each entity
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		CalculateBin(i);
}

void MyGrid::EmptyBins(void)
{
	m_pEntityMngr->ClearDimensionSetAll();

	// iterate over map and clear bins
	map<uint, vector<uint>>::iterator it = m_dBins.begin();

	while (it != m_dBins.end())
	{
		it->second.clear();
		it++;
	}
}

vector<uint> MyGrid::GetEntitiesbyCellId(uint a_nId)
{
	return m_dBins[a_nId];
}

void MyGrid::Print(void)
{
	for (uint i = 0; i < m_uBins; i++) 
	{
		cout << "Contents of cell " << i  << ":"<< endl;
		for (uint j = 0; j < m_dBins[i].size(); j++)
			cout << m_dBins[i][j] << endl;
		cout << "" << endl;
	}
}

void MyGrid::Display(vector3 a_v3Color)
{
	if (m_bVisible) 
	{
		for (int i = 0; i < m_uWidth; i++)
		{
			for (int j = 0; j < m_uWidth; j++)
			{
				m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, vector3(-30 + (20 * j), -30 + (20 * i), 0)) * glm::scale(vector3(m_uCellSize)), a_v3Color, RENDER_WIRE);
			}
		}
	}
}

void Simplex::MyGrid::SetVisible(bool a_bVisible)
{
	m_bVisible = a_bVisible;
}

void MyGrid::Release(void)
{
	// empty grid
	m_uCellSize = 0;
	m_uBins = 0;

	EmptyBins();
}

void MyGrid::Init(void)
{
	// initialize values
	m_uWidth = 0;
	m_uBins = 0;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_bVisible = false;
}

void MyGrid::CalculateBin(uint a_uRBIndex)
{
	if (a_uRBIndex >= m_pEntityMngr->GetEntityCount())
		return;

	// get global max and min
	MyRigidBody* myRigidBody = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();
	vector3 v3CenterG = myRigidBody->GetCenterGlobal();
	vector3 v3MinG = myRigidBody->GetMinGlobal();
	vector3 v3MaxG = myRigidBody->GetMaxGlobal();

	// calculate bin id for max and min
	uint minX = v3MinG.x + 40.0f;
	minX *= m_fCF;
	uint minY = v3MinG.y + 40.0f;
	minY *= m_fCF;
	uint minId = 17;
	if (minX < 4 && minY < 4)
		minId = minX + (minY * m_uWidth);

	uint maxX = v3MaxG.x + 40.0f;
	maxX *= m_fCF;
	uint maxY = v3MaxG.y + 40.0f;
	maxY *= m_fCF;
	uint maxId = 17;
	if (maxX < 4 && maxY < 4)
		maxId = maxX + (maxY * m_uWidth);

	// same cell, add only one
	if (minId == maxId)
	{
		m_dBins[minId].push_back(a_uRBIndex);
		m_pEntityMngr->AddDimension(a_uRBIndex, minId);
	}
	// spans two cells, add to both
	else if (minX == maxX || minY == maxY) 
	{
		m_dBins[minId].push_back(a_uRBIndex);
		m_pEntityMngr->AddDimension(a_uRBIndex, minId);

		m_dBins[maxId].push_back(a_uRBIndex);
		m_pEntityMngr->AddDimension(a_uRBIndex, maxId);
	}
	// spans four cells, add to all
	else
	{
		// calculate other two cell ids
		uint minmaxId = 17;
		if (minX < 4 && maxY < 4)
			minmaxId = minX + (maxY * m_uWidth);

		uint maxminId = 17;
		if (maxX < 4 && minY < 4)
			maxminId = maxX + (minY * m_uWidth);

		m_dBins[minId].push_back(a_uRBIndex);
		m_pEntityMngr->AddDimension(a_uRBIndex, minId);

		m_dBins[maxId].push_back(a_uRBIndex);
		m_pEntityMngr->AddDimension(a_uRBIndex, maxId);

		m_dBins[minmaxId].push_back(a_uRBIndex);
		m_pEntityMngr->AddDimension(a_uRBIndex, minmaxId);

		m_dBins[maxminId].push_back(a_uRBIndex);
		m_pEntityMngr->AddDimension(a_uRBIndex, maxminId);
	}
}