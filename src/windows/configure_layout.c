#ifdef _WIN32

void configure_layout(uint8_t nlayers, uint16_t *p_layout_array_keyfile)
{
	uint8_t last_keypress = 0xff, nlayers_ready = 0, ch = 0, cur_layer_ready = 0, cur_layer = 0,
		row = 0, col = 0, manual_input = 0;
	uint8_t *p_end_strtol = NULL;
	uint16_t *p_layout_array = NULL;
	uint16_t rowcol = 0, rowcol_last = 0, keycode = 0;
	uint8_t str_buf[8] = { 0 }, scan_buf[8] = { 0 };

	if (nlayers <= 6 && nlayers >= 1)
	{
		p_layout_array = p_layout_array_keyfile;
		goto SET_LAYER;
	}

	printf("\n\n");
	printf("Please enter number of layers (1 min, 6 max): ");
	while (1)
	{
		if (nlayers_ready)
		{
			nlayers_ready = 0;
			break;
		}
		*str_buf = _getch();
		nlayers = strtol(str_buf, &p_end_strtol, 10);
		printf("\n\n");
		printf("Number of layers: %d", nlayers);
		if (nlayers > 6 || nlayers < 1)
		{
			printf("\n\n");
			printf("Too few/many layers, try again (or press Esc to quit): ");

			while (1)
			{
				ch = _getch();
				if (ch == ASCII_ESC)
					exit(0);
				*str_buf = ch;
				nlayers = strtol(str_buf, &p_end_strtol, 10);
				printf("\rToo few/many layers, try again (or press Esc to quit): %d", nlayers);
				if (nlayers <= 6 && nlayers >= 1)
				{
					nlayers_ready = 1;
					printf("\n\n");
					printf("Number of layers: %d", nlayers);
					break;
				}
			}
		}
		else
			break;
	}

	// create layout array dynamically with calloc()

	p_layout_array = (uint16_t*)calloc(NUMKEYS * nlayers, sizeof(uint16_t));



SET_LAYER:

	printf("\n\n");
	printf("Set current layer: ");
	while (1)
	{
		if (cur_layer_ready)
		{
			cur_layer_ready = 0;
			break;
		}
		*str_buf = _getch();
		cur_layer = strtol(str_buf, &p_end_strtol, 10);
		printf("\n\n");
		printf("Current layer: %d", cur_layer);
		if (cur_layer > nlayers || cur_layer < 1)
		{
			printf("\n\n");
			printf("Not a valid layer, try again (or press Esc to quit): ");

			while (1)
			{
				ch = _getch();
				if (ch == ASCII_ESC)
					exit(0);
				*str_buf = ch;
				cur_layer = strtol(str_buf, &p_end_strtol, 10);
				printf("\rNot a valid layer, try again (or press Esc to quit): %d", cur_layer);
				if (cur_layer > 0 && cur_layer <= nlayers)
				{
					cur_layer_ready = 1;
					printf("\n\n");
					printf("Current layer: %d", cur_layer);
					break;
				}
			}
		}
		else
			break;
	}

	printf("\n\n");
	printf("Please press a key to be mapped.");
	printf("\n");
	printf(
		"ESC - save layer to file and exit"
		" | "
		"ENTER - continue"
		" | "
		"BACKSPACE - change current layer"
		" | "
		"\n"
		"TAB - switch auto/manual keycode input"
		);
	printf("\n\n");

	Sleep(1000);		// give the user some time to release Enter key, or else GetAsyncKeyState() will
						// immediately return last key pressed
	
	enable_service_mode();

GET_KEY:

	if (manual_input)
	{
		while (1)
		{
			rowcol = read_matrix_pos();

			if (rowcol != 0xffff && rowcol != rowcol_last)
			{
				// don't worry about the magic numbers, read_matrix_pos() just returns a short, lower byte contains number
				// of rows and higher byte contains number of columns
				row = rowcol & 0x0f;	// decipher with bitmask and assign value
				col = (rowcol & 0xff00) / 256;
				printf("\r%32c", '\0');
				printf("\rRow: %d Column: %d Keycode: ", row, col);
			}

			if (_kbhit())
			{
				fgets(str_buf, sizeof(str_buf), stdin);
				sscanf(str_buf, "%s", scan_buf);
				keycode = strtol(scan_buf, &p_end_strtol, 10);
				p_layout_array[(row * NUMCOLS + col) + (cur_layer - 1) * NUMKEYS] = keycode;

				while (1)
				{
					ch = _getch();
					switch (ch)
					{
					case ASCII_ESC:
						print_keyfile(p_layout_array, nlayers);
						free(p_layout_array);
						disable_service_mode();
						exit(0);
					case ASCII_ENTER:
						rowcol_last = rowcol;
						printf("\nRow: %d Column: %d Keycode: ", row, col);
						goto GET_KEY;
					case ASCII_BKSPC:
						rowcol_last = rowcol;
						goto SET_LAYER;
					case ASCII_TAB:
						rowcol_last = rowcol;
						last_keypress = 0xff;
						manual_input ^= 1;
						printf("\n\n");
						printf("+++ Manual key input disabled +++");
						printf("\n\n");
						keybd_event(VK_TAB, 0, 2, 0);
						GetAsyncKeyState(VK_TAB);
						goto GET_KEY;
					}
				}
			}
		}
	}

	while (1)
	{
		rowcol = read_matrix_pos();

		if (rowcol != 0xffff && rowcol != rowcol_last)
		{
			// don't worry about the magic numbers, read_matrix_pos() just returns a short, lower byte contains number
			// of rows and higher byte contains number of columns
			row = rowcol & 0x0f;	// decipher with bitmask and assign value
			col = (rowcol & 0xff00) / 256;
			printf("\rRow: %d Column: %d Keycode: ", row, col);
		}

		for (uint8_t n = 0; n < sizeof(vk_code_array); n++)
			if (GetAsyncKeyState(*(vk_code_array + n)) & 0x8000 && (last_keypress != n))
			{
				last_keypress = n;
				p_layout_array[(row * NUMCOLS + col) + (cur_layer - 1) * NUMKEYS] = *(hid_code_array + n);
				printf("\r%32c", '\0');
				printf("\rRow: %d Column: %d Keycode: ", row, col);
				printf("%s", *(vk_name_array + n));
				// make sure Esc/Enter/Backspace can't fall through the while-loop
				if (n == 0 || n == 45 || n == 28 || n == 32)
					_getch();

				while (1)
				{
					ch = _getch();
					switch (ch)
					{
					case ASCII_ESC:
						print_keyfile(p_layout_array, nlayers);
						free(p_layout_array);
						disable_service_mode();
						exit(0);
					case ASCII_ENTER:
						keybd_event(VK_RETURN, 0, 2, 0);
						GetAsyncKeyState(VK_RETURN);
						rowcol_last = rowcol;
						printf("\nRow: %d Column: %d Keycode: ", row, col);
						goto GET_KEY;
					case ASCII_BKSPC:
						rowcol_last = rowcol;
						goto SET_LAYER;
					case ASCII_TAB:
						rowcol_last = rowcol;
						manual_input ^= 1;
						printf("\n\n");
						printf("+++ Manual key input enabled +++");
						printf("\n\n");
						goto GET_KEY;
					}

				}

			}
		Sleep(50);

		// we need to implement a cheap hack to read the Pause key, because it will freeze console output
		// when depressed until another key is depressed. We will call keybd_event() to send a non-defined 
		// key to the console to activate output again

		if (GetAsyncKeyState(VK_PAUSE) & 0x8000 && (last_keypress != 0xfe))
		{
			keybd_event(VK_OEM_8, 0, 0, 0);
			keybd_event(VK_OEM_8, 0, 2, 0);
			last_keypress = 0xfe;
			p_layout_array[(row * NUMCOLS + col) + (cur_layer - 1) * NUMKEYS] = KB_PAUSE;
			printf("\r%32c", '\0');
			printf("\rRow: %d Column: %d Keycode: ", row, col);
			printf("%s", "Pause");

			while (1)
			{
				ch = _getch();
				switch (ch)
				{
				case ASCII_ESC:
					print_keyfile(p_layout_array, nlayers);
					free(p_layout_array);
					disable_service_mode();
					exit(0);
				case ASCII_ENTER:
					keybd_event(VK_RETURN, 0, 2, 0);
					GetAsyncKeyState(VK_RETURN);
					rowcol_last = rowcol;
					printf("\nRow: %d Column: %d Keycode: ", row, col);
					goto GET_KEY;
				case ASCII_BKSPC:
					rowcol_last = rowcol;
					goto SET_LAYER;
				case ASCII_TAB:
					rowcol_last = rowcol;
					manual_input ^= 1;
					printf("\n\n");
					printf("+++ Manual key input enabled +++");
					printf("\n\n");
					goto GET_KEY;
				}
			}
		}
		rowcol_last = rowcol;

	}
}

#endif