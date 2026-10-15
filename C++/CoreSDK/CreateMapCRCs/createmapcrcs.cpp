//////////////////////////////////////////////////////////////////////////////////////////
// This example demonstrates one technique for adding a CRC to files produced
// by MapLink Pro.
//
// The CRC has been added to the end of the file. In most cases the addition of additional
// bytes will be ignored by MapLink as each object knows what it is expecting.
//
// To make use of the CRC you will need to add a special callback to the TSLFileLoader.
// This callback will call your code with the data as a TSLifstream. You will then need to
// calculate the CRC and return the approriate status adjusting the stream's length to remove
// the CRC Marker and CRC.
//
// Assumptions:
//   The code assumes that an 'unsigned int' is 32bits in size.
//
// Note: Optimisation is turned of for 64bit windows build to avoid false positive
//       anti-virus result
//
// Copyright (c) 2010-2013 Envitia PLC.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
# ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#  define _WIN32_WINNT 0x0601	// Change this to the appropriate value to target other versions of Windows.
# endif						
# include <stdio.h>
# include <tchar.h>
# include <io.h>
#endif

#include <Windows.h>
#include "MapLink.h"
#include "crc_32.h"
#include <cassert>
#include <stdio.h>
#include <vector>
#include <ctype.h>
#include <iostream>

using std::vector;
using std::cerr;
using std::cout;
using std::endl;


// Endian header flag definition and CRC marker
const unsigned int TCRC = 0x54435243; // ASCII for TCRC
const unsigned int CRCT = 0x43524354; // ASCII for CRCT

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This is called when we list the sub-directories in the starting
// directory.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void directoryListCallback(const TSLSimpleString &foundFile, void *userData)
{
  if (userData == NULL)
  {
    cerr << "Error: directoryListCallback has been passed a NULL pointer for the user data\n";
    exit(0);
  }
  vector<TSLSimpleString> *directories = (vector<TSLSimpleString> *)userData;
  directories->push_back(foundFile);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recursivly descend into the sub-directories collecting them as
// we go.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void listSubDirectories(const TSLSimpleString &path, vector<TSLSimpleString> &subDirectories)
{
  const size_t size = subDirectories.size(); // Record the size of the vector as it is now.

  const char *ptr = path.c_str();
  // Get the sub-directories at this level.
  TSLFileHelper::listSubDirectories(ptr, directoryListCallback, (void*)&subDirectories);

  // Process any new additions so we get the full directory depth and breadth.
  const size_t newSize = subDirectories.size();
  if (size != newSize)
  {
    // you can not use iterators as this is a recursive call and the vector data may be reallocated.
    for (size_t i = size; i < newSize; ++i)
    {
      listSubDirectories(subDirectories[i], subDirectories);
    }
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This is called for each file found in each subdirectory.
//
// The method checks the file does not already have a CRC and if not
// calculates one and adds this to the end of the file.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void crcFileNameCallback(const TSLSimpleString &filename, const TSLSimpleString &directory, void *userData)
{
  // Report file be processed.
  cout << "Processing file '" << filename.c_str() << "'\n";
  cout.flush();

  TSLSimpleString fullPath;
  TSLFileHelper::composePathname(fullPath, directory.c_str(), filename.c_str(), NULL);

  if (TSLFileHelper::fileAccess(fullPath.c_str(), 02 | 04) != 0)
  {
    // Read or Write problem
    cerr << "The file (" << filename.c_str() << ") access mode must be set to read and write. "
      "File has not been processed.\n";
    return;
  }

  // Open the file - binary read (large file open).
  FILE *fin = TSLFileHelper::fopen(fullPath.c_str(), "rb");

  if (fin == NULL)
  {
    perror(filename.c_str());
    cerr << "Unable to open the file (" << filename.c_str() << "). File has not been processed.\n";
    return;
  }
  // We first carryout a sanity check and make sure we have not processed this file first.
  // size of CRC plus marker.
  const TSLFileOffset size = (sizeof(unsigned int) + sizeof(unsigned int));

  // seek to where we would expect the marker to be.
  TSLFileHelper::fseek(fin, -size, SEEK_END);

  // Read in the marker
  unsigned int crcMarker;
  fread(&crcMarker, sizeof(unsigned int), 1, fin);

  if (crcMarker == TCRC || crcMarker == CRCT)
  {
    TSLFileHelper::fclose(fin);
    cerr << "Warning: The file (" << filename.c_str() << ") may already have a marker applied. "
      "File has not been processed.\n";
    return ;
  }
  TSLFileHelper::fseek(fin, 0, SEEK_SET); // seek to the begining so we can start the CRC generation process.
  // Calculate the 32 bit CRC
  UNS_32_BITS crc = 0xffffffff;
  size_t charcnt = 0;

  if (!crc32file(fin, crc, charcnt))
  {
    TSLFileHelper::fclose(fin);
    perror(filename.c_str());
    cerr << "While reading the file '" << filename.c_str() << "' an error was detected."
      "File has not been processed.\n";
    return ;
  }

  // close the file.
  TSLFileHelper::fclose(fin);

  // Sanity Check. Make sure we processed the same number of bytes as the file size reports.
  const TSLFileLength lengthOfFile = TSLFileHelper::fileSize(fullPath.c_str());
  if (lengthOfFile != charcnt)
  {
    cerr << "Warning: The reported file length does not match the number of "
      "bytes processed for calculating the CRC (" << filename.c_str() << ").\n";
    cerr << "Length processed " << charcnt << " Actual length " << lengthOfFile << endl;
    // We continue in any case...
  }

  // Add the CRC to the end of the file.
  fin = TSLFileHelper::fopen(fullPath.c_str(), "a+b"); // append at end in binary mode (removes EOF marker before write.).
  if (fin == NULL)
  {
    perror(filename.c_str());
    cerr << "Unable to open the file (" << filename.c_str() << "). File has not been processed.\n";
    return;
  }
  TSLFileHelper::fseek(fin, 0, SEEK_END);      // Append the CRC Marker and CRC to the end of the file (should not be required).

  fwrite(&TCRC, sizeof(unsigned int), 1, fin); // write the CRC marker out. This allows us to work out the endian'ess of the reader.
  fwrite(&crc, sizeof(unsigned int), 1, fin);  // write the CRC out.

  // fini.
  TSLFileHelper::fclose(fin);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// usage - description of command line options
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void usage( const char * appname ) 
{
  cout <<  "Usage : " << appname << " [-h | -help] [-f full_path_to_directory]\n";
  exit( 0 ) ;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Main program entry point.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[])
{
  if( argc < 3 )
  {
    usage( argv[0] );
    return -1;
  }

  TSLSimpleString path ;

  assert(sizeof(unsigned int) == 4); // This must be 4 bytes (32bits).

  // Simple command line parser
  int c = 0;
  while (c < argc)
  {
    if (strcmp(argv[c], "-h") == 0 ||
        strcmp(argv[c], "-help") == 0)
    {
      usage( argv[0] );
    }
    else
    if (strcmp(argv[c], "-f") == 0)
    {
      c++;
      char *ptr = argv[c];
      while(isspace(*ptr)) ptr ++;  // skip white space

      path = ptr;
    }
    ++c;
  }

  // Check the starting point exists.
  if (!TSLFileHelper::directoryExists(path.c_str()))
  {
    cerr << "The starting directory does not exist: Directory " << path.c_str() << ". Trying the path as a file\n";

    // See if we have a filename
    if (!TSLFileHelper::fileExists(path.c_str()))
    {
      cerr << "The starting directory does not exist: Directory " << path.c_str() << endl;
      return -1;
    }
     // Extract the directory.
    TSLSimpleString directory, basename, extension;
    TSLFileHelper::decomposePathname(path.c_str(), directory, basename, extension);

    path = directory;
  }

  // Obtain all the sub-directories.
  vector<TSLSimpleString> subDirectories;
  listSubDirectories(path, subDirectories);

  // put the initial directory into the pool or we miss any toplevel files.
  subDirectories.push_back(path);

  vector<TSLSimpleString>::const_iterator i(subDirectories.begin());
  vector<TSLSimpleString>::const_iterator end(subDirectories.end());

  // Calculate the CRC's and add to files.
  while (i != end)
  {
    TSLFileHelper::listDirectory((*i).c_str(), crcFileNameCallback, NULL);
    ++i;
  }

  return 0;
}

