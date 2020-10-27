#ifndef ARD_SSL_H
#define ARD_SSL_H
#include "mbedtls/platform.h"
#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"

typedef struct wifi_sslclient_context {
    int socket;
    mbedtls_ssl_context ssl_ctx;
    mbedtls_ssl_config ssl_conf;

    mbedtls_ctr_drbg_context drbg_ctx;
    mbedtls_entropy_context entropy_ctx;

    mbedtls_x509_crt ca_cert;
    mbedtls_x509_crt client_cert;
    mbedtls_pk_context client_key;

    unsigned long handshake_timeout;
} wifi_sslclient_context;


void wifi_ssl_init(wifi_sslclient_context *ssl_client);
int wifi_start_ssl_client(wifi_sslclient_context *ssl_client, const char *host, uint32_t port, int timeout, const char *rootCABuff, const char *cli_cert, const char *cli_key, const char *pskIdent, const char *psKey);
void wifi_stop_ssl_socket(wifi_sslclient_context *ssl_client, const char *rootCABuff, const char *cli_cert, const char *cli_key);
int wifi_data_to_read(wifi_sslclient_context *ssl_client);
int wifi_send_ssl_data(wifi_sslclient_context *ssl_client, const uint8_t *data, uint16_t len);
int wifi_get_ssl_receive(wifi_sslclient_context *ssl_client, uint8_t *data, int length);
int wifi_verify_ssl_fingerprint(wifi_sslclient_context *ssl_client, const char* fp, const char* domain_name);
int wifi_verify_ssl_dn(wifi_sslclient_context *ssl_client, const char* domain_name);

#endif
