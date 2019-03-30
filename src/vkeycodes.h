#ifndef _WIN32

#define VK_LBUTTON		        0x01	// Left mouse button
#define VK_RBUTTON		        0x02	// Right mouse button
#define VK_CANCEL		        0x03	// Control-break processing
#define VK_MBUTTON		        0x04	// Middle mouse button (three-button mouse)
#define VK_XBUTTON1		        0x05	// X1 mouse button
#define VK_XBUTTON2		        0x06	// X2 mouse button
//              		        0x07	// Undefined
#define VK_BACK		            0x08	// BACKSPACE key
#define VK_TAB		            0x09	// TAB key
//              		        0x0A-0B	// Reserved
#define VK_CLEAR		        0x0C	// CLEAR key
#define VK_RETURN		        0x0D	// ENTER key
//              		        0x0E-0F	// Undefined
#define VK_SHIFT		        0x10	// SHIFT key
#define VK_CONTROL		        0x11	// CTRL key
#define VK_MENU		            0x12	// ALT key
#define VK_PAUSE		        0x13	// PAUSE key
#define VK_CAPITAL		        0x14	// CAPS LOCK key
#define VK_KANA		            0x15	// IME Kana mode
#define VK_HANGUEL		        0x15	// IME Hanguel mode (maintained for compatibility; use VK_HANGUL)
#define VK_HANGUL		        0x15	// IME Hangul mode
//              		        0x16	// Undefined
#define VK_JUNJA		        0x17	// IME Junja mode
#define VK_FINAL		        0x18	// IME final mode
#define VK_HANJA		        0x19	// IME Hanja mode
#define VK_KANJI		        0x19	// IME Kanji mode
//              		        0x1A	// Undefined
#define VK_ESCAPE		        0x1B	// ESC key
#define VK_CONVERT		        0x1C	// IME convert
#define VK_NONCONVERT	        0x1D	// IME nonconvert
#define VK_ACCEPT		        0x1E	// IME accept
#define VK_MODECHANGE	        0x1F	// IME mode change request
#define VK_SPACE		        0x20	// SPACEBAR
#define VK_PRIOR		        0x21	// PAGE UP key
#define VK_NEXT		            0x22	// PAGE DOWN key
#define VK_END		            0x23	// END key
#define VK_HOME		            0x24	// HOME key
#define VK_LEFT		            0x25	// LEFT ARROW key
#define VK_UP		            0x26	// UP ARROW key
#define VK_RIGHT		        0x27	// RIGHT ARROW key
#define VK_DOWN		            0x28	// DOWN ARROW key
#define VK_SELECT		        0x29	// SELECT key
#define VK_PRINT		        0x2A	// PRINT key
#define VK_EXECUTE		        0x2B	// EXECUTE key
#define VK_SNAPSHOT		        0x2C	// PRINT SCREEN key
#define VK_INSERT		        0x2D	// INS key
#define VK_DELETE		        0x2E	// DEL key
#define VK_HELP		            0x2F	// HELP key
//                              0x30	// 0 key
//                              0x31	// 1 key
//                              0x32	// 2 key
//                              0x33	// 3 key
//                              0x34	// 4 key
//                              0x35	// 5 key
//                              0x36	// 6 key
//                              0x37	// 7 key
//                              0x38	// 8 key
//                              0x39	// 9key
//                              0x3A-40	// Undefined
//                              0x41	// A key
//                              0x42	// B key
//                              0x43	// C key
//                              0x44	// D key
//                              0x45	// E key
//                              0x46	// F key
//                              0x47	// G key
//                              0x48	// H key
//                              0x49	// I key
//                              0x4A	// J key
//                              0x4B	// K key
//                              0x4C	// L key
//                              0x4D	// M key
//                              0x4E	// N key
//                              0x4F	// O key
//                              0x50	// P key
//                              0x51	// Q key
//                              0x52	// R key
//                              0x53	// S key
//                              0x54	// T key
//                              0x55	// U key
//                              0x56	// V key
//                              0x57	// W key
//                              0x58	// X key
//                              0x59	// Y key
//                              0x5A	// Z key
#define VK_LWIN		            0x5B	// Left Windows key (Natural keyboard)
#define VK_RWIN		            0x5C	// Right Windows key (Natural keyboard)
#define VK_APPS		            0x5D	// Applications key (Natural keyboard)
//                              0x5E	// Reserved
#define VK_SLEEP		        0x5F	// Computer Sleep key
#define VK_NUMPAD0		        0x60	// Numeric keypad 0 key
#define VK_NUMPAD1		        0x61	// Numeric keypad 1 key
#define VK_NUMPAD2		        0x62	// Numeric keypad 2 key
#define VK_NUMPAD3		        0x63	// Numeric keypad 3 key
#define VK_NUMPAD4		        0x64	// Numeric keypad 4 key
#define VK_NUMPAD5		        0x65	// Numeric keypad 5 key
#define VK_NUMPAD6		        0x66	// Numeric keypad 6 key
#define VK_NUMPAD7		        0x67	// Numeric keypad 7 key
#define VK_NUMPAD8		        0x68	// Numeric keypad 8 key
#define VK_NUMPAD9		        0x69	// Numeric keypad 9 key
#define VK_MULTIPLY		        0x6A	// Multiply key
#define VK_ADD		            0x6B	// Add key
#define VK_SEPARATOR	        0x6C	// Separator key
#define VK_SUBTRACT		        0x6D	// Subtract key
#define VK_DECIMAL		        0x6E	// Decimal key
#define VK_DIVIDE		        0x6F	// Divide key
#define VK_F1		            0x70	// F1 key
#define VK_F2		            0x71	// F2 key
#define VK_F3		            0x72	// F3 key
#define VK_F4		            0x73	// F4 key
#define VK_F5		            0x74	// F5 key
#define VK_F6		            0x75	// F6 key
#define VK_F7		            0x76	// F7 key
#define VK_F8		            0x77	// F8 key
#define VK_F9		            0x78	// F9 key
#define VK_F10		            0x79	// F10 key
#define VK_F11		            0x7A	// F11 key
#define VK_F12		            0x7B	// F12 key
#define VK_F13		            0x7C	// F13 key
#define VK_F14		            0x7D	// F14 key
#define VK_F15		            0x7E	// F15 key
#define VK_F16		            0x7F	// F16 key
#define VK_F17		            0x80	// F17 key
#define VK_F18		            0x81	// F18 key
#define VK_F19		            0x82	// F19 key
#define VK_F20		            0x83	// F20 key
#define VK_F21		            0x84	// F21 key
#define VK_F22		            0x85	// F22 key
#define VK_F23		            0x86	// F23 key
#define VK_F24		            0x87	// F24 key
//              		        0x88-8F	// Unassigned
#define VK_NUMLOCK		        0x90	// NUM LOCK key
#define VK_SCROLL		        0x91	// SCROLL LOCK key
//                              0x92-96	// OEM specific
//                              0x97-9F	// Unassigned
#define VK_LSHIFT		        0xA0	// Left SHIFT key
#define VK_RSHIFT		        0xA1	// Right SHIFT key
#define VK_LCONTROL		        0xA2	// Left CONTROL key
#define VK_RCONTROL		        0xA3	// Right CONTROL key
#define VK_LMENU		        0xA4	// Left MENU key
#define VK_RMENU		        0xA5	// Right MENU key
#define VK_BROWSER_BACK		    0xA6	// Browser Back key
#define VK_BROWSER_FORWARD	    0xA7	// Browser Forward key
#define VK_BROWSER_REFRESH	    0xA8	// Browser Refresh key
#define VK_BROWSER_STOP		    0xA9	// Browser Stop key
#define VK_BROWSER_SEARCH	    0xAA	// Browser Search key
#define VK_BROWSER_FAVORITES	0xAB	// Browser Favorites key
#define VK_BROWSER_HOME		    0xAC	// Browser Start and Home key
#define VK_VOLUME_MUTE		    0xAD	// Volume Mute key
#define VK_VOLUME_DOWN		    0xAE	// Volume Down key
#define VK_VOLUME_UP		    0xAF	// Volume Up key
#define VK_MEDIA_NEXT_TRACK		0xB0	// Next Track key
#define VK_MEDIA_PREV_TRACK		0xB1	// Previous Track key
#define VK_MEDIA_STOP		    0xB2	// Stop Media key
#define VK_MEDIA_PLAY_PAUSE		0xB3	// Play/Pause Media key
#define VK_LAUNCH_MAIL		    0xB4	// Start Mail key
#define VK_LAUNCH_MEDIA_SELECT	0xB5	// Select Media key
#define VK_LAUNCH_APP1		    0xB6	// Start Application 1 key
#define VK_LAUNCH_APP2		    0xB7	// Start Application 2 key
//                      		0xB8-B9	// Reserved
#define VK_OEM_1		        0xBA	// Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
#define VK_OEM_PLUS		        0xBB	// For any country/region, the '+' key
#define VK_OEM_COMMA		    0xBC	// For any country/region, the ',' key
#define VK_OEM_MINUS		    0xBD	// For any country/region, the '-' key
#define VK_OEM_PERIOD		    0xBE	// For any country/region, the '.' key
#define VK_OEM_2		        0xBF	// Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
#define VK_OEM_3		        0xC0	// Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
//		                        0xC1-D7	// Reserved
//                      		0xD8-DA	// Unassigned
#define VK_OEM_4		        0xDB	// Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
#define VK_OEM_5		        0xDC	// Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
#define VK_OEM_6		        0xDD	// Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
#define VK_OEM_7		        0xDE	// Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
#define VK_OEM_8		        0xDF	// Used for miscellaneous characters; it can vary by keyboard.
//                      		0xE0	// Reserved
//                      		0xE1	// OEM specific
#define VK_OEM_102		        0xE2	// Either the angle bracket key or the backslash key on the RT 102-key keyboard
//                      		0xE3-E4	// OEM specific
#define VK_PROCESSKEY		    0xE5	// IME PROCESS key
//                      		0xE6	// OEM specific
#define VK_PACKET		        0xE7	// Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
//                      		0xE8	// Unassigned
//                      		0xE9-F5	// OEM specific
#define VK_ATTN		            0xF6	// Attn key
#define VK_CRSEL		        0xF7	// CrSel key
#define VK_EXSEL		        0xF8	// ExSel key
#define VK_EREOF		        0xF9	// Erase EOF key
#define VK_PLAY		            0xFA	// Play key
#define VK_ZOOM		            0xFB	// Zoom key
#define VK_NONAME		        0xFC	// Reserved
#define VK_PA1		            0xFD	// PA1 key
#define VK_OEM_CLEAR		    0xFE	//

#endif