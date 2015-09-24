// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_USB_USB_DEVICE_HANDLE_H_
#define CHROME_BROWSER_USB_USB_DEVICE_HANDLE_H_

#include <map>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "chrome/browser/usb/usb_interface.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"

struct libusb_device_handle;
struct libusb_iso_packet_descriptor;
struct libusb_transfer;

typedef libusb_device_handle* PlatformUsbDeviceHandle;
typedef libusb_iso_packet_descriptor* PlatformUsbIsoPacketDescriptor;
typedef libusb_transfer* PlatformUsbTransferHandle;

class UsbContext;
class UsbConfigDescriptor;
class UsbDevice;
class UsbInterfaceDescriptor;

namespace base {
  class MessageLoopProxy;
}  

namespace net {
class IOBuffer;
}  

enum UsbTransferStatus {
  USB_TRANSFER_COMPLETED = 0,
  USB_TRANSFER_ERROR,
  USB_TRANSFER_TIMEOUT,
  USB_TRANSFER_CANCELLED,
  USB_TRANSFER_STALLED,
  USB_TRANSFER_DISCONNECT,
  USB_TRANSFER_OVERFLOW,
  USB_TRANSFER_LENGTH_SHORT,
};

typedef base::Callback<void(UsbTransferStatus, scoped_refptr<net::IOBuffer>,
    size_t)> UsbTransferCallback;

class UsbDeviceHandle : public base::RefCountedThreadSafe<UsbDeviceHandle> {
 public:
  enum TransferRequestType { STANDARD, CLASS, VENDOR, RESERVED };
  enum TransferRecipient { DEVICE, INTERFACE, ENDPOINT, OTHER };

  scoped_refptr<UsbDevice> device() const;
  PlatformUsbDeviceHandle handle() const { return handle_; }

  
  
  
  
  
  virtual void Close();

  
  
  virtual bool ClaimInterface(const int interface_number);
  virtual bool ReleaseInterface(const int interface_number);
  virtual bool SetInterfaceAlternateSetting(
      const int interface_number,
      const int alternate_setting);
  virtual bool ResetDevice();
  virtual bool GetSerial(base::string16* serial);

  
  virtual void ControlTransfer(const UsbEndpointDirection direction,
                               const TransferRequestType request_type,
                               const TransferRecipient recipient,
                               const uint8 request,
                               const uint16 value,
                               const uint16 index,
                               net::IOBuffer* buffer,
                               const size_t length,
                               const unsigned int timeout,
                               const UsbTransferCallback& callback);

  virtual void BulkTransfer(const UsbEndpointDirection direction,
                            const uint8 endpoint,
                            net::IOBuffer* buffer,
                            const size_t length,
                            const unsigned int timeout,
                            const UsbTransferCallback& callback);

  virtual void InterruptTransfer(const UsbEndpointDirection direction,
                                 const uint8 endpoint,
                                 net::IOBuffer* buffer,
                                 const size_t length,
                                 const unsigned int timeout,
                                 const UsbTransferCallback& callback);

  virtual void IsochronousTransfer(const UsbEndpointDirection direction,
                                   const uint8 endpoint,
                                   net::IOBuffer* buffer,
                                   const size_t length,
                                   const unsigned int packets,
                                   const unsigned int packet_length,
                                   const unsigned int timeout,
                                   const UsbTransferCallback& callback);

 protected:
  friend class base::RefCountedThreadSafe<UsbDeviceHandle>;
  friend class UsbDevice;

  
  UsbDeviceHandle(scoped_refptr<UsbContext> context,
                  UsbDevice* device, PlatformUsbDeviceHandle handle,
                  scoped_refptr<UsbConfigDescriptor> interfaces);

  
  UsbDeviceHandle();
  virtual ~UsbDeviceHandle();

  UsbDevice* device_;

 private:
  friend void HandleTransferCompletion(PlatformUsbTransferHandle handle);

  class InterfaceClaimer;
  struct Transfer;

  
  
  void RefreshEndpointMap();

  
  
  scoped_refptr<InterfaceClaimer> GetClaimedInterfaceForEndpoint(
      unsigned char endpoint);

  
  
  
  void SubmitTransfer(PlatformUsbTransferHandle handle,
                      UsbTransferType transfer_type,
                      net::IOBuffer* buffer,
                      const size_t length,
                      scoped_refptr<base::MessageLoopProxy> message_loop_proxy,
                      const UsbTransferCallback& callback);

  
  
  void TransferComplete(PlatformUsbTransferHandle transfer);

  
  void InternalClose();

  PlatformUsbDeviceHandle handle_;

  scoped_refptr<UsbConfigDescriptor> interfaces_;

  typedef std::map<int, scoped_refptr<InterfaceClaimer> > ClaimedInterfaceMap;
  ClaimedInterfaceMap claimed_interfaces_;

  typedef std::map<PlatformUsbTransferHandle, Transfer> TransferMap;
  TransferMap transfers_;

  
  typedef std::map<int, int> EndpointMap;
  EndpointMap endpoint_map_;

  
  
  scoped_refptr<UsbContext> context_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(UsbDeviceHandle);
};

#endif  
