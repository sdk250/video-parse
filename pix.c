#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <regex.h>
#include "cJSON.h"
#define ERRMSG "\33[1;31mError: \33[0m"
#define PORT 80
#define BUFSIZE 2048
#define SOCKBUF 2097152
#define API_H5 "h5.pipix.com"
#define API_PIX "is.snssdk.com"
#define USER_AGENT "Mozilla/5.0 (Linux; Android 12; XT2201-2) " \
	"AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.57 Mobile Safari/537.36"
#define ACCEPT "Application/json; Charset: UTF-8"
#define CONNECTION "Close"

unsigned long int fn(char *restrict, const char *restrict, const char *restrict);
char *regex(char *restrict, const char *restrict);

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		fprintf(stderr, "%sParameter is little.\n", ERRMSG);
		exit(EXIT_FAILURE);
	}
	char *msg = (char *)malloc(BUFSIZE * sizeof(char));
	char *addr = (char *)malloc(BUFSIZE * sizeof(char));
	char *buffer = (char *)malloc(SOCKBUF * sizeof(char));
	char *buffer_ = (char *)malloc(SOCKBUF * sizeof(char));
	char *god_comment = NULL;
	char *ch = NULL;
	cJSON *json = NULL;
	cJSON *obj = NULL;
	cJSON *o = NULL;
	FILE *fp = NULL;

	memset(addr, '\0', BUFSIZE * sizeof(char));
	memset(msg, '\0', BUFSIZE * sizeof(char));
	memset(buffer, '\0', SOCKBUF * sizeof(char));
	memset(buffer_, '\0', SOCKBUF * sizeof(char));
	strcpy(addr, regex(argv[1], "/{1}[a-z0-9A-Z]{5,}") + 1);
	sprintf(msg, "GET /s/%s HTTP/1.1\n"
		"Host: %s\n"
		"User-Agent: %s\n"
		"Accept: %s\n"
		"Connection: %s\n"
		"\n"
		"{}",
		addr, API_H5, USER_AGENT, ACCEPT, CONNECTION
	);
	fn(buffer, msg, API_H5);

	memset(msg, '\0', BUFSIZE * sizeof(char));
	memset(addr, '\0', BUFSIZE * sizeof(char));
	strcpy(addr, regex(buffer, "item/?[0-9]{10,}"/*"(http|https)://h5.?pipix.?com/{1}item/{1}[0-9]{1,}\\?"*/) + 5);
	memset(buffer, '\0', SOCKBUF * sizeof(char));
	sprintf(msg, "GET /bds/cell/detail/?cell_type=1&aid=1319&app_name=super&cell_id=%s HTTP/1.1\n"
		"Host: %s\n"
		"User-Agent: %s\n"
		"Accept: %s\n"
		"Connection: %s\n"
		"\n"
		"{}",
		addr, API_PIX, USER_AGENT, ACCEPT, CONNECTION
	);
	fn(buffer, msg, API_PIX);

	memset(msg, '\0', BUFSIZE * sizeof(char));
	memset(addr, '\0', BUFSIZE * sizeof(char));
	strcpy(buffer_, strchr(buffer, '{'));
	ch = strchr(buffer_, '\n');
	*ch = '\0';
	strcat(buffer_, ch + 5);
	/*unsigned int j = 0;
	for (unsigned int i = 0; *(buffer_ + i) != '\0'; i++)
	{
		if (*(buffer + i) == '\n'
	*/
	printf("%s\nlength: %zd\nstrlen: %zd\n", buffer_, sizeof(buffer_), strlen(buffer_));
	
	fp = fopen("data", "w");
	fwrite(buffer_, sizeof(char), strlen(buffer_), fp);
	json = cJSON_Parse(buffer_);
	obj = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data")->child, "item");
	o = cJSON_GetObjectItem(obj, "comments");
	printf("%s\n", obj->valuestring);
	printf("%d\n", o->type);
	puts(god_comment);

	fclose(fp);
	cJSON_Delete(json);
	free(msg);
	free(addr);
	free(buffer);
	free(buffer_);
	return 0;
}

unsigned long int fn(char *restrict buffer, const char *restrict msg, const char *restrict addr)
{
	int sockfd = -1;
	struct sockaddr_in addr_;
	struct hostent *host = NULL;
	long int recvb = 0;
	unsigned long int recvbyte = 0;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		fprintf(stderr, "%sCreate socket-description fail. socket:%d\n", ERRMSG, sockfd);
	printf("Create socket-file-description suceess: %d\n", sockfd);

	if (!(host = gethostbyname(addr)))
		fprintf(stderr, "%sGet IP of %s fail.\n", ERRMSG, addr);
	memset(&addr_, 0, sizeof(struct sockaddr));
	addr_.sin_family = AF_INET;
	addr_.sin_addr = *((struct in_addr *)host->h_addr_list[0]);
	addr_.sin_port = htons(PORT);

	if ((connect(sockfd, (struct sockaddr *)&addr_, sizeof(struct sockaddr))) < 0)
		fprintf(stderr, "%sConnect to %s of %s fail.\n", ERRMSG, inet_ntoa(addr_.sin_addr), addr);
	printf("Connect to \33[1;46m%s\33[0m of %s success.\n", inet_ntoa(addr_.sin_addr), addr);
	if ((send(sockfd, msg, strlen(msg), MSG_WAITALL)) < 0)
		fprintf(stderr, "%sRequest %s fail.\n", ERRMSG, addr);
	while ((recvb = recv(sockfd, buffer + recvbyte, SOCKBUF, MSG_WAITALL)) > 0)
		recvbyte += recvb;
	close(sockfd);
	return recvbyte;
}

char *regex(char *restrict string, const char *restrict pattern)
{
	char *buffer = (char *)malloc(SOCKBUF * sizeof(char));
	char ebuf[256] = {'\0'};
	regex_t reg;
	const size_t nmatch = 10;
	regmatch_t pmatch[nmatch];
	int reg_err_code = 0;

	memset(buffer, '\0', SOCKBUF * sizeof(char));
	if ((reg_err_code = regcomp(&reg, pattern, REG_EXTENDED)) != 0)
	{
		regerror(reg_err_code, &reg, ebuf, sizeof(ebuf) / sizeof(char));
		fprintf(stderr, "%s%s: Pattern: %s\n", ERRMSG, ebuf, pattern);
		exit(EXIT_FAILURE);
	}
	if ((reg_err_code = regexec(&reg, string, nmatch, pmatch, 0)) == REG_NOMATCH)
	{
		printf("%s\n%s\n", "Not found result: Your share link is error.", string);
		exit(EXIT_SUCCESS);
	} else if (reg_err_code != 0)
	{
		regerror(reg_err_code, &reg, ebuf, sizeof(ebuf) / sizeof(char));
		fprintf(stderr, "%s%s: Pattern2: %s\n", ERRMSG, ebuf, pattern);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < nmatch && pmatch[i].rm_so != -1; i++)
		strncpy(buffer, string + pmatch[i].rm_so, pmatch[i].rm_eo - pmatch[i].rm_so);
	strncpy(string, buffer, strlen(buffer) + 1);
	regfree(&reg);
	free(buffer);
	return string;
}