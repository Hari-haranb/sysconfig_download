#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <stdlib.h>

#define ENCRYPT_FILE "encrypt_file.txt"

unsigned char key[32];

static unsigned char byte_convert(unsigned char val)
{
	if (val >= 'a' && val <= 'f')
		return val - 'W';
	if (val >= '0' && val <= '9')
		return val - '0';
	
	return 0xff;
}
void decrypt_openssl(const char* decoded_key)
{
	FILE *file_p;
	uint8_t iv[12];
	uint8_t tag[16];

	int i,j;
	for (i = 0, j = 0; i < strlen(decoded_key); j++) {
	key[j] = 0xff & ( byte_convert(decoded_key[i++]) << 4 |  byte_convert(decoded_key[i++]));
   	}

	file_p = fopen(ENCRYPT_FILE,"r");
	if(file_p == NULL){
		printf("Unable to open the file\n");
		exit(1);
	}
	fseek(file_p, 0, SEEK_END);
	long fsize = ftell(file_p);
	fseek(file_p, 0, SEEK_SET);

	if(file_p == NULL){
		printf("File cannot be open \n");
		exit(1);
	}
	char *encrypted_text;
	encrypted_text = malloc(fsize);

	memset(encrypted_text, 0, fsize);

	int rc = fread(encrypted_text, 1, fsize, file_p);

	fclose(file_p);

	size_t out_len = 0;
	size_t in_len = 0;
	
	EVP_CIPHER_CTX *ctx = NULL;
	
	ctx = EVP_CIPHER_CTX_new();

	rc = EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
        
	if(!rc){
		printf("Failed to init (%d)\n", rc);
		exit(1);
	}

	memcpy(iv, encrypted_text, 12);

	rc = EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);

	if(!rc){
		printf("Failed to set key (%d)\n", rc);
		exit(1);
	}
	in_len = fsize  - 12 - 16;
	char outbuf[in_len];

	memset(outbuf, 0, in_len);

	rc = EVP_DecryptUpdate(ctx, outbuf, &out_len,  &encrypted_text[12], in_len);
	if(!rc){
		
		printf("Failed to decrypt update (%d)\n", rc);
		exit(1);
	}

	rc = EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG,16, &encrypted_text[in_len + 12]);
	if(!rc){
		printf("Failed to set tag (%d)\n", rc);
		exit(1);
	}
	
	rc = EVP_DecryptFinal_ex(ctx, outbuf, &out_len);
	if(!rc){
		printf("Failed to decrypt final (%d)\n", rc);
		exit(1);
	}

	FILE *op;
	op = fopen("decrypt.json", "w");
	if(op == NULL){
		printf("download failed \n");
		exit(1);
	}
	else
		printf("\n Decrypted text downloaded successfully(decrypt.json) \n");

	fprintf(op,"%s",&outbuf);
	fclose(op);
}
