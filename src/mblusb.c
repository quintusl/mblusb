/*
* Command line tool for 'BlUSB' - a programmable Bluetooth-USB-Hybrid keyboard firmware for AVR �Cs
*
* Created: 15.04.2016 14:02:35
* Author : Superfreak
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <libusb.h>
#include "layout.h"
#include "vkeycodes.h"

#define USB_ENABLE_VENDOR_RQ	0x11
#define USB_DISABLE_VENDOR_RQ	0x10
#define USB_READ_BR				0x20
#define USB_WRITE_BR			0x21

#define USB_READ_MATRIX			0x30
#define USB_READ_LAYOUT			0x40
#define USB_WRITE_LAYOUT		0x41
#define USB_READ_DEBOUNCE		0x50
#define USB_WRITE_DEBOUNCE		0x51
#define USB_READ_MACROS			0x60
#define USB_WRITE_MACROS		0x61
#define USB_READ_VERSION		0x70

/************************************************************************/
/*                        Function prototypes                           */
/************************************************************************/

void print_keyfile(char *p_layout_array, uint8_t nlayers);
void print_macrosfile(char *p_macros_array);
char *fill_layout_array(uint8_t nlayers);
char *fill_macros_array(void);
uint8_t parse_keyfile(char *keyfile_namestring);
void parse_macrosfile(char *keyfile_namestring);
uint16_t read_matrix_pos(void);
void read_pwm(void);
void read_version(void);
void write_pwm(int pwm_USB, int pwm_BT);
void read_debounce(void);
void write_debounce(int debounce_value);
char *read_layout(void);
void write_layout(uint8_t nlayers, char *p_layout_array);
char *read_macros(void);
void write_macros(unsigned char *p_char_ctrl_buf);
void print_usage(char **argv);
void enable_service_mode(void);
void disable_service_mode(void);
#ifdef _WIN32
void configure_layout(uint8_t nlayers, char *p_layout_array_keyfile);
#endif

/************************************************************************/
/*                         Type definitions                             */
/************************************************************************/

typedef unsigned char			uchar;
typedef unsigned int			uint;

/************************************************************************/
/*                          Global variables                            */
/************************************************************************/

libusb_device **dev_list;
libusb_device *dev = NULL;
libusb_device_handle *handle = NULL;
struct libusb_device_descriptor dev_descr;

uint8_t string_buffers[NUMLAYERS_MAX][1024];
uint8_t verbose;

/************************************************************************/
/*                          Global constants                            */
/************************************************************************/

const uint16_t vendor = 0x04b3, product = 0x301c; // IBM Enhanced Performance Keyboard

const uint8_t vk_code_array[] =
{
	VK_ESCAPE, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7,	VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, VK_SNAPSHOT,
	VK_SCROLL, VK_OEM_5, VK_KEY_1, VK_KEY_2, VK_KEY_3, VK_KEY_4, VK_KEY_5, VK_KEY_6, VK_KEY_7, VK_KEY_8,
	VK_KEY_9, VK_KEY_0,	VK_OEM_4, VK_OEM_6, VK_BACK, VK_INSERT,	VK_HOME, VK_PRIOR, VK_TAB, VK_KEY_Q, VK_KEY_W,
	VK_KEY_E, VK_KEY_R,	VK_KEY_T, VK_KEY_Z,	VK_KEY_U, VK_KEY_I,	VK_KEY_O, VK_KEY_P,	VK_OEM_1, VK_OEM_PLUS, VK_RETURN,
	VK_DELETE, VK_END, VK_NEXT, VK_CAPITAL, VK_KEY_A, VK_KEY_S, VK_KEY_D, VK_KEY_F, VK_KEY_G, VK_KEY_H, VK_KEY_J,
	VK_KEY_K, VK_KEY_L,	VK_OEM_3, VK_OEM_7,	VK_OEM_2, VK_LSHIFT, VK_OEM_102, VK_KEY_Y, VK_KEY_X, VK_KEY_C, VK_KEY_V,
	VK_KEY_B, VK_KEY_N, VK_KEY_M, VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_MINUS, VK_RSHIFT, VK_UP, VK_LCONTROL, VK_LWIN,
	VK_LMENU, VK_SPACE,	VK_RMENU, VK_RWIN, VK_APPS, VK_RCONTROL, VK_LEFT, VK_DOWN, VK_RIGHT, VK_NUMLOCK, VK_DIVIDE,
	VK_MULTIPLY, VK_SUBTRACT, VK_ADD, VK_RETURN, VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
	VK_NUMPAD6,	VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9, VK_DECIMAL
};

const uint8_t vk_name_array[][16] =
{
	"ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "Print Screen", "Scroll Lock",
	"OEM_5", "KB 1", "KB 2", "KB 3", "KB 4", "KB 5", "KB 6", "KB 7", "KB 8", "KB 9", "KB 0", "OEM_4", "OEM_6",
	"Backspace", "Insert", "Home", "Page up", "Tab", "Q", "W", "E", "R", "T", "Z", "U", "I", "O", "P", "OEM_1",
	"OEM_PLUS", "Enter", "Delete", "End", "Page down", "Caps Lock", "A", "S", "D", "F", "G", "H", "J", "K", "L",
	"OEM_3", "OEM_7", "OEM_2", "Left Shift", "OEM_102", "Y", "X", "C", "V", "B", "N", "M", "OEM_COMMA", "OEM_PERIOD",
	"OEM_MINUS", "Right Shift", "Up Arrow", "Left Ctrl", "Left Win", "Left Alt", "Space", "Right Alt",	"Right Win",
	"Win Menu", "Right Ctrl", "Left Arrow", "Down Arrow", "Right Arrow", "Num Lock", "NP_Divide", "NP_Multiply", "NP_Subtract",
	"NP_Add", "Enter", "NP_0", "NP_1", "NP_2", "NP_3", "NP_4", "NP_5", "NP_6", "NP_7", "NP_8", "NP_9",
	"NP_Dot"
};

const uint16_t hid_code_array[] =
{
	KB_ESC, KB_F1, KB_F2, KB_F3, KB_F4, KB_F5, KB_F6, KB_F7, KB_F8, KB_F9, KB_F10, KB_F11, KB_F12, KB_PSCRN, KB_SCRLK,
	KB_TILDE, KB_1, KB_2, KB_3, KB_4, KB_5, KB_6, KB_7, KB_8, KB_9, KB_0, KB_MINUS, KB_EQUAL, KB_BKSPC, KB_INS,
	KB_HOME, KB_PGUP, KB_TAB, KB_Q, KB_W, KB_E, KB_R, KB_T, KB_Z, KB_U, KB_I, KB_O, KB_P, KB_LBRCE, KB_RBRCE,
	KB_ENTER, KB_DEL, KB_END, KB_PGDN, KB_CAPLK, KB_A, KB_S, KB_D, KB_F, KB_G, KB_H, KB_J,	KB_K, KB_L,	KB_SMCLN,
	KB_QUOTE, KB_BSLSH, KB_LSHFT, KB_PIPE, KB_Y, KB_X, KB_C, KB_V, KB_B, KB_N, KB_M, KB_COMMA, KB_DOT, KB_SLASH,
	KB_RSHFT, KB_UP, KB_LCTRL, KB_LGUI,	KB_LALT, KB_SPACE, KB_RALT, KB_RGUI, KB_APP, KB_RCTRL, KB_LEFT, KB_DOWN,
	KB_RIGHT, KB_NUMLK, KP_SLASH, KP_ASTRX, KP_MINUS, KP_PLUS, KB_RETURN, KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8,
	KP_9, KP_DOT
};

/************************************************************************/
/*                           Functions                                  */
/************************************************************************/

void print_keyfile(char *p_layout_array, uint8_t nlayers)
{
	FILE *keyfile;
	char char_buf[8] = { 0 }, scan_buf[256] = { 0 }, string_buf[256] = { 0 }, string_buf2[3072] = { 0 };

	printf("\n\n");
	printf("Enter filename: ");
	fgets(scan_buf, sizeof(scan_buf), stdin);
	sscanf(scan_buf, "%s", string_buf);

	keyfile = fopen(string_buf, "wb");

	if (!keyfile)
		perror("Error creating/opening file");
	else
	{
		for (uint8_t i = 0; i < nlayers; i++)
		{
			for (uint8_t j = 0; j < NUMKEYS; j++)
			{
				sprintf(string_buf, "%u", *(p_layout_array + i*NUMKEYS + j));
				if (j < 1 && i < 1)
				{
					strcpy(string_buf2, string_buf);
					strcpy(char_buf, ", ");
					strcat(string_buf2, char_buf);
				}
				else
				{
					if (j < NUMKEYS - 1)
					{
						strcat(string_buf2, string_buf);
						strcpy(char_buf, ", ");
						strcat(string_buf2, char_buf);
					}
					else
						strcat(string_buf2, string_buf);

				}
			}

			if (i != nlayers - 1)
			{
				strcpy(char_buf, "\n\n");
				strcat(string_buf2, char_buf);
			}
			else
			{
				strcpy(char_buf, "\n");
				strcat(string_buf2, char_buf);
			}

		}

		fprintf(keyfile, "%s", string_buf2);

		fclose(keyfile);
	}
}

void print_macrosfile(char *p_layout_array)
{
	FILE *keyfile;
	char char_buf[8] = { 0 }, scan_buf[256] = { 0 }, string_buf[256] = { 0 }, string_buf2[3072] = { 0 };

	printf("\n\n");
	printf("Enter filename: ");
	fgets(scan_buf, sizeof(scan_buf), stdin);
	sscanf(scan_buf, "%s", string_buf);

	keyfile = fopen(string_buf, "wb");

	if (!keyfile)
		perror("Error creating/opening file");
	else
	{
		
		for (uint8_t i = 0; i < NUM_MACROKEYS*LEN_MACRO; i++)	
		{	
			sprintf(string_buf, "%u", *(p_layout_array + i));	

			// need to use strcpy first because string_buf2 is empty
			if (i == 0)	
			{	
				strcpy(string_buf2, string_buf);	
				strcpy(char_buf, ", ");	
				strcat(string_buf2, char_buf);	
			}	
			else	
			{	
				// append comma after all strings except the last
				if (i < NUM_MACROKEYS*LEN_MACRO - 1)
				{	
					strcat(string_buf2, string_buf);	
					strcpy(char_buf, ", ");	
					strcat(string_buf2, char_buf);	
				}	
				else	
					strcat(string_buf2, string_buf);	

			}

			// append newline character after each macro key report (which is a multiple of 8)
			if (i) if ((i+1) % 8 == 0)
					{
						strcpy(char_buf, "\n");
						strcat(string_buf2, char_buf);
					}
			

		}	

		/*// end of array, append a newline character
		strcpy(char_buf, "\n");
		strcat(string_buf2, char_buf);*/

		fprintf(keyfile, "%s", string_buf2);

		fclose(keyfile);
	}
}

char *fill_macros_array()
{
	uint8_t chardump = 0, n = 0;
	char string_buf[8] = { 0 };
	static char macros_array[192] = { 0 };
	char *end_p = NULL;
	uint8_t token = 0;

	/*printf("\n\n");
	printf("string_buffers:");
	printf("\n\n");
	for (uint16_t j = 0; j < 1024; j++)
		printf("%c", *((uint8_t*)string_buffers+j));*/
	

	for (uint16_t pos = 0; token < NUM_MACROKEYS*LEN_MACRO; pos++)
	{	
		chardump = *((uint8_t*)string_buffers+pos);
		/* comma and newline character demarcate values, but	*
		 * make sure not to repeat a value in case a comma is	*
		 * followed by a newline character						*/
		if (chardump == ',' || (chardump == '\n' && n != 0))	
		{	
			//printf("token %-2d\n", pos);	
			// we have another token, flush string buffer 	
			macros_array[token++] = strtol(string_buf, &end_p, 10);	
			n = 0;	
			continue;	
		}	
		else	
		{	
			if (isdigit(chardump))	
			{	
				//printf("notoken %-2d\n", pos);	
				string_buf[n] = chardump;	
				string_buf[n + 1] = '\0';	
				n++;	
			}	

		}
	}	
	
	/*printf("\r");
	printf("macros array:");
	printf("\n\n");
	for (uint8_t i = 0; i < 192; i++)
		printf("%u ", macros_array[i]);
	printf("\n\n");*/

	return macros_array;
}

char *fill_layout_array(uint8_t nlayers)
{
	uint8_t chardump = 0, n = 0;
	char string_buf[8] = { 0 };
	char *end_p = NULL;
	char *p_layout_array_keyfile = NULL;

	p_layout_array_keyfile = (char *)calloc(NUMKEYS * nlayers, sizeof(char));

	for (uint8_t layer = 0; layer < nlayers; layer++)
	{

		uint8_t token = 0;

		for (uint16_t pos = 0; string_buffers[layer][pos]; pos++)
		{
			chardump = string_buffers[layer][pos];
			//printf("char: %-5c", chardump);
			if (chardump == ',' || chardump == '\n')
			{
				//printf("token %-2d\n", pos);
				// we have another token, flush string buffer 
				*(p_layout_array_keyfile + layer*NUMKEYS + (token++)) = strtol(string_buf, &end_p, 10);
				n = 0;
				continue;
			}
			else
			{
				if (isdigit(chardump))
				{
					//printf("notoken %-2d\n", pos);
					string_buf[n] = chardump;
					string_buf[n + 1] = '\0';
					n++;
				}

			}
		}
	}

	return p_layout_array_keyfile;
}


uint8_t parse_keyfile(char *keyfile_namestring)
{
	uint8_t nlayers = 0, chardump = 0;
	FILE *keyfile;

	for (uint16_t i = 0; i < sizeof(string_buffers); i++)
		*((uint8_t*)string_buffers + i) = 0;

	keyfile = fopen(keyfile_namestring, "rb");

	if (!keyfile)
	{
		printf("\n\n");
		perror("Error opening file");
		exit(0);
	}
	else
		while (!feof(keyfile))
		{
			uint16_t pos = 0;
			do {

				chardump = fgetc(keyfile);
				string_buffers[nlayers][pos] = chardump;
				pos++;

			} while (chardump != '\n' && !feof(keyfile));
			//string_buffers[str_count][strlen(string_buffers[str_count])] = '\0';
			//printf("\r");

			//printf("Count: %d String: %s\n", str_count, string_buffers[str_count]);

			nlayers++;

			do {
				chardump = fgetc(keyfile);
			} while (isspace(chardump));
			ungetc(chardump, keyfile);

			//string_buf2[strlen(string_buf2) - 2] = '\0';

		}


	//for (i = 0; i < str_count; i++)
	//printf("%s\n\n", string_buffers[i]);

	//printf("Layers: %d\n", str_count);

	fclose(keyfile);

	return nlayers;
}

void parse_macrosfile(char *keyfile_namestring)
{
	uint8_t chardump = 0;
	FILE *keyfile;
	uint16_t pos = 0;

	for (uint16_t i = 0; i < sizeof(string_buffers); i++)
		*((uint8_t*)string_buffers + i) = 0;

	keyfile = fopen(keyfile_namestring, "rb");

	if (!keyfile)
	{
		printf("\n\n");
		perror("Error opening file");
		exit(0);
	}
	else
		while (!feof(keyfile))
		{
			do {
				chardump = fgetc(keyfile);
				string_buffers[0][pos++] = chardump;
			} while (chardump != '\n' && !feof(keyfile));
			
			do {
				chardump = fgetc(keyfile);
			} while (isspace(chardump));
			ungetc(chardump, keyfile);
		}

	fclose(keyfile);
}

void enable_service_mode()
{
	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_ENABLE_VENDOR_RQ, 0, 0, 0, 0, 1000);
}

void disable_service_mode()
{
	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_DISABLE_VENDOR_RQ, 0, 0, 0, 0, 1000);
}

uint16_t read_matrix_pos()
{
	static uint8_t buffer[8] = { 0 }, buffer_last[8] = { 0 }, buffer_changed = 0;
	uint8_t i = 0;

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_READ_MATRIX, 0, 0, buffer, sizeof(buffer), 1000);

	if (buffer[7])
		for (i = 0; i < 2; i++)
			if (buffer[i] ^ buffer_last[i])
			{
				buffer_changed = 1;
				break;
			}

	if (buffer_changed)
	{
		buffer_changed = 0;
		for (i = 0; i < 2; i++)
			buffer_last[i] = buffer[i];
		if (verbose)
		{
			printf("\r%24c", '\0');
			printf("\rRow: %d Column: %d", buffer[0], buffer[1]);			
		}
		return buffer[0] | buffer[1] << 8;			// return row, col
	}
	else
		return 0xffff;
}

void read_version()
{
	static uint8_t buffer[8] = { 0 };

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_READ_VERSION, 0, 0, buffer, sizeof(buffer), 1000);

	printf("\n\n");
	printf("Firmware Version: %u.%u\n", buffer[0], buffer[1]);

}

void read_pwm()
{
	static uint8_t buffer[8] = { 0 };

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_READ_BR, 0, 0, buffer, sizeof(buffer), 1000);

	printf("\n\n");
	printf("PWM_USB:%-5u PWM_BT:%u\n", buffer[0], buffer[1]);

}

void write_pwm(int pwm_USB, int pwm_BT)
{
	static uint8_t buffer[8] = { 0 };

	if ((pwm_USB < 0 || pwm_USB > 255) || (pwm_BT < 0 || pwm_BT > 255))
	{
		printf("\n\n");
		printf("Value out of range, no changes applied. Exiting.\n");
		exit(0);
	}

	buffer[0] = (uint8_t)pwm_USB;
	buffer[1] = (uint8_t)pwm_BT;

	printf("\n\n");
	printf("PWM_USB:%-5u PWM_BT:%u\n", pwm_USB, pwm_BT);

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_WRITE_BR, 0, 0, buffer, sizeof(buffer), 1000);
	
}

void read_debounce()
{
	static uint8_t buffer[8] = { 0 };

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_READ_DEBOUNCE, 0, 0, buffer, sizeof(buffer), 1000);

	printf("\n\n");
	printf("Debounce time: %u ms\n", buffer[0]);

}

void write_debounce(int debounce_value)
{
	static uint8_t buffer[8] = { 0 };

	if ((debounce_value < 1 || debounce_value > 255))
	{
		printf("\n\n");
		printf("Value out of range, no changes applied. Exiting.\n");
		exit(0);
	}

	buffer[0] = (uint8_t)debounce_value;

	printf("\n\n");
	printf("Debounce time: %u ms\n", debounce_value);

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_WRITE_DEBOUNCE, 0, 0, buffer, sizeof(buffer), 1000);

}

char* read_macros()
{
	static unsigned char char_ctr_buf[192];
	uint8_t bad_value1 = 0, bad_value2 = 0, macro_cnt = 0;

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_ENDPOINT | LIBUSB_ENDPOINT_IN |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_READ_MACROS, 0, 0, char_ctr_buf, sizeof(char_ctr_buf), 1000);

	printf("\n\n");

	for (uint8_t i = 0; i < sizeof(char_ctr_buf); i++)
	{
		if (char_ctr_buf[i] == 0) bad_value1++;
		if (char_ctr_buf[i] == 255) bad_value2++;
	}

	if (bad_value1 == sizeof(char_ctr_buf) || bad_value2 == sizeof(char_ctr_buf))
	{
		printf("Bad EEPROM value!\n");
		exit(0);
	}

	printf("Macro key table\n\n");
	printf("            Mods%-3cRsvd%-3cKey1%-3cKey2%-3cKey3%-3cKey4%-3cKey5%-3cKey6\n\n", '\0', '\0', '\0', '\0', '\0', '\0', '\0');
	printf("Macro %-6u", ++macro_cnt);

	for (uint8_t i = 0; i < sizeof(char_ctr_buf); i++)
	{
		printf("%-7u", char_ctr_buf[i]);
		if(i && i != sizeof(char_ctr_buf) - 1)
			if ((i + 1) % 8 == 0)
			{
				printf("\n");
				printf("Macro %-6u", ++macro_cnt);
			}
	}

	return (char*)char_ctr_buf;
}

char *read_layout()
{
	static unsigned char char_ctr_buf[2048] = { 0 };
	uint8_t layout_buf[2048] = {0};
	uint8_t row_cnt, i, j, k;
	uint8_t num_layers;

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_ENDPOINT | LIBUSB_ENDPOINT_IN |
		LIBUSB_REQUEST_TYPE_VENDOR, USB_READ_LAYOUT, 0, 0, char_ctr_buf, sizeof(char_ctr_buf), 1000);

	num_layers = char_ctr_buf[0];

	memcpy(layout_buf, char_ctr_buf+2, sizeof(layout_buf));

	printf("\n\n");
		
	if (num_layers == 0)
	{
		printf("No layers configured.\n");
		exit(0);
	}

	if (num_layers > 6)
	{
		printf("More than 6 layers reported, bad flash value!\n");
		exit(0);
	}

	printf("Number of layers: %d", num_layers);
	printf("\n\n");

	/*for (uint16_t i = 1; i < num_layers * NUMKEYS + 1; i++)
		printf("%-10u", *((uint16_t*)char_ctr_buf + i));*/

	printf("Layer 1");
	printf("\n\n");
	printf("    ");
	for (j = 1; j < NUMCOLS + 1; j++)
		printf("C%-5u", j);
	printf("\n\n");
	row_cnt = 1;
	printf("R%u  ", row_cnt++);
	for (k = 0; k < num_layers; k++)
	{

		for (i = 0; i < NUMKEYS; i++)
		{
			printf("%-6u", *((uint16_t*)layout_buf + i + k*NUMKEYS));
			if (i && i != NUMKEYS - 1)
				if ((i + 1) % 20 == 0)
				{
					printf("\n\r");
					printf("R%u  ", row_cnt++);
				}
		}
		if (i == NUMKEYS && k == num_layers - 1) break;
		row_cnt = 1;
		printf("\n\n");
		printf("Layer %u", k + 2);
		printf("\n\n");
		printf("    ");
		for (j = 1; j < NUMCOLS + 1; j++)
			printf("C%-5u", j);
		printf("\n\n");
		printf("R%u  ", row_cnt++);
	}

	return (char *)char_ctr_buf;
}

void write_layout(uint8_t nlayers, char *p_layout_array)
{
	/*uint8_t char_ctrl_buf_p[128] = { 0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

	char_ctrl_buf_p[0] = nlayers;*/
	
	uint8_t *p_char_ctrl_buf = NULL;
	
	p_char_ctrl_buf = (uint8_t *)realloc(p_char_ctrl_buf, nlayers * 2 * NUMKEYS + 1);

	p_char_ctrl_buf[0] = nlayers;

	for (uint16_t i = 1, j=0; j < 2 * nlayers*NUMKEYS; i++, j++)
		p_char_ctrl_buf[i] = *((uint8_t*)p_layout_array+j);

	/*libusb_control_transfer(handle, LIBUSB_RECIPIENT_ENDPOINT | LIBUSB_ENDPOINT_OUT | \
		LIBUSB_REQUEST_TYPE_VENDOR,	USB_WRITE_LAYOUT, 0, 0, NULL, 0, 1000);*/

	libusb_control_transfer(handle, LIBUSB_RECIPIENT_ENDPOINT | LIBUSB_ENDPOINT_OUT | \
		LIBUSB_REQUEST_TYPE_VENDOR, USB_WRITE_LAYOUT, 0, 0, p_char_ctrl_buf, nlayers*2*NUMKEYS+1, 1000);

	/*Sleep(100);

	printf("\nWriting to flash. %3d%% completed.", 0);

	for (uint8_t i = 0; i < nlayers * 2; i++)
	{
		for (uint8_t j = 0; j < NUMKEYS; j++)
			char_ctr_buf[j] = *((uint8_t*)p_layout_array + j + i*NUMKEYS);

		libusb_control_transfer(handle, LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR,
			USB_WRITE_LAYOUT, 0, 0, char_ctr_buf, NUMKEYS, 1000);
		
		printf("\rWriting to flash. %3d%% completed.", ((i+1) * 100 / (nlayers * 2)));		
		Sleep(100);
	}*/

	free(p_layout_array);
}

void write_macros(unsigned char *p_char_ctrl_buf)
{
	libusb_control_transfer(handle, LIBUSB_RECIPIENT_ENDPOINT | LIBUSB_ENDPOINT_OUT | \
		LIBUSB_REQUEST_TYPE_VENDOR, USB_WRITE_MACROS, 0, 0, p_char_ctrl_buf, NUM_MACROKEYS*LEN_MACRO, 1000);
}

void print_usage(char **argv)
{
	printf("\n\n");
	printf("Usage: %s [-option] [-optional parameter] [filename]", argv[0]);
	printf("\n\n");
	printf("Options:");
	printf("\n\n");
#ifdef _WIN32
	printf("-read_matrix\n");
#endif
	printf("-read_pwm\n");
	printf("-write_pwm [value_USB value_BT] (Valid range: 0-255)\n");
	printf("-read_debounce\n");
	printf("-write_debounce [value] (Valid range: 1-255)\n");
	printf("-read_macros\n");
	printf("-write_macros [filename]\n");
	printf("-read_layout\n");
	printf("-write_layout [filename]\n");
#ifdef _WIN32
	printf("-configure_layout [-keyfile filename]\n");
#endif
	printf("-read_version\n");
}

int main(int argc, char **argv)
{

	uint8_t nlayers = 0;			// dummy arguments for configure_layout() if no existing key file is used
	char *p_layout_array_keyfile = NULL;
	char *p_macros_array_macrosfile = NULL;
	char scan_buf[256] = { 0 };
	int err = 0;

	printf("\n");
	printf("+++ Matrix discovery and layout configuration tool +++");
	printf("\n\n");

	libusb_init(NULL);

	uint8_t cnt = libusb_get_device_list(NULL, &dev_list);

	//loop through device list and find device
	for (uint8_t i = 0; i < cnt; i++)
	{
		dev = dev_list[i];
		libusb_get_device_descriptor(dev, &dev_descr);
		/*printf("No. %d   Vendor: %x   Name: %s   Product: %x   Name: %s   Bus: %d   Address: %d\n",
		i, 	dev_descr.idVendor, dev_descr.iManufacturer, dev_descr.idProduct, dev_descr.iProduct,
		libusb_get_bus_number(dev), libusb_get_device_address(dev));
		Sleep(100);*/
		if ((vendor == dev_descr.idVendor) && (product == dev_descr.idProduct))
		{
			printf("Device found: No. %d   Vendor: %x   Name: %u   Product: %x   Name: %u   Bus: %d   Address: %d",
				i, dev_descr.idVendor, dev_descr.iManufacturer, dev_descr.idProduct, dev_descr.iProduct,
				libusb_get_bus_number(dev), libusb_get_device_address(dev));
			err = libusb_open(dev, &handle);
			if (err)
			{
				printf("\n\n");
				printf("LIBUSB error code: %s", libusb_error_name(err));
				printf("\n\n");
				printf
					(
						"Don't panic! This is a simple driver issue. "
						"If you have not already, download Zadig at\n" "http://zadig.akeo.ie and install the WinUSB driver.\n"
						"You can also give the LibUSB-win32 driver a try, whatever is going to work for you.\n"
						"Quirky Windows(c) likes a little tinkering!"
						);
			}
			libusb_free_device_list(dev_list, 1);
			break;
		}
	}

	if (handle == NULL)
	{
		printf("\n\n");
		fprintf(stderr, "Can't get handle for specified device!\n");
		libusb_exit(NULL);
		exit(1);
	}

	if (handle)
	{
#ifdef _WIN32
		if (argc > 3 && strcmp(argv[2], "-keyfile") == 0)
		{
			sscanf(argv[3], "%s", scan_buf);
			nlayers = parse_keyfile(scan_buf);
			printf("\nKey file parsed. Number of layers: %d", nlayers);
			p_layout_array_keyfile = fill_layout_array(nlayers);
			printf("\nLayout array filled.");
			configure_layout(nlayers, p_layout_array_keyfile);
		}

		if (argc > 1 && strcmp(argv[1], "-configure_layout") == 0)
			configure_layout(nlayers, p_layout_array_keyfile);

		if (argc > 1 && strcmp(argv[1], "-read_matrix") == 0)
		{
			verbose = 1;
			enable_service_mode();
			printf("\n\n");
			printf("Press Esc to quit");
			printf("\n\n");

			while (1)
			{
				read_matrix_pos();
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				{
					printf("\n");
					disable_service_mode();
					libusb_close(handle);
					libusb_exit(NULL);
					exit(0);
				}
			}

		}
#endif

		if (argc > 1 && strcmp(argv[1], "-read_version") == 0)
		{
			read_version();
		}

		if (argc > 1 && strcmp(argv[1], "-read_pwm") == 0)
		{
			read_pwm();
		}

		if (argc > 3 && strcmp(argv[1], "-write_pwm") == 0)
		{
			write_pwm(atoi(argv[2]), atoi(argv[3]));
		}

		if (argc > 1 && strcmp(argv[1], "-read_debounce") == 0)
		{
			read_debounce();
		}

		if (argc > 2 && strcmp(argv[1], "-write_debounce") == 0)
		{
			write_debounce(atoi(argv[2]));
		}

		if (argc > 1 && strcmp(argv[1], "-read_layout") == 0)
		{
			p_layout_array_keyfile = read_layout();
			// address 0 = nlayers, array data starts at address 1
			print_keyfile(p_layout_array_keyfile + 1, p_layout_array_keyfile[0]);
		}

		if (argc > 1 && strcmp(argv[1], "-write_layout") == 0)
		{
			if (argv[2])
			{
				sscanf(argv[2], "%s", scan_buf);
				nlayers = parse_keyfile(scan_buf);
				printf("\nKey file parsed. Number of layers: %d", nlayers);
				p_layout_array_keyfile = fill_layout_array(nlayers);
				printf("\nLayout array filled.");
				//enable_service_mode();
				write_layout(nlayers, p_layout_array_keyfile);
				//disable_service_mode();
				printf("\nDone.\n");
			}
			else
			{
				printf("\n\n");
				printf("You must provide a filename!");
				print_usage(argv);
			}
		}

		if (argc > 1 && strcmp(argv[1], "-read_macros") == 0)
		{
			p_macros_array_macrosfile = read_macros();
			print_macrosfile(p_macros_array_macrosfile);
		}

		if (argc > 1 && strcmp(argv[1], "-write_macros") == 0)
		{
			if (argv[2])
			{
				sscanf(argv[2], "%s", scan_buf);
				parse_macrosfile(scan_buf);
				printf("\nMacro file parsed.");
				p_macros_array_macrosfile = fill_macros_array();
				printf("\nMacro array filled.");
				//enable_service_mode();
				write_macros((unsigned char*)p_macros_array_macrosfile);
				//disable_service_mode();
				printf("\nDone.\n");
			}
			else
			{
				printf("\n\n");
				printf("You must provide a filename!");
				print_usage(argv);
			}
		}

		if (argc < 2)
			print_usage(argv);
	}

	libusb_close(handle);
	libusb_exit(NULL);
	return 0;
}
