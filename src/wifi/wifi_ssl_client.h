#ifndef WIFI_SSL_H
#define WIFI_SSL_H
#include <stddef.h>
#include <stdint.h>

/* Keep the new TLS ABI out of SDK translation units using legacy mbedTLS. */
typedef struct wifi_sslclient_context wifi_sslclient_context;


wifi_sslclient_context* wifi_ssl_client_create();
void wifi_ssl_client_destroy(wifi_sslclient_context *ssl_client);
void wifi_ssl_init(wifi_sslclient_context *ssl_client);
void wifi_ssl_set_socket(wifi_sslclient_context *ssl_client, int socket);
void wifi_ssl_set_timeout(wifi_sslclient_context *ssl_client, unsigned long handshake_timeout);
int wifi_ssl_get_socket(wifi_sslclient_context *ssl_client);
unsigned long wifi_ssl_get_timeout(wifi_sslclient_context *ssl_client);
uint32_t wifi_ssl_set_rootCA(wifi_sslclient_context *ssl_client, char *rootCABuff);
uint32_t wifi_ssl_get_rootCA(wifi_sslclient_context *ssl_client);
uint32_t wifi_ssl_set_cliCert(wifi_sslclient_context *ssl_client, char *cli_cert);
uint32_t wifi_ssl_get_cliCert(wifi_sslclient_context *ssl_client);
uint32_t wifi_ssl_set_cliKey(wifi_sslclient_context *ssl_client, char *cli_key);
uint32_t wifi_ssl_get_cliKey(wifi_sslclient_context *ssl_client);
uint32_t wifi_ssl_set_pskIdent(wifi_sslclient_context *ssl_client, char *pskIdent);
uint32_t wifi_ssl_get_pskIdent(wifi_sslclient_context *ssl_client);
uint32_t wifi_ssl_set_psKey(wifi_sslclient_context *ssl_client, char *psKey);
uint32_t wifi_ssl_get_psKey(wifi_sslclient_context *ssl_client);
int wifi_start_ssl_client(wifi_sslclient_context *ssl_client, const char *host, uint32_t port, int timeout, const char *rootCABuff, const char *cli_cert, const char *cli_key, const char *pskIdent, const char *psKey);
void wifi_stop_ssl_socket(wifi_sslclient_context *ssl_client, const char *rootCABuff, const char *cli_cert, const char *cli_key);
int wifi_data_to_read(wifi_sslclient_context *ssl_client);
int wifi_send_ssl_data(wifi_sslclient_context *ssl_client, const uint8_t *data, uint16_t len);
int wifi_get_ssl_receive(wifi_sslclient_context *ssl_client, uint8_t *data, int length);
int wifi_verify_ssl_fingerprint(wifi_sslclient_context *ssl_client, const char* fp, const char* domain_name);
int wifi_verify_ssl_dn(wifi_sslclient_context *ssl_client, const char* domain_name);
void wifi_ssl_strerror( int errnum, char *buffer, size_t buflen );

#endif
