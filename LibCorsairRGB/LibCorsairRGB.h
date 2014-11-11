#ifdef LIBCORSAIRRGB_EXPORTS
#define LIBCORSAIRRGB_API __declspec(dllexport)
#else
#define LIBCORSAIRRGB_API __declspec(dllimport)
#endif

#include "keymaps.h"

lcrgb_keymap lcrgb_current_keymap;

extern "C" {
	LIBCORSAIRRGB_API extern const unsigned char lcrgb_ISO_UK;
	LIBCORSAIRRGB_API extern const unsigned char lcrgb_ANSI_US;
  LIBCORSAIRRGB_API int lcrgb_set_keymap(unsigned char keymap);

  // Set the level of verbosity to use when running commands
  LIBCORSAIRRGB_API void lcrgb_set_verbosity_level(unsigned char level);

  // Initialise the library by finding and attaching to the keyboard
  LIBCORSAIRRGB_API int lcrgb_initialise(void);

  // De-Initialise the library by closing handles to the keyboard and giving back control
  LIBCORSAIRRGB_API void lcrgb_deinitialise(void);

  // Set the RGB value of a key
  LIBCORSAIRRGB_API void lcrgb_set_position(unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b);
  LIBCORSAIRRGB_API void lcrgb_set_key(lcrgb_key_enum key, unsigned char r, unsigned char g, unsigned char b);
  LIBCORSAIRRGB_API void lcrgb_set_key_code(unsigned char key_code, unsigned char r, unsigned char g, unsigned char b);

  // Flush the light buffer to the keyboard
  LIBCORSAIRRGB_API int lcrgb_flush_light_buffer(void);
};
