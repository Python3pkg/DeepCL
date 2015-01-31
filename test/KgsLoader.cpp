// Copyright Hugh Perkins 2015 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License, 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <string>

#include "FileHelper.h"
#include "stringhelper.h"

#include "KgsLoader.h"

using namespace std;

#undef STATIC
#undef VIRTUAL
#define STATIC
#define VIRTUAL

STATIC int KgsLoader::getNumRecords( std::string filepath ) {
    long filesize = FileHelper::getFilesize( filepath );
    int recordsSize = filesize - 4; // because of 'END' at the end
    int numRecords = recordsSize / getRecordSize();
    return numRecords;
}

STATIC int KgsLoader::loadKgs( std::string filepath, int *p_numPlanes, int *p_boardSize, unsigned char *data, int *labels ) {
    return loadKgs( filepath, p_numPlanes, p_boardSize, data, labels, 0, getNumRecords( filepath ) );
}

STATIC int KgsLoader::loadKgs( std::string filepath, int *p_numPlanes, int *p_boardSize, unsigned char *data, int *labels, int recordStart, int numRecords ) {
    long pos = (long)recordStart * getRecordSize();
    const int recordSize = getRecordSize();
    const int boardSize = 19;
    const int numPlanes = 8;
    const int boardSizeSquared = boardSize * boardSize;
    unsigned char *kgsData = reinterpret_cast<unsigned char *>( FileHelper::readBinaryChunk( filepath, pos, (long)numRecords * recordSize ) );
    for( int n = 0; n < numRecords; n++ ) {
        long recordPos = n * recordSize;
        if( kgsData[recordPos + 0 ] != 'G' ) {
            throw std::runtime_error("alignment error, for record " + toString(n) );
        }
        int row = kgsData[ recordPos + 2 ];
        int col = kgsData[ recordPos + 3 ];
        labels[n] = row * boardSize + col;
        for( int plane = 0; plane < numPlanes; plane++ ) {
            for( int intraBoardPos = 0; intraBoardPos < boardSizeSquared; intraBoardPos++ ) {
                unsigned char thisbyte = kgsData[ recordPos + intraBoardPos + 4 ];
                thisbyte = ( thisbyte >> plane ) & 1;
                data[ ( n * numPlanes + plane * boardSizeSquared ) + intraBoardPos ] = thisbyte;
            }
        }
    }
    *p_numPlanes = numPlanes;
    *p_boardSize = boardSize;
    return numRecords;
}

STATIC int KgsLoader::getRecordSize() {
    const int boardSize = 19;
    const int boardSizeSquared = boardSize * boardSize;
    const int recordSize = 2 + 2 + boardSizeSquared;
    return recordSize;
}

