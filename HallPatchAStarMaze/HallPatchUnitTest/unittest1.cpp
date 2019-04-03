#include "stdafx.h"
#include "CppUnitTest.h"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Function declarations
__declspec(dllimport) char* GetTeam();
__declspec(dllimport) bool SetMaze(const int** data, int width, int height);
__declspec(dllimport) int** GetMaze(int& width, int& height);
__declspec(dllimport) bool GetNextPosition(int& xpos, int& ypos);
__declspec(dllimport) bool SetStart(int xpos, int ypos);
__declspec(dllimport) bool GetStart(int& xpos, int& ypos);
__declspec(dllimport) bool SetEnd(int xpos, int ypos);
__declspec(dllimport) bool GetEnd(int& xpos, int& ypos);
__declspec(dllimport) bool Restart();
__declspec(dllimport) bool MazeDestructor();


namespace HallPatchUnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestGetTeam) // GetTeam()
		{
			bool ret = true;
			char *team;
			team = GetTeam();
			char names[] = "Adam Hall, David Patch";
			if (*team != *names)
			{
				ret = false;
			}
			Assert::IsTrue(ret);
		}

		TEST_METHOD(TestSetMazeWidth) // SetMaze(...) Width
		{
			bool ret = false;
			int* height = new int;
			int h = 10;
			height = &h;
			int* width = new int;
			int w = 10;
			width = &w;

			int** data;
			data = new int*[*width];
			for (int i = 0; i < *width; i++)
			{
				data[i] = new int[*height];
			}
			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					data[i][j] = 1;
				}
			}

			ret = SetMaze((const int **)data, *width, *height);

			Assert::IsTrue(ret);
		}

		TEST_METHOD(TestSetMazeHeight) // SetMaze(...) Height
		{
			bool ret = false;
			int* height = new int;
			int h = 10;
			height = &h;
			int* width = new int;
			int w = 10;
			width = &w;

			int** data;
			data = new int*[*width];
			for (int i = 0; i < *width; i++)
			{
				data[i] = new int[*height];
			}
			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					data[i][j] = 1;
				}
			}

			ret = SetMaze((const int **)data, *width, *height);

			Assert::IsTrue(ret);
		}

		TEST_METHOD(TestGetMaze) // GetMaze(...)
		{
			bool ret = true;
			int* height = new int;
			int h = 10;
			height = &h;
			int* width = new int;
			int w = 10;
			width = &w;

			int** data;
			data = new int*[*width];
			for (int i = 0; i < *width; i++)
			{
				data[i] = new int[*height];
			}
			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					data[i][j] = 1;
				}
			}
			
			SetMaze((const int**)data, *width, *height);
			int** map = GetMaze(*width, *height);

			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					if (data[i][j] != map[i][j])
					{
						ret = false;
					}
				}
			}

			Assert::IsTrue(ret);
		}

		TEST_METHOD(TestGetNextPosition) // GetNextPosition(...)
		{
			bool ret = false;
			int* height = new int;
			int h = 10;
			height = &h;
			int* width = new int;
			int w = 10;
			width = &w;
			int* x = new int;
			int* y = new int;

			int** data;
			data = new int*[*width];
			for (int i = 0; i < *width; i++)
			{
				data[i] = new int[*height];
			}
			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					data[i][j] = 0;
				}
			}

			SetMaze((const int**)data, *width, *height);

			SetStart(1, 1);
			SetEnd(5, 5);

			ret = GetNextPosition(*x, *y);

			Assert::IsTrue(ret);
		}

		TEST_METHOD(TestSetStartBadX) // SetStart(...) bad X
		{
			bool ret = true;
			int* x = new int;
			int* y = new int;
			ret = SetStart(-1, 4);
			GetStart(*x, *y);

			Assert::IsFalse(ret);
			delete x;
			delete y;
		}

		TEST_METHOD(TestSetStartBadY) // SetStart(...) bad Y
		{
			bool ret = true;
			int* x = new int;
			int* y = new int;
			ret = SetStart(2, -1);
			GetStart(*x, *y);

			Assert::IsFalse(ret);
			delete x;
			delete y;
		}

		TEST_METHOD(TestGetStart) // GetStart(...)
		{
			bool ret = false;
			int* height = new int;
			int h = 10;
			height = &h;
			int* width = new int;
			int w = 10;
			width = &w;

			int** data;
			data = new int*[*width];
			for (int i = 0; i < *width; i++)
			{
				data[i] = new int[*height];
			}
			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					data[i][j] = 0;
				}
			}

			SetMaze((const int**)data, *width, *height);

			int x;
			int y;
			SetStart(1, 1);
			SetEnd(5, 5);

			ret = GetStart(x, y);

			Assert::IsTrue(ret);
		}

		TEST_METHOD(TestSetEndBadX) // SetEnd() bad X
		{
			bool ret = true;
			int* x = new int;
			int* y = new int;
			ret = SetEnd(-1, 4);
			GetEnd(*x, *y);

			Assert::IsFalse(ret);
		}

		TEST_METHOD(TestSetEndBadY) // SetEnd() bad Y
		{
			bool ret = true;
			int* x = new int;
			int* y = new int;
			ret = SetEnd(2, -1);
			GetEnd(*x, *y);

			Assert::IsFalse(ret);
		}

		TEST_METHOD(TestGetEnd) // GetEnd()
		{
			bool ret = true;
			
			int* height = new int;
			int h = 10;
			height = &h;
			int* width = new int;
			int w = 10;
			width = &w;

			int** data;
			data = new int*[*width];
			for (int i = 0; i < *width; i++)
			{
				data[i] = new int[*height];
			}
			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					data[i][j] = 1;
				}
			}

			SetMaze((const int**)data, *width, *height);

			int* x = new int;
			int* y = new int;
			SetStart(0, 0);
			SetEnd(1, 1);
			GetEnd(*x, *y);
			if (*x != 1 || *y != 1)
			{
				ret = false;
			}

			Assert::IsTrue(ret);
			delete x;
			delete y;
		}

		TEST_METHOD(TestRestart) // Restart()
		{
			bool ret = false;
			int* height = new int;
			int h = 10;
			height = &h;
			int* width = new int;
			int w = 10;
			width = &w;
			int* x = new int;
			int* y = new int;

			int** data;
			data = new int*[*width];
			for (int i = 0; i < *width; i++)
			{
				data[i] = new int[*height];
			}
			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					data[i][j] = 0;
				}
			}

			SetMaze((const int**)data, *width, *height);

			SetStart(1, 1);
			SetEnd(5, 5);
			bool atEnd = false;

			while (!atEnd)
			{
				atEnd = GetNextPosition(*x, *y);
			}
			
			ret = Restart();

			Assert::IsTrue(ret);
		}

		TEST_METHOD(TestMazeDtr) // MazeDestructor()
		{
			bool ret = false;
			int* height = new int;
			int h = 10;
			height = &h;
			int* width = new int;
			int w = 10;
			width = &w;
			int* x = new int;
			int* y = new int;

			int** data;
			data = new int*[*width];
			for (int i = 0; i < *width; i++)
			{
				data[i] = new int[*height];
			}
			for (int i = 0; i < *height; i++)
			{
				for (int j = 0; j < *width; j++)
				{
					data[i][j] = 0;
				}
			}

			SetMaze((const int**)data, *width, *height);

			SetStart(1, 1);
			SetEnd(5, 5);

			ret = MazeDestructor();

			Assert::IsTrue(ret);
		}
	};
}