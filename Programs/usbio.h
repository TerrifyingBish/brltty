/*
 * BRLTTY - A background process providing access to the Linux console (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2004 by The BRLTTY Team. All rights reserved.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation.  Please see the file COPYING for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#include <stdint.h>

typedef struct {
  uint8_t bLength;            /* Descriptor size in bytes (18). */
  uint8_t bDescriptorType;    /* Descriptor type (1 == device). */
  uint16_t bcdUSB;            /* USB revision number. */
  uint8_t bDeviceClass;       /* Device class. */
  uint8_t bDeviceSubClass;    /* Device subclass. */
  uint8_t bDeviceProtocol;    /* Device protocol. */
  uint8_t bMaxPacketSize0;    /* Maximum packet size in bytes for endpoint 0. */
  uint16_t idVendor;          /* Vendor identifier. */
  uint16_t idProduct;         /* Product identifier. */
  uint16_t bcdDevice;         /* Product revision number. */
  uint8_t iManufacturer;      /* String index for manufacturer name. */
  uint8_t iProduct;           /* String index for product description. */
  uint8_t iSerialNumber;      /* String index for serial number. */
  uint8_t bNumConfigurations; /* Number of configurations. */
} __attribute__((packed)) UsbDeviceDescriptor;

typedef struct {
  uint8_t bLength;             /* Descriptor size in bytes (9). */
  uint8_t bDescriptorType;     /* Descriptor type (2 == configuration). */
  uint16_t wTotalLength;       /* Block size in bytes for all descriptors. */
  uint8_t bNumInterfaces;      /* Number of interfaces. */
  uint8_t bConfigurationValue; /* Configuration number. */
  uint8_t iConfiguration;
  uint8_t bmAttributes;        /* Configuration attributes. */
  uint8_t MaxPower;            /* Maximum power in milliamps. */
} __attribute__((packed)) UsbConfigurationDescriptor;

typedef struct {
  uint8_t bLength;         /* Descriptor size in bytes (2 + numchars/2). */
  uint8_t bDescriptorType; /* Descriptor type (3 == string). */
  uint16_t wData[1];       /* First 16-bit character. */
} __attribute__((packed)) UsbStringDescriptor;

typedef struct {
  uint8_t bLength;            /* Descriptor size in bytes (9). */
  uint8_t bDescriptorType;    /* Descriptor type (4 == interface). */
  uint8_t bInterfaceNumber;   /* Interface number. */
  uint8_t bAlternateSetting;  /* Interface alternative. */
  uint8_t bNumEndpoints;      /* Number of endpoints. */
  uint8_t bInterfaceClass;    /* Interface class. */
  uint8_t bInterfaceSubClass; /* Interface subclass. */
  uint8_t bInterfaceProtocol; /* Interface protocol. */
  uint8_t iInterface;
} __attribute__((packed)) UsbInterfaceDescriptor;

typedef struct {
  uint8_t bLength;          /* Descriptor size in bytes (7). */
  uint8_t bDescriptorType;  /* Descriptor type (5 == endpoint). */
  uint8_t bEndpointAddress; /* Endpoint number (ored with 0X80 if input. */
  uint8_t bmAttributes;     /* Endpoint type and attributes. */
  uint16_t wMaxPacketSiz;   /* Maximum packet size in bytes. */
  uint8_t bInterval;        /* Maximum interval in milliseconds between transfers. */
  uint8_t bRefresh;
  uint8_t bSynchAddress;
} __attribute__((packed)) UsbEndpointDescriptor;

typedef struct {
  uint8_t bLength;         /* Descriptor size in bytes. */
  uint8_t bDescriptorType; /* Descriptor type. */
} __attribute__((packed)) UsbDescriptorHeader;

typedef union {
  UsbDeviceDescriptor device;
  UsbConfigurationDescriptor configuration;
  UsbInterfaceDescriptor interface;
  UsbEndpointDescriptor endpoint;
  UsbStringDescriptor string;
  UsbDescriptorHeader header;
  unsigned char bytes[0XFF];
} UsbDescriptor;

typedef struct {
  uint8_t bRequestType; /* Recipient, direction, and type. */
  uint8_t bRequest;     /* Request code. */
  uint16_t wValue;      /* Request value. */
  uint16_t wIndex;      /* Recipient number (language for strings). */
  uint16_t wLength;     /* Data length in bytes. */
} __attribute__((packed)) UsbSetupPacket;

typedef struct UsbDeviceStruct UsbDevice;
typedef int (*UsbDeviceChooser) (UsbDevice *device, void *data);

extern UsbDevice *usbOpenDevice (const char *path);
extern UsbDevice *usbFindDevice (UsbDeviceChooser chooser, void *data);
extern void usbCloseDevice (UsbDevice *device);
extern const UsbDeviceDescriptor *usbDeviceDescriptor (UsbDevice *device);
extern int usbResetDevice (UsbDevice *device);

extern int usbSetConfiguration (
  UsbDevice *device,
  unsigned int configuration
);

extern char *usbGetDriver (
  UsbDevice *device,
  unsigned int interface
);
extern int usbControlDriver (
  UsbDevice *device,
  unsigned int interface,
  int code,
  void *data
);
extern char *usbGetSerialDevice (
  UsbDevice *device,
  unsigned int interface
);

extern int usbClaimInterface (
  UsbDevice *device,
  unsigned int interface
);
extern int usbReleaseInterface (
  UsbDevice *device,
  unsigned int interface
);
extern int usbSetAlternative (
  UsbDevice *device,
  unsigned int interface,
  unsigned int alternative
);

extern int usbResetEndpoint (
  UsbDevice *device,
  unsigned int endpoint
);
extern int usbClearEndpoint (
  UsbDevice *device,
  unsigned int endpoint
);

extern int usbControlTransfer (
  UsbDevice *device,
  unsigned char recipient,
  unsigned char direction,
  unsigned char type,
  unsigned char request,
  unsigned short value,
  unsigned short index,
  void *data,
  int length,
  int timeout
);
extern int usbGetDescriptor (
  UsbDevice *device,
  unsigned char type,
  unsigned char number,
  unsigned int index,
  UsbDescriptor *descriptor,
  int timeout
);
extern char *usbGetString (
  UsbDevice *device,
  unsigned char number,
  int timeout
);

extern int usbBulkRead (
  UsbDevice *device,
  unsigned char endpoint,
  void *data,
  int length,
  int timeout
);
extern int usbBulkWrite (
  UsbDevice *device,
  unsigned char endpoint,
  void *data,
  int length,
  int timeout
);

extern struct usbdevfs_urb *usbSubmitRequest (
  UsbDevice *device,
  unsigned char endpoint,
  unsigned char type,
  void *buffer,
  int length,
  unsigned int flags,
  void *data
);
extern struct usbdevfs_urb *usbReapRequest (
  UsbDevice *device,
  int wait
);
