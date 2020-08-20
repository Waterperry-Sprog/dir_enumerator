#include <stdio.h>
#include <stdlib.h>
#include "string_ops.h"

//general usage - ALWAYS run build_char_set with your desired option.
	//then, you can run generate(char_set, max_length) with either the literal 'char_set' or your custom set.
	//or you can run get_next(string) to get the next string within the set char set.

//modes
	// 1 - lower
	// 2 - lower + num
	// 3 - lower + upper
	// 4 - lower + upper + num
	// 5 - upper
	// 6 - upper + num
	// 7 - num

//char-sets
char low_alph[] = "abcdefghijklmnopqrstuvwxyz";
char upp_alph[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char nums[] = "0123456789";

//variables
static char *char_set;
static int set_size = 0;
static int backtrack = 0;


//prototypes
void build_char_set(int);
char next(char);
char *get_next(char*);
void generate(char *set, int max_length);

void build_char_set(int mode){
	char_set = malloc(sizeof(char));
	if(mode < 5) {
		char *t = join(char_set, low_alph);
		free(char_set);
		char_set = t;
		set_size+=26;
	}
	if(mode > 2 && mode < 7){
		char *t = join(char_set, upp_alph);
		free(char_set);
		char_set = t;
		set_size+=26;
	}
	if(mode%2 == 0 || mode == 7){
		char *t = join(char_set, nums);
		free(char_set);
		char_set = t;
		set_size += 10;
	}
}

//return the next character in the sequence. raise a global flag if the sequencer has reset.
char next(char c){
	int i = 0;
	while(char_set[i++]!=c){
		if(i == set_size) {
			printf("error! sequencing character {%c : %d} outside of char list.\n", c,c);
			printf("you're probably using the wrong charset.\n");
			return 0;
		}
	}
	if(i != set_size) return char_set[i];
	backtrack = 1;
	return char_set[0];
}

void permute(char *set, int length){
	char *str = malloc((length + 1) * sizeof(char));
	for(int i = 0; i < length; i++){
		str[i] = char_set[0];
	}

	int i = length-1, c = 0;

	while(i!=-1){
		backtrack = 0;
		c = i - 1;
		printf("%s\n", str);
		str[i] = next(str[i]);
		while(backtrack && i!=-1){
			backtrack = 0;
			if(c<0) return;
			str[c] = next(str[c]);
			c--;
		}
		i = length - 1;
	}

}

char *get_next(char *s){
	backtrack = 0;
	int i = len(s) - 1;
	s[i] = next(s[i]);
	while(backtrack && i != 0){
		backtrack = 0;
		i--;
		s[i] = next(s[i]);
	}
	if(backtrack) {
		int retlen = len(s)+1;
		char *s2 = malloc(sizeof(char) * retlen);
		for(int i = 0; i < retlen; i++){
			s2[i] = 'a';
		}
		s = s2;
	}
	backtrack = 0;
	return s;
}

void generate(char *set, int max_length){
	for(int i = 1; i <= max_length; i++){
		permute(set, i);
	}
}
