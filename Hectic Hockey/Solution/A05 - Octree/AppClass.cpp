#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)
	
	uint uinstances = 4;
	int nsquare = static_cast<int>(std::sqrt(uinstances));
	m_uObjects = nsquare * nsquare;
	uint uindex = -1;
	for (int i = 0; i < nsquare; i++)
	{
		for (int j = 0; j < nsquare; j++)
		{
			uindex++;
			m_pEntityMngr->AddEntity("HockeyModels\\Puck.obj");
			vector3 v3position = vector3(glm::sphericalRand(34.0f));
			v3position.z = 0;
			matrix4 m4position = glm::translate(v3position);
			m_pEntityMngr->SetModelMatrix(m4position);
			m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
			m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
			m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
			m_pEntityMngr->GetEntity(-1)->tag = Puck;
		}
	}

	// Creeper Entity Creation
	m_pEntityMngr->AddEntity("HockeyModels\\RedPlayer.obj", "Player1");
	vector3 v3position = vector3(40,0,0);
	matrix4 m4position = glm::translate(v3position);
	m_pEntityMngr->SetModelMatrix(m4position);
	// New variables being set
	m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
	m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
	m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
	m_pEntityMngr->GetEntity(-1)->tag = Player;

	// Steve Entity Creation
	m_pEntityMngr->AddEntity("HockeyModels\\BluePlayer.obj", "Player2");
	v3position = vector3(-40,0,0);
	m4position = glm::translate(v3position);
	m_pEntityMngr->SetModelMatrix(m4position);
	// New variables being set
	m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
	m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
	m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
	m_pEntityMngr->GetEntity(-1)->tag = Player;

	// Goal 1 Entity Creation
	m_pEntityMngr->AddEntity("HockeyModels\\Goal.obj");
	v3position = vector3(50, 0, 0);
	m4position = glm::translate(v3position);
	m_pEntityMngr->SetModelMatrix(m4position);
	// New variables being set
	m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
	m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
	m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
	m_pEntityMngr->GetEntity(-1)->tag = Goal;
	m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleOBB(true);
	m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleBS(false);

	// Goal 2 Entity Creation
	m_pEntityMngr->AddEntity("HockeyModels\\Goal.obj");
	v3position = vector3(-50, 0, 0);
	m4position = glm::translate(v3position);
	m_pEntityMngr->SetModelMatrix(m4position);
	// New variables being set
	m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
	m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
	m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
	m_pEntityMngr->GetEntity(-1)->tag = Goal;
	m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleOBB(true);
	m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleBS(false);

	// Top Left Wall
	for (int i = 15; i < 45; i += 10)
	{
		m_pEntityMngr->AddEntity("HockeyModels\\Wall.obj");
		v3position = vector3(-45, i, 0);
		m4position = glm::translate(v3position) * glm::rotate(IDENTITY_M4, glm::radians(90.0f), AXIS_Z);
		m_pEntityMngr->SetModelMatrix(m4position);
		// New variables being set
		m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
		m_pEntityMngr->GetEntity(-1)->tag = Wall;
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleOBB(false);
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleBS(false);
	}

	// Bottom Left Wall
	for (int i = -15; i > -45; i -= 10)
	{
		m_pEntityMngr->AddEntity("HockeyModels\\Wall.obj");
		v3position = vector3(-45, i, 0);
		m4position = glm::translate(v3position) * glm::rotate(IDENTITY_M4, glm::radians(90.0f), AXIS_Z);
		m_pEntityMngr->SetModelMatrix(m4position);
		// New variables being set
		m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
		m_pEntityMngr->GetEntity(-1)->tag = Wall;
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleOBB(false);
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleBS(false);
	}
	
	// Top Wall
	for (int i = -35; i < 45; i += 10)
	{
		m_pEntityMngr->AddEntity("HockeyModels\\Wall.obj");
		v3position = vector3(i, 45, 0);
		m4position = glm::translate(v3position);
		m_pEntityMngr->SetModelMatrix(m4position);
		// New variables being set
		m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
		m_pEntityMngr->GetEntity(-1)->tag = Wall;
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleOBB(false);
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleBS(false);
	}

	// Top Right Wall
	for (int i = 15; i < 45; i += 10)
	{
		m_pEntityMngr->AddEntity("HockeyModels\\Wall.obj");
		v3position = vector3(45, i, 0);
		m4position = glm::translate(v3position) * glm::rotate(IDENTITY_M4, glm::radians(90.0f), AXIS_Z);
		m_pEntityMngr->SetModelMatrix(m4position);
		// New variables being set
		m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
		m_pEntityMngr->GetEntity(-1)->tag = Wall;
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleOBB(false);
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleBS(false);
	}

	// Bottom Right Wall
	for (int i = -15; i > -45; i -= 10)
	{
		m_pEntityMngr->AddEntity("HockeyModels\\Wall.obj");
		v3position = vector3(45, i, 0);
		m4position = glm::translate(v3position) * glm::rotate(IDENTITY_M4, glm::radians(90.0f), AXIS_Z);
		m_pEntityMngr->SetModelMatrix(m4position);
		// New variables being set
		m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
		m_pEntityMngr->GetEntity(-1)->tag = Wall;
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleOBB(false);
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleBS(false);
	}

	// Bottom Wall
	for (int i = -35; i < 45; i += 10)
	{
		m_pEntityMngr->AddEntity("HockeyModels\\Wall.obj");
		v3position = vector3(i, -45, 0);
		m4position = glm::translate(v3position);
		m_pEntityMngr->SetModelMatrix(m4position);
		// New variables being set
		m_pEntityMngr->GetEntity(-1)->currentPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->previousPosition = v3position;
		m_pEntityMngr->GetEntity(-1)->velocity = vector3(0);
		m_pEntityMngr->GetEntity(-1)->tag = Wall;
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleOBB(false);
		m_pEntityMngr->GetEntity(-1)->GetRigidBody()->SetVisibleBS(false);
	}

	m_pGrid = new MyGrid(20, 0, 80);
	m_pEntityMngr->Update();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	// Move Player1
	m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player1"))->velocity = m_v3Creeper;

	// Move Player2
	if (m_bShowBS)
	{
		m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player2"))->debug = true;
		m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player1"))->debug = true;
	}
	else
	{
		m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player2"))->debug = false;
		m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player1"))->debug = false;
	}

	m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player2"))->velocity = m_v3Steve;

	//Update Entity Manager
	m_pEntityMngr->Update();

	m_pGrid->HashEntities();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

	// Set previous positions of players
	//m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player1"))->previousPosition = m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player1"))->currentPosition;
	//m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player2"))->previousPosition = m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Player2"))->currentPosition;

	m_v3Creeper = vector3(0);
	m_v3Steve = vector3(0);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	m_pGrid->Display();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pGrid);

	//release the model
	SafeDelete(m_pCreeper);

	//release the rigid body for the model
	SafeDelete(m_pCreeperRB);

	//release the model
	SafeDelete(m_pSteve);

	//release the rigid body for the model
	SafeDelete(m_pSteveRB);

	//release GUI
	ShutdownGUI();
}