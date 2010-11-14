
#include <iostream>
#include <fstream>
using namespace std;

#include "cg1p.h"


/* constructor -- cgp1() {{{
 * @ none
 * < none
 * ****************************** */
cgp1::cgp1()
:	m_pchFiletype(NULL),
	m_iByteLengh(0),
	m_iOutWidth(0),
	m_iOutHigh(0),
	m_iHorNumberOfParts(0),
	m_iVerNumberOfParts(0),
	m_pBlockList(NULL)
{

}
/* end of constructor cgp1 }}} */ 



/* destructor -- cgp1() {{{
 * @ none
 * < none
 * ****************************** */
cgp1::~cgp1()
{
	delete[] m_pchFiletype;

	long int i = 0;
	for (i = 0; i < (this->m_iHorNumberOfParts * this->m_iVerNumberOfParts); i++)
		m_pBlockList[i]->deleteMem();
	
	delete[] m_pBlockList;
}
/* end of destructor cgp1 }}} */ 



/* method -- void print() {{{
 * @ none
 * < none
 * ****************************** */
void cgp1::print()
{
	cout	<< "UINT: " << sizeof(UINT) << endl
			<< "UBYTE: " << sizeof(UBYTE) << endl
			<< "USHORT: " << sizeof(USHORT) << endl << endl;

	cout.write(m_pchFiletype, 4);
	cout	<< endl
			<< "**************" << endl
			<< "Length of file in Bytes: " << m_iByteLengh << endl
			<< "Output width in pixels: " << m_iOutWidth << endl
			<< "Output high in pixels: " << m_iOutHigh << endl
			<< "Number of horizontal parts: " << m_iHorNumberOfParts << endl
			<< "Number of vertical parts: " << m_iVerNumberOfParts << endl << endl;

}
/* end of print }}} */ 



/* method -- void readFromFile (char *filename) {{{
 * @ pointer to filename
 * < false if file cant be open
 * ****************************** */
bool cgp1::readFromFile(char *filename)
{
	/* open file in stream */
	ifstream file(filename, ios::in | ios::binary | ios::ate);
	
	if (file.is_open())
	{

		char *pzzlid = new char[4];

		file.seekg (0, ios::beg);

		/* read id */
		m_pchFiletype = new char[4];
		file.read (m_pchFiletype, 4);

		/* read lengh of file */ 
		file.read((char*)&m_iByteLengh, sizeof(UINT));

		/* read output width and high */ 
		file.read((char*)&m_iOutWidth, sizeof(USHORT));
		file.read((char*)&m_iOutHigh, sizeof(USHORT));

		/* read number of parts */ 
		file.read((char*)&m_iHorNumberOfParts, sizeof(USHORT));
		file.read((char*)&m_iVerNumberOfParts, sizeof(USHORT));

		this->print();

		/* create pzzlBlock array */
		m_pBlockList = new puzzleBlock*[m_iHorNumberOfParts * m_iVerNumberOfParts];

		int k = 0;
		for (k = 0; k < (m_iHorNumberOfParts * m_iVerNumberOfParts); k++)
		{
			/* read the first puzzleBlock */
			file.read (pzzlid, 4);
			cout.write(pzzlid, 4);

			/* create new puzzleBlock instance and feed it */
			m_pBlockList[k] = new puzzleBlock;

			file.read((char*)&(m_pBlockList[k]->m_iBlockLength), sizeof(UINT));
			file.read((char*)&(m_pBlockList[k]->m_iIndex), sizeof(UINT));
			file.read((char*)&(m_pBlockList[k]->m_iNumberComponents), sizeof(USHORT));
			file.read((char*)&(m_pBlockList[k]->m_iFormat), sizeof(USHORT));

			/* calculate the number of pixels */
			m_pBlockList[k]->m_iNumberOfPixels = (m_pBlockList[k]->m_iBlockLength - 8) / m_pBlockList[k]->m_iNumberComponents;

			m_pBlockList[k]->print();


			/* create a pixelfield */
			m_pBlockList[k]->m_pPixelField = new UBYTE* [m_pBlockList[k]->m_iNumberOfPixels];


			/* read them all in a two dimensional pointer array */
			long int i = 0;
			int j = 0;

			/* if we have color, we have to read three values*/
			if (this->m_pBlockList[k]->m_iNumberComponents == 3 )
			{	
				/* if we have RGB */
				if (this->m_pBlockList[k]->m_iFormat == 2)
				{
					for (i = 0; i < m_pBlockList[k]->m_iNumberOfPixels; i++)
					{
						m_pBlockList[k]->m_pPixelField[i] = new UBYTE[3];
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][0]), sizeof(UBYTE)); 
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][1]), sizeof(UBYTE)); 
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][2]), sizeof(UBYTE)); 
					}
				}
				/* if we have BGR */
				else if (this->m_pBlockList[k]->m_iFormat == 3)
				{
					for (i = 0; i < m_pBlockList[k]->m_iNumberOfPixels; i++)
					{
						m_pBlockList[k]->m_pPixelField[i] = new UBYTE[3];
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][2]), sizeof(UBYTE)); 
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][1]), sizeof(UBYTE)); 
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][0]), sizeof(UBYTE)); 
					}
				}
				/* if we have GBR */
				else if (this->m_pBlockList[k]->m_iFormat == 4)
				{
					for (i = 0; i < m_pBlockList[k]->m_iNumberOfPixels; i++)
					{
						m_pBlockList[k]->m_pPixelField[i] = new UBYTE[3];
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][1]), sizeof(UBYTE)); 
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][2]), sizeof(UBYTE)); 
						file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][0]), sizeof(UBYTE)); 
					}
				}
			}
			/* if we have only grey, we have to read one value */
			else if (this->m_pBlockList[k]->m_iNumberComponents == 1)
			{
				for (i = 0; i < m_pBlockList[k]->m_iNumberOfPixels; i++)
				{
					m_pBlockList[k]->m_pPixelField[i] = new UBYTE[3];
					file.read((char*)&(m_pBlockList[k]->m_pPixelField[i][0]), sizeof(UBYTE)); 
					m_pBlockList[k]->m_pPixelField[i][1] = m_pBlockList[k]->m_pPixelField[i][0];
					m_pBlockList[k]->m_pPixelField[i][2] = m_pBlockList[k]->m_pPixelField[i][0];
				}
			}


		}	
		if (file.eof())
			cout << "end of file" << endl;
		cout << file.tellg() << endl;
		file.close();


	}
	else
		return false;

	return true;

}
/* end of readFromFile () }}} */ 



/* method -- sortBlocklist() {{{
 * @ none
 * < none
 * ****************************** */
void cgp1::sortBlocklist()
{
	puzzleBlock* pTemp = NULL;

	long int i = 0;
	long int k = 0;
	long int iNumBlocks = this->m_iHorNumberOfParts * this->m_iVerNumberOfParts;
	int iMin;

	cout << "Sorting ...\n";
	for (i = 0; i < iNumBlocks; i++)
	{
		iMin = i;
		for (k = i + 1; k < iNumBlocks; k++)
		{
			if (this->m_pBlockList[k]->m_iIndex < this->m_pBlockList[iMin]->m_iIndex)
				iMin = k;
		}
		pTemp = this->m_pBlockList[i];
		this->m_pBlockList[i] = this->m_pBlockList[iMin];
		this->m_pBlockList[iMin] = pTemp;
	}

	for (i = 0; i < iNumBlocks; i++)
	{
		cout << this->m_pBlockList[i]->m_iIndex << endl;
	}
		
}
/* end of }}} */ 



/* method -- writeToTga() {{{
 * @ none
 * < none
 * ****************************** */
void cgp1::writeToTga()
{
	/* open file in stream */
	ofstream file("outputest.tga", ios::out | ios::trunc | ios::binary);
	
	if (file.is_open())
	{
		/* id length */
		UBYTE idLength = 0;
		file.write((char*)&idLength, sizeof(UBYTE));

		/* Color Map Type, 0 for "true color" */
		UBYTE colMapType = 0;
		file.write((char*)&colMapType, sizeof(UBYTE));
		
		/* Image Type , we use true color uncompressed */
		UBYTE imageType = 2;
		file.write((char*)&imageType, sizeof(UBYTE));

		/* following 5 bytes are for color map specs, witch
		 * we do not use */
		USHORT firstEntryIdx = 0;
		USHORT colorMapLength = 0;
		UBYTE colorMapESize = 0;
		file.write((char*)&firstEntryIdx, sizeof(USHORT));
		file.write((char*)&colorMapLength, sizeof(USHORT));
		file.write((char*)&colorMapESize, sizeof(UBYTE));

		/* following 10 bytes are image specs */
		USHORT imgXorigin = 0;
		USHORT imgYorigin = 0;
		USHORT imgWidth = this->m_iOutWidth;
		USHORT imgHigh = this->m_iOutHigh;
		UBYTE pixelDepth = 24;
		UBYTE attributeByte = 0x20;
		file.write((char*)&imgXorigin, sizeof(USHORT));
		file.write((char*)&imgYorigin, sizeof(USHORT));
		file.write((char*)&imgWidth, sizeof(USHORT));
		file.write((char*)&imgHigh, sizeof(USHORT));
		file.write((char*)&pixelDepth, sizeof(UBYTE));
		file.write((char*)&attributeByte, sizeof(UBYTE));

		/* write data */
		long int i,k,l;
		i = k = l = 0;

		/* calc the block width and height */
		int iBlockWidth = this->m_iOutWidth / this->m_iHorNumberOfParts;
		int iBlockHeight = this->m_iOutHigh / this->m_iVerNumberOfParts;

		/* current x y position of used pixel */
		long int curX = 0;
		long int curY = 0;

		long int curBlock = 0;
		long int curBlockX = 0;
		long int curBlockY = 0;

		while (curY < this->m_iOutHigh)
		{
			curX = 0;
			while (curX < this->m_iOutWidth)
			{
				for (i = 0; i < 3; i++)
					file.write((char*)&(this->m_pBlockList[curBlock]->m_pPixelField[curBlockX + (curBlockY * iBlockWidth)][i]), sizeof(UBYTE));
				
				if (curBlockX == iBlockWidth - 1)
				{
					curBlock++;
					curBlockX = 0;
				}
				else
					curBlockX++;

				curX++;
			}
			if (curBlockY == iBlockHeight - 1)
				curBlockY = 0;
			else
			{
				curBlock = curBlock - this->m_iHorNumberOfParts;
				curBlockY++;
			}
			curY++;
		}

		file.close();
	}

}
/* end of writeToTga }}} */ 



/* constructor -- puzzleBlock() {{{
 * @ none
 * < none
 * ****************************** */
puzzleBlock::puzzleBlock()
:	m_iBlockLength(0),
	m_iIndex(0),
	m_iNumberComponents(0),
	m_iFormat(0),
	m_pPixelField(NULL),
	m_iNumberOfPixels(0)
{

}
/* end of constructor puzzleBlock }}} */ 



/* destructor -- puzzleBlock() {{{
 * @ none
 * < none
 * ****************************** */
void puzzleBlock::deleteMem()
{
	long int i = 0;
	for (i = 0; i < this->m_iNumberOfPixels; i++)
		delete[] m_pPixelField[i];

	delete[] m_pPixelField;
}
/* end of destructor puzzleBlock }}} */ 



/* method -- void puzzleBlock::print() {{{
 * @ none
 * < none
 * ****************************** */
void puzzleBlock::print()
{

	cout	<< "puzzleBlock Instance: " << endl
			<< "*********************" << endl
			<< "Length of block in Bytes: " << m_iBlockLength << endl
			<< "block index: " << m_iIndex << endl
			<< "Number of components: " << m_iNumberComponents << endl
			<< "Number of Pixels: " << m_iNumberOfPixels << endl
			<< "Format: " << m_iFormat << endl << endl;

	//int i = 0;
	//for (i = 0; i < this->m_iNumberComponents; i++)
	//{
	//	cout << "Component " << i << ": " << int(this->m_pComponents[i]) << endl;
	//}

}
/* end of print }}} */ 
