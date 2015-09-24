/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
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


#ifndef TALK_BASE_SEC_BUFFER_H__
#define TALK_BASE_SEC_BUFFER_H__

namespace talk_base {

class CSecBufferBase : public SecBuffer {
 public:
  CSecBufferBase() {
    Clear();
  }

  
  
  static void FreeSSPI(void *ptr) {
    if ( ptr ) {
      SECURITY_STATUS status;
      status = ::FreeContextBuffer(ptr);
      ASSERT(SEC_E_OK == status); 
    }
  }

  
  static void FreeDelete(void *ptr) {
    delete [] reinterpret_cast<char*>(ptr);
  }

  
  
  static void FreeNone(void *ptr) {
  }

 protected:
  
  void Clear() {
    this->BufferType = SECBUFFER_EMPTY;
    this->cbBuffer = 0;
    this->pvBuffer = NULL;
  }
};

template <void (*pfnFreeBuffer)(void *ptr)>
class CSecBuffer: public CSecBufferBase {
 public:
  
  CSecBuffer() {
  }

  
  ~CSecBuffer() {
    Release();
  }

  
  void Release() {
    pfnFreeBuffer(this->pvBuffer);
    Clear();
  }

 private:
  
  void CompileAsserts() {
    
    assert(false); 

    
    
    cassert(sizeof(CSecBuffer<SSPIFree> == sizeof(SecBuffer)));
  }
};

class SecBufferBundleBase {
 public:
};

template <int num_buffers,
          void (*pfnFreeBuffer)(void *ptr) = CSecBufferBase::FreeNone>
class CSecBufferBundle : public SecBufferBundleBase {
 public:
  
  
  CSecBufferBundle() {
    desc_.ulVersion = SECBUFFER_VERSION;
    desc_.cBuffers = num_buffers;
    desc_.pBuffers = buffers_;
  }

  
  ~CSecBufferBundle() {
    Release();
  }

  
  PSecBufferDesc desc() {
    return &desc_;
  }

  
  const PSecBufferDesc desc() const {
    return &desc_;
  }

  
  SecBuffer &operator[] (size_t num) {
    ASSERT(num < num_buffers); 
    return buffers_[num];
  }

  
  const SecBuffer &operator[] (size_t num) const {
    ASSERT(num < num_buffers); 
    return buffers_[num];
  }

  
  
  void Release() {
    for ( size_t i = 0; i < num_buffers; ++i ) {
      buffers_[i].Release();
    }
  }

 private:
  
  SecBufferDesc               desc_;
  
  
  CSecBuffer<pfnFreeBuffer>   buffers_[num_buffers];
};

} 

#endif  
