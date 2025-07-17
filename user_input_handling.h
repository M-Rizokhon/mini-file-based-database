// Handling user input, by parsing the user input

#pragma once
#include "file_system.h"
// manual parser function
vector<string> splitIntoWords(string sentence) {
	vector<string> words;

	string currentWord;
	for (char c : sentence) {
		if (c == ' ') {
			if (!currentWord.empty()) {
				words.push_back(currentWord);
				currentWord = "";
			}
		}
		else {
			currentWord += c;
		}
	}

	// push the last word (if any)
	if (!currentWord.empty()) {
		words.push_back(currentWord);
	}

	return words;
}


bool invalid_input(vector<string>words) {
	vector<string> keywords = { "READ", "CREATE", "UPDATE", "DELETE" };

	// first checking if the first word is keyword
	bool is_valid = false;
	for (string keyword : keywords) {
		if (words[0] == keyword)
			is_valid = true;
	}

	if (!is_valid) {
		cerr << "ERROR: KEYWORD IS MISSING" << endl;
		return true;
	}


	// second checking if there are exactly 2 words
	if (words.size() > 2) {
		cerr << "ERROR: KEYWORD MUST ACCOMPANY BY THE FILE NAME ONLY" << endl;
		return true;
	}

	return false;

}

// Now I need to handle the user input
void handle_user() {
	cout << "---Welcome to mini database engine (ctrl+c to exit)---" << endl;
	cout << "(type \"HELP\" to get information!)" << endl;
	FileSystem file_system;
	while (true) {
		string user_input;
		getline(cin, user_input);

		// user_input must consist of 2 parts : keyword (e.g. CREATE) + filename ("temp.txt")
		// if user types wrong keyword or more than 2 blocks of parts, I will give a proper error message
		// 'HELP' is an exception here
		vector<string> words = splitIntoWords(user_input);

		// handling special inputs like HELP or VIEW
		if (words[0] == "HELP") {
			file_system.help_user();
			continue;
		}
		else if (words[0] == "VIEW") {
			file_system.view_files();
			continue;
		}

		if (invalid_input(words)) {
			continue;
		}


		// now handling the four operations
		if (words[0] == "CREATE") {
			file_system.create_file(words[1]);
		}
		else if (words[0] == "READ") {
			file_system.read_file(words[1]);
		}
		else if (words[0] == "UPDATE") {
			file_system.update_file(words[1]);
		}
		else if (words[0] == "DELETE") {
			file_system.delete_file(words[1]);
		}
		else {
			cout << "ERROR : ERROR : ERROR" << endl;
		}

	}
}

