
#define cgp1_h

/* custom type declarations */
typedef unsigned char UBYTE;
typedef unsigned short int USHORT;
typedef unsigned int UINT;
/*typedef unsigned char[4] ID;*/


/* forward dec */
class puzzleBlock;


/* class -- cgp1 () {{{
 * class to manage the computer graphics I
 * file format
 * ****************************** */
/*
 * ID ‚CG1P’ // definiert den Dateityp CG1Puzzle
 * UINT Länge der Datei in Bytes (ohne ID und diese Länge)
 * USHORT Ausgabebreite in Pixeln
 * USHORT Ausgabehöhe in Pixeln
 * USHORT Anzahl der Puzzleteile in der Breite (für die Ausgabe)
 * USHORT Anzahl der Puzzleteile in der Höhe (für die Ausgabe)
 */
class cgp1
{
	private:
		char *m_pchFiletype;
		UINT m_iByteLengh;
		USHORT m_iOutWidth;
		USHORT m_iOutHigh;
		USHORT m_iHorNumberOfParts;
		USHORT m_iVerNumberOfParts;
		puzzleBlock **m_pBlockList;
	public:
		cgp1();
		~cgp1();
		int readFromFile(char *filename);
		void print();
		bool writeToTga(char* filename);
		void sortBlocklist();
};
/* end of cgp1 }}} */ 



/* class -- puzzleBlock () {{{
 * 
 * 
 * ****************************** */
/*
 * ID ‚PZZL’ definiert den Block für ein Puzzleteil
 * UINT Länge des Datenblocks (eines Puzzleteils) in Bytes (ohne ID und Länge)
 * UINT Index zur korrekten Sortierung
 * USHORT Anzahl Komponenten (Bsp. 3 für RGB, 1 für Grau)
 * USHORT Pixelformat (siehe 5.3.)
 */
class puzzleBlock
{
	private:
		friend class cgp1;
		UINT m_iBlockLength;
		UINT m_iIndex;
		USHORT m_iNumberComponents;
		USHORT m_iFormat;
		UBYTE **m_pPixelField;
		long int m_iNumberOfPixels;
	public:
		puzzleBlock();
		~puzzleBlock();
		void deleteMem();
		void print();
};
/* end of }}} */ 
