#pragma once
#include <iostream>
#include <fstream>
#include "generate.cpp"
#include <filesystem>

using std::ofstream;
using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::vector;
using std::ifstream;

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
		file << maze.startpoint[1] << "," << maze.startpoint[2] << " " << maze.endpoint[1] << "," << maze.endpoint[2] << endl;
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

	void import_file() {
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
		imported_maze.open(current_file_path);
		if (imported_maze.fail())
		{
			cout << "Fehler, kaputte datei :)";
			return;
		}
		while (!imported_maze.eof()) // solange nicht am ende der datei
		{
			imported_maze >> content;
			cout << content << endl;
		}
		

	}
	/*	Checklist
	* rwos und cols erkennen damit startpoint erkannt wird
	* .txt
	*/
	
private:
	string export_name;
	string import_name;
	const string path = "F:/TeamRocket_maze/TeamRocket_maze/";// pfad angeben
	 
};
