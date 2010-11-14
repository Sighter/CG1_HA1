
#include "cg1p.h"
#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
	/* strip first arg (program name) */
	argv++;
	argc--;

	if (argc != 1)
	{
		cout << "Program have to be called with exact one argument!\n";
		return 1;
	}


	cgp1* myfile = new cgp1;

	myfile->readFromFile(argv[0]);

	myfile->sortBlocklist();

	myfile->writeToTga();

	delete myfile;

	return 0;
}
