/*
 * Usage of CDK Matrix
 *
 * File:   example1.cc
 * Author: Stephen Perkins
 * Email:  stephen.perkins@utdallas.edu
 */

#include <iostream>
#include "cdk.h"
#include <vector>
#include <fstream>
#include <stdint.h>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20 
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

const int maxLength = 25;

class BinaryFileHeader{
	public:
		uint32_t mNum;
		uint32_t vNum;
		uint64_t recNum;

};

class BinaryFileRecord{
	public:
		uint8_t sLength;
		char sBuffer[maxLength];
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"C0", "a", "b", "c"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);
  ifstream binFile("cs3377.bin", ios::in | ios::binary);
  BinaryFileHeader *headerFile = new BinaryFileHeader();
  BinaryFileRecord *records = new BinaryFileRecord();

  binFile.read((char*) headerFile, sizeof(BinaryFileHeader));

  char let[256];

  sprintf(let, "Magic: 0x%0X", headerFile -> mNum);
  setCDKMatrixCell(myMatrix, 1, 1, let);

  sprintf(let, "Version: %d", headerFile -> vNum);
  setCDKMatrixCell(myMatrix, 1, 2, let);

  sprintf(let, "NumRecords: %ld", headerFile->recNum);
  setCDKMatrixCell(myMatrix, 1, 3, let);

  drawCDKMatrix(myMatrix, true);

  






  for(int i = 0; i < (int)headerFile->recNum; i++){
	binFile.read((char*) records, sizeof(BinaryFileRecord));
	sprintf(let, "strlen: %d", records->sLength);
	int raj = i+ 2;
	setCDKMatrixCell(myMatrix, raj, 1, let);
	setCDKMatrixCell(myMatrix, raj, 2, records->sBuffer);

 }
 


  /*
   * Dipslay a message
   */
//  setCDKMatrixCell(myMatrix, 2, 2, "Test Message");
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
