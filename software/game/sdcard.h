/*
	NOTE: In order to fix some bugs with the SD Card module,
	replace the altera_up_sd_card_avalon_interface.c file in
	
	C:\path_to_quartus_II\ip\University_Program\Memory\altera_up_sd_card_avalon_interface\HAL\src

	with the one on Connect
*/

#ifndef SDCARD_H_
#define SDCARD_H_

#include <stdbool.h>
#include "altera_up_sd_card_avalon_interface.h"

alt_up_sd_card_dev* sdcard;


//Initializes SD Card module
void initialize_sdcard() {
	sdcard = alt_up_sd_card_open_dev("/dev/sdcard");
	if(sdcard == NULL) printf("\nError: SD Card Module cannot be initialized");
	if(!alt_up_sd_card_is_Present()){
		printf("\nSD Card not connected");
		return;
	}
	if(alt_up_sd_card_is_FAT16()) printf("\nSD Card Module initialized");
	else printf("Error: File system not recognized.\n");
}

//Opens a file whose name is stored in filename.
//If create is true, file will be created if it can't be found.
//Returns: short int containing file handle if successful,
// -1 if file could not be opened, -2 if file is already open
short int sdcard_fopen(char* filename, bool create) {
	return alt_up_sd_card_fopen(filename, create); 
}

//Closes file denoted by handle in sdcard_file
//Returns: true if successful, false if failed
bool sdcard_fclose(short int sdcard_file) {
	return alt_up_sd_card_fclose(sdcard_file); 
}


//Writes a single byte of data to the file denoted by handle in sdcard_file
//Returns: true if successful, false if failed
bool sdcard_write(short int sdcard_file, unsigned char data) {
	return alt_up_sd_card_write(sdcard_file, data);
}

//Writes a string to the file identified by sdcard_file
//Returns: true if successful, false if failed
bool sdcard_print(short int sdcard_file, unsigned char* string) {
	int i;
	for(i = 0; i < strlen(string); i++) {
		if(!sdcard_write(sdcard_file, string[i])) return false;
	}
	return true;
}

//Reads a single byte of data to the file identified by sdcard_file
//Returns: a data byte if successful, -1 if file handle is invalid, -2 if it could not read from SD card
short int sdcard_read(short int sdcard_file) {
	return alt_up_sd_card_read(sdcard_file);
}

//Tests SD Card module
void sdcard_test() {
	short int wfile_handle;
	short int rfile_handle;
	char* write_filename = "wfile.txt";
	char* read_filename = "rfile.txt";
	unsigned char* content = "abcdefg";
	short int read_byte = 0;

	printf("\nTesting SD Card Module:\n");

	//Test Writing Files
	wfile_handle = sdcard_fopen(write_filename, 1);

	switch(wfile_handle) {
		case -1:
			printf("File could not be opened.\n");
			break;
		case -2:
			printf("File is already open.\n");
			break;
		default:
			printf("%s opened. File Handle: %d\n", write_filename, wfile_handle);
			break;
	}

	if(sdcard_print(wfile_handle, content)) {
		printf("%s has been written to the file\n", content);
	} else printf("sdcard_print() failed.\n");
	
	if(sdcard_fclose(wfile_handle)) printf("File has been closed.\n");
		else printf("File could not be closed.\n");

	//Test Reading Files
	rfile_handle = sdcard_fopen(read_filename, 0);

	switch(rfile_handle) {
		case -1:
			printf("File could not be opened.\n");
			break;
		case -2:
			printf("File is already open.\n");
			break;
		default:
			printf("%s opened. File Handle: %d\n", read_filename, rfile_handle);
			while(read_byte != '.') {
				read_byte = sdcard_read(rfile_handle);
				printf("Got character: %c\n", read_byte);
			}
			break;
	}

	if(sdcard_fclose(rfile_handle)) printf("File has been closed.\n");
	else printf("File could not be closed.\n");

	printf("Finished testing SD Card Module");
}

#endif
