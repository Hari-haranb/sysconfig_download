#include<stdio.h>
#include<json.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#define ENCRYPT_FILE		"encrypt_file.txt"
#define JSON_OUTPUT_FILE	"json_message.json"
char parse_url[512];
const char* parse_out;
void libcurl() {

	CURL *curl;
	FILE *fp;
	int result;

	fp = fopen (ENCRYPT_FILE,"wb");

	curl =curl_easy_init();

	curl_easy_setopt(curl,CURLOPT_URL,parse_url);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
	curl_easy_setopt(curl,CURLOPT_FAILONERROR,1L);

	result = curl_easy_perform(curl);

	if(result == CURLE_OK)
	printf("\n Encrypted file Downloaded successfully \n");
	else
	{
	printf("ERROR:%s\n",curl_easy_strerror(result));
	exit(1);
	}
	fclose(fp);

	curl_easy_cleanup(curl);
}
void json_parse() {

	char* url;
	int string_len = 10;
	struct json_object *obj =json_object_from_file(JSON_OUTPUT_FILE);
	struct json_object *location;
 	struct json_object *obj1=NULL;
	struct json_object *host;
	struct json_object *protocol;
	struct json_object *path;
	struct json_object *ptkey;

	json_object_object_get_ex(obj, "location", &obj1);
	json_object_object_get_ex(obj1, "protocol", &location);
	json_object_object_get_ex(obj1, "host", &protocol);
	json_object_object_get_ex(obj1, "path", &host);

	int protocol_len = json_object_get_string_len(protocol);
	int host_len = json_object_get_string_len(host);
	int url_len = string_len + protocol_len + host_len;

	snprintf(parse_url,url_len,"https://%s/%s\n", json_object_get_string(protocol),json_object_get_string(host));
       
	json_object_object_get_ex(obj, "ptkey", &ptkey);
	parse_out = json_object_get_string(ptkey);
}
const char* encrypt_file(){

	json_parse();
	printf("\n JSON message parsed successfully \n");

	libcurl();

        return parse_out;
}                                                                                                                                   
