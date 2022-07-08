#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sysconfig_download.h"

int main()
{
       
	const char* decode_key;
	const char* parse_key;

	mqtt_pub_sub();
	
	parse_key = encrypt_file();
        
	decode_key= decode(parse_key);

	decrypt_openssl(decode_key);


	
}
