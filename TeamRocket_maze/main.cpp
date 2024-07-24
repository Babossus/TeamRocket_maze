#pragma once
#include <iostream>
#include "generate.cpp"
#include "player.cpp"
#include "share.cpp"
#define KEYBOARD 0
#define ALGORYTHM 1
#define IMPORT 2
#define EXPORT 3

using std::cin;
using std::cout;


// schonmal zeigen, dass es diese Funktionen gibt
bool terminal(int* execute);
int input(string text);
void finish(pair<bool, bool> easter_egg);

int main()
{

	
	int execute = 0;
	// Teminal funtionen abrufen
	while (terminal(&execute) == true)
	{
		
		if (execute == KEYBOARD)
		{
			system("cls");
			int x = input("Geben Sie die X Laenge der Achse an.\n");
			int y = input("Geben Sie die Y Laenge der Achse an.\n");
			generate* generate_maze = new generate(x, y);
			player* controller = new player(*generate_maze);
			generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status(), controller->direction);
			while (controller->manuel() == false) // dies wird ja immer ausgef�hrt aber durch get finish nichtmehr
			{
				system("cls");
				generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status(), controller->direction);

			}
			cout << "Druecken Sie eine belibige Taste zum Fortfahren";
			_getch();
			system("cls");
			finish(controller->get_egg_status());
			generate_maze->cleanup(); // muss raus aus der while schleife | nach jedem spiel einmal cleanup um speicher zu entlasten
			delete controller;
			_getch();
		}	
		else if (execute == ALGORYTHM)
		{
			system("cls");
			int x = input("Geben Sie die X Laenge der Achse an.\n");
			int y = input("Geben Sie die Y Laenge der Achse an.\n");
			generate* generate_maze = new generate(x, y);
			player* controller = new player(*generate_maze);
			controller->automatic();
			system("cls");
			generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status(), controller->direction);
			cout << "\nDruecken Sie eine belibige Taste zum Fortfahren";
			_getch();
			finish(controller->get_egg_status());
			generate_maze->cleanup();
			delete controller;
		}
		else if (execute == IMPORT) {
			system("cls");
			share* dateien = new share();
			generate* generate_maze = new generate(5, 5);
			dateien->import_file(generate_maze);
			cout << "Was m�chten Sie mit dem importiertem Labyrinth machen?\n1. Selber Spielen\n2. Den Algorythmus das Labyrinth laufen lassen\n";
			char input;
			input = _getch();
			system("cls");

			if (input == '1') {
				player* controller = new player(*generate_maze);
				generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status(), controller->direction);
				while (controller->manuel() == false) // dies wird ja immer ausgef�hrt aber durch get finish nichtmehr
				{
					system("cls");
					generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status(), controller->direction);

				}

				cout << "Druecken Sie eine belibige Taste zum Fortfahren";
				_getch();
				finish(controller->get_egg_status());
				delete dateien;
				delete controller;
				_getch();
			}
			else if (input =='2') {
				//new generate(x, y); - hier schauen weil man da nur print und start- und endpoint erstellen
				player* controller = new player(*generate_maze);
				controller->automatic();
				system("cls");
				generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status(), controller->direction);
				cout << "Druecken Sie eine belibige Taste zum Fortfahren";
				_getch();
				finish(controller->get_egg_status());
				delete dateien;
				delete controller;
				_getch();
			}
			generate_maze->cleanup();
		}
		else if (execute == EXPORT) {
			system("cls");
			int x = input("Geben Sie die X L�nge der Achse an.\n");
			int y = input("Geben Sie die Y L�nge der Achse an.\n");
			generate* generate_maze = new generate(x, y);
			share* dateien = new share();
			dateien->export_file(*generate_maze);
			cout <<"Sie haben das Labyrinth erfolgreich exportiert\n" << "Druecken Sie eine belibige Taste zum Fortfahren"; 
			_getch();
			delete dateien;
			generate_maze->cleanup();

		}

	}
	
}

bool terminal(int* execute) {

	char input;
	char input_2;

	// erste Befragung
	cout << "Was moechten Sie machen?\n1.Spielen\n2.Importieren\n3.Exportieren\n4.Verlassen\n";
	input = _getch();
	system("cls");

	// zweite Befragung
	switch (input)
	{
	case '1': // Spielen

		// zweite switch
		cout << "Wie moechten Sie spielen?\n1.Tastatur\n2.Algorythmus\n3.Zurueck\n";
		input_2 = _getch();
		system("cls");
		switch (input_2)
		{
		case '1':
			*execute = KEYBOARD;
			return true;

		case '2':
			*execute = ALGORYTHM;
			return true;
		}
		*execute = 6;
		return true;
	case '2': // Import
		*execute = IMPORT;
		return true;
	case '3': // Export
		*execute = EXPORT;
		return true;
	default:
		return false;
		// falls falssche eingabe...
	}
}

int input(string text) {
	int value;
	cout << text;
	cin >> value;
	system("cls");
	return value;

}

void finish(pair<bool, bool> easter_egg) {
	system("cls");
	cout << R"(
              .-=========-.
              \'-=======-'/
              _|   .=.   |_
             ((|  {{1}}  |))
              \|   /|\   |/
               \__ '`' __/
                 _`) (`_
               _/_______\_
              /___________\
Glueckwunsch!

							)" << endl;
	if (easter_egg.first == true)
	{
		cout << R"(
		It takes strength to resist the dark
		side by finishing without the  easter egg!
		
            .-.
           |o,o|
        ,| _\=/_      .-""-.
        ||/_/_\_\    /[] _ _\
        |_/|(_)|\\  _|_o_LII|_
           \._./// / | ==== | \
           |\_/|"` |_| ==== |_|
           |_|_|    ||" ||  ||
           |-|-|    ||LI  o ||
           |_|_|    ||'----'||
          /_/ \_\  /__|    |__\
)";
	}
}

//		Meine Anforderungen
//wenn startpunkt = endpunkt dann ende 
//nur 1 pfad
//startpunkt und endpunkt parameter
//  setze 0 an Position
//der pfad darf keinen anderen schneiden ( +2 == 1) 
//es darf kein 4x4 feld entstehen (+1 dr�ber und drunter muss ne 1 sein
// bei abbruch automatisch neu
// er pr�ft ob ziel neben sich ist
// 
// der ganze platz soll genutzt werden
// es m�ssen 50% derzahlen nullen sein sonst constructor neu
//vielleicht ein easter egg dass man zum start bringen muss ( zweites ziel



/* TO DO
* 
* 
*
* 
*/