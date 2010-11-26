
#include "cg1p.h"
#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
	/* temp return value */
	int r = 0;

	int iFilenameLength = 0;

	char* pchInputFilename;
	char* pchOutputFilename = "output.tga";

	/* check, whether type sizes are right, because
	 * there is no real standart */
	if (sizeof(UBYTE) != 1 ||
		sizeof(USHORT) != 2 ||
		sizeof(UINT) != 4)
	{
		cout << "Your type specifications have not the right size. Please redefine them" << endl;
		return 1;
	}

	/* strip first arg (program name) */
	argv++;
	argc--;

	/* check if enough arguments are given */
	if (argc != 1)
	{
		cout << "Program have to be called with exact one argument!\n";
		return 1;
	}
	else
	{
		pchInputFilename = argv[0];
	}

	/* create a new cgp1 instance */
	cgp1* myfile = new cgp1;

	/* calculate filename length */
	while (pchInputFilename[iFilenameLength] != '\0')
		iFilenameLength++;


	/* read file, if this function returns false
	 * something went wrong */
	r = myfile->readFromFile(pchInputFilename);
	
	if (r == -1)
	{
		cout << "Something went wrong while reading input file. Exiting ..." << endl;
		return 1;
	}
	else if (r == -2)
	{
		cout << "Wrong filetype. Exiting ..." << endl;
		return 1;
	}

	/* sort blocklist */
	myfile->sortBlocklist();

	/* create the output filename */
	/* check, whether ending .cg1 is present */
	if (pchInputFilename[iFilenameLength - 4] == '.' &&
		pchInputFilename[iFilenameLength - 3] == 'c' &&
		pchInputFilename[iFilenameLength - 2] == 'g' &&
		pchInputFilename[iFilenameLength - 1] == '1')
	{
		/* replace ending with .tga */
		pchInputFilename[iFilenameLength - 4] = '.';
		pchInputFilename[iFilenameLength - 3] = 't';
		pchInputFilename[iFilenameLength - 2] = 'g';
		pchInputFilename[iFilenameLength - 1] = 'a';
		/* move pointer */
		pchOutputFilename = pchInputFilename;
	}
	/* if ending ist not present, use default output
	 * file name */
	
	/* write our datastructure to tga file */
	myfile->writeToTga(pchOutputFilename);

	delete myfile;

	return 0;
}
