#include "Map.h"



Map::Map()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			map[i][0] = '|'; //Cột 0
			map[i][18] = '|'; //Cột cuối cùng 
			map[i][j] = ' '; //Khoảng trắng
		}
	}
}


Map::~Map()
{
}

void Map::veMap()
{
	// Print map
	//MoveCursorToRoot();
	cout << "					";
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{

			cout << map[i][j];
			if (j >= 19)
				cout << endl << "					";
		}
	}
}