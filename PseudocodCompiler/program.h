#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#define PROG_DATA 1
#define PROG_CODE 2

class Program
{
	std::set<char> forbChar = {
		'.', ',', '-', '>', '<', '(', ')'
	};
	enum CR_TYPE {
		nazwa = 0,
		value = 1,
		arrow = 3,
		gotoInstruction = 4,
		endInstruction = 5,
		ifStatment = 6,
		equal = 10,
		lesst = 11,
		greatert = 12,
		notequal = 13,
		nothing = 100,
	};


	int PROG_PART = 0;

	void deleteSpaces( std::string &s );
	bool isSpacesOnly( std::string &s );
	
	bool isCorrect( std::string &s, CR_TYPE cr );
	CR_TYPE whatIsThat( std::string &s );

	std::fstream _file;
	std::vector<std::pair<std::string, float>> _variables;

	void saveVar( std::vector<std::pair<std::string, float>>& vec, std::string varName, float varValue );
	void read(std::fstream &file);
	void readCode( std::string &line );
	void error(int number);
public:
	Program(std::string file_name);
	Program() {};
	
	virtual ~Program();
	
	
	
	std::string oneWord( std::string &s ); //zwraca pierwsze slowo w ciagu
};

