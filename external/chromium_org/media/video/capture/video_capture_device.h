// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H_
#define MEDIA_VIDEO_CAPTURE_VIDEO_CAPTURE_DEVICE_H_

#include <list>
#include <string>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "media/base/media_export.h"
#include "media/base/video_frame.h"
#include "media/video/capture/video_capture_types.h"

namespace media {

class MEDIA_EXPORT VideoCaptureDevice {
 public:
  
  
  
  
  
  
  
  
  class MEDIA_EXPORT Name {
   public:
    Name() {}
    Name(const std::string& name, const std::string& id)
        : device_name_(name), unique_id_(id) {}

#if defined(OS_WIN)
    
    enum CaptureApiType {
      MEDIA_FOUNDATION,
      DIRECT_SHOW,
      API_TYPE_UNKNOWN
    };

    Name(const std::string& name,
         const std::string& id,
         const CaptureApiType api_type)
        : device_name_(name), unique_id_(id), capture_api_class_(api_type) {}
#endif  
    ~Name() {}

    
    const std::string& name() const { return device_name_; }

    
    
    const std::string& id() const { return unique_id_; }

    
    
    
    const std::string GetModel() const;

    
    const std::string GetNameAndModel() const;

    
    
    
    bool operator==(const Name& other) const {
      return other.id() == unique_id_;
    }
    bool operator<(const Name& other) const {
      return unique_id_ < other.id();
    }

#if defined(OS_WIN)
    CaptureApiType capture_api_type() const {
      return capture_api_class_.capture_api_type();
    }
#endif  

   private:
    std::string device_name_;
    std::string unique_id_;
#if defined(OS_WIN)
    
    
    class CaptureApiClass {
     public:
      CaptureApiClass():  capture_api_type_(API_TYPE_UNKNOWN) {}
      CaptureApiClass(const CaptureApiType api_type)
          :  capture_api_type_(api_type) {}
      CaptureApiType capture_api_type() const {
        DCHECK_NE(capture_api_type_,  API_TYPE_UNKNOWN);
        return capture_api_type_;
      }
     private:
      CaptureApiType capture_api_type_;
    };

    CaptureApiClass capture_api_class_;
#endif  
    
  };

  
  typedef std::list<Name> Names;

  class MEDIA_EXPORT Client {
   public:
    
    class Buffer : public base::RefCountedThreadSafe<Buffer> {
     public:
      int id() const { return id_; }
      void* data() const { return data_; }
      size_t size() const { return size_; }

     protected:
      friend class base::RefCountedThreadSafe<Buffer>;

      Buffer(int id, void* data, size_t size)
          : id_(id), data_(data), size_(size) {}
      virtual ~Buffer() {}

      const int id_;
      void* const data_;
      const size_t size_;
    };

    virtual ~Client() {}

    
    
    
    
    
    
    
    
    
    
    virtual scoped_refptr<Buffer> ReserveOutputBuffer(
        media::VideoFrame::Format format,
        const gfx::Size& dimensions) = 0;

    
    
    
    
    
    
    
    
    virtual void OnIncomingCapturedFrame(
        const uint8* data,
        int length,
        base::Time timestamp,
        int rotation,  
        const VideoCaptureFormat& frame_format) = 0;

    
    
    
    
    
    
    
    virtual void OnIncomingCapturedBuffer(const scoped_refptr<Buffer>& buffer,
                                          media::VideoFrame::Format format,
                                          const gfx::Size& dimensions,
                                          base::Time timestamp,
                                          int frame_rate) = 0;

    
    
    virtual void OnError() = 0;
  };

  
  
  static VideoCaptureDevice* Create(const Name& device_name);
  virtual ~VideoCaptureDevice();

  
  static void GetDeviceNames(Names* device_names);

  
  
  
  
  static void GetDeviceSupportedFormats(const Name& device,
                                        VideoCaptureFormats* supported_formats);

  
  
  
  virtual void AllocateAndStart(const VideoCaptureParams& params,
                                scoped_ptr<Client> client) = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual void StopAndDeAllocate() = 0;
};

}  

#endif  
