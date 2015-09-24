/*
 * libjingle
 * Copyright 2004--2010, Google Inc.
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

#ifndef TALK_BASE_STREAM_H__
#define TALK_BASE_STREAM_H__

#include "talk/base/basictypes.h"
#include "talk/base/criticalsection.h"
#include "talk/base/logging.h"
#include "talk/base/messagehandler.h"
#include "talk/base/scoped_ptr.h"
#include "talk/base/sigslot.h"

namespace talk_base {



enum StreamState { SS_CLOSED, SS_OPENING, SS_OPEN };

enum StreamResult { SR_ERROR, SR_SUCCESS, SR_BLOCK, SR_EOS };

//  SE_WRITE: Data can be written, so Write is likely to not return SR_BLOCK
enum StreamEvent { SE_OPEN = 1, SE_READ = 2, SE_WRITE = 4, SE_CLOSE = 8 };

class Thread;

class StreamInterface : public MessageHandler {
 public:
  virtual ~StreamInterface();

  virtual StreamState GetState() const = 0;

  
  
  
  
  
  
  
  //  SR_SUCCESS: some number of bytes were successfully written, which is
  
  
  
  
  
  virtual StreamResult Read(void* buffer, size_t buffer_len,
                            size_t* read, int* error) = 0;
  virtual StreamResult Write(const void* data, size_t data_len,
                             size_t* written, int* error) = 0;
  
  
  virtual void Close() = 0;

  
  
  
  
  
  
  
  
  sigslot::signal3<StreamInterface*, int, int> SignalEvent;

  
  
  
  void PostEvent(Thread* t, int events, int err);
  
  void PostEvent(int events, int err);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  

  
  
  
  
  
  
  
  virtual const void* GetReadData(size_t* data_len) { return NULL; }
  virtual void ConsumeReadData(size_t used) {}

  
  
  
  
  // written.  GetWriteBuffer does not require a matching call to
  // ConsumeWriteData if no data is written.  Write, ForceWrite, and
  
  
  
  
  
  virtual void* GetWriteBuffer(size_t* buf_len) { return NULL; }
  virtual void ConsumeWriteBuffer(size_t used) {}

  
  
  // was written.  Otherwise, the method is unsupported, or an unrecoverable
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  virtual bool SetPosition(size_t position) { return false; }

  
  
  virtual bool GetPosition(size_t* position) const { return false; }

  
  
  virtual bool GetSize(size_t* size) const { return false; }

  
  
  virtual bool GetAvailable(size_t* size) const { return false; }

  
  
  virtual bool GetWriteRemaining(size_t* size) const { return false; }

  // Communicates the amount of data which will be written to the stream.  The
  
  
  
  
  virtual bool ReserveSize(size_t size) { return true; }

  
  
  
  
  

  
  inline bool Rewind() { return SetPosition(0); }

  
  // data is written, or something other than SR_SUCCESS is returned.  Note that
  // unlike Write, the argument 'written' is always set, and may be non-zero
  
  
  StreamResult WriteAll(const void* data, size_t data_len,
                        size_t* written, int* error);

  
  
  StreamResult ReadAll(void* buffer, size_t buffer_len,
                       size_t* read, int* error);

  
  
  
  
  StreamResult ReadLine(std::string *line);

 protected:
  StreamInterface();

  
  virtual void OnMessage(Message* msg);

 private:
  DISALLOW_EVIL_CONSTRUCTORS(StreamInterface);
};


class StreamAdapterInterface : public StreamInterface,
                               public sigslot::has_slots<> {
 public:
  explicit StreamAdapterInterface(StreamInterface* stream, bool owned = true);

  
  virtual StreamState GetState() const {
    return stream_->GetState();
  }
  virtual StreamResult Read(void* buffer, size_t buffer_len,
                            size_t* read, int* error) {
    return stream_->Read(buffer, buffer_len, read, error);
  }
  virtual StreamResult Write(const void* data, size_t data_len,
                             size_t* written, int* error) {
    return stream_->Write(data, data_len, written, error);
  }
  virtual void Close() {
    stream_->Close();
  }

  


  virtual bool SetPosition(size_t position) {
    return stream_->SetPosition(position);
  }
  virtual bool GetPosition(size_t* position) const {
    return stream_->GetPosition(position);
  }
  virtual bool GetSize(size_t* size) const {
    return stream_->GetSize(size);
  }
  virtual bool GetAvailable(size_t* size) const {
    return stream_->GetAvailable(size);
  }
  virtual bool GetWriteRemaining(size_t* size) const {
    return stream_->GetWriteRemaining(size);
  }
  virtual bool ReserveSize(size_t size) {
    return stream_->ReserveSize(size);
  }

  void Attach(StreamInterface* stream, bool owned = true);
  StreamInterface* Detach();

 protected:
  virtual ~StreamAdapterInterface();

  
  
  virtual void OnEvent(StreamInterface* stream, int events, int err) {
    SignalEvent(this, events, err);
  }
  StreamInterface* stream() { return stream_; }

 private:
  StreamInterface* stream_;
  bool owned_;
  DISALLOW_EVIL_CONSTRUCTORS(StreamAdapterInterface);
};


class StreamTap : public StreamAdapterInterface {
 public:
  explicit StreamTap(StreamInterface* stream, StreamInterface* tap);

  void AttachTap(StreamInterface* tap);
  StreamInterface* DetachTap();
  StreamResult GetTapResult(int* error);

  
  virtual StreamResult Read(void* buffer, size_t buffer_len,
                            size_t* read, int* error);
  virtual StreamResult Write(const void* data, size_t data_len,
                             size_t* written, int* error);

 private:
  scoped_ptr<StreamInterface> tap_;
  StreamResult tap_result_;
  int tap_error_;
  DISALLOW_EVIL_CONSTRUCTORS(StreamTap);
};


class StreamSegment : public StreamAdapterInterface {
 public:
  
  
  explicit StreamSegment(StreamInterface* stream);
  explicit StreamSegment(StreamInterface* stream, size_t length);

  
  virtual StreamResult Read(void* buffer, size_t buffer_len,
                            size_t* read, int* error);
  virtual bool SetPosition(size_t position);
  virtual bool GetPosition(size_t* position) const;
  virtual bool GetSize(size_t* size) const;
  virtual bool GetAvailable(size_t* size) const;

 private:
  size_t start_, pos_, length_;
  DISALLOW_EVIL_CONSTRUCTORS(StreamSegment);
};

// NullStream gives errors on read, and silently discards all written data.

class NullStream : public StreamInterface {
 public:
  NullStream();
  virtual ~NullStream();

  
  virtual StreamState GetState() const;
  virtual StreamResult Read(void* buffer, size_t buffer_len,
                            size_t* read, int* error);
  virtual StreamResult Write(const void* data, size_t data_len,
                             size_t* written, int* error);
  virtual void Close();
};


class FileStream : public StreamInterface {
 public:
  FileStream();
  virtual ~FileStream();

  
  virtual bool Open(const std::string& filename, const char* mode);
  virtual bool OpenShare(const std::string& filename, const char* mode,
                         int shflag);

  
  
  virtual bool DisableBuffering();

  virtual StreamState GetState() const;
  virtual StreamResult Read(void* buffer, size_t buffer_len,
                            size_t* read, int* error);
  virtual StreamResult Write(const void* data, size_t data_len,
                             size_t* written, int* error);
  virtual void Close();
  virtual bool SetPosition(size_t position);
  virtual bool GetPosition(size_t* position) const;
  virtual bool GetSize(size_t* size) const;
  virtual bool GetAvailable(size_t* size) const;
  virtual bool ReserveSize(size_t size);

  bool Flush();

#if defined(POSIX)
  
  
  bool TryLock();
  bool Unlock();
#endif

  
  static bool GetSize(const std::string& filename, size_t* size);

 protected:
  virtual void DoClose();

  FILE* file_;

 private:
  DISALLOW_EVIL_CONSTRUCTORS(FileStream);
};

#ifdef POSIX
class POpenStream : public FileStream {
 public:
  POpenStream() : wait_status_(-1) {}
  virtual ~POpenStream();

  virtual bool Open(const std::string& subcommand, const char* mode);
  
  virtual bool OpenShare(const std::string& subcommand, const char* mode,
                         int shflag);

  
  
  
  int GetWaitStatus() const { return wait_status_; }

 protected:
  virtual void DoClose();

 private:
  int wait_status_;
};
#endif  

// data.  Data is read and written at the current seek position.  Reads return

class MemoryStreamBase : public StreamInterface {
 public:
  virtual StreamState GetState() const;
  virtual StreamResult Read(void* buffer, size_t bytes, size_t* bytes_read,
                            int* error);
  virtual StreamResult Write(const void* buffer, size_t bytes,
                             size_t* bytes_written, int* error);
  virtual void Close();
  virtual bool SetPosition(size_t position);
  virtual bool GetPosition(size_t* position) const;
  virtual bool GetSize(size_t* size) const;
  virtual bool GetAvailable(size_t* size) const;
  virtual bool ReserveSize(size_t size);

  char* GetBuffer() { return buffer_; }
  const char* GetBuffer() const { return buffer_; }

 protected:
  MemoryStreamBase();

  virtual StreamResult DoReserve(size_t size, int* error);

  
  char* buffer_;
  size_t buffer_length_;
  size_t data_length_;
  size_t seek_position_;

 private:
  DISALLOW_EVIL_CONSTRUCTORS(MemoryStreamBase);
};

// MemoryStream dynamically resizes to accomodate written data.

class MemoryStream : public MemoryStreamBase {
 public:
  MemoryStream();
  explicit MemoryStream(const char* data);  
  MemoryStream(const void* data, size_t length);  
  virtual ~MemoryStream();

  void SetData(const void* data, size_t length);

 protected:
  virtual StreamResult DoReserve(size_t size, int* error);
  
  static const int kAlignment = 16;
  char* buffer_alloc_;
};


class ExternalMemoryStream : public MemoryStreamBase {
 public:
  ExternalMemoryStream();
  ExternalMemoryStream(void* data, size_t length);
  virtual ~ExternalMemoryStream();

  void SetData(void* data, size_t length);
};


class FifoBuffer : public StreamInterface {
 public:
  
  explicit FifoBuffer(size_t length);
  virtual ~FifoBuffer();
  
  bool GetBuffered(size_t* data_len) const;
  
  bool SetCapacity(size_t length);

  
  virtual StreamState GetState() const;
  virtual StreamResult Read(void* buffer, size_t bytes,
                            size_t* bytes_read, int* error);
  virtual StreamResult Write(const void* buffer, size_t bytes,
                             size_t* bytes_written, int* error);
  virtual void Close();
  virtual const void* GetReadData(size_t* data_len);
  virtual void ConsumeReadData(size_t used);
  virtual void* GetWriteBuffer(size_t *buf_len);
  virtual void ConsumeWriteBuffer(size_t used);

 private:
  StreamState state_;  
  scoped_array<char> buffer_;  
  size_t buffer_length_;  
  size_t data_length_;  
  size_t read_position_;  
  Thread* owner_;  
  mutable CriticalSection crit_;  
  DISALLOW_EVIL_CONSTRUCTORS(FifoBuffer);
};


class LoggingAdapter : public StreamAdapterInterface {
 public:
  LoggingAdapter(StreamInterface* stream, LoggingSeverity level,
                 const std::string& label, bool hex_mode = false);

  void set_label(const std::string& label);

  virtual StreamResult Read(void* buffer, size_t buffer_len,
                            size_t* read, int* error);
  virtual StreamResult Write(const void* data, size_t data_len,
                             size_t* written, int* error);
  virtual void Close();

 protected:
  virtual void OnEvent(StreamInterface* stream, int events, int err);

 private:
  LoggingSeverity level_;
  std::string label_;
  bool hex_mode_;
  LogMultilineState lms_;

  DISALLOW_EVIL_CONSTRUCTORS(LoggingAdapter);
};


class StringStream : public StreamInterface {
 public:
  explicit StringStream(std::string& str);
  explicit StringStream(const std::string& str);

  virtual StreamState GetState() const;
  virtual StreamResult Read(void* buffer, size_t buffer_len,
                            size_t* read, int* error);
  virtual StreamResult Write(const void* data, size_t data_len,
                             size_t* written, int* error);
  virtual void Close();
  virtual bool SetPosition(size_t position);
  virtual bool GetPosition(size_t* position) const;
  virtual bool GetSize(size_t* size) const;
  virtual bool GetAvailable(size_t* size) const;
  virtual bool ReserveSize(size_t size);

 private:
  std::string& str_;
  size_t read_pos_;
  bool read_only_;
};




class StreamReference : public StreamAdapterInterface {
  class StreamRefCount;
 public:
  
  
  
  explicit StreamReference(StreamInterface* stream);
  StreamInterface* GetStream() { return stream(); }
  StreamInterface* NewReference();
  virtual ~StreamReference();

 private:
  class StreamRefCount {
   public:
    explicit StreamRefCount(StreamInterface* stream)
        : stream_(stream), ref_count_(1) {
    }
    void AddReference() {
      CritScope lock(&cs_);
      ++ref_count_;
    }
    void Release() {
      int ref_count;
      {  
        CritScope lock(&cs_);
        ref_count = --ref_count_;
      }
      if (ref_count == 0) {
        delete stream_;
        delete this;
      }
    }
   private:
    StreamInterface* stream_;
    int ref_count_;
    CriticalSection cs_;
    DISALLOW_EVIL_CONSTRUCTORS(StreamRefCount);
  };

  
  explicit StreamReference(StreamRefCount* stream_ref_count,
                           StreamInterface* stream);

  StreamRefCount* stream_ref_count_;
  DISALLOW_EVIL_CONSTRUCTORS(StreamReference);
};


// end-of-stream (returns SR_EOS), and all the data has been written successful
StreamResult Flow(StreamInterface* source,
                  char* buffer, size_t buffer_len,
                  StreamInterface* sink, size_t* data_len = NULL);


}  

#endif  
