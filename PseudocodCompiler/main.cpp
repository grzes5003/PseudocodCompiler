#include "program.h"


int main() {

	std::vector<std::pair<std::string, float>> _variables = { 
		std::pair<std::string, float>("alfa", 1),
		std::pair<std::string, float>("betaaa1", 17), 
		std::pair<std::string, float>("a", -15), 
		std::pair<std::string, float>("=beta", 11), 
	};

	std::string b = "if  (  alfa     == =beta )";
	std::string s = "if( 1 == 1 ) alfa <- 1 + 3";
	Program p;
	p._variables = _variables;
	p.readCode( s );
	std::cout << "hehsz" << std::endl;
	//std::cout << s << std::endl;
	system( "pause" );
	return 0;
}