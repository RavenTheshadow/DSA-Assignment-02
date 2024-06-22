#include <crtdbg.h>
#include "restaurant.cpp"
#include "main.h"
int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	string fileName = "test04.txt";


	simulate(fileName);

	DD.clear();
	cout << "SUCCESSS\n";
	_CrtDumpMemoryLeaks();
	return 0;
}