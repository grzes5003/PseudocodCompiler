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
	else if( cr == arrow ) {
		if( s != "<-" ) {
			return false;
		}
	}
	return true;
}

Program::CR_TYPE Program::whatIsThat( std::string & s ) {
	if( s.at(0) == 'i' && s.at(1) == 'f' ) return ifStatment;
	else if( s == "goto" ) return gotoInstruction;
	else if( s == "end" ) return endInstruction;
	else if( s == "<-" ) return arrow;
	else if( s == "==" ) return equal;
	else if( s == "<" ) return lesst;
	else if( s == ">" ) return greatert;
	else if( s == "<=" ) return lessOrEq;
	else if( s == ">=" ) return greaterOrEq;
	else if( s == "!=" ) return notequal;
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

bool Program::IFStatment( std::string & s ) {
	std::string nawias, reszta;
	bool upload = false, resztaUp = false;
	for( unsigned int i = 0; i < s.size(); i++ ) {
		if( s.at( i ) == '(' ) {
			upload = true;
		}
		else if( s.at( i ) == ')' ) {
			upload = false;
			resztaUp = true;
		}
		else if( upload ) {
			nawias += s.at( i );
		}
		else if( resztaUp ) {
			reszta += s.at( i );
		}
	}
	std::string leftPart, midPart, rightPart;
	float leftValue, rightValue;
	//wczytywanie nawiasowych danych
	leftPart = oneWord( nawias );
	midPart = oneWord( nawias );
	rightPart = oneWord( nawias );
	//nie wiem czy to tyle w nawiasie

	/*std::cout << "lewy = \"" << leftPart << "\"" << std::endl;
	std::cout << "srodek = \"" << midPart << "\"" << std::endl;
	std::cout << "prawy = \"" << rightPart << "\"" << std::endl;*/

	//ogarniamy lewa czesc if'a
	if( nazwa == whatIsThat( leftPart ) ) {
		leftValue = valueOfVar( leftPart, _variables );
	}
	else if( value == whatIsThat( leftPart ) ) {
		std::string::size_type sz;
		leftValue = std::stof( leftPart, &sz );
	}
	else {
		error( 1 ); //bad left part of statment, neither name or value
	}

	//ogarniamy prawa czesc if'a
	if( nazwa == whatIsThat( rightPart ) ) {
		rightValue = valueOfVar( rightPart, _variables );
	}
	else if( value == whatIsThat( rightPart ) ) {
		std::string::size_type sz;
		rightValue = std::stof( rightPart, &sz );
	}
	else {
		error( 1 ); //bad right part of statment, neither name or value
	}

	//debugg
	if( DEBBUG_MODE ) {
		std::cout << "co to:" << leftPart << ": druga to:" << rightPart << ":" << std::endl;
		std::cout << "co to:" << whatIsThat( leftPart ) << ": druga to:" << whatIsThat( rightPart ) << std::endl;
		std::cout << "pierwsza wartosc to:" << leftValue << ": druga to:" << rightValue << std::endl;
	}
	//ogarniamy statment w if'e
	switch( whatIsThat( midPart ) ) {
	case equal:
		return (leftValue == rightValue);
		break;
	case lesst:
		return (leftValue < rightValue);
		break;
	case greatert:
		return (leftValue > rightValue);
		break;
	case notequal:
		return (leftValue != rightValue);
		break;
	case greaterOrEq:
		return (leftValue >= rightValue);
		break;
	case lessOrEq:
		return (leftValue <= rightValue);
		break;
	default:
		error( 1 ); //bad midd part of if statment
	}

	s = reszta;
	return false;
}


std::string Program::oneWord( std::string & s ) {
	deleteSpaces( s );
	std::string result;
	int i = 0;
	while( i < s.size() && s.at( i ) != ' ' && s.at(i) != '(' ) { //mam nadzieje ze dzialac bd, po dodaniu '('
		result += s.at( i );
		i++;
	}
	if( i == s.size() )
		i--;
	s.erase(s.begin(),s.begin()+i);
	return result;
}

void Program::saveVar( std::vector<std::pair<std::string,float>>& vec, std::string varName, float varValue ) {
	for( unsigned int i = 0; i < vec.size(); i++ ) {
		if( varName == vec.at( i ).first ) {
			error( 1 ); //variable of this name already exists
		}
	}
	vec.push_back( std::pair<std::string, float>( varName, varValue ) );
}

void Program::read( std::fstream & file ) {
	
	std::string line;
	line_number = 1;

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
	//enum Part {
	//	left,
	//	right
	//};
	
	std::string recentWord;
	//Part part = left;

	std::string varName;

	recentWord = oneWord(line);

	if( DEBBUG_MODE ) {
		std::cout << "recent word1 = " << recentWord << std::endl;
	}

	if( whatIsThat( recentWord ) == ifStatment ) {
		if( !IFStatment( line ) ) {
			if( DEBBUG_MODE ) {
				std::cout << "false value of if statment " << std::endl;
			}
			return;
		}
		recentWord = oneWord( line );
		if( DEBBUG_MODE ) {
			std::cout << "analize an after if statment" << std::endl;
		}
	}

	int number;

	//lewa strona
	switch( whatIsThat(recentWord) ) {
		case nazwa:
			if( DEBBUG_MODE ) {
				std::cout << "left part is variable :" <<recentWord << ": "<< std::endl;

			}
			varName = recentWord;
			break;
		case gotoInstruction:
			//goto linijka
			if( DEBBUG_MODE ) {
				std::cout << "goto instruction" << std::endl;
			}
			number = std::stoi( oneWord( line ) ); //moze byc zle
			return;
		default:
			error( 1 ); //syntax error
	}
	
	recentWord = oneWord( line );

	if( DEBBUG_MODE ) {
		std::cout << "recent word2 = " << recentWord << std::endl;
	}

	if( !isCorrect( recentWord, arrow ) ) {
		error( 1 ); //no arrow in line, syntax error
	}

	recentWord = oneWord( line );

	if( DEBBUG_MODE ) {
		std::cout << "recent word3 = " << recentWord << std::endl;
	}

	float value1,value2,result;

	switch( whatIsThat( recentWord ) ) {
		case nazwa:
			if( recentWord == "sqrt(" || recentWord == "sqrt" ) {
				value1 = sqrt( valueOfVar( oneWord( line ), _variables ) );
				break;
			}
			value1 = valueOfVar(recentWord, _variables);
			break;
		case value:
			std::string::size_type sz;
			value1 = std::stof(recentWord, &sz);
			break;
		default:
			error( 1 ); //syntax error
	}

	recentWord = oneWord( line );

	std::string nextWord = oneWord( line );
	if( whatIsThat( nextWord ) == value ) {
		std::string::size_type sz;
		value2 = std::stof( nextWord, &sz );
	}
	else if( whatIsThat( nextWord ) == nazwa ) {
		value2 = valueOfVar( nextWord, _variables );
	}

	if( recentWord == "+" ) {
		result = value1 + value2;
	}
	else if( recentWord == "-" ) {
		result = value1 - value2;
	}
	else if( recentWord == "/" ) {
		result = (int)(value1 / value2);
	}
	else if( recentWord == "*" ) {
		result = value1 * value2;
	}
	else {
		error( 1 ); //syntax error
	}


	//zapisz pod lewa strona; (pod varName)
	changeVar( _variables, varName, result );
}

void Program::error( int number ) {
	std::cout << "error lol" << std::endl;
}

float Program::valueOfVar( std::string _name, std::vector<std::pair<std::string, float>>& base ) {
	for( unsigned int i = 0; i < base.size(); i++ ) {
		if( _name == base.at( i ).first ) {
			return base.at( i ).second;
		}
	}
	error( 1 ); //cannot find variable whit this name
	return 0.0f;
}

void Program::changeVar( std::vector<std::pair<std::string, float>>& vec, std::string varName, float varNewValue ) {
	valueOfVar( varName, vec );
	int number=0;

	while( number < vec.size() && vec.at(number).first != varName ) {
		number++;
	}
	//mam nazdieje ze nie jest to kolejny numer
	vec.at( number ).second = varNewValue;
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
