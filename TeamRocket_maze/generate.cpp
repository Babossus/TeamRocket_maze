#ifndef GENERATE_CPP
#define GENERATE_CPP
#include <iostream>
#include <vector>
#include <stack>
#include <tuple>


// deklaration von makros für präprozessor --- hier wollte ich es nicht in die Himmelsrichtungen umbenennen da der generator immer nach Norden Schaut
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
				// Das hier ist optimierung, um den richtigen Wert zu finden, da wir it Zufall arbeiten. Je höher die Dichte desto unwahrscheinlicher das schnell ein Spielfeld generiert wird. Je geringer die Dichte desto langweiliger das Spielfeld, daher hat sich 0.55 als guter Wert erwiesen.
				check_coverage(0.55); // überprüft dichte ohne rand
			}			
		}
		multiple_ways(row, col);
		

		*(maze + (startpoint[0]) + (startpoint[1]) *row) = 5;		
		*(maze + (endpoint[0]) + (endpoint[1]) * row) = 9;
	}

	// Reservierung im RAM aufheben
	void cleanup() {
		free(maze);
		delete this;
	}
	

	// Ausgabe von dem Spielfeld
	void print_maze(pair<int, int> coordinates, int steps, pair<bool, bool> easter_egg, int richtung) {
		print_line();
		cout << "Spielfeld:\t" << "Koordinaten: " <<"(" << coordinates.first << "," << coordinates.second << ")\t" << "Zuege: " << steps ;
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
				case 3: //4
					cout << "<>";
					break;
				case 5:
					switch (richtung)
					{
					case 0:
						cout << '/' <<'\\';
						break;
					case 1:
						cout << "<-";
						break;
					case 2:
						cout << '\\'<<'/';
						break;
					case 3:
						cout << "->";
						break;
					default:
						break;
					}
					break;
				case 8:
					cout << "SP";
					break;
				case 9:
					cout << "EP";
					break;
					//
				case 2:
					cout << "HI";
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
		// Deklariert einen Stack, jedes Element des Stacks hält dabei 4 Ints und einen eindimensionalen Vector der Integer enthält.
		stack<tuple<int, int, int, int, vector<int>>> path_stack;
		// Wir wollen hier keinen leeren Stack, um in die while Schleife zu kommen, daher packen wir einen Start eintrag in den Stack herein.
		path_stack.push(make_tuple(startX, startY, endX, endY, vector<int>()));

		// Hier nutzen wir das Prinzp der Rekursion nur ohne die Rekursion und deren Nachteile, da wir iterativ arbeiten.
		while (!path_stack.empty()) {

			int x, y, endX, endY;
			// Directions speichert im verlauf der Funktion in welche Richtung unser Generator sich bewegen kann um weiter nach dem Ziel zu suchen.
			vector<int> directions;

			// Ähnlich wie bei der Rekrusion wo das nächst kleinere Problem von Paramtern dargestellt wird, verwenden wir hier tie um unsere werte mit den nächst kleineren vom Stack zu füllen.
			tie(x, y, endX, endY, directions) = path_stack.top();
			// Da wir die Werte jetzt haben entfernen wir den aktuellen Eintrag vom Stapel.
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
				// Da wir ja keine Rekursion verwenden, aber dessen Prinzip, übergeben wir hier das nächst kleinere Problem in Form eines Eintrags und legen es auf den Stack herauf.
				path_stack.push(make_tuple(newX, newY, endX, endY, possible_directions));
			}
			else {
				// er geht auf die koordinaten vom letztem schritt (tupel)
				pair<int, int> previous_position_coordinates = find_previous_cell();
				// was ist wenn wir bewegungsunfähig sind, tritt eigentlich nicht ein, aber den fall sollte man schon beachten.
				if (previous_position_coordinates.first == -1 && previous_position_coordinates.second == -1) {
					possible = false;
					return;
				}
				// es gibt keine Richtung mehr in die wir gehen können? Dann müssen wir einen Schritt zurück, indem wir unseren ausgestreuten Brotkrumen folgen.
				path_stack.push(make_tuple(previous_position_coordinates.first, previous_position_coordinates.second, endX, endY, vector<int>()));
			}
		}
	}

	// Die folgenden 4 Funktionen zeigen die Effektivität von early returns nur zu gur, alles bleibt schön übersichtlich, wir haben Gewissheit das gewissen szenarien nicht mehr eintreten können, und auch die Performance steigt an, da wir wir im zweifel weniger bedingungen prüfen müssen.

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
	void multiple_ways(int row, int col) {
		int border = (2 * row) + (2 * col);
		int delete_wall_number = (int)(3 * (row * col - border) / 100);
		int max_try = 0;
		for (int i = 0; i < (delete_wall_number); i++) { 	
			if (max_try >= row * col) {
				return;
			}
			int right = 0;
			int left  = 0;
			int down  = 0;
			int up    = 0;
			int rnd_x = (random(2, row-2));
			int rnd_y = (random(2, col-2));
			if (*(maze + (rnd_x)+(rnd_y)*row) == 1) {
				//right 
				if (*(maze + (rnd_x + 1) + (rnd_y)*row) == 0) {
					right++;
				}
				// left
				if (*(maze + (rnd_x - 1) + (rnd_y)*row) == 0) {
					left++;
				}
				// down
				if (*(maze + (rnd_x)+(rnd_y + 1) * row) == 0) {
					down++;
				}
				// up
				if (*(maze + (rnd_x)+(rnd_y - 1) * row) == 0) {
					up++;
				}
			}
			if (right + left == 2 && up + down == 0 || right + left == 0 && up + down == 2){
				*(maze + (rnd_x)+(rnd_y)*row) = 0; // 2 zum testen
			}
			else{
				i--;
				max_try++;
			}
		}
	}
};
#endif

