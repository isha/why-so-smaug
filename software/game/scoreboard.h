#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include<stdio.h>
#include<string.h>
#include "player.h"

#define MAX_HIGH_SCORES 10

typedef struct {
	Player player;
} Score;

Score high_scores[MAX_HIGH_SCORES];

Score create_score(Player player) {
	Score score;
	score.player = player;
	return score;
}
void update_scoreboard(Player player) {
	Score new_score = create_score(player);

	int i = MAX_HIGH_SCORES;
	while (new_score.player.score >= high_scores[i-1].player.score && i>0) i--;

	int j;
	for (j = MAX_HIGH_SCORES-1; j>i; j--) {
		high_scores[j] = high_scores[j-1];
	}

	high_scores[i] = new_score;	// this line is creating problems
}

void test_scoreboard() {
	int s[MAX_HIGH_SCORES] = {99,80,73,70,68,60,55,54,50,25};

	int i;
	for (i=0; i<MAX_HIGH_SCORES; i++){
		Score score;
		score.player.score = s[i];
		high_scores[i] = score;
	}
}

void print_scoreboard(){
	int i;
	for (i = 0; i < MAX_HIGH_SCORES; i++){
		printf("\nAt %d score is %d", i, high_scores[i].player.score);
	}
}




// read data from SD card
// compare score with data from SD card
// insert data into SD card

//void read_scores_from_sd_card() {
//	int file_handle;
//	char* filename = "scoreboard.txt";
//
//	file_handle = sdcard_fopen(filename, TRUE);
//
//	switch(file_handle) {
//		case -1:
//			alt_printf("File could not be opened");
//			break;
//		case -2:
//			alt_printf("File is already open");
//		default:
//			alt_printf("%s has been opened!", filename);
//	}

//	// save into highscores[]
//}

#endif
