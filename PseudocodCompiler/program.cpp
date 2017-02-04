#include "program.h"

void Program::deleteSpaces( std::string & s ) {
	int it = 0;
	for( unsigned int i = 0; i < s.size(); i++ ) {
		if( s.at( i ) == ' ' ) it = i+1;
		else if(i == 0 && s.at( i ) != ' ' ) return;
		else break;
	}
	s.erase( s.begin(), s.begin()+it );
}

bool Program::isSpacesOnly( std::string & s ) {
	for( unsigned int i = 0; i < s.size(); i++ ) {
		if( s.at( i ) != ' ' )
			return false;
	}
	return true;
}

bool Program::isCorrect( std::string & s, CR_TYPE cr ) {
	if( cr == nazwa ) {
		for( unsigned int i = 0; i < s.size(); i++ ) {
			if( forbChar.find( s.at( i ) ) != forbChar.end() )
				error( 1 ); //usage of forbitten characters
		}
	}
	else if( cr == value ) {
		for( unsigned int i = 0; i < s.size(); i++ ) {
			if( !isdigit( s.at( i ) ) ) {
				error( 1 ); //usage of nondigit characters
			}
		}
	}
	return true;
}

Program::CR_TYPE Program::whatIsThat( std::string & s ) {
	if( s == "if" ) return ifStatment;
	else if( s == "goto" ) return gotoInstruction;
	else if( s == "end" ) return endInstruction;
	else if( s == "<-" ) return arrow;
	for( unsigned int i = 0; i < _variables.size(); i++ ) {
		if( s == _variables.at( i ).first ) {
			return nazwa;
		}
	}
	for( unsigned int i = 0; i < s.size(); i++ ) {
		if( !isdigit( s.at( i ) ) )
			return nothing;
	}
	return value;
}


std::string Program::oneWord( std::string & s ) {
	deleteSpaces( s );
	std::string result;
	int i = 0;
	while( i < s.size() && s.at( i ) != ' ' ) {
		result += s.at( i );
		i++;
	}
	if( i == s.size() )
		i--;
	s.erase(s.begin(),s.begin()+i);
	return result;
}

void Program::saveVar( std::vector<std::pair<std::string,float>>& vec, std::string varName, float varValue ) {

}

void Program::read( std::fstream & file ) {
	
	std::string line;
	int line_number = 1;

	while( getline( file, line ) ) {
		deleteSpaces( line );
		if( line.at( 0 ) == '#' ) {
		///sprawdzenie ktory to fragment kodu
			if(line == "#DATA" && PROG_PART == 0) {
				PROG_PART = PROG_DATA;
				//ok
			}
			else if( line == "#DATA" && PROG_PART == PROG_DATA ) {
				error( 1 );
			}
			else if( line == "#DATA" && PROG_PART == PROG_CODE ) {
				error( 1 );
			}
			else if( line == "#CODE" && PROG_PART == PROG_DATA ) {
				PROG_PART = PROG_CODE;
				//ok
			}
			else if( line == "#CODE" && PROG_PART == PROG_CODE ) {
				error( 1 );
			}
		///koniec sprawdznia
		}
		else if( PROG_PART == PROG_DATA ) {
			//wez linijke i wyciagnij z niej dane
			std::string varname;
			float value = 0;
			std::string svalue;

			if( !isSpacesOnly(line) ) {
				varname = oneWord( line );
				if( !isSpacesOnly( line ) ) {
					svalue = oneWord( line );
					//moze byc problem
					std::string::size_type sz;
					value = std::stof( svalue, &sz );
				}
				//save variable in vector
				saveVar( _variables, varname, value );
			}

		}
		else if( PROG_PART == PROG_CODE ) {
			readCode( line );
		}
		line_number++;
	}
}

void Program::readCode( std::string & line ) {
	enum Part {
		left,
		right
	};
	
	std::string recentWord;
	Part part = left;
	recentWord = oneWord(line);

	switch( whatIsThat(recentWord) ) {
	case ifStatment:
		
		break;
	if( part == left ) {

	}
	}
}

void Program::error( int number ) {
}

Program::Program( std::string file_name ) {
	_file.open( file_name, std::ios::in );
	if( !_file.good() ) {
		std::cout << "cannot open the file \"" << file_name << "\"" << std::endl;
		system( "pause" );
		exit(1);
	}
}

Program::~Program() {
}
