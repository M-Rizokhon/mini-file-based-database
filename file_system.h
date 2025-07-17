// FileSystem class to implement all necessary functions
// for manipulating files, such as creating or deleting.

// FileSystem class has 6 main functions, and a few helper functions:
// - create_file -> creating new files
// - read_file  -> reading already existing files
// - delete_file -> deleting already existing files
// - update_file -> update an already existing files, such as adding or replacing lines
// - help_user   -> provide information about commands available in the program
// - view_files  -> view all existing files



#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace std;



class FileSystem {
private:
	vector<string> file_names;

	// helper functions for update_file function
	void replace_line(const string& filename, int line_number);
	void remove_line(const string& filename, int line_number);
	void handle_option(const string& filename);
	int file_size(const string& filename);
	void handle_option1(const string& filename);
	void handle_option2(const string& filename);
	void handle_option3(const string& filename);

	// helper function for view_files() function
	string file_metadata(const string& filename);

public:
	void create_file(const string& filename);
	void read_file(const string& filename);
	void delete_file(const string& filename);
	void update_file(const string& filename);
	void help_user();
	void view_files();
};

// -----------------------------------------------------------------
// ----- DEFINITION OF 5 CORE FUNCTIONS -----

// create a new file
void FileSystem::create_file(const string& filename) {
	// check if file already exists.
	// cannot create already existing file 
	for (string file : file_names) {
		if (file == filename) {
			cout << "ERROR : FILE ALREADY EXIST." << endl;
			return;
		}
	}

	// create file and print a message
	ofstream file(filename);
	cout << "FILE CREATED : " << filename << endl;
	file_names.push_back(filename);
}



// read already existing file
void FileSystem::read_file(const string& filename) {
	// can only read from file if it exists
	// check if it exists
	bool exists = false;
	for (string file : file_names) {
		if (file == filename) {
			exists = true;
			break;
		}
	}

	if (!exists) {
		cout << "ERROR : FILE DOES NOT EXIST." << endl;
		return;
	}

	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "ERROR : FAILED TO OPEN THE FILE." << endl;
		return;
	}

	// read from the file, and print each line to the console
	if (file.peek() == ifstream::traits_type::eof()) {
		cout << "FILE IS EMPTY" << endl;
		file.close();
		return;
	}
	string line;
	while (getline(file, line)) {
		cout << line << '\n';
	}

	file.close();
}



// update already existing files
void FileSystem::update_file(const string& filename) {
	// check if file exists
	bool exists = false;
	for (string file : file_names) {
		if (file == filename) {
			exists = true;
			break;
		}
	}

	if (!exists) {
		cout << "ERROR : FILE DOES NOT EXIST." << endl;
		return;
	}

	// pass to the function that handles different
	// ways of updating
	handle_option(filename);
}



// delete an existing file 
void FileSystem::delete_file(const string& filename) {
	// first checking if the file exists
	bool exists = false;
	for (string file_name : file_names) {
		if (filename == file_name) {
			exists = true;
			break;
		}
	}
	if (!exists) {
		cerr << "ERROR: FILE DOES NOT EXIST" << endl;
		return;
	}


	remove(filename.c_str());
	cout << "FILE IS DELETED PERMANENTLY" << endl;

	// remove filename from the vector of file_names
	// --- an explanation of how does this work ---
	// - remove(...) rearranges the vector so that all elements
	// not equal to 'filename' are moved upfront, and
	// it returns an iterator pointing to the front where garbage 
	// elements begin
	// - erase(start, end) actually erases elements of the vector from
	// start to the end.
	file_names.erase(remove(file_names.begin(), file_names.end(), filename), file_names.end());
}



// give information about the program
void FileSystem::help_user() {
	cout << "--- HELP ---" << endl;
	cout << "THERE ARE A FEW COMMANDS AVAILABLE IN THE SOFTWARE : \n";

	cout << " - CREATE <FILENAME>  -> create new files\n";
	cout << " - READ <FILENAME>    -> read data from an existing file\n";
	cout << " - UPDATE <FILENAME>  -> update the existing file (e.g. add new data)\n";
	cout << " - DELETE <FILENAME>  -> delete an existing file\n";
	cout << " - VIEW  -> view all exisiting files\n";
	cout << " - HELP  -> get all information about the software" << endl;
}



// view all existing files 
void FileSystem::view_files() {
	if (file_names.empty()) {
		cout << "NO FILES YET." << endl;
		return;
	}

	cout << "THE LIST OF EXISTING FILES : \n";
	for (string filename : file_names)
		cout << file_metadata(filename) << endl;
	cout << endl;
}

// -----------------------------------------------------------------------
// ----- DEFINITIONS OF HELPER FUNCTIONS OF "update_file"



// replace a line in a file
void FileSystem::replace_line(const string& filename, int line_number) {
	// read and write all lines, replacing one
	ifstream fin(filename);
	ofstream fout("temp.txt");
	if (!fin || !fout) {
		cout << "ERROR : FAILED TO OPEN THE FILE." << endl;
		return;
	}

	string line;
	int current = 1;
	while (getline(fin, line)) {
		if (current == line_number) {
			cout << "ENTER NEW DATA FOR THE LINE: ";
			string data;
			getline(cin, data);
			fout << data << '\n';
			cout << "THE LINE CHANGED SUCCESSFULLY." << endl;
		}
		else {
			fout << line << '\n';
		}
		current++;
	}

	fin.close();
	fout.close();

	remove(filename.c_str());
	rename("temp.txt", filename.c_str());
}

// remove a line in a file
void FileSystem::remove_line(const string& filename, int line_number) {
	// read and write all lines, replacing one
	ifstream fin(filename);
	ofstream fout("temp.txt");
	if (!fin || !fout) {
		cerr << "ERROR : FAILED TO OPEN THE FILE" << endl;
		return;
	}

	string line;
	int current = 1;
	while (getline(fin, line)) {
		if (current == line_number) {
			cout << "THE LINE REMOVED SUCCESSFULLY" << endl;
			continue;
		}
		else {
			fout << line << '\n';
		}
		current++;
	}

	fin.close();
	fout.close();

	remove(filename.c_str());
	rename("temp.txt", filename.c_str());
}

// helper function to find the number of lines in the file
int FileSystem::file_size(const string& filename) {
	ifstream countFile(filename);
	if (!countFile.is_open()) {
		cerr << "ERROR : FAILED TO OPEN THE FILE" << endl;
		return -1;
	}

	int file_length = 0;
	string line;
	while (getline(countFile, line)) {
		file_length++;
	}
	countFile.close();

	return file_length;
}





void FileSystem::handle_option(const string& filename) {
	cout << "CHOOSE AN OPTION : \n";
	cout << "1. CHANGE BY LINE\n";
	cout << "2. ADD NEW DATA\n";
	cout << "3. REMOVE A LINE\n";
	cout << "4. EXIT\n";
	int option;
	cin >> option;
	cin.ignore();

	// each update method has a function
	// to manipulate the file in different ways
	switch (option) {
	case 1:
		handle_option1(filename);
		break;
	case 2:
		handle_option2(filename);
		break;
	case 3:
		handle_option3(filename);
		break;
	case 4:
		return;
	default:
		cout << "ERROR : INVALID INPUT" << endl;
	}
}


void FileSystem::handle_option1(const string& filename) {
	while (true) {
		// first, count the number of lines in the file
		int file_length = file_size(filename);
		string line;

		// get the input from the user
		int line_number;
		cout << "ENTER THE NUMBER OF THE LINE TO CHANGE ('-1' to quit): ";
		cin >> line_number;
		cin.ignore();   // to consume leftover newline


		if (line_number == -1)
			break;

		if (line_number >= file_length || line_number < 0) {
			cout << "ERROR : INVALID INPUT." << endl;
			continue;
		}


		replace_line(filename, line_number);
	}
}


void FileSystem::handle_option2(const string& filename) {
	ofstream file(filename, ios::app);
	if (!file) {
		cerr << "ERROR : FAILED TO OPEN THE FILE." << endl;
		return;
	}
	while (true) {
		cout << "ENTER NEW DATA TO ADD TO THE FILE (-1 TO QUIT) : ";
		string line;
		getline(cin, line);
		if (line == "-1")
			return;
		file << line << '\n';
	}
}


void FileSystem::handle_option3(const string& filename) {
	while (true) {
		// first count the number of lines in the file
		int file_length = file_size(filename);
		string line;


		// get input from the user
		int line_number;
		cout << "ENTER THE LINE NUMBER TO DELETE ('-1' to quit): ";
		cin >> line_number;
		cin.ignore();   // clean leftover newline

		if (line_number == -1) {
			break;
		}

		if (line_number >= file_length || line_number < 0) {
			cerr << "ERROR : INVALID INPUT" << endl;
			break;
		}


		remove_line(filename, line_number);
	}
}





// --------------------------------------------------------------------------------------------
// Defining the helper function of view_files
// function returns metadata of a file in the form:
// - <filename> (created: <time>, size: <size>)
// NOTE : I did not implement this function on my own, 
// but with the help of Github copilot!
string FileSystem::file_metadata(const string& filename) {
	string data = " - " + filename;

	// Get file size
	uintmax_t size = std::filesystem::file_size(filename);

	// Get last write time and convert to time_t
	auto ftime = std::filesystem::last_write_time(filename);
	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
		ftime - std::filesystem::file_time_type::clock::now()
		+ std::chrono::system_clock::now()
	);
	std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

	// Format time
	std::ostringstream oss;

	// Use localtime_s for safer conversion
	std::tm time_info;
	localtime_s(&time_info, &cftime);
	oss << " (last modified: " << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S")
		<< ", size: " << size << " bytes)";
	data += oss.str();

	return data;
}

