#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>

int decode_Length(const char* b64input) { 
	int len = strlen(b64input);
	int pd = 0;
	if (b64input[len-1] == '=' && b64input[len-2] == '=')
	pd = 2;
	else if (b64input[len-1] == '=') 
	pd = 1;
	return (int)len*0.75 - pd;
}

int Base64Decode(const char* b64message, char** buffer) { 
	BIO *bio, *b64;
	int decodeLen = decode_Length(b64message);
	int len = 0;
	*buffer = (char*)malloc(decodeLen+1);
	FILE* stream = fmemopen(b64message, strlen(b64message), "r");

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new_fp(stream, BIO_NOCLOSE);
	bio = BIO_push(b64, bio);
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	len = BIO_read(bio, *buffer, strlen(b64message));
	(*buffer)[len] = '\0';
	
	BIO_free_all(bio);
	fclose(stream);

	return (0); 
}

const char* decode(const char* parse_key) {
  
	const char* base64DecodeOutput;
	Base64Decode(parse_key, &base64DecodeOutput);
	return(base64DecodeOutput);
}
