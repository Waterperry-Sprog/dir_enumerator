#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include "brute/brute.h"
#include "LinkedList.h"

#define DEBUG 0
#define BUF_SIZE 1000
#define TOLERANCE 5

int is_error_getting(char*);
int parse(char*);
void proc_args(int, char**);

char ua0[] = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/62.0.3202.62 Safari/537.36";
char ua1[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36 RuxitSynthetic/1.0 v106115785045586345 t1236787695256497497";
char ua2[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36 RuxitSynthetic/1.0 v2176234390366593739 t6331743126571670211";
char ua3[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36 RuxitSynthetic/1.0 v3526981220060533722 t6281935149377429786";
char ua4[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36 RuxitSynthetic/1.0 v375773916935922917 t3345461284722333977";
char ua5[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36 RuxitSynthetic/1.0 v1886929434576063943 t7889551165227354132";
char ua6[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36 RuxitSynthetic/1.0 v3984897232428080245 t7607367907735283829";
char ua7[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36 RuxitSynthetic/1.0 v2576531858668889742 t4157550440124640339";
char ua8[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36 RuxitSynthetic/1.0 v8499071069320348013 t1191530496833852085";
char ua9[] = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.87 Safari/537.36 RuxitSynthetic/1.0 v3770199914252011773 t1236787695256497497";
int ua_ptr = 0, use_wordlist = 0;
char *uas[] = {ua0, ua1, ua2, ua3, ua4, ua5, ua6, ua7, ua8, ua9};
char *UA_STR, *wordlist;
struct addrinfo params, *servinfo;
int sockfd, conn_err, status, err_count = 0;
int UPPERLEN = 4, LOWERLEN = 1;
char *WEBPORT;
char *TARGET_IP;


void usage(char *progname){
	printf("Usage: %s [-i <Target IP> -p <Port> { -w <wordlist> | -l <lower-length> -u <upper-length> }]\n", progname);
	printf("\t-w : provide the full path to a wordlist of directories to enumerate.\n");
	printf("\t-l : the shortest length of file name to try to brute force\n");
	printf("\t-u : the longest length of file name to try to brute force\n");
}

int main(int argc, char **argv){

	TARGET_IP = malloc(sizeof(char) * 11);
	TARGET_IP = "127.0.0.1";
	WEBPORT = malloc(sizeof(char) *6);
	WEBPORT = "8000";

	if(DEBUG) printf("%s:%s\n", TARGET_IP, WEBPORT);

	if(argc < 2) {
		usage(argv[0]);
		printf("proceeding with default values.\n\t->enumerating %s:%s with brute from %d to %d\n", TARGET_IP, WEBPORT, LOWERLEN, UPPERLEN);
	}
	else proc_args(argc, argv);

	if(UPPERLEN <= LOWERLEN) {
		printf("upper bruteforce length should be greater than lower\n");
		return 0;
	}

	UA_STR = uas[ua_ptr];

	List *l;
	node *n;
	char *x;

	if(use_wordlist){
		List *l = createList();
		import(l, wordlist);
		if(DEBUG) printList(l);
		n = l->head;
		x = n->data;
	}
	else{
		build_char_set(1);
		x = malloc(sizeof(char) * 20);
		for(int i = 0; i < LOWERLEN; i++){
			x[i] = 'a';
		}
	}

	memset(&params, 0, sizeof params);
	params.ai_family = AF_UNSPEC;
	params.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(TARGET_IP, WEBPORT, &params, &servinfo);

	while(1){
		if(err_count > TOLERANCE) {
			printf("Maximum error count reached. Try increasing the tolerance value"
				" and make sure that all the options you provided are correct.\n");
			exit(0);
		}
		if((len(x) >= UPPERLEN && (! use_wordlist)) || ((use_wordlist) && n==NULL)){
			printf("< end >\n");
			return 0;
		}

		if(is_error_getting(x)){
			printf("Directory found - /%s\n",x);
		}

		if(use_wordlist) {
			x = n->data;
			n = n->next;
		}
		else x = get_next(x);
		if(DEBUG) printf("got next, from %s\n", x);
	}
	printf("< end >\n");
	return 0;
}

void proc_args(int argc, char **argv){
	for(int i = 0; i < argc; i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'w':use_wordlist=1;
					if(i+1 >= argc || argv[i+1][0] == '-') {
						printf("incorrect usage.\n");
						exit(0);
					}
					wordlist = argv[i+1];
					break;
				case 'l':LOWERLEN = atoi(argv[i+1]);
					if(i+1 >= argc || argv[i+1][0] == '-') {
						printf("incorrect usage.\n");
						exit(0);
					}
					break;
				case 'u':UPPERLEN = atoi(argv[i+1]);
					if(i+1 >= argc || argv[i+1][0] == '-') {
						printf("incorrect usage.\n");
						exit(0);
					}
					break;
				case 'i':if(i+1 >= argc || argv[i+1][0] == '-') {
						printf("incorrect usage.\n");
						exit(0);
					}
					TARGET_IP = argv[i+1];
					break;
				case 'p':if(i+1>=argc || argv[i+1][0] == '-'){
						printf("incorrect usage.\n");
						exit(0);
					}
					WEBPORT = argv[i+1];
					break;
			}
		}
	}
}

char *build_get(char *url){
	char *rqst = malloc(sizeof(char));
	rqst = join(rqst, "GET /");
	rqst = join(rqst, url);
	rqst = join(rqst, "\nUser-Agent: ");
	rqst = join(rqst, UA_STR);
	rqst = join(rqst, "\nAccept: */*\r\n\r\n");
	return rqst;
}

int is_error_getting(char *str){
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);	//open socket
	conn_err = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);	//attempt connection

	if(DEBUG) printf("entered is_error_getting.\n");
	if(conn_err){
		printf("Error connecting - cycling User Agent to elude detection.\n");
		err_count++;
		ua_ptr++;
		UA_STR = uas[ua_ptr];
		return 0;
	}

	if(DEBUG) printf("building string.\n");

	char *x = build_get(str);

	if(DEBUG) printf("sending request.\n");

	int bytes_rem = send(sockfd, x, len(x), 0);
	if(bytes_rem == -1) {
		perror("error on socket!");
		exit(errno);
	}

	if(DEBUG) printf("getting response.\n");

	char *response = malloc(sizeof(char) * BUF_SIZE);
	int resp_code = recv(sockfd, response, BUF_SIZE, 0);
	if(resp_code == -1) {
		perror("response from target invalid!");
		exit(errno);
	}

	if(DEBUG) printf("begin parse.\n");
	int r_code = parse(response);

	close(sockfd);

	return r_code;
}

int parse(char *response){
	if(contains(response, "404")) return 0;
	return 1;
}
