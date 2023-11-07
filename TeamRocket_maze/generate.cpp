#ifndef GENERATE_CPP
#define GENERATE_CPP
#include <iostream>
#include <vector>
#include <stack>
#include <tuple>


// deklaration von makros für präprozessor
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

using std::ofstream;
using std::cout;
using std::string;
using std::endl;
using std::pair;
using std::make_pair;
using std::vector;
using std::tuple;
using std::stack;


class generate
{
public:

	int* maze;
	int startpoint[2];
	int endpoint[2];
	int egg_position[2];
	int row;
	int col;
	
	// constructor von hier aus wir alles aufgebaut
	generate(int row, int col) :row(row), col(col) {  
	
		// c-alloc reserviert platz im arbeitsspeicher und setz den gesamten Reservierten berecih auf 0
		maze = /*die eingabe für maze wird in ein int* gewandelt(gecasted)*/(int*)calloc(row * col, sizeof(int));

		// path wird solange generiert bis er das Ziel gefunden hat
		while (possible == false)
		{
			startpoint[0] = random(2, row - 1) - 1;
			startpoint[1] = random(2, col - 1) - 1;
			endpoint[0] = random(2, row - 1) - 1;
			endpoint[1] = random(2, col - 1) - 1;
			egg_position[0] = random(2, row - 1) - 1;
			egg_position[1] = random(2, col - 1) - 1;
			breadcrums.clear();

			for (int i = 0; i < row * col; i++) {
				*(maze + i) = 1;
			}
			path(maze, row, col, startpoint[0], startpoint[1], endpoint[0], endpoint[1]);
			if (possible == true)
			{
				check_coverage(0.55); // überprüft dichte 
			}
			
		}

		*(maze + (startpoint[0]) + (startpoint[1]) *row) = 5;		
		*(maze + (endpoint[0]) + (endpoint[1]) * row) = 9;
	}

	// Reservierung im RAM aufheben
	void cleanup() {
		free(maze);
		delete this;
	}
	

	// Ausgabe von dem Spielfeld
	void print_maze(pair<int, int> coordinates, int steps, pair<bool, bool> easter_egg) {
		print_line();
		cout << "Spielfeld:\t" << "Koordineaten: " <<"(" << coordinates.first << "," << coordinates.second << ")\t" << "Zuege: " << steps ;
		if (easter_egg.second == true)
		{
			if (easter_egg.first == true)
			{
				cout << "\tEaster EGG: Gemeistert";
			}
			else
			{
				cout << "\tEaster EGG: Gefunden";
			}
		}
		cout << endl;
		print_line();

		for (int i = 0; i < col; i++)
		{
			for (int f = 0; f < row; f++)
			{
				switch (*(maze + (f) + (i) * row))
				{
				case 1:
					cout << char(219) << char(219);// cooler block im unicode
					break;
				case 4:
					cout << "<>";
					break;
				case 5:
					cout << ":P";
					break;
				case 8:
					cout << "SP";
					break;
				case 9:
					cout << "EP";
					break;
				default:
					cout << "  ";
					break;
				}				
			}
			cout << endl;
		}
	}


	// überprüft ob wegelement sich auf dem Ziel befindet
	bool is_goal(int x, int y) {
		if (x == endpoint[0] && y == endpoint[1]) { //(x == endpoint[0] && y == endpoint[1])
			return true;
		}
		return false;
	}

	// überprüft ob wegelement sich neben dem Ziel befindet
	bool near_goal(int x, int y) {

		// hoch
		if (is_goal(x, y - 1))
		{
			return true;
		}

		// herunter
		if (is_goal(x, y + 1))
		{
			return true;
		}

		// links
		if (is_goal(x - 1, y))
		{
			return true;
		}

		// rechts
		if (is_goal(x + 1, y))
		{
			return true;
		}
		return false;
	}


private:

	bool possible = false;
	bool coverage = false;
	bool egg = false;
	bool goal = false;
	
	vector<pair<int, int>> breadcrums;

	// Random 
	int random(int min, int max) //range : [min, max]
	{
		static bool first = true;
		if (first)
		{
			srand(time(NULL)); //seeding for the first time only!
			first = false;		// seeding entfernen
		}
		return min + rand() % ((max + 1) - min);
	}

	// Generierung des weges
	void path(int* maze, int rows, int cols, int startX, int startY, int endX, int endY) {
		stack<tuple<int, int, int, int, vector<int>>> path_stack;
		path_stack.push(make_tuple(startX, startY, endX, endY, vector<int>()));//

		while (!path_stack.empty()) {

			int x, y, endX, endY;
			vector<int> directions;

			tie(x, y, endX, endY, directions) = path_stack.top();
			path_stack.pop();

			// auf aktuelle Position "0" setzten und die Position in "breadcrums" abspeichern
			*(maze + (x)+((y)*row)) = 0;
			breadcrums.push_back(make_pair(x, y));

			//überprüft ob er auf oder neben dem Ziel steht
			if (is_goal(x, y) || near_goal(x, y)) {
				possible = true;
				return;
			}

			// mögliche Richtungen werden überpfüft
			vector<int> possible_directions;
			// überprüft ob er hoch gehen kann
			if (can_up(x, y)) {
				possible_directions.push_back(UP);
			}

			// überprüft ob er herunter gehen kann
			if (can_down(x, y)) {
				possible_directions.push_back(DOWN);
			}

			// überprüft ob er links gehen kann
			if (can_left(x, y)) {
				possible_directions.push_back(LEFT);
			}

			// überprüft ob er rechts gehen kann
			if (can_right(x, y)) {
				possible_directions.push_back(RIGHT);
			}

			// eine der gültigen richtungen wird ausgewählt nach zufallsprinzip
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
					possible = false;
					return;
				}
				path_stack.push(make_tuple(previous_position_coordinates.first, previous_position_coordinates.second, endX, endY, vector<int>()));
			}
		}
	}

	// überprüft oben (early return)
	bool can_up(int x, int y) {
		// ein weiter nicht gleich weg
		if (*(maze + (x)+(y - 1) * row) == 0)
		{
			return false;
		}

		// Für Border 
		if ((y - 1) <= 0)
		{
			return false;
		}

		// zwei weiter schauen
		if (*(maze + (x)+(y - 2) * row) == 0)
		{
			return false;
		}

		// überprüft die Schrägen in der Richtung 
		// Parameter verändert
		if (!seperator_check(x-1, y-1) || !seperator_check(x + 1, y - 1))
		{
			return false;
		}
		return true;
	}
	// überprüft unten
	bool can_down(int x, int y) {
		if (*(maze + (x)+(y + 1) * row) == 0)
		{
			return false;
		}

		// Für Border 
		if ((y + 1) >= col - 1)
		{
			return false;
		}

		// zwei weiter schauen
		if (*(maze + (x)+(y + 2) * row) == 0)
		{
			return false;
		}

		// überprüft die Schrägen in der Richtung
		if (!seperator_check(x - 1, y + 1) || !seperator_check(x + 1, y + 1))
		{
			return false;
		}
		return true;
	}
	// überprüft links
	bool can_left(int x, int y) {
		if (*(maze + (x - 1) + (y)*row) == 0)
		{
			return false;
		}

		// Für Border 
		if ((x - 1) <= 0)
		{
			return false;
		}

		// zwei weiter schauen
		if (*(maze + (x - 2) + (y)*row) == 0)
		{
			return false;
		}

		// überprüft die Schrägen in der Richtung
		if (!seperator_check(x - 1, y + 1) || !seperator_check(x - 1, y - 1))
		{
			return false;
		}
		return true;
	}
	// überprüft Rechts
	bool can_right(int x, int y) {
		if (*(maze + (x + 1) + (y)*row) == 0)
		{
			return false;
		}

		// Für Border 
		if ((x + 1) >= row - 1)
		{
			return false;
		}

		// zwei weiter schauen
		if (*(maze + (x + 2) + (y)*row) == 0)
		{
			return false;
		}

		// überprüft die Schrägen in der Richtung
		if (!seperator_check(x + 1, y + 1) || !seperator_check(x + 1, y - 1))
		{
			return false;
		}
		return true;
	}

	// vereinfacht dieüberprüfung von Schrägen
	bool seperator_check(int x, int y) {
		if (*(maze + (x) + (y)*row) == 0)
		{
			return false;
		}
		return true;
		
	}

	// im falle einer Sackgasse
	pair<int, int> find_previous_cell(){
		if (breadcrums.empty())
		{
			return make_pair(-1, -1);
		}
		breadcrums.erase(breadcrums.begin() + breadcrums.size() - 1); 
		if (breadcrums.empty())
		{
			return make_pair(-1, -1);
		}		
		pair<int,int> previous_position = breadcrums[breadcrums.size() - 1];
		breadcrums.erase(breadcrums.begin() + breadcrums.size() - 1);
		return previous_position;
	}

	// überprüft ob wegelement sich auf dem easteregg befindet
	bool is_egg(int x, int y) {
		if (x == egg_position[0] && y == egg_position[1]) {
			return true;
		}
		return false;
	}

	// überprüft ob wegelement sich neben dem easteregg befindet
	bool near_egg(int x, int y) {

		// hoch
		if (is_egg(x, y - 1))
		{
			return true;
		}

		// herunter
		if (is_egg(x, y + 1))
		{
			return true;
		}

		// links
		if (is_egg(x - 1, y))
		{
			return true;
		}

		// rechts
		if (is_egg(x + 1, y))
		{
			return true;
		}
		return false;
	}

	void check_coverage(float percentage) {
		int counter = 0;

		for (int i = 0; i < row * col; i++) {
			if (*(maze + i) == 0)
			{
				counter++;
			}
		}

		int border = (2 * row) + (2 * col);
		double coverage = ((double)counter / (((double)row * (double)col) - (double)border));

		if (coverage >= percentage)
		{
			possible = true;
			return;
		}

		possible = false;
		return;
	}

	void print_line() {
		string print_frame;
		for (int i = 0; i < 25; i++)
		{
			print_frame += "==";
		}
		for (int i = 0; i < row-25; i++)
		{
			print_frame += "==";
		}
		cout << print_frame << endl;
	}

};
#endif

