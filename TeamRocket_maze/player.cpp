#pragma once
#include <conio.h>
#include"generate.cpp"
// deklaration von makros für präprozessor
#define NORTH 0
#define WEST 1
#define SOUTH 2
#define EAST 3

#define WALK 'w'
#define TURN 'q'





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
	int direction = 0;



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
		
		case WALK:
			if (direction == 0 && can_move(x, y - 1) == true) {
				move(x, y - 1);
			}
			else if (direction == 1 && can_move(x - 1, y) == true) {
				move(x - 1, y);
			}
			else if (direction == 2 && can_move(x, y + 1) == true) {
				move(x, y + 1);
			}
			else if (direction == 3 && can_move(x + 1, y) == true) {
				move(x + 1, y);
			}
			step++;
			break;

		case TURN:
			if (direction == 3) {
				direction = 0;
			}
			else {
				direction++;
			}
			step++;
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
			*(maze.maze + (x)+((y)*maze.row)) = 4;
			breadcrums.push_back(make_pair(x, y));

			//überprüft ob er auf dem Ziel steht und ob der kürzeste weg erreicht ist
			if (maze.is_goal(x, y) == true) {

				find_shortest_way(); 
				if (fast_way == false) {
					x = maze.startpoint[0];
					y = maze.startpoint[1];
					breadcrums.clear();
					for (int u = 0; u < maze.row * maze.col; u++) {
						if (*(maze.maze + u) == 4 || *(maze.maze + u) == 3) {
							*(maze.maze + u) = 0;
						}
					}
				}
				if (fast_way == true) {
					endpoint_reached = true;

					maze.print_maze(get_coordinates(), get_move_count(), get_egg_status(), 0);
					*(maze.maze + (maze.startpoint[0]) + (maze.startpoint[1]) * maze.row) = 8;
					maze.print_maze(get_coordinates(), get_move_count(), get_egg_status(), 0);
					*(maze.maze + (maze.endpoint[0]) + (maze.endpoint[1]) * maze.row) = 9;
					maze.print_maze(get_coordinates(), get_move_count(), get_egg_status(), 0);
					return;
				}

			}


			// mögliche directions werden überpfüft
			vector<int> possible_directions;

			direction = 0; // 0 soll Norden darstellen 1 Westen, 2 Süden, 3 Osten

			for (int i = 0; i < 4; i++) {

				// überprüft ob er nach Norden gehen kann
				if (direction == 0 && can_move(x, y - 1) == true) {
					possible_directions.push_back(NORTH);
				}

				// überprüft ob er nach Westen gehen kann
				if (direction == 1 &&  can_move(x - 1, y) == true) {
					possible_directions.push_back(WEST);
				}

				// überprüft ob er nach Süden gehen kann
				if (direction == 2 &&  can_move(x, y + 1) == true) {
					possible_directions.push_back(SOUTH);
				}

				// überprüft ob er nach Osten gehen kann
				if (direction == 3 &&  can_move(x + 1, y) == true) {
					possible_directions.push_back(EAST);
				}

				if (direction == 3) {
					direction = 0;
				}
				else {
					direction++;
				}
				step++;
			}
	
			// sucht eine zufällige Richtung von den möglichen aus und geht einen Schritt
			if (!possible_directions.empty()) {
				int randomIndex = rand() % (possible_directions.size());
				int direction = possible_directions[randomIndex];

				int newX, newY;
				switch (direction) {
				case NORTH: // Norden
					while (!direction == NORTH)
					{
						direction++;
					}
					newX = x;
					newY = y - 1; 
					break;
				case WEST: // Westen
					while (!direction == WEST)
					{
						direction++;
					}
					newX = x - 1;
					newY = y;
					break;
				case SOUTH: // Süden
					while (!direction == SOUTH)
					{
						direction++;
					}
					newX = x;
					newY = y + 1; 
					break;
				case EAST: // Osten
					while (!direction == EAST)
					{
						direction++;
					}
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
	vector<pair<int, int>> breadcrums_save;
	bool fast_way = false;
	int count_fast = 0;
	bool only_once = true;

	bool can_move(int px, int py) {
		if (*(maze.maze + (px)+(py)*maze.row) == 1 || *(maze.maze + (px)+(py)*maze.row) == 4)
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
		if (breadcrums.empty()) {
			return make_pair(-1, -1);
		}
		breadcrums.erase(breadcrums.begin() + breadcrums.size() - 1);
		if (breadcrums.empty()) {
			return make_pair(-1, -1);
		}
		pair<int, int> previous_position = breadcrums[breadcrums.size() - 1];
		breadcrums.erase(breadcrums.begin() + breadcrums.size() - 1);
		return previous_position;
	}

	void find_shortest_way() {
		pair<int, int> previous_position;
		if (only_once == true)
		{
			for (int i = 0; i < breadcrums.size(); i++) { 
				breadcrums_save.push_back(breadcrums[i]);
			}
			only_once = false;
		}
		count_fast++;
		if (count_fast <= 500) {
			if (breadcrums_save >= breadcrums) {
				breadcrums_save.clear();
				for (int i = 0; i < breadcrums.size(); i++) {
					breadcrums_save.push_back(breadcrums[i]);
				}
			}
			fast_way = false;
			return;
		}
		step = 0;

		fast_way = true;

		do {


			previous_position = breadcrums_save[breadcrums_save.size() -1];
			breadcrums_save.erase(breadcrums_save.begin() + breadcrums_save.size() -1);

			*(maze.maze + (previous_position.first) + ((previous_position.second) * maze.row)) = 3;
			step++;

		} while (!breadcrums_save.empty());
		
		return;	
	}
};