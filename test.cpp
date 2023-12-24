#include <iostream>
#include <iomanip>
using namespace std;

void Fill(int size,int mass[][10])
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)		
			mass[i][j] = 0;
}

void Show(int size,int mass[][10])
{
	cout << endl << endl;
	cout << " ";
	for (int i = 0; i < 10; i++)
		cout << i << " ";

	cout << endl;
	for (int i = 0; i < size; i++)
	{
		cout << i;
		for (int j = 0; j < size; j++)
		{
			if (mass[j][i] == 0) cout << " |";
			else if (mass[j][i] == 1) cout << "o|";
			else if (mass[j][i] == 2) cout << (char)766  << "|";
		}
		cout << "\n";
	}
}

void Bot_ships(int size, int mass[][10], int size_ship, int num_ship)
{
	int x, y;
	int direct;

	direct = rand() % 2;
	for (int i = 0; i < num_ship; i++) {
		if (direct % 2 == 0)
		{
			switch (num_ship)
			{
			case 1:
				do
				{x = rand() % size;
				y = rand() % (size - (size_ship - 1));

				} while (mass[x][y] != 0);
				
				break;
			case 2:
				do
				{
					x = rand() % size;
					y = rand() % (size - (size_ship - 1));
				} while (mass[x][y] != 0 || mass[x][y + 1] != 0 || mass[x][y + 2] != 0); break;
			case 3:
				do
				{
					x = rand() % size;
					y = rand() % (size - (size_ship - 1));
				} while (mass[x][y] != 0 || mass[x][y + 1] != 0); break;
			case 4:
				do
				{
					x = rand() % size;
					y = rand() % (size - (size_ship - 1));
				} while (mass[x][y] != 0); break;
			}


				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + size_ship; j++)
						if (i >= 0 && i < size && j >= 0 && j < size)
							mass[i][j] = 2;

				for (int i = y; i < y + size_ship; i++)
					mass[x][i] = 1;
			
		}
		else
		{
			switch (num_ship)
			{
			case 1:

				do
				{
					x = rand() % size;
				y = rand() % (size - (size_ship - 1));
				} while (mass[x][y] != 0);
				break;
			case 2:
				do
				{
					x = rand() % size;
					y = rand() % (size - (size_ship - 1));
				} while (mass[x][y] != 0 || mass[x + 1][y] != 0 || mass[x + 2][y] != 0); break;
			case 3:
				do
				{
					x = rand() % size;
					y = rand() % (size - (size_ship - 1));
				} while (mass[x][y] != 0 || mass[x][y + 1] != 0); break;
			case 4:
				do
				{
					x = rand() % size;
					y = rand() % (size - (size_ship - 1));
				} while (mass[x][y] != 0); break;

			}		
			for (int i = x - 1; i <= x + size_ship; i++)
				for (int j = y - 1; j <= y + 1; j++)
					if (i >= 0 && i < size && j >= 0 && j < size)
						mass[i][j] = 2;

			for (int i = x; i < x + size_ship; i++) mass[i][y] = 1;
		}
	}
}



void User_ships(int size, int user[][10], int size_ship, int num_ship)
{
	for (int i = 0; i < num_ship; i++) {
		int direct = 0;
		int x, y;
		Show(size,user);
		do
		{
			
			cout << "Enter direct\n1-Vertical\n2-Horizontal\n";
			cin >> direct;
		} while (direct < 1 || direct > 2);

		if (direct % 2 == 0)
		{
			switch (num_ship)
			{
			case 1:
				do
				{
					cout << "Enter x\n"; cin >> x;
					cout << "Enter y\n"; cin >> y;
				} while (x<0 || x > size || y < 0 || y > 6); break;
			case 2:
				do
				{
					cout << "Enter x\n"; cin >> x;
					cout << "Enter y\n"; cin >> y;
				} while (user[x][y] != 0 || user[x][y + 1] != 0 || user[x][y + 2] != 0 || y > 7);
				break;
			case 3:
				do
				{
					cout << "Enter x\n"; cin >> x;
					cout << "Enter y\n"; cin >> y;
				} while (user[x][y] != 0 || user[x][y + 1] != 0 || y > 7);	break;
			case 4:
				do
				{
					cout << "Enter x\n"; cin >> x;
					cout << "Enter y\n"; cin >> y;
				} while (user[x][y] != 0);
				break;
			}
			for (int i = x - 1; i <= x + size_ship; i++)
				for (int j = y - 1; j <= y + 1; j++)
					if (i >= 0 && i < size && j >= 0 && j < size)
						user[i][j] = 2;

			for (int i = x; i < x + size_ship; i++) user[i][y] = 1;
			cout << endl << endl;
		}
		else
		{
			switch (num_ship)
			{
			case 1:do
			{
				
				cout << "Enter x\n"; cin >> x;
				cout << "Enter y\n"; cin >> y;
			} while (x < 0 || x > 6 || y < 0 || y > size); break;
			case 2:
				do
				{
					cout << "Enter x\n"; cin >> x;
					cout << "Enter y\n"; cin >> y;
				} while (user[x][y] != 0 || user[x + 1][y] != 0 || user[x + 2][y] != 0 || x > 7);
				break;
			case 3:
				do
				{
					cout << "Enter x\n"; cin >> x;
					cout << "Enter y\n"; cin >> y;
				} while (user[x][y] != 0 || user[x + 1][y] != 0 || x > 7);
				break;
			case 4:
				do
				{
					cout << "Enter x\n"; cin >> x;
					cout << "Enter y\n"; cin >> y;
				} while (user[x][y] != 0);
				break;
			}
			for (int i = x - 1; i <= x + 1; i++)
				for (int j = y - 1; j <= y + size_ship; j++)
					if (i >= 0 && i < size && j >= 0 && j < size)
						user[i][j] = 2;

			for (int i = y; i < y + size_ship; i++)
				user[x][i] = 1;
			

			Show(size, user);
			cout << endl << endl;
		}
	}
}

void mask_show( int map[][10],int mass[][10])
{
	cout << " ";
	for (int i = 0; i < 10; i++)
		cout << i << " ";

	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << i;
		for (int j = 0; j < 10; j++)
		{
			if (map[j][i] == 1) {
				if (mass[j][i] == 0) cout << " |";
				else if (mass[j][i] == 1) cout << "o|";
				else if (mass[j][i] == 2) cout << (char)766 << "|";
			}
			else
			{
				if (mass[j][i] == 0) cout << " |";
				else if (mass[j][i] == 1) cout << "o|";
				else if (mass[j][i] == 2) cout << (char)766 << "|";

			}
		}
		cout << "\n";
	}
}
void mask_show2(int map1[][10], int mass[][10])
{
	cout << " ";
	for (int i = 0; i < 10; i++)
		cout << i << " ";

	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << i;
		for (int j = 0; j < 10; j++)
		{
			if (map1[j][i] == 1) {
				if (mass[j][i] == 0) cout << " |";
				else if (mass[j][i] == 1) cout << "o|";
				else if (mass[j][i] == 2) cout << (char)766 << "|";
			}
			else
			{
				if (mass[j][i] == 0) cout << " |";
				else if (mass[j][i] == 1) cout << "o|";
				else if (mass[j][i] == 2) cout << (char)766 << "|";

			}
		}
		cout << "\n";
	}
}


int main()
{

	srand(time(NULL));
	int const size = 10; 
	int mass[size][size];
	int user[size][size];
	int map[size][size];
	int map1[size][size];
	int x, y;
	int user_hp = 20;
	int bot_hp = 20;
	
	Fill(size, mass);
	Fill(size, user);
	Fill(size, map);
	Fill(size, map1);
	

	Bot_ships(size,mass,4,1);
	Bot_ships(size,mass,3,2);
	Bot_ships(size,mass,2,3);
	Bot_ships(size,mass,1,4);
	
	
	User_ships(size, user, 4, 1);
	User_ships(size, user, 3, 2);
	User_ships(size, user, 2, 3);
	User_ships(size, user, 1, 4);
	Show(size, user);
	
	bool end = 1;
	while (end)
	{
	
		if (user_hp > 0 || bot_hp > 0) {
			bool turn = 1;
			bool turn2 = 1;

			while (turn)
			{

				cout << "Enter coordinates:  " << endl;
				cin >> x;
				cin >> y;

				if (mass[x][y] == 1) {
					cout << "Hit!" << endl;
					map[x][y] = 1;
					bot_hp--;
					mask_show(mass, map);
				}
				else {
					cout << "Missed!" << endl;
					map[x][y] = 2;
					mask_show(mass, map);
					turn = 0;
				}
			}
			system("pause");
			system("cls");
			
			while (turn2)
			{
				int x, y;
				x = rand() % size;
				y = rand() % size;

				if (user[x][y] == 1) {
					cout << "Bot hit" << endl;
					map1[x][y] = 1;
					cout << x << " " << y << endl;
					user_hp--;
					mask_show2(user, map1);
				}
				else {
					cout << "Bot missed" << endl;
					cout << x << " " << y << endl;
					map1[x][y] = 2;
					mask_show2(user, map1);
					turn2 = 0;
				}


			}	
			system("pause");
			system("cls");
			
		}
		else {
			if (user_hp <= 0) {
				cout << "Bot win" << endl;
				end = 0;
			}
			else {
				cout << "You win" << endl;
				end = 0;
			}

		}
	}
	
}