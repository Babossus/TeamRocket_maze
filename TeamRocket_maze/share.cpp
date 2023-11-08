#pragma once
#include <iostream>
#include <fstream>
#include "generate.cpp"
#include <filesystem>
#include <string>

using std::ofstream;
using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::vector;
using std::ifstream;
using std::stoi;

class share
{
public:
	share() {}

	void export_file(generate maze) {
		
		
		cout << "geben Sie einen Dateinamen an\n";
		cin >> export_name;
		export_name += ".txt";
		// Datei wird bei gleichem namen übershcrieben
		ofstream file(export_name);
		file << maze.startpoint[0] << "," << maze.startpoint[1] << " " << maze.endpoint[0] << "," << maze.endpoint[1] << endl;
		for (int i = 0; i < maze.col; i++)
		{
			for (int f = 0; f < maze.row; f++)
			{

				if (*(maze.maze + (f)+(i)*maze.row) == 9 || *(maze.maze + (f)+(i)*maze.row) == 5|| *(maze.maze + (f)+(i)*maze.row) == 8)
				{
					*(maze.maze + (f)+(i)*maze.row) = 0;
				}
				
				file << *(maze.maze + (f)+(i)*maze.row);

			}
			file << endl;
		}
		file.close();
	}

	void import_file(generate* maze) {
		vector<std::filesystem::directory_entry> files_found;
		int file_choice;
		string current_file_path;

		for (const auto & entry: std::filesystem::directory_iterator(path))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".txt")
			{
				files_found.push_back(entry);
			}
		}
		if (files_found.empty())
		{
			cout << "Keine Spielfelder Gefunden\n";
			return;
		}
		cout << "wählen Sie eine Datei aus:\n";
		for (int i = 0; i < files_found.size(); i++)
		{
			cout << i + 1 << ". " << files_found[i].path().filename() << endl;
		}
		cin >> file_choice;
		file_choice--;
		current_file_path = files_found[file_choice].path().filename().string();
		
		ifstream imported_maze;
		string content;
		int counter = 0;
		string maze_raw;

		imported_maze.open(current_file_path);
		if (imported_maze.fail())
		{
			cout << "Fehler, kaputte datei :)";
			return;
		}
		while (!imported_maze.eof()) // solange nicht am ende der datei
		{
			imported_maze >> content;
			if (counter == 0)
			{
				int pos = content.find(",");
				maze->startpoint[0] = stoi(content.substr(0, pos));
				maze->startpoint[1] = stoi(content.substr(pos + 1));	
			}
			if (counter == 1)
			{
				int pos = content.find(",");
				maze->endpoint[0] = stoi(content.substr(0, pos));
				maze->endpoint[1] = stoi(content.substr(pos + 1));
			}
			if (counter == 2)
			{
				maze->row = content.size();
			}
			if (counter != 0 && counter != 1)
			{
				maze_raw += content;
			}
			counter++;
		}
		maze->col = ((maze_raw.length()) / maze->row);

		// Maze muss einem neuen Speicherbereich zugewiesen werden, da sich die spiechergrösse, im Vergleich zum Konstruktor verändert hat.
		maze->maze = /*die eingabe für maze wird in ein int* gewandelt(gecasted)*/(int*)calloc(maze->row * maze->col, sizeof(int));

		// Ein wenig Ghetto, aber eine schnelle Lösung einen Char zu einem Int zu machen.
		string str2char = "";

		// Schreibe jeden Wert in das Spielfeld hinein...
		// Ist an dieser Stelle zwar ein bisschen Ghetto, da beriets der Konstruktor, ein Maze generiert hat, aber aufrgund vón Zeitmanagement, wird die generate class so belassen wie sie ist.
		for (int i = 0; i < maze->col; i++)
		{
			for (int f = 0; f < maze->row; f++)
			{
				str2char = maze_raw[(f)+(i)*maze->row];
				*(maze->maze + (f)+(i)*(maze->row)) = stoi(str2char);
			}
		}

		// Setze Start und Endpunkt
		// Da wir davon ausgehen, dass der Index hier bei 1 beginnt also unsere erste Startposition (1,1) wäre, müssen wir -1 rechnen um es an ein Array anzupassen.
		*(maze->maze + (maze->startpoint[0]) + (maze->startpoint[1]) * (maze->row)) = 5;
		*(maze->maze + (maze->endpoint[0]) + (maze->endpoint[1]) * (maze->row)) = 9;

	}
	
private:
	string export_name;
	string import_name;
	const string path = "C:/Users/Matteo/source/repos/TeamRocket_maze/TeamRocket_maze/";// pfad angeben
	 
};
