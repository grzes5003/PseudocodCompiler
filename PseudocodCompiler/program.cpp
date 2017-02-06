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
	std::string prefixDEBBUG = "IFStatment(): ";
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
		std::cout << prefixDEBBUG <<"co to:" << leftPart << ": druga to:" << rightPart << ":" << std::endl;
		std::cout << prefixDEBBUG << "co to:" << whatIsThat( leftPart ) << ": druga to:" << whatIsThat( rightPart ) << std::endl;
		std::cout << prefixDEBBUG << "pierwsza wartosc to:" << leftValue << ": druga to:" << rightValue << std::endl;
		std::cout << prefixDEBBUG << "reszta :" << reszta << ": " << std::endl;
	}
	
	s = reszta;
	
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

	return false;
}


std::string Program::oneWord( std::string & s ) {
	deleteSpaces( s );
	std::string result = "";
	int i = 0;
	
	if( s.size() == 0 ) {
		return result;
	}
	
	if( i == 0
		&&( 
		s.at( i ) == ' '
		|| s.at( i ) == '+'
		|| s.at( i ) == '-'
		|| s.at( i ) == '<'
		|| s.at( i ) == '>'
		|| s.at( i ) == '='
		|| s.at( i ) == '*'
		|| s.at( i ) == '/'
			) ) {
		result += s.at( i );
		i++;
	}

	if( result != "*"
		&& result != "-"
		&& result != "+"
		&& result != "/"
		)
	while( i < s.size() 
		&& s.at( i ) != ' '
		&& s.at( i ) != '+'
		&& s.at( i ) != '-' 
		&& s.at( i ) != '<' 
		&& s.at( i ) != '>' 
		&& s.at( i ) != '='
		&& s.at( i ) != '*'
		&& s.at( i ) != '/'
		) {
		result += s.at( i );
		i++;
	}

	if(i>0 )
	if( s.at( i-1) == '='
		|| s.at( i-1 ) == '<'
		|| s.at( i-1 ) == '>' ) {
		result += s.at( i );
		i++;
		//result += s.at( i );
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
	std::string prefixDEBBUG = "read():";

	std::string line;
	line_number = 1;

	while( getline( file, line ) ) {
		deleteSpaces( line );
		if( line.at( 0 ) == '#' ) {
		///sprawdzenie ktory to fragment kodu
			if( DEBBUG_MODE ) {
				std::cout << prefixDEBBUG << "word started with # :" << line << ":" << std::endl;
			}
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
			else if( line == "#END" ) {
				summarise();
			}
		///koniec sprawdznia
		}
		else if( PROG_PART == PROG_DATA ) {
			//wez linijke i wyciagnij z niej dane
			std::string varname;
			float value = 0;
			std::string svalue;

			if( !isSpacesOnly(line) ) {

				line += " "; //naprawa oneWord;
				varname = oneWord( line );

				if( DEBBUG_MODE ) {
					std::cout << prefixDEBBUG << "new var, called :" << varname << ":" << std::endl;
				}
				
				if( !isSpacesOnly( line ) ) {
					svalue = oneWord( line );
					
					if( DEBBUG_MODE ) {
						std::cout << prefixDEBBUG << "value of var (in string) :" << svalue << ":" << std::endl;
					}
					
					//moze byc problem
					std::string::size_type sz;
					value = std::stof( svalue, &sz );
				}
				else
					if( DEBBUG_MODE ) {
						std::cout << prefixDEBBUG << "var, with default var :" << 0.f << ":" << std::endl;
					}
				//save variable in vector
				saveVar( _variables, varname, value );
			}

		}
		else if( PROG_PART == PROG_CODE ) {
			line += " "; //dodalem spacje na koniec by nie bylo problemow ze zostajacymi znakami
			readCode( line );
		}
		line_number++;
	}
}

void Program::readCode( std::string & line ) {
	std::string prefixDEBBUG = "readCode(): ";
	if( DEBBUG_MODE ) {
		std::cout << prefixDEBBUG << "input variable line= \"" << line << "\""<< std::endl;
	}
	//enum Part {
	//	left,
	//	right
	//};
	
	std::string recentWord;
	//Part part = left;

	std::string varName;

	//recentWord = oneWord(line);

	deleteSpaces( line );

	if( whatIsThat( line ) == ifStatment ) {
		if( DEBBUG_MODE ) {
			std::cout << prefixDEBBUG << " IF STATMENT " << std::endl;
			std::cout << prefixDEBBUG << "line now is = " << line << std::endl;
			std::cout << prefixDEBBUG << "word now is = " << recentWord << std::endl;
		}

		if( !IFStatment( line ) ) {
			if( DEBBUG_MODE ) {
				std::cout << prefixDEBBUG << "false value of if statment " << std::endl;
			}
			return;
		}


		if( DEBBUG_MODE ) {
			std::cout << prefixDEBBUG << "analize after if statment" << std::endl;
			std::cout << prefixDEBBUG << "recent line now is = " << line << std::endl;
			std::cout << prefixDEBBUG << "recent word now is = " << recentWord << std::endl;
		}
	}
	recentWord = oneWord( line );
	if( DEBBUG_MODE ) {
		std::cout << prefixDEBBUG << "recent word1 = " << recentWord << std::endl;
	}

	int number;

	//lewa strona
	switch( whatIsThat(recentWord) ) {
		case nazwa:
			if( DEBBUG_MODE ) {
				std::cout << prefixDEBBUG << "left part is variable :" <<recentWord << ": "<< std::endl;

			}
			varName = recentWord;
			break;
		case gotoInstruction:
			//goto linijka
			number = std::stoi( oneWord( line ) ); //moze byc zle //ale jest dobrze
			line_number = number;
			if( DEBBUG_MODE ) {
				std::cout << prefixDEBBUG << "goto instruction, line number:" << number<< ":"<<std::endl;
			}
			return;
		default:
			error( 1 ); //syntax error
	}
	
	recentWord = oneWord( line );

	if( DEBBUG_MODE ) {
		std::cout << prefixDEBBUG << "mid word = " << recentWord << std::endl;
	}

	if( !isCorrect( recentWord, arrow ) ) {
		error( 1 ); //no arrow in line, syntax error
	}

	///--------------
	///prawa strona
	///--------------
	
	recentWord = oneWord( line );

	if( recentWord.size() == 0 ) error( 1 ); //no expression on the right side

	if( DEBBUG_MODE ) {
		std::cout << prefixDEBBUG << "recent word2 = " << recentWord << std::endl;
		std::cout << prefixDEBBUG << "left = :" << line << ": "<< std::endl;
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
	if( DEBBUG_MODE ) {
		std::cout << prefixDEBBUG << "recent word3 (operator) = " << recentWord << std::endl;
		std::cout << prefixDEBBUG << "left = :" << line << ": " << std::endl;
	}

	if( recentWord.size() == 0 ) {
		changeVar( _variables, varName, value1 );
		return;
	}


	std::string nextWord = oneWord( line );
	if( DEBBUG_MODE ) {
		std::cout << prefixDEBBUG << "recent word4 = " << nextWord << std::endl;
		std::cout << prefixDEBBUG << "left = :" << line << ": " << std::endl;
	}
	if( nextWord.size() == 0 ) error( 0 ); //no expression on the right side (<- x * 'missing')

	if( whatIsThat( nextWord ) == value ) {
		std::string::size_type sz;
		value2 = std::stof( nextWord, &sz );
	}
	else if( whatIsThat( nextWord ) == nazwa ) {
		value2 = valueOfVar( nextWord, _variables );
	}

	if( recentWord == "+" ) {
		result = value1 + value2;
		if( DEBBUG_MODE ) {
			std::cout << prefixDEBBUG << "sum, result = " << result << std::endl;
		}
	}
	else if( recentWord == "-" ) {
		result = value1 - value2;
		if( DEBBUG_MODE ) {
			std::cout << prefixDEBBUG << "sub, result = " << result << std::endl;
		}
	}
	else if( recentWord == "/" ) {
		result = (int)(value1 / value2);
		if( DEBBUG_MODE ) {
			std::cout << prefixDEBBUG << "div, result = " << result << std::endl;
		}
	}
	else if( recentWord == "*" ) {
		result = value1 * value2;
		if( DEBBUG_MODE ) {
			std::cout << prefixDEBBUG << "mult, result = " << result << std::endl;
		}
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

void Program::summarise() {
	exit( 0 );
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
	if( DEBBUG_MODE ) {
		std::cout << "changeVar(): variable name :"<<varName<<": in array :" << number<< ": change var to value :"<< varNewValue << ":"<<std::endl;
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
	read(_file);
}

Program::~Program() {
}
