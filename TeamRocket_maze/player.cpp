#pragma once
#include <conio.h>
#include"generate.cpp"
// deklaration von makros für präprozessor

#define KEY_UP 72 
#define KEY_DOWN 80 
#define KEY_LEFT 75 
#define KEY_RIGHT 77 

#define KEY_W 'w'
#define KEY_A 'a'
#define KEY_S 's'
#define KEY_D 'd'

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


using std::pair;

/*
* start und ende erkennen durch suchen von 8 und 9 im pointer
* 
* 
* Der spieler soll nach oben, untern, links und rechts gehen können
* Er darf nicht auf mauern treten
* Das Spiel endet sobald er auf das ziel tritt
* automatic: er hinterlässt eine spur




	Was jetzt
	die move funktion seperieren so dass automatik und manuel jeweils drauf zugreiden können
*/

class player
{
public:


	// construktor | Parameter als wert übergeben
	player(generate maze) :maze(maze) {}

	bool manuel() {
		// überprüft ob dass easter egg aktiviert ist
		if (x == maze.egg_position[0] && y == maze.egg_position[1])
		{
			egg_pickup = true;
		}
		if (egg_pickup == true && x == maze.startpoint[0] && y == maze.startpoint[1])
		{
			easter_egg = true;
		}

		// überprüfen ob er sich auf ende befindet
		if (x == maze.endpoint[0] && y == maze.endpoint[1])
		{
			
			return true;
		}

		// schritt
		cout << "Machen Sie einen Schritt\n";
		switch (input = _getch())
		{
		// Hoch
		case KEY_UP:
			if (can_move(x, y - 1) == true)
			{
				move(x, y - 1);
			}
			break;
		case KEY_W:
			if (can_move(x, y - 1) == true)
			{
				move(x, y - 1);
			}
			break;

		// Runter
		case KEY_DOWN:
			if (can_move(x, y + 1) == true)
			{
				move(x, y + 1);
			}
			break;
		case KEY_S:
			if (can_move(x, y + 1) == true)
			{
				move(x, y + 1);
			}
			break;

		// Links
		case KEY_LEFT:
			if (can_move(x - 1, y) == true)
			{
				move(x - 1, y);
			}
			break;
		case KEY_A:
			if (can_move(x - 1, y) == true)
			{
				move(x - 1, y);
			}
			break;

		//Rechts
		case KEY_RIGHT:
			if (can_move(x + 1, y) == true)
			{
				move(x + 1, y);
			}
			break;
		case KEY_D:
			if (can_move(x + 1, y) == true)
			{
				move(x + 1, y);
			}
			break;

		// bei anderer Eingabe
		default:
			break;
		}

		return false;
	}
	


	void automatic() {

		int x = maze.startpoint[0];
		int y = maze.startpoint[1];
		int endX = maze.endpoint[0];
		int endY = maze.endpoint[1];
		vector<int> directions;


		stack<tuple<int, int, int, int, vector<int>>> path_stack;
		path_stack.push(make_tuple(x, y, endX, endY, vector<int>()));
		while (endpoint_reached == false) {



			tie(x, y, endX, endY, directions) = path_stack.top();
			path_stack.pop();

			// auf aktuelle Position "4" setzten und die Position in "breadcrums" abspeichern
			*(maze.maze + (x)+((y)* maze.row)) = 4;
			breadcrums.push_back(make_pair(x, y));

			//überprüft ob er auf oder neben dem Ziel steht
			if (maze.is_goal(x, y)) { // || maze.near_goal(x, y)
				endpoint_reached = true;
				maze.print_maze(get_coordinates(),get_move_count(),get_egg_status());
				*(maze.maze + (maze.startpoint[0]) + (maze.startpoint[1]) * maze.row) = 8;
				maze.print_maze(get_coordinates(), get_move_count(), get_egg_status());
				*(maze.maze + (maze.endpoint[0]) + (maze.endpoint[1]) * maze.row) = 9;
				maze.print_maze(get_coordinates(), get_move_count(), get_egg_status());
				return;
			}


			step++;


			// mögliche Richtungen werden überpfüft
			vector<int> possible_directions;
			// überprüft ob er hoch gehen kann
			if (can_move(x, y - 1) == true) {
				possible_directions.push_back(UP);
			}

			// überprüft ob er herunter gehen kann
			if (can_move(x, y + 1) == true) {
				possible_directions.push_back(DOWN);
			}

			// überprüft ob er links gehen kann
			if (can_move(x - 1, y) == true) {
				possible_directions.push_back(LEFT);
			}

			// überprüft ob er rechts gehen kann
			if (can_move(x + 1, y) == true) {
				possible_directions.push_back(RIGHT);
			}
			if (!possible_directions.empty()) {
				int randomIndex = rand() % (possible_directions.size());
				int direction = possible_directions[randomIndex];

				int newX, newY;
				switch (direction) {
				case UP:
					newX = x;
					newY = y - 1;
					break;
				case DOWN:
					newX = x;
					newY = y + 1;
					break;
				case LEFT:
					newX = x - 1;
					newY = y;
					break;
				case RIGHT:
					newX = x + 1;
					newY = y;
					break;
				}
				path_stack.push(make_tuple(newX, newY, endX, endY, possible_directions));
			}
			else {
				// er geht auf die koordinaten vom letztem schritt (tupel)
				pair<int, int> previous_position_coordinates = find_previous_cell();
				if (previous_position_coordinates.first == -1 && previous_position_coordinates.second == -1) {
					endpoint_reached = false;
					return;
				}
				path_stack.push(make_tuple(previous_position_coordinates.first, previous_position_coordinates.second, endX, endY, vector<int>()));
				
			}
		}
	}




	int get_finish() {
		return finish;
	}
	
	int get_move_count() {
		return step;
	}

	pair<int, int> get_coordinates() {
		return make_pair(x, y);
	}

	pair<bool, bool> get_egg_status() {
		return make_pair(easter_egg, egg_pickup);
	}


private:

	generate maze;
	int x = maze.startpoint[0];
	int y = maze.startpoint[1];
	int input;
	int step = 0;
	bool finish = false;
	bool easter_egg = false;
	bool egg_pickup = false;
	bool endpoint_reached = false;
	vector<pair<int, int>> breadcrums;
	
	bool can_move(int px, int py) {
		if (*(maze.maze + (px)+(py) * maze.row) == 1 || *(maze.maze + (px)+(py)*maze.row) == 4)
		{
			return false;
		}
		return true;
	}

	void move(int px, int py) {
		*(maze.maze + (x)+(y)*maze.row) = 0;
		*(maze.maze + (maze.startpoint[0]) + (maze.startpoint[1]) * maze.row) = 8;
		*(maze.maze + (px)+(py)*maze.row) = 5;

		x = px;
		y = py;

		step++;
	}

	pair<int, int> find_previous_cell() {
		if (breadcrums.empty())
		{
			return make_pair(-1, -1);
		}
		breadcrums.erase(breadcrums.begin() + breadcrums.size() - 1);
		if (breadcrums.empty())
		{
			return make_pair(-1, -1);
		}
		pair<int, int> previous_position = breadcrums[breadcrums.size() - 1];
		breadcrums.erase(breadcrums.begin() + breadcrums.size() - 1);
		return previous_position;
	}

};

