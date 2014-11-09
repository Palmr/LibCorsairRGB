#ifdef LIBCORSAIRRGB_EXPORTS
#define LIBCORSAIRRGB_API __declspec(dllexport)
#else
#define LIBCORSAIRRGB_API __declspec(dllimport)
#endif

#include "keymaps.h"

class LibCorsairRGB
{
  static struct libusb_device_handle *device_handle;
  static struct libusb_context *ctxt;
  static unsigned char verbose_level;
  static char red_val[144];
  static char grn_val[144];
  static char blu_val[144];
public:
  // Set the level of verbosity to use when running commands
  static LIBCORSAIRRGB_API void setVerbosityLevel(unsigned char level);

  // Initialise the library by finding and attaching to the keyboard
  static LIBCORSAIRRGB_API int initialise(void);

  // De-Initialise the library by closing handles to the keyboard and giving back control
  static LIBCORSAIRRGB_API void deinitialise(void);

  // Set the RGB value of a key
  static LIBCORSAIRRGB_API void setKey(unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b);
  static LIBCORSAIRRGB_API void setKey(k70_iso_keymap key, unsigned char r, unsigned char g, unsigned char b);

  // Flush the light buffer to the keyboard
  static LIBCORSAIRRGB_API void flushLightBuffer(void);
};
