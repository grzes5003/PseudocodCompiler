#include "program.h"


int main() {
	std::string s = "xyzvbnvbn  ";
	Program p;
	std::cout << p.oneWord( s ) << std::endl;
	std::cout << s.size() << std::endl;
	system( "pause" );
	return 0;
}