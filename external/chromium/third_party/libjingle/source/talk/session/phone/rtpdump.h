/*
 * libjingle
 * Copyright 2010, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_SESSION_PHONE_RTPDUMP_H_
#define TALK_SESSION_PHONE_RTPDUMP_H_

#include <cstring>
#include <string>
#include <vector>

#include "talk/base/basictypes.h"
#include "talk/base/stream.h"

namespace talk_base {
class ByteBuffer;
}

namespace cricket {


struct RtpDumpFileHeader {
  RtpDumpFileHeader(uint32 start_ms, uint32 s, uint16 p);
  void WriteToByteBuffer(talk_base::ByteBuffer* buf);

  static const std::string kFirstLine;
  static const size_t kHeaderLength = 16;
  uint32 start_sec;   
  uint32 start_usec;  
  uint32 source;      
  uint16 port;        
  uint16 padding;     
};

struct RtpDumpPacket {
  RtpDumpPacket() {}

  RtpDumpPacket(const void* d, size_t s, uint32 elapsed, bool rtcp)
      : elapsed_time(elapsed),
        is_rtcp(rtcp) {
    data.resize(s);
    memcpy(&data[0], d, s);
  }

  bool IsValidRtpPacket() const;
  
  
  bool GetRtpSeqNum(uint16* seq_num) const;
  bool GetRtpTimestamp(uint32* ts) const;
  bool GetRtpSsrc(uint32* ssrc) const;

  static const size_t kHeaderLength = 8;
  uint32 elapsed_time;      
  bool is_rtcp;             
  std::vector<uint8> data;  
};

class RtpDumpReader {
 public:
  explicit RtpDumpReader(talk_base::StreamInterface* stream)
      : stream_(stream),
        file_header_read_(false),
        first_line_and_file_header_len_(0),
        start_time_ms_(0) {
  }
  virtual ~RtpDumpReader() {}

  virtual talk_base::StreamResult ReadPacket(RtpDumpPacket* packet);

 protected:
  talk_base::StreamResult ReadFileHeader();
  bool RewindToFirstDumpPacket() {
    return stream_->SetPosition(first_line_and_file_header_len_);
  }

 private:
  
  bool CheckFirstLine(const std::string& first_line);

  talk_base::StreamInterface* stream_;
  bool file_header_read_;
  size_t first_line_and_file_header_len_;
  uint32 start_time_ms_;
  DISALLOW_COPY_AND_ASSIGN(RtpDumpReader);
};

class RtpDumpLoopReader : public RtpDumpReader {
 public:
  explicit RtpDumpLoopReader(talk_base::StreamInterface* stream);
  virtual talk_base::StreamResult ReadPacket(RtpDumpPacket* packet);

 private:
  
  
  void UpdateStreamStatistics(const RtpDumpPacket& packet);

  
  
  void CalculateIncreases();

  
  
  
  void UpdateDumpPacket(RtpDumpPacket* packet);

  int loop_count_;
  
  
  
  uint32 elapsed_time_increases_;
  uint16 rtp_seq_num_increase_;
  uint32 rtp_timestamp_increase_;
  
  
  
  uint32 packet_count_;
  uint32 frame_count_;
  
  
  uint32 first_elapsed_time_;
  uint16 first_rtp_seq_num_;
  uint32 first_rtp_timestamp_;
  uint32 prev_elapsed_time_;
  uint16 prev_rtp_seq_num_;
  uint32 prev_rtp_timestamp_;

  DISALLOW_COPY_AND_ASSIGN(RtpDumpLoopReader);
};

class RtpDumpWriter {
 public:
  explicit RtpDumpWriter(talk_base::StreamInterface* stream);

  
  
  talk_base::StreamResult WriteRtpPacket(const void* data, size_t data_len) {
    return WritePacket(data, data_len, GetElapsedTime(), false);
  }
  talk_base::StreamResult WriteRtcpPacket(const void* data, size_t data_len) {
    return WritePacket(data, data_len, GetElapsedTime(), true);
  }
  talk_base::StreamResult WritePacket(const RtpDumpPacket& packet) {
    return WritePacket(&packet.data[0], packet.data.size(), packet.elapsed_time,
                       packet.is_rtcp);
  }
  uint32 GetElapsedTime() const;

  bool GetDumpSize(size_t* size) {
    
    
    return stream_ && size && stream_->GetPosition(size);
  }

 protected:
  talk_base::StreamResult WriteFileHeader();

 private:
  talk_base::StreamResult WritePacket(const void* data, size_t data_len,
                                      uint32 elapsed, bool rtcp);

  talk_base::StreamInterface* stream_;
  bool file_header_written_;
  uint32 start_time_ms_;  
  DISALLOW_COPY_AND_ASSIGN(RtpDumpWriter);
};

}  

#endif  
