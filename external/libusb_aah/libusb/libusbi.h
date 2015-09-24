/*
 * Internal header for libusb
 * Copyright (C) 2007-2009 Daniel Drake <dsd@gentoo.org>
 * Copyright (c) 2001 Johannes Erdfelt <johannes@erdfelt.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef LIBUSBI_H
#define LIBUSBI_H

#include <config.h>

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdarg.h>
#ifdef HAVE_POLL_H
#include <poll.h>
#endif

#include <libusb.h>
#include <version.h>

#define API_EXPORTED LIBUSB_CALL DEFAULT_VISIBILITY

#define DEVICE_DESC_LENGTH		18

#define USB_MAXENDPOINTS	32
#define USB_MAXINTERFACES	32
#define USB_MAXCONFIG		8

struct list_head {
	struct list_head *prev, *next;
};

#define list_entry(ptr, type, member) \
	((type *)((uintptr_t)(ptr) - (uintptr_t)(&((type *)0L)->member)))

#define list_for_each_entry(pos, head, member, type)			\
	for (pos = list_entry((head)->next, type, member);			\
		 &pos->member != (head);								\
		 pos = list_entry(pos->member.next, type, member))

#define list_for_each_entry_safe(pos, n, head, member, type)	\
	for (pos = list_entry((head)->next, type, member),			\
		 n = list_entry(pos->member.next, type, member);		\
		 &pos->member != (head);								\
		 pos = n, n = list_entry(n->member.next, type, member))

#define list_empty(entry) ((entry)->next == (entry))

static inline void list_init(struct list_head *entry)
{
	entry->prev = entry->next = entry;
}

static inline void list_add(struct list_head *entry, struct list_head *head)
{
	entry->next = head->next;
	entry->prev = head;

	head->next->prev = entry;
	head->next = entry;
}

static inline void list_add_tail(struct list_head *entry,
	struct list_head *head)
{
	entry->next = head;
	entry->prev = head->prev;

	head->prev->next = entry;
	head->prev = entry;
}

static inline void list_del(struct list_head *entry)
{
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;
}

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *mptr = (ptr);    \
        (type *)( (char *)mptr - offsetof(type,member) );})

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))

#define TIMESPEC_IS_SET(ts) ((ts)->tv_sec != 0 || (ts)->tv_nsec != 0)

enum usbi_log_level {
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
};

void usbi_log(struct libusb_context *ctx, enum usbi_log_level level,
	const char *function, const char *format, ...);

void usbi_log_v(struct libusb_context *ctx, enum usbi_log_level level,
	const char *function, const char *format, va_list args);

#if !defined(_MSC_VER) || _MSC_VER >= 1400

#ifdef ENABLE_LOGGING
#define _usbi_log(ctx, level, ...) usbi_log(ctx, level, __FUNCTION__, __VA_ARGS__)
#else
#define _usbi_log(ctx, level, ...) do { (void)(ctx); } while(0)
#endif

#ifdef ENABLE_DEBUG_LOGGING
#define usbi_dbg(...) _usbi_log(NULL, LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define usbi_dbg(...) do {} while(0)
#endif

#define usbi_info(ctx, ...) _usbi_log(ctx, LOG_LEVEL_INFO, __VA_ARGS__)
#define usbi_warn(ctx, ...) _usbi_log(ctx, LOG_LEVEL_WARNING, __VA_ARGS__)
#define usbi_err(ctx, ...) _usbi_log(ctx, LOG_LEVEL_ERROR, __VA_ARGS__)

#else 


static inline void usbi_info(struct libusb_context *ctx, const char *fmt, ...)
{
#ifdef ENABLE_LOGGING
	va_list args;
	va_start(args, fmt);
	usbi_log_v(ctx, LOG_LEVEL_INFO, "", fmt, args);
	va_end(args);
#else
	(void)ctx;
#endif
}

static inline void usbi_warn(struct libusb_context *ctx, const char *fmt, ...)
{
#ifdef ENABLE_LOGGING
	va_list args;
	va_start(args, fmt);
	usbi_log_v(ctx, LOG_LEVEL_WARNING, "", fmt, args);
	va_end(args);
#else
	(void)ctx;
#endif
}

static inline void usbi_err(struct libusb_context *ctx, const char *fmt, ...)
{
#ifdef ENABLE_LOGGING
	va_list args;
	va_start(args, fmt);
	usbi_log_v(ctx, LOG_LEVEL_ERROR, "", fmt, args);
	va_end(args);
#else
	(void)ctx;
#endif
}

static inline void usbi_dbg(const char *fmt, ...)
{
#ifdef ENABLE_DEBUG_LOGGING
	va_list args;
	va_start(args, fmt);
	usbi_log_v(NULL, LOG_LEVEL_DEBUG, "", fmt, args);
	va_end(args);
#else
	(void)fmt;
#endif
}

#endif 

#define USBI_GET_CONTEXT(ctx) if (!(ctx)) (ctx) = usbi_default_context
#define DEVICE_CTX(dev) ((dev)->ctx)
#define HANDLE_CTX(handle) (DEVICE_CTX((handle)->dev))
#define TRANSFER_CTX(transfer) (HANDLE_CTX((transfer)->dev_handle))
#define ITRANSFER_CTX(transfer) \
	(TRANSFER_CTX(USBI_TRANSFER_TO_LIBUSB_TRANSFER(transfer)))

#define IS_EPIN(ep) (0 != ((ep) & LIBUSB_ENDPOINT_IN))
#define IS_EPOUT(ep) (!IS_EPIN(ep))
#define IS_XFERIN(xfer) (0 != ((xfer)->endpoint & LIBUSB_ENDPOINT_IN))
#define IS_XFEROUT(xfer) (!IS_XFERIN(xfer))

#if defined(THREADS_POSIX)
#include <os/threads_posix.h>
#elif defined(OS_WINDOWS)
#include <os/threads_windows.h>
#endif

#if defined(OS_LINUX) || defined(OS_DARWIN) || defined(OS_OPENBSD)
#include <unistd.h>
#include <os/poll_posix.h>
#elif defined(OS_WINDOWS)
#include <os/poll_windows.h>
#endif

#if defined(OS_WINDOWS) && !defined(__GCC__)
#undef HAVE_GETTIMEOFDAY
int usbi_gettimeofday(struct timeval *tp, void *tzp);
#define LIBUSB_GETTIMEOFDAY_WIN32
#define HAVE_USBI_GETTIMEOFDAY
#else
#ifdef HAVE_GETTIMEOFDAY
#define usbi_gettimeofday(tv, tz) gettimeofday((tv), (tz))
#define HAVE_USBI_GETTIMEOFDAY
#endif
#endif

extern struct libusb_context *usbi_default_context;

struct libusb_context {
	int debug;
	int debug_fixed;

	int ctrl_pipe[2];

	struct list_head usb_devs;
	usbi_mutex_t usb_devs_lock;

	struct list_head open_devs;
	usbi_mutex_t open_devs_lock;

	struct list_head flying_transfers;
	usbi_mutex_t flying_transfers_lock;

	
	struct list_head pollfds;
	usbi_mutex_t pollfds_lock;

	unsigned int pollfd_modify;
	usbi_mutex_t pollfd_modify_lock;

	
	libusb_pollfd_added_cb fd_added_cb;
	libusb_pollfd_removed_cb fd_removed_cb;
	void *fd_cb_user_data;

	
	usbi_mutex_t events_lock;

	
	int event_handler_active;

	usbi_mutex_t event_waiters_lock;
	usbi_cond_t event_waiters_cond;

#ifdef USBI_TIMERFD_AVAILABLE
	int timerfd;
#endif
};

#ifdef USBI_TIMERFD_AVAILABLE
#define usbi_using_timerfd(ctx) ((ctx)->timerfd >= 0)
#else
#define usbi_using_timerfd(ctx) (0)
#endif

struct libusb_device {
	usbi_mutex_t lock;
	int refcnt;

	struct libusb_context *ctx;

	uint8_t bus_number;
	uint8_t device_address;
	uint8_t num_configurations;
	enum libusb_speed speed;

	struct list_head list;
	unsigned long session_data;
	unsigned char os_priv[0];
};

struct libusb_device_handle {
	
	usbi_mutex_t lock;
	unsigned long claimed_interfaces;

	struct list_head list;
	struct libusb_device *dev;
	unsigned char os_priv[0];
};

enum {
  USBI_CLOCK_MONOTONIC,
  USBI_CLOCK_REALTIME
};


struct usbi_transfer {
	int num_iso_packets;
	struct list_head list;
	struct timeval timeout;
	int transferred;
	uint8_t flags;

	usbi_mutex_t lock;
};

enum usbi_transfer_flags {
	
	USBI_TRANSFER_TIMED_OUT = 1 << 0,

	
	USBI_TRANSFER_OS_HANDLES_TIMEOUT = 1 << 1,

	
	USBI_TRANSFER_CANCELLING = 1 << 2,

	
	USBI_TRANSFER_DEVICE_DISAPPEARED = 1 << 3,
};

#define USBI_TRANSFER_TO_LIBUSB_TRANSFER(transfer) \
	((struct libusb_transfer *)(((unsigned char *)(transfer)) \
		+ sizeof(struct usbi_transfer)))
#define LIBUSB_TRANSFER_TO_USBI_TRANSFER(transfer) \
	((struct usbi_transfer *)(((unsigned char *)(transfer)) \
		- sizeof(struct usbi_transfer)))

static inline void *usbi_transfer_get_os_priv(struct usbi_transfer *transfer)
{
	return ((unsigned char *)transfer) + sizeof(struct usbi_transfer)
		+ sizeof(struct libusb_transfer)
		+ (transfer->num_iso_packets
			* sizeof(struct libusb_iso_packet_descriptor));
}


struct usb_descriptor_header {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
};


int usbi_io_init(struct libusb_context *ctx);
void usbi_io_exit(struct libusb_context *ctx);

struct libusb_device *usbi_alloc_device(struct libusb_context *ctx,
	unsigned long session_id);
struct libusb_device *usbi_get_device_by_session_id(struct libusb_context *ctx,
	unsigned long session_id);
int usbi_sanitize_device(struct libusb_device *dev);
void usbi_handle_disconnect(struct libusb_device_handle *handle);

int usbi_handle_transfer_completion(struct usbi_transfer *itransfer,
	enum libusb_transfer_status status);
int usbi_handle_transfer_cancellation(struct usbi_transfer *transfer);

int usbi_parse_descriptor(unsigned char *source, const char *descriptor,
	void *dest, int host_endian);
int usbi_get_config_index_by_value(struct libusb_device *dev,
	uint8_t bConfigurationValue, int *idx);


struct usbi_pollfd {
	
	struct libusb_pollfd pollfd;

	struct list_head list;
};

int usbi_add_pollfd(struct libusb_context *ctx, int fd, short events);
void usbi_remove_pollfd(struct libusb_context *ctx, int fd);
void usbi_fd_notification(struct libusb_context *ctx);


struct discovered_devs {
	size_t len;
	size_t capacity;
	struct libusb_device *devices[0];
};

struct discovered_devs *discovered_devs_append(
	struct discovered_devs *discdevs, struct libusb_device *dev);


struct usbi_os_backend {
	
	const char *name;

	int (*init)(struct libusb_context *ctx);

	void (*exit)(void);

	int (*get_device_list)(struct libusb_context *ctx,
		struct discovered_devs **discdevs);

	int (*open)(struct libusb_device_handle *handle);

	void (*close)(struct libusb_device_handle *handle);

	int (*get_device_descriptor)(struct libusb_device *device,
		unsigned char *buffer, int *host_endian);

	int (*get_active_config_descriptor)(struct libusb_device *device,
		unsigned char *buffer, size_t len, int *host_endian);

	int (*get_config_descriptor)(struct libusb_device *device,
		uint8_t config_index, unsigned char *buffer, size_t len,
		int *host_endian);

	int (*get_configuration)(struct libusb_device_handle *handle, int *config);

	int (*set_configuration)(struct libusb_device_handle *handle, int config);

	int (*claim_interface)(struct libusb_device_handle *handle, int interface_number);

	int (*release_interface)(struct libusb_device_handle *handle, int interface_number);

	int (*set_interface_altsetting)(struct libusb_device_handle *handle,
		int interface_number, int altsetting);

	int (*clear_halt)(struct libusb_device_handle *handle,
		unsigned char endpoint);

	int (*reset_device)(struct libusb_device_handle *handle);

	int (*kernel_driver_active)(struct libusb_device_handle *handle,
		int interface_number);

	int (*detach_kernel_driver)(struct libusb_device_handle *handle,
		int interface_number);

	int (*attach_kernel_driver)(struct libusb_device_handle *handle,
		int interface_number);

	void (*destroy_device)(struct libusb_device *dev);

	int (*submit_transfer)(struct usbi_transfer *itransfer);

	int (*cancel_transfer)(struct usbi_transfer *itransfer);

	void (*clear_transfer_priv)(struct usbi_transfer *itransfer);

	int (*handle_events)(struct libusb_context *ctx,
		struct pollfd *fds, POLL_NFDS_TYPE nfds, int num_ready);

	int (*clock_gettime)(int clkid, struct timespec *tp);

#ifdef USBI_TIMERFD_AVAILABLE
	
	clockid_t (*get_timerfd_clockid)(void);
#endif

	size_t device_priv_size;

	size_t device_handle_priv_size;

	size_t transfer_priv_size;

	size_t add_iso_packet_size;
};

extern const struct usbi_os_backend * const usbi_backend;

extern const struct usbi_os_backend linux_usbfs_backend;
extern const struct usbi_os_backend darwin_backend;
extern const struct usbi_os_backend openbsd_backend;
extern const struct usbi_os_backend windows_backend;

#endif

