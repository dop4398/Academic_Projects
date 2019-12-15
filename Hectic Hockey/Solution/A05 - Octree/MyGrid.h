#pragma once
/*----------------------------------------------
Programmer: Julianna Apicella (jea3348@rit.edu)
Date: 2019/11
----------------------------------------------*/
#ifndef __MYGRIDCLASS_H_
#define __MYGRIDCLASS_H_

#include "MyEntityManager.h"

using namespace std;

/// goal is to implement this: http://www.cs.ucf.edu/~jmesit/publications/scsc%202005.pdf

namespace Simplex
{ //System Class
class MyGrid
{
	map<uint, vector<uint>> m_dBins; // hash table to store location and a list of contents

	uint m_uCellSize; // size of cell
	float m_fMin; // min corner of grid
	float m_fMax; // max corner of grid

	uint m_uWidth; // dimension of grid
	uint m_uBins; // number of cells in grid

	MeshManager* m_pMeshMngr = nullptr;//Mesh Manager Singleton
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton

	float m_fCF; // conversion factor for hashing

	bool m_bVisible; //whether or not grid is visible

public:
	MyGrid(uint a_nCellSize, float a_fMin, float a_fMax);
	MyGrid(MyGrid const& other);
	MyGrid& operator=(MyGrid const& other);
	~MyGrid(void);
	void Swap(MyGrid& other);
	void HashEntities(void);
	void EmptyBins(void);
	vector<uint> GetEntitiesbyCellId(uint a_nId);
	void Print(void);
	void Display(vector3 a_v3Color = C_VIOLET);
	void SetVisible(bool a_bVisible);
private:
	void Release(void);
	void Init(void);
	void CalculateBin(uint a_uRBIndex);
}; //class

} //namespace Simplex

#endif //__GRIDCLASS_H_
