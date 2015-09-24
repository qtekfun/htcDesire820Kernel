// Copyright (c) 2010 The WebM project authors. All Rights Reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source



#include "EbmlIDs.h"
#include "EbmlBufferWriter.h"
#include "WebMElement.h"

#include <stdio.h>
int main(int argc, char *argv[]) {
  
  unsigned char data[8192];
  EbmlGlobal ebml;
  ebml.buf = data;
  ebml.offset = 0;
  ebml.length = 8192;

  writeHeader(&ebml);
  {
    EbmlLoc startSegment;
    Ebml_StartSubElement(&ebml, &startSegment, Segment); 
    {
      
      EbmlLoc startInfo;
      Ebml_StartSubElement(&ebml, &startInfo, Info);
      Ebml_SerializeString(&ebml, 0x4D80, "muxingAppLibMkv");
      Ebml_SerializeString(&ebml, 0x5741, "writingAppLibMkv");
      Ebml_EndSubElement(&ebml, &startInfo);
    }

    {
      EbmlLoc trackStart;
      Ebml_StartSubElement(&ebml, &trackStart, Tracks);
      writeVideoTrack(&ebml, 1, 1, "V_MS/VFW/FOURCC", 320, 240, 29.97);
      
      Ebml_EndSubElement(&ebml, &trackStart);
    }

    {
      EbmlLoc clusterStart;
      Ebml_StartSubElement(&ebml, &clusterStart, Cluster); 
      Ebml_SerializeUnsigned(&ebml, Timecode, 0);

      unsigned char someData[4] = {1, 2, 3, 4};
      writeSimpleBlock(&ebml, 1, 0, 1, 0, 0, someData, 4);
      Ebml_EndSubElement(&ebml, &clusterStart);
    }    
    Ebml_EndSubElement(&ebml, &startSegment);
  }

  
  FILE *file_out = fopen("test.mkv", "wb");
  size_t bytesWritten = fwrite(data, 1, ebml.offset, file_out);
  fclose(file_out);
  return 0;
}
