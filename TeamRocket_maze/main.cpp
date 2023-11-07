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
			int x = input("Geben Sie die X - achse an.\n");
			int y = input("Geben Sie die Y - achse an.\n");
			generate* generate_maze = new generate(x, y);
			player* controller = new player(*generate_maze);
			generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status());
			while (controller->manuel() == false) // dies wird ja immer ausgeführt aber durch get finish nichtmehr
			{
				system("cls");
				generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status());

			}
			cout << "Druecken Sie eine belibige Taste zum Fortfahren";
			_getch();
			system("cls");
			finish(controller->get_egg_status());
			generate_maze->cleanup(); // muss raus aus der while schleife | nach jedem spiel einmal cleanup um speicher zu entlasten
			delete controller;
		}	
		else if (execute == ALGORYTHM)
		{
			system("cls");
			int x = input("Geben Sie die X - achse an.\n");
			int y = input("Geben Sie die Y - achse an.\n");
			generate* generate_maze = new generate(x, y);
			player* controller = new player(*generate_maze);
			controller->automatic();
			system("cls");
			generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status());
			cout << "Druecken Sie eine belibige Taste zum Fortfahren";
			_getch();
			finish(controller->get_egg_status());
			generate_maze->cleanup(); // muss raus aus der while schleife | nach jedem spiel einmal cleanup um speicher zu entlasten
			delete controller;
		}
		else if (execute == IMPORT) {
			system("cls");
			generate* generate_maze;
			player* controller = new player(*generate_maze);
			share* dateien = new share(*generate_maze);
			dateien->import_file();
			cout << "Was möchten Sie mit dem importiertem Labyrinth machen?\n1. Selber Spielen\n2. den Algorythmus das Labyrinth laufen lassen\n";
			char input;
			input = _getch();
			switch (input)
			{
			case '1':
				player * controller = new player(*generate_maze);
				generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status());
				while (controller->manuel() == false) // dies wird ja immer ausgeführt aber durch get finish nichtmehr
				{
					system("cls");
					generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status());

				}
				cout << "Druecken Sie eine belibige Taste zum Fortfahren";
				_getch();
				break;
			case '2':
				//new generate(x, y); - hier schauen weil man da nur print und start- und endpoint erstellen
				new player(*generate_maze);
				controller->automatic();
				system("cls");
				generate_maze->print_maze(controller->get_coordinates(), controller->get_move_count(), controller->get_egg_status());
				cout << "Druecken Sie eine belibige Taste zum Fortfahren";
				_getch();
				break;
			default:
				break;
			}
			if (//eingabe)
			{
			
			}
			delete dateien;
			delete controller;
			generate_maze->cleanup();
			// mann muss endscheiden können ob man dann  selbst läuft oder einliest
		}
		else if (execute == EXPORT) {
			system("cls");
			int x = input("Geben Sie die X - achse an.\n");
			int y = input("Geben Sie die Y - achse an.\n");
			generate* generate_maze = new generate(x, y);
			share* dateien = new share(*generate_maze);
			dateien->export_file();
			cout <<"Sie haben das Labyrinth erfolgreich Exportiert\n" << "Druecken Sie eine belibige Taste zum Fortfahren"; // schauen wegen if schleife obes wirklich erfolgreich war vllt mit bool
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
	cout << "was moechten Sie machen?\n1.Spielen\n2.Importieren\n3.Exportieren\n4.Verlassen\n";
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
//es darf kein 4x4 feld entstehen (+1 drüber und drunter muss ne 1 sein
// bei abbruch automatisch neu
// er prüft ob ziel neben sich ist
// 
// der ganze platz soll genutzt werden
// es müssen 50% derzahlen nullen sein sonst constructor neu
//vielleicht ein easter egg dass man zum start bringen muss ( zweites ziel



/* TO DO
* 
* 
*
* 
*/