#include <iostream>
#include <fstream>
#include "generate.cpp"

using namespace std;

class share
{
public:
	share(generate maze) :maze(maze) {}
	void export() {

		string export_name;
		cout << "geben SIe einen Dateinamen an";
		cin >> export_name;
		fstream file_out;
		file_out.open(export_name, ios_base::out);
		if (!file_out.is_open()) {
			cout << "failed to export" << export_name << endl;
		}
		else {
			file_out << maze.print_export;
			cout << "Done Writing";
		}


	}

private:
	generate maze;
};
