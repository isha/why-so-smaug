/*
 * scoreboard.h
 *
 *  Created on: Oct 15, 2013
 *      Author: Home
 */

#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "player.h"
#include "sdcard.h"

#define MAX_HIGH_SCORES 10
#define MAX_TOKEN_SIZE 30
#define PLAYER_NAME 1
#define PLAYER_SCORE 2
#define PLAYER_TIME 3
#define SCORE_X 20
#define SCORE_Y 10
#define SCORE_SPACING 2

typedef struct {
	unsigned char* player_name;
	int player_score;
} Score;

Score* high_scores[MAX_HIGH_SCORES]; //scores from [0-9]
char* scoreboard_string = "";
int total_entries = 0;

void append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

void clear_token(char* s) {
	memset(s, '\0', sizeof(s));
	// free(s);
}

Score* construct_score(char* player_name, int player_score) {
	Score* score = malloc(sizeof(Score));
	score->player_name = player_name;
	score->player_score = player_score;
	return score;
}

// assumes scoreboard is already sorted
void update_scoreboard(Player* player) {
	printf("out\n");
	if (total_entries != 0) {
		printf("in\n");
		int i = total_entries - 1;

		while (player->score > high_scores[i]->player_score && i>0) {
			i--;
			printf("i = %d\n", i);
		}
		if (player->score > high_scores[i]->player_score) {
			int j;
			for (j = total_entries; j>i; j--) {
				printf("j = %d\n", j);
				if (j < MAX_HIGH_SCORES) {
					high_scores[j] = high_scores[j-1];
				}
			}

			high_scores[i] = construct_score(player->screen_name, player->score);
			if (total_entries < 10) {
				total_entries++;
			}
		} else {
			high_scores[0] = construct_score(player->screen_name, player->score);
		}

	}
}


void read_scores_from_sd_card() {

		int rfile_handle;
		char* filename = "score.txt";
		char* tokenPtr;
		char* player_name;
		unsigned char read_byte = ' ';
		short int entry_number = -1;
		short int column_position = PLAYER_NAME;
		unsigned char* current_token;
		unsigned char* name_holder[MAX_HIGH_SCORES];
		short int i = 0;
		char temp;

		clear_token(current_token);

		rfile_handle = (unsigned char)sdcard_fopen(filename, 0);

		switch(rfile_handle) {
			case -1:
				printf("File could not be opened.\n");
				break;
			case -2:
				printf("File is already open.\n");
				break;
			default:
				printf("%s opened. File Handle: %d\n", filename, 0);

				while(1) {
					read_byte = sdcard_read(rfile_handle);

					if (read_byte == '.') break;
					else if (read_byte == ',') {
						if (column_position == PLAYER_NAME) {
							name_holder[entry_number] = malloc(40 * sizeof(char));
							strcpy(name_holder[entry_number], current_token);
							high_scores[entry_number]->player_name = name_holder[entry_number];
							column_position++;
							clear_token(current_token);
						}
						else if (column_position == PLAYER_SCORE) {
							high_scores[entry_number]->player_score = strtol(current_token, &current_token, 10);
							column_position = PLAYER_NAME;
							clear_token(current_token);
						}
					}
					else if (read_byte == ':') {
						entry_number++;
						total_entries++;
						high_scores[entry_number] = construct_score("",0);
					}
					else {
						append(current_token, read_byte);
					}

				}
				break;
		}

		if(sdcard_fclose(rfile_handle)) printf("File has been closed.\n");
		else printf("File could not be closed.\n");

		// ensure the rest of the entries in the highscores are set to null
}

// should only be called if an update needs to be made to scoreboard on sd card
// the current, updated scoreboard should already be in a single string
void write_scores_to_sd_card() {
	short int wfile_handle;
	char* write_filename = "score.txt";
	unsigned char* content = "XYZABCDEFGH";
	short int read_byte = 0;

	lengthen_scoreboard_string();

	// regardless, write our updated string to file
	wfile_handle = sdcard_fopen(write_filename, 0);

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

	if(sdcard_print(wfile_handle, scoreboard_string)) {
		printf("%s has been written to the file\n", scoreboard_string);
	} else printf("sdcard_print() failed.\n");

	if(sdcard_fclose(wfile_handle)) printf("File has been closed.\n");
		else printf("File could not be closed.\n");
}

int count_chars_in_score_file() {
	int rfile_handle;
	char* filename = "score.txt";
	unsigned char read_byte = ' ';
	short int i = 0;
	int char_count = 0;


	rfile_handle = (unsigned char)sdcard_fopen(filename, 0);

	switch(rfile_handle) {
		case -1:
			printf("File could not be opened.\n");
			break;
		case -2:
			printf("File is already open.\n");
			break;
		default:
			printf("%s opened. File Handle: %d\n", filename, 0);

			while(1) {
				i = sdcard_read(rfile_handle);
				if (i==-1) {
					break;
				}
				else {
					char_count++;
				}
			}

			break;
	}

	if(sdcard_fclose(rfile_handle)) printf("File has been closed.\n");
	else printf("File could not be closed.\n");

	return char_count;
}

void convert_scoreboard_to_string() {
	printf("\nScore");

	int scoreboard_size = total_entries;
	int current_entry = 0;
	char char_buffer[30];
	printf("\nScoreboard size: %d", scoreboard_size);
	printf("\nTotal entries: %d", total_entries);

//	while (current_entry < scoreboard_size) {
//		printf("\nCurrent entry: %d", current_entry);
//		append(scoreboard_string, ':');
//		strcat(scoreboard_string, high_scores[current_entry]->player_name);
//		append(scoreboard_string, ',');
//		sprintf(char_buffer, "%d", high_scores[current_entry]->player_score);
//		strcat(scoreboard_string, char_buffer);
//		append(scoreboard_string, ',');
//		current_entry++;
//	}
//
//	append(scoreboard_string, '.');
//}
}

// returns the number of entries currently in the scoreboard
int get_scoreboard_size() {
	int entry_number = 0;
	while (high_scores[entry_number] != NULL) {
		// printf("\n%p", &high_scores[entry_number]);
		entry_number++;
		// printf("\n%d", entry_number);
	}
	return entry_number;
}

void initialize_scoreboard() {

}

void sdcard_write_test() {
	short int wfile_handle;
	char* write_filename = "wfile.txt";
	unsigned char* content = "abcdefg";

	printf("\nTesting SD Card Module:\n");

	//Test Writing Files
	wfile_handle = sdcard_fopen(write_filename, 0);

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
}

int lengthen_scoreboard_string() {
	int chars_in_file = count_chars_in_score_file();
	int chars_in_updated_scoreboard = strlen(scoreboard_string);
	int counter = chars_in_updated_scoreboard;

	// if what should be written to the file is shorter than or equal to what is currently in the file
	if (chars_in_updated_scoreboard <= chars_in_file) {
		// then, we need to append enough characters to the string that will be written
		while (counter <= chars_in_file) {
			append(scoreboard_string, '*');
			counter++;
		}
	}
}

void display_scoreboard() {
	int i, j;
	int entry_number = 0;

	char string1[50];
	char string2[50];
	char string3[50];

	char player_numbers[MAX_HIGH_SCORES][2];
	char player_names[MAX_HIGH_SCORES][50];
	char player_scores[MAX_HIGH_SCORES][10];

	for (i=0; i<RESOLUTION_X; i++){
		for (j=0; j<RESOLUTION_Y; j++){
				draw_pixel_fast(pixel_buffer, 0, i, j);
				old_pixel_colors[i][j] = pixel_colors[i][j];
				alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
				draw_pixel_fast(pixel_buffer, 0, i, j);
				alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		}
	}

	alt_up_char_buffer_clear(char_buffer);

	// display titles
	sprintf(string1, "%s", "HIGH SCORES");
	sprintf(string2, "%s", "NAME");
	sprintf(string3, "%s", "SCORE");

	alt_up_char_buffer_string(char_buffer, string1, 35, SCORE_Y);
	alt_up_char_buffer_string(char_buffer, string2, SCORE_X, SCORE_Y+5);
	alt_up_char_buffer_string(char_buffer, string3, SCORE_X+38, SCORE_Y+5);

	while (entry_number < total_entries) {

		sprintf(player_numbers[entry_number], "%d", entry_number+1);
		sprintf(player_names[entry_number], "%s", high_scores[entry_number]->player_name);
		sprintf(player_scores[entry_number], "%d", high_scores[entry_number]->player_score);

		alt_up_char_buffer_string(char_buffer, player_numbers[entry_number], SCORE_X-4, SCORE_Y + 6 + (entry_number+1)*SCORE_SPACING);
		alt_up_char_buffer_string(char_buffer, player_names[entry_number], SCORE_X, SCORE_Y + 6 + (entry_number+1)*SCORE_SPACING);
		alt_up_char_buffer_string(char_buffer, player_scores[entry_number], SCORE_X+38, SCORE_Y + 6 + (entry_number+1)*SCORE_SPACING);

		entry_number++;
	}



}

#endif /* SCOREBOARD_H_ */
