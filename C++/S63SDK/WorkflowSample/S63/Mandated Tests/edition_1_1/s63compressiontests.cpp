//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63compressiontests.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    S63CompressionTests::S63CompressionTests(const S63ConfigurationObject &config)
      : S63TestBase(config)
      , m_test1Paths()
      , m_test2Paths()
    {
      string configurationFile = config.m_configurationLocation;
      configurationFile.append( "s63compressiontests.ini" ) ;

      readConfiguration(configurationFile);
    }

    S63CompressionTests::~S63CompressionTests()
    {

    }

    void S63CompressionTests::readConfiguration(const string &configFile)
    {
      char buffer[1024];

      //General Info
      TSLProfileHelper::setDefaultFilename( configFile.c_str() );
      TSLProfileHelper::setDefaultSection( "TEST1" );

      int count = 0;
      while ( TSLProfileHelper::lookupProfile("compressedFile", buffer, 1024, "", count) &&
        buffer[0] != '\0' )
      {
        string compressedFile(m_dataLocation);
        compressedFile += buffer;

        if ( !TSLProfileHelper::lookupProfile("comparisonFile", buffer, 1024, "", count++) )
          break;

        string comparisonFile(m_dataLocation);
        comparisonFile += buffer;

        m_test1Paths.push_back( make_pair(compressedFile, comparisonFile) );
      }

      TSLProfileHelper::setDefaultSection( "TEST2" );

      count = 0;
      while ( TSLProfileHelper::lookupProfile("compressedFile", buffer, 1024, "", count++) &&
        buffer[0] != '\0' )
      {
        string compressedFile(m_dataLocation);
        compressedFile += buffer;

        m_test2Paths.push_back( compressedFile );
      }
    }

    bool S63CompressionTests::test1()
    {
      //TSLCompression allows the use of the same decompression algorithm that S63 uses,
      //but only via the ::decompress function and not the ::decompressFile. Therefore
      //we will need to load the file into memory

      //Loop through the files to test
      const size_t numberOfFilesToCheck = m_test1Paths.size();
      for ( size_t i = 0; i < numberOfFilesToCheck; ++i )
      {
        //get the pair of file pathes
        pair<string, string> &  files = m_test1Paths[i];

        struct _stat st1, st2;

        //Find the file size of the compressed file
        //this will fail if file doesn't exist
        if ( _stat( files.first.c_str(), &st1 ) )
          return false;

        //allocate a buffer that can hold the entire compressed file
        unsigned char * fileData = new unsigned char[st1.st_size];

        //if allocation fails we're in trouble. We'd better bomb out now
        if ( !fileData )
          return false;

        //Open the compressed file for reading (binary mode)
        FILE* compressedFile = fopen(files.first.c_str(), "rb");

        //failed to open file - (bad permissions?)
        if ( !compressedFile )
        {
          delete [] fileData;
          return false;
        }

        //read the entire contents of the file in one go
        if ( fread(fileData, 1, st1.st_size, compressedFile) != st1.st_size )
        {
          fclose(compressedFile);
          delete [] fileData;
          return false;
        }

        //close the file handle
        fclose(compressedFile);

        unsigned char * uncompressedData = 0;
        TSLFileLength uncompressedDataLen = 0;
        //Ask MapLink to decompress the compressed memory
        const TSLCompressionResult result = TSLCompression::decompress(fileData, 
          st1.st_size, 
          &uncompressedData, 
          &uncompressedDataLen);

        //delete the memory buffer holding the compressed file
        delete [] fileData;

        //Check that the decompression was successful
        if ( result != TSLSuccess )
          return false;

        //Query the file size of the decompressed file
        if ( _stat( files.second.c_str(), &st2 ) )
        {
          //File doesn't exist?
          TSLCompression::freeBufferData(uncompressedData);
          return false;
        }

        //If the decompressed file doesn't match the decompressed data length
        //then it isn't going to match when we compare the 
        if ( uncompressedDataLen != st2.st_size )
        {
          TSLCompression::freeBufferData(uncompressedData);
          return false;
        }

        //allocate a memory for the entire uncompressed file
        fileData = new unsigned char[st2.st_size];

        //if allocation fails we're in trouble. We'd better bomb out now
        if ( !fileData )
        {
          TSLCompression::freeBufferData(uncompressedData);
          return false;
        }

        //Open the uncompressed file for reading (binary mode)
        FILE* uncompressedFile = fopen(files.second.c_str(), "rb");

        //failed to open file - (bad permissions?)
        if ( !uncompressedFile )
        {
          TSLCompression::freeBufferData(uncompressedData);
          delete [] fileData;
          return false;
        }

        //read the entire contents of the file in one go
        if ( fread(fileData, 1, st2.st_size, uncompressedFile) != st2.st_size )
        {
          TSLCompression::freeBufferData(uncompressedData);
          fclose(uncompressedFile);
          delete [] fileData;
          return false;
        }

        //close the file handle
        fclose(uncompressedFile);

        //do a binary comparison on the data
        const bool dataMatches = memcmp(fileData, uncompressedData, (size_t)uncompressedDataLen) == 0;

        //free the decompressed data
        TSLCompression::freeBufferData(uncompressedData);
        //free the buffer holding the uncompressed file
        delete [] fileData;

        //if the data doesn't match we should abort now
        if ( !dataMatches )
          return false;

      }
      return true;
    }

    bool S63CompressionTests::test2()
    {
      //TSLCompression allows the use of the same decompression algorithm that S63 uses,
      //but only via the ::decompress function and not the ::decompressFile. Therefore
      //we will need to load the file into memory

      //Loop through the files to test
      const size_t numberOfFilesToCheck = m_test2Paths.size();
      for ( size_t i = 0; i < numberOfFilesToCheck; ++i )
      {
        //Get the path to the erroneous file to test
        string &  file = m_test2Paths[i];

        struct _stat st;
        //Find the file size of the compressed file
        //this will fail if file doesn't exist
        if ( _stat( file.c_str(), &st ) )
          return false;

        //allocate a buffer that can hold the entire compressed file
        unsigned char * fileData = new unsigned char[st.st_size];

        //if allocation fails we're in trouble. We'd better bomb out now
        if ( !fileData )
          return false;

        //Open the compressed file for reading (binary mode)
        FILE* compressedFile = fopen(file.c_str(), "rb");

        //failed to open file - (bad permissions?)
        if ( !compressedFile )
        {
          delete [] fileData;
          return false;
        }

        //read the entire contents of the file in one go
        if ( fread(fileData, 1, st.st_size, compressedFile) != st.st_size )
        {
          fclose(compressedFile);
          delete [] fileData;
          return false;
        }

        //close the file handle
        fclose(compressedFile);

        unsigned char * uncompressedData = 0;
        TSLFileLength uncompressedDataLen = 0;
        //Ask MapLink to decompress the compressed memory
        const TSLCompressionResult result = TSLCompression::decompress(fileData, 
          st.st_size, 
          &uncompressedData, 
          &uncompressedDataLen);

        //delete the memory buffer holding the compressed file
        delete [] fileData;

        //On the off chance that the compression worked, free the buffer
        if ( uncompressedData )
        {
          TSLCompression::freeBufferData(uncompressedData);
        }

        //If decompression worked then we'd better bomb out!
        if ( result == TSLSuccess )
          return false;
      }
      return true;
    }

  };
};