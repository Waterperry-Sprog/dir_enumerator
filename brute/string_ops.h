#include <stdlib.h>

char *append(char*,char);
char *join(char*,char*);
int len(char*);

char *append(char *str, char c){
	int len_s = len(str);
	char *new = malloc(len_s + sizeof(char));

	for(int i = 0; i < len_s; i++){
		new[i] = str[i];
	}
	new[len_s] = c;
	return new;
}

char *join(char *a, char *b){
	int a_len = len(a);
	int b_len = len(b);
	char *str = malloc(a_len + b_len + 1);
	for(int i = 0; i < a_len; i++){
		str[i] = a[i];
	}
	for(int i = a_len; i < a_len + b_len; i++){
		str[i] = b[i - a_len];
	}
	str[a_len + b_len] = 0;
	return str;
}

int len(char *str){
	int len = 0;
	while(str[len]!='\0') len++;
	return len;
}

int contains(char *a, char *b){
	int i = 0, a_len = len(a), b_len = len(b);
	while(i != a_len){
		while(a[i] != b[0]){
			if(a[i] == 0) return 0;
			i++;
		}
		int matched = 1;
		for(int j = 1; j < b_len; j++){
			if(a[i+j] != b[j]) {
				matched = 0;
				break;
			}
		}
		if(matched) return 1;
		i++;
	}
	return 0;
}
