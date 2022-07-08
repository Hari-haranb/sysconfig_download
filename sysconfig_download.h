/* using mqtt mosquitto send sysconfig message to AWS IoT core and get the response JSON in config/<DID>*/
void mqtt_pub_sub();

/* JSON message parsed by using JSON-C & encrypted file downloaded by using libcurl */
char* encrypt_file();

/* parsed key decoded by using openssl library interface */
const char* decode(const char* parse_key);

/* decrypted file downloaded by using openssl library interface (l-crypto) */
void decrypt_openssl(const char* decoded_key);
