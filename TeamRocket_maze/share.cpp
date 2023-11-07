#include <iostream>
#include <fstream>
#include "generate.cpp"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class share
{
public:
	share(generate maze) :maze(maze) {}

	void export_file() {

		
		cout << "geben Sie einen Dateinamen an\n";
		cin >> export_name;
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
		for (const auto & entry: filesystem::directory_iterator)
		{

		}
	}
	/*	Checklist
	* rwos und cols erkennen damit startpoint erkannt wird
	* 
	*/
	
private:
	generate maze;
	string export_name;
	string import_name;
	const string path = "";// pfad angeben
};
