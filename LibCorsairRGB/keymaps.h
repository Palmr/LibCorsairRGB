#ifndef KEYMAPS_H
#define KEYMAPS_H
  #ifdef LIBCORSAIRRGB_EXPORTS
  #define LIBCORSAIRRGB_API __declspec(dllexport)
  #else
  #define LIBCORSAIRRGB_API __declspec(dllimport)
  #endif

	#include <map>

	extern "C" {
		// Enum of key names
		LIBCORSAIRRGB_API enum lcrgb_key_enum {
		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
		f7,
		f8,
		f9,
		f10,
		f11,
		f12,
		num0,
		num1,
		num2,
		num3,
		num4,
		num5,
		num6,
		num7,
		num8,
		num9,
		minus,
		equals,
		numpad0,
		numpad1,
		numpad2,
		numpad3,
		numpad4,
		numpad5,
		numpad7,
		numpad6,
		numpad8,
		numpad9,
		numlock,
		numpaddivide,
		numpadmultiply,
		numpadminus,
		numpadplus,
		numpadenter,
		numpaddecimal,
		a,
		b,
		c,
		d,
		e,
		f,
		g,
		h,
		i,
		j,
		k,
		l,
		m,
		n,
		o,
		p,
		q,
		r,
		s,
		t,
		u,
		v,
		w,
		x,
		y,
		z,
		arrowup,
		arrowdown,
		arrowleft,
		arrowright,
		mediastop,
		mediaprev,
		mediaplaypause,
		medianext,
		mute,
		del,
		insert,
		end,
		home,
		pagedown,
		pageup,
		lshift,
		rshift,
		lctrl,
		rctrl,
		backslash,
		forwardslash,
		lwinkey,
		rwinkey,
		alt,
		altgr,
		rsquare,
		lsquare,
		printscreen,
		scolllock,
		pausebreak,
		winlock,
		brightness,
		comma,
		period,
		hash,
		semicolon,
		escape,
		grave,
		tab,
		capslock,
		backspace,
		enter,
		space,
		contextmenu,
		apostrophe,
		macrorecord,
		macrorecord1,
		macrorecord2,
		macrorecord3,
		g1,
		g2,
		g3,
		g4,
		g5,
		g6,
		g7,
		g8,
		g9,
		g10,
		g11,
		g12,
		g13,
		g14,
		g15,
		g16,
		g17,
		g18,

		missing
	};

		/*
		LIBCORSAIRRGB_API struct keyboard_matrix {
			int width;
			int height;
			lcrgb_key_enum *matrix;
		};

		keyboard_matrix k70_uk_matrix2;*/

		// 2d array ty map key positions in x,y to key name
		extern LIBCORSAIRRGB_API lcrgb_key_enum k70_uk_matrix[7][22];
	};

	// Map of enums to USB keycodes
	typedef std::map<lcrgb_key_enum, unsigned char> lcrgb_keymap;

	// stub to create a keymap with the uk keycode values
	lcrgb_keymap init_uk_keymap(void);
	// stub to create a keymap with the us keycode values
	lcrgb_keymap init_us_keymap(void);

	// uk keymap created using the stub above
	extern lcrgb_keymap UK_KEYMAP;
	// us keymap created using the stub above
	extern lcrgb_keymap US_KEYMAP;
#endif
