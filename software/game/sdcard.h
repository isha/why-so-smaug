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
	if(sdcard == NULL) printf("Error: SD Card Module cannot be initialized\n");
	if(alt_up_sd_card_is_Present()) {
		if(alt_up_sd_card_is_FAT16()) printf("SD Card Module initialized\n");
		else printf("Error: File system not recognized.\n");
	} else printf("No Card Connected.\n");
}

//Opens a file whose name is stored in filename.
//If create is true, file will be created if it can't be found.
//Returns: short int containing file handle if successful,
// -1 if file could not be opened, -2 if file is already open
short sdcard_fopen(char* filename, bool create) {
	return alt_up_sd_card_fopen(filename, create); 
}

//Closes file denoted by handle in sdcard_file
//Returns: true if successful, false if failed
bool sdcard_fclose(short sdcard_file) {
	return alt_up_sd_card_fclose(sdcard_file); 
}


//Writes a single byte of data to the file denoted by handle in sdcard_file
//Returns: true if successful, false if failed
bool sdcard_write(short sdcard_file, unsigned char data) {
	return alt_up_sd_card_write(sdcard_file, data);
}

//Writes a string to the file identified by sdcard_file
//Returns: true if successful, false if failed
bool sdcard_print(short sdcard_file, char* string, int string_length) {
	int i;
	for(i = 0; i < string_length; i++) {
		if(!sdcard_write(sdcard_file, (unsigned) string[i])) return false;
	}
	return true;
}

//Reads a single byte of data to the file identified by sdcard_file
//Returns: a data byte if successful, -1 if file handle is invalid, -2 if it could not read from SD card
short sdcard_read(short sdcard_file) {
	return alt_up_sd_card_read(sdcard_file);
}

//Tests SD Card module
void sdcard_test() {
	int  i = 0;
	int file_handle;
	char* filename = "file.txt";
	char* content = "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789";
	char file_data[62];
	short file_byte;

	printf("\nTesting SD Card Module:\n")

	file_handle = sdcard_fopen(filename, true);
	switch(file_handle) {
		case -1:
			printf("File could not be opened\n");
			break;
		case -2:
			printf("File is already open\n");
		default:
			printf("%s has been opened!\n", filename);
	}

	if(sdcard_print(file_handle, content, 62)) {
		printf("%s has been written to the file\n", content);
		for(i = 0; i < 62; i++) {
			file_byte = sdcard_read(file_handle);
			if(file_byte == -1) {
				printf("Invalid file handle\n");
				break;
			} else if(file_byte == -2) {
				printf("Cannot read from SD Card\n");
				break;
			} else {
				file_data[i] = (char) file_byte;
			}
		}
		printf("File contains text: %s\n", file_data);
	} else printf("sdcard_print() failed.\n");
	
	sdcard_fclose(file_handle);
}

#endif
