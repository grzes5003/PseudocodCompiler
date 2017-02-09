#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

#define PROG_DATA 1
#define PROG_CODE 2
#define DEBBUG_MODE 0

class Program
{
	int line_number;


	std::set<char> forbChar = {
		'.', ',', '-', '>', '<', '(', ')', '='
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
		greaterOrEq = 14,
		lessOrEq = 15,
		
		nothing = 100,
	};


	int PROG_PART = 0;

	void deleteSpaces( std::string &s );
	bool isSpacesOnly( std::string &s );
	
	bool isCorrect( std::string &s, CR_TYPE cr );
	CR_TYPE whatIsThat( std::string &s );
	std::fstream& GotoLine( std::fstream& file, unsigned int num );

	std::fstream _file;

	void read(std::fstream &file);
	void error(int number);
	void summarise();

	void saveVar( std::vector<std::pair<std::string, float>>& vec, std::string varName, float varValue );
	float valueOfVar( std::string _name, std::vector<std::pair<std::string, float>>& base );
	void changeVar( std::vector<std::pair<std::string, float>>& vec, std::string varName, float varNewValue );
public:
	Program(std::string file_name);
	Program() {};
	
	virtual ~Program();
	
	bool IFStatment( std::string &s );
	void readCode( std::string &line );
	std::vector<std::pair<std::string, float>> _variables;
	
	
	std::string oneWord( std::string &s ); //zwraca pierwsze slowo w ciagu
};

