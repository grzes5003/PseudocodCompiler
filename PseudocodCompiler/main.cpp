#include "program.h"


int main() {

	std::vector<std::pair<std::string, float>> _variables = { 
		std::pair<std::string, float>("alfa", 1),
		std::pair<std::string, float>("betaaa1", 17), 
		std::pair<std::string, float>("a", -15), 
		std::pair<std::string, float>("beta", 11), 
	};

	std::string b = "if(alfa<=5)  beta <- betaaa1* a  ";
	std::string s = " goto 15  ";
	Program p("E:/Projekty/Kodowe/STUDIA_1_ROK/DODATKOWE/NAUKA_CPP/PseudocodCompiler/Debug/code.txt");
	
	p._variables = _variables;
	//p.readCode( s );
	//p.IFStatment( b );
	//std::cout << s << std::endl;
	//std::cout << s << std::endl;
	
	_variables = p._variables;
	for( unsigned int i = 0; 0; i++ ) {
		std::cout << "--------------" << std::endl;
		std::cout << p.oneWord(s) << std::endl;
		//std::cout << "reszta to:"<< b << std::endl;
	}
	
	system( "pause" );
	return 0;
}