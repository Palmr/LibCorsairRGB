#include "stdafx.h"
#include "LibCorsairRGB.h"
#include "keymaps.h"
#include "libusb-1.0\libusb.h"
#include <stdio.h>

#define CORSAIR_RGB_VID 0x1B1C
#define CORSAIR_K70_RGB_PID 0x1B13
#define CORSAIR_K95_RGB_PID 0x1B11
 

libusb_context *ctxt = NULL;
struct libusb_device_handle *device_handle;
unsigned char verbose_level = 0;
char red_val[144];
char grn_val[144];
char blu_val[144];
//lcrgb_keymap lcrgb_current_keymap = NULL;

extern "C" {

	LIBCORSAIRRGB_API const unsigned char lcrgb_ISO_UK = 1;
	LIBCORSAIRRGB_API const unsigned char lcrgb_ANSI_US = 2;

	
  int lcrgb_set_keymap(unsigned char keymap) {
		switch (keymap) {
			case lcrgb_ISO_UK:
				lcrgb_current_keymap = UK_KEYMAP;
				break;
			case lcrgb_ANSI_US:
				lcrgb_current_keymap = US_KEYMAP;
				break;
			default:
				fprintf(stderr, "Cannot set keymap to %d, unkown keymap id\n", keymap);
				return 1;
		}

		return 0;
	}

	void lcrgb_set_verbosity_level(unsigned char level){
		verbose_level = level;
	}

	// Initialise the library by finding and attaching to the keyboard
	int lcrgb_initialise(void){
		int status = 0;

		// Set a default keymap, I'm British so the UK layout is default ;)
		lcrgb_set_keymap(lcrgb_ISO_UK);

		// Initialise libusb
		status = libusb_init(&ctxt);
		if (status != 0) {
			fprintf(stderr, "Failed to initialize libusb: %d\n", status);
			return status;
		}

		// Get a list of currently connected USB devices
		libusb_device **device_list;
		ssize_t num_devices = libusb_get_device_list(ctxt, &device_list);
		if (verbose_level > 0) {
			printf("Searching for Corsair RGB device:\n");
		}

		if (num_devices >= 0) {
			struct libusb_device_descriptor device_descriptor;
			int i = 0;
			// If there are devices, loop through them looking for one that has the correct vid/pid
			for (; i < num_devices; ++i) {
				(void) memset(&device_descriptor, 0, sizeof(device_descriptor));
				status = libusb_get_device_descriptor(device_list[i], &device_descriptor);
				if (status == 0) {
					if (verbose_level > 0) {
						printf("  [%d/%d] %04x:%04x (bus %d, device %d)\n", (i+1), num_devices,
																																device_descriptor.idVendor, device_descriptor.idProduct,
																																libusb_get_bus_number(device_list[i]), libusb_get_device_address(device_list[i]));
					}

					if (device_descriptor.idVendor == CORSAIR_RGB_VID && (device_descriptor.idProduct == CORSAIR_K70_RGB_PID || device_descriptor.idProduct == CORSAIR_K95_RGB_PID)) {
						status = libusb_open(device_list[i], &device_handle);
						if (status != 0) {
							fprintf(stderr, "Failed to open device (libusb error: %s).\n", libusb_error_name(status));
						}
						else {
							if (verbose_level > 0) {
								printf("  Keyboard found!\n");
							}
						}
						break;
					}
				}
				else {
					fprintf(stderr, "Failed to get device description (libusb error: %s).\n", libusb_error_name(status));
				}
			}
			if (i == num_devices) {
				fprintf(stderr, "Could not find device, is it plugged in?\n");
			}
			libusb_free_device_list(device_list, 1);
			device_list = NULL;
		}
		else {
			fprintf(stderr, "Failed to get device list: %s\n", libusb_error_name((int)num_devices));
		}

		status = libusb_claim_interface(device_handle, 3);

		if(status == 0) {
			if (verbose_level > 0) {
				printf("USB interface claimed successfully :)\n");
			}
		}
		else {
			if (verbose_level > 0) {
				fprintf(stderr, "USB interface claim failed with error %d :(\n", libusb_error_name(status));
			}
			return 1;
		}

		return device_handle != NULL ? 0 : -99;

	}

	// De-Initialise the library by closing handles to the keyboard and giving back control
	void lcrgb_deinitialise(void){
		if (device_handle == NULL || ctxt == NULL) {
			fprintf(stderr, "LibCorsairRGB was not initialised, cannot deinitialise?\n");
		}
		else {
			libusb_release_interface(device_handle, 0);
			libusb_close(device_handle);

			libusb_exit(ctxt);

			if (verbose_level > 0) {
				printf("LibCorsairRGB successfully deinitialised\n");
			}
		}
	}

	// Set the RGB value of a key at a given x, y coordinate
	void lcrgb_set_position(unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b){
		lcrgb_key_enum key = k70_uk_matrix[y][x];
		lcrgb_set_key(key, r, g, b);
	}

	// Set the RGB value of a key by key name
	void lcrgb_set_key(lcrgb_key_enum key, unsigned char r, unsigned char g, unsigned char b){
		lcrgb_keymap::iterator mapped_key = lcrgb_current_keymap.find(key);
		if (mapped_key == lcrgb_current_keymap.end()) return;

		unsigned char key_code = mapped_key -> second;
		lcrgb_set_key_code(key_code, r, g, b);
	}

	// Set the RGB value of a key by key code
	void lcrgb_set_key_code(unsigned char key_code, unsigned char r, unsigned char g, unsigned char b){
		if (key_code == 0xFF) return;

		// For some reason rgb values are only 0-7? Not quite 16.8 million colours...
		r = (unsigned char)((7.0f/255.0f)*r);
		g = (unsigned char)((7.0f/255.0f)*g);
		b = (unsigned char)((7.0f/255.0f)*b);

		if (r > 7) r = 7;
		if (g > 7) g = 7;
		if (b > 7) b = 7;

		red_val[key_code] = 7 - r;
		grn_val[key_code] = 7 - g;
		blu_val[key_code] = 7 - b;
	}

	// Flush the light buffer to the keyboard
	int lcrgb_flush_light_buffer(void) {
		if (device_handle == NULL) {
			fprintf(stderr, "LibCorsairRGB was not initialised, cannot flush light buffer\n");
			return 1;
		}

		// Perform USB control message to keyboard
		//
		// Request Type:  0x21
		// Request:       0x09
		// Value          0x0300
		// Index:         0x03
		// Size:          64

		unsigned char data_pkt[5][64] = {0};

		data_pkt[0][0] = 0x7F;
		data_pkt[0][1] = 0x01;
		data_pkt[0][2] = 0x3C;

		data_pkt[1][0] = 0x7F;
		data_pkt[1][1] = 0x02;
		data_pkt[1][2] = 0x3C;

		data_pkt[2][0] = 0x7F;
		data_pkt[2][1] = 0x03;
		data_pkt[2][2] = 0x3C;

		data_pkt[3][0] = 0x7F;
		data_pkt[3][1] = 0x04;
		data_pkt[3][2] = 0x24;

		data_pkt[4][0] = 0x07;
		data_pkt[4][1] = 0x27;
		data_pkt[4][4] = 0xD8;

		for(int i = 0; i < 60; i++) {
			data_pkt[0][i+4] = red_val[i*2+1] << 4 | red_val[i*2];
		}

		for(int i = 0; i < 12; i++) {
			data_pkt[1][i+4] = red_val[i*2+121] << 4 | red_val[i*2+120];
		}

		for(int i = 0; i < 48; i++) {
			data_pkt[1][i+16] = grn_val[i*2+1] << 4 | grn_val[i*2];
		}

		for(int i = 0; i < 24; i++) {
			data_pkt[2][i+4] = grn_val[i*2+97] << 4 | grn_val[i*2+96];
		}

		for(int i = 0; i < 36; i++) {
			data_pkt[2][i+28] = blu_val[i*2+1] << 4 | blu_val[i*2];
		}

		for(int i = 0; i < 36; i++) {
			data_pkt[3][i+4] = blu_val[i*2+73] << 4 | blu_val[i*2+72];
		}

		libusb_control_transfer(device_handle, 0x21, 0x09, 0x0300, 0x03, data_pkt[0], 64, 1000);

		libusb_control_transfer(device_handle, 0x21, 0x09, 0x0300, 0x03, data_pkt[1], 64, 1000);
		libusb_control_transfer(device_handle, 0x21, 0x09, 0x0300, 0x03, data_pkt[2], 64, 1000);
		libusb_control_transfer(device_handle, 0x21, 0x09, 0x0300, 0x03, data_pkt[3], 64, 1000);
		libusb_control_transfer(device_handle, 0x21, 0x09, 0x0300, 0x03, data_pkt[4], 64, 1000);

		return 0;
	}
};
