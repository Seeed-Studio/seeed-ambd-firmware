/* Provide SSL/TLS functions to ESP32 with Arduino IDE
*
* Adapted from the ssl_client1 example of mbedtls.
*
* Original Copyright (C) 2006-2015, ARM Limited, All Rights Reserved, Apache 2.0 License.
* Additions Copyright (C) 2017 Evandro Luis Copercini, Apache 2.0 License.
*/

#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <lwip/api.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "wiring.h"
#include "../erpc/erpc_port.h"
#include "wifi_ssl_client.h"
#include "wio_tls.h"
#include "elog.h"

struct wifi_sslclient_context {
    int socket;
    wio_mbedtls_ssl_context ssl_ctx;
    wio_mbedtls_ssl_config ssl_conf;
    wio_mbedtls_ctr_drbg_context drbg_ctx;
    wio_mbedtls_entropy_context entropy_ctx;
    wio_mbedtls_x509_crt ca_cert;
    wio_mbedtls_x509_crt client_cert;
    wio_mbedtls_pk_context client_key;
    unsigned long handshake_timeout;
    char *rootCA;
    char *cli_cert;
    char *cli_key;
    char *pskIdent;
    char *psKey;
    unsigned char tls_initialized;
};

/* Prefer forward-secret AEAD suites, with no static RSA/CBC fallback. */
static const int certificate_ciphersuites[] = {
    WIO_MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    WIO_MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
    WIO_MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    WIO_MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
    0
};

static int _handle_error(int err, const char *file, int line)
{
    (void)file;
    (void)line;
    if (err == -30848)
    {
        return err;
    }
#ifdef WIO_MBEDTLS_ERROR_C
    char error_buf[100];
    wio_mbedtls_strerror(err, error_buf, 100);
    log_e("[%s():%d]: (%d) %s", file, line, err, error_buf);
#else
    log_e("[%s():%d]: code %d", file, line, err);
#endif
    return err;
}

#define handle_error(e) _handle_error(e, __FUNCTION__, __LINE__)

wifi_sslclient_context *wifi_ssl_client_create()
{
    wifi_sslclient_context *sslclient = pvPortMalloc(sizeof(wifi_sslclient_context));
    if (sslclient != NULL)
    {
        memset(sslclient, 0, sizeof(*sslclient));
        sslclient->socket = -1;
        sslclient->handshake_timeout = 30000;
        wifi_ssl_init(sslclient);
    }
    return sslclient;
}

void wifi_ssl_client_destroy(wifi_sslclient_context *ssl_client)
{
    if (ssl_client == NULL)
        return;
    wifi_stop_ssl_socket(ssl_client, NULL, NULL, NULL);
    if (ssl_client->rootCA != NULL)
    {
        erpc_free(ssl_client->rootCA);
        ssl_client->rootCA = NULL;
    }
    if (ssl_client->cli_cert != NULL)
    {
        erpc_free(ssl_client->cli_cert);
        ssl_client->cli_cert = NULL;
    }
    if (ssl_client->cli_key != NULL)
    {
        erpc_free(ssl_client->cli_key);
        ssl_client->cli_key = NULL;
    }
    if (ssl_client->pskIdent != NULL)
    {
        erpc_free(ssl_client->pskIdent);
        ssl_client->pskIdent = NULL;
    }
    if (ssl_client->psKey != NULL)
    {
        erpc_free(ssl_client->psKey);
        ssl_client->psKey = NULL;
    }
    vPortFree(ssl_client);
}

void wifi_ssl_init(wifi_sslclient_context *ssl_client)
{
    if (ssl_client == NULL)
        return;
    wifi_stop_ssl_socket(ssl_client, NULL, NULL, NULL);
    wio_mbedtls_ssl_init(&ssl_client->ssl_ctx);
    wio_mbedtls_ssl_config_init(&ssl_client->ssl_conf);
    wio_mbedtls_ctr_drbg_init(&ssl_client->drbg_ctx);
    wio_mbedtls_entropy_init(&ssl_client->entropy_ctx);
    wio_mbedtls_x509_crt_init(&ssl_client->ca_cert);
    wio_mbedtls_x509_crt_init(&ssl_client->client_cert);
    wio_mbedtls_pk_init(&ssl_client->client_key);
    ssl_client->tls_initialized = 1;
}

void wifi_ssl_set_socket(wifi_sslclient_context *ssl_client, int socket)
{
    ssl_client->socket = socket;
}
void wifi_ssl_set_timeout(wifi_sslclient_context *ssl_client, unsigned long handshake_timeout)
{
    ssl_client->handshake_timeout = handshake_timeout;
}

int wifi_ssl_get_socket(wifi_sslclient_context *ssl_client)
{
    return ssl_client->socket;
}
unsigned long wifi_ssl_get_timeout(wifi_sslclient_context *ssl_client)
{
    return ssl_client->handshake_timeout;
}

uint32_t wifi_ssl_set_rootCA(wifi_sslclient_context *ssl_client, char *rootCABuff)
{
    if (ssl_client->rootCA != NULL)
    {
        erpc_free(ssl_client->rootCA);
        ssl_client->rootCA = NULL;
    }
    ssl_client->rootCA = rootCABuff;
    return (uint32_t)(uintptr_t)ssl_client->rootCA;
}

uint32_t wifi_ssl_get_rootCA(wifi_sslclient_context *ssl_client)
{
    return (uint32_t)(uintptr_t)ssl_client->rootCA;
}

uint32_t wifi_ssl_set_cliCert(wifi_sslclient_context *ssl_client, char *cli_cert)
{
    if (ssl_client->cli_cert != NULL)
    {
        erpc_free(ssl_client->cli_cert);
        ssl_client->cli_cert = NULL;
    }
    ssl_client->cli_cert = cli_cert;
    return (uint32_t)(uintptr_t)ssl_client->cli_cert;
}

uint32_t wifi_ssl_get_cliCert(wifi_sslclient_context *ssl_client)
{
    return (uint32_t)(uintptr_t)ssl_client->cli_cert;
}

uint32_t wifi_ssl_set_cliKey(wifi_sslclient_context *ssl_client, char *cli_key)
{
    if (ssl_client->cli_key != NULL)
    {
        erpc_free(ssl_client->cli_key);
        ssl_client->cli_key = NULL;
    }
    ssl_client->cli_key = cli_key;
    return (uint32_t)(uintptr_t)ssl_client->cli_key;
}

uint32_t wifi_ssl_get_cliKey(wifi_sslclient_context *ssl_client)
{
    return (uint32_t)(uintptr_t)ssl_client->cli_key;
}

uint32_t wifi_ssl_set_pskIdent(wifi_sslclient_context *ssl_client, char *pskIdent)
{
    if (ssl_client->pskIdent != NULL)
    {
        erpc_free(ssl_client->pskIdent);
        ssl_client->pskIdent = NULL;
    }
    ssl_client->pskIdent = pskIdent;
    return (uint32_t)(uintptr_t)ssl_client->pskIdent;
}

uint32_t wifi_ssl_get_pskIdent(wifi_sslclient_context *ssl_client)
{
    return (uint32_t)(uintptr_t)ssl_client->pskIdent;
}

uint32_t wifi_ssl_set_psKey(wifi_sslclient_context *ssl_client, char *psKey)
{
    if (ssl_client->psKey != NULL)
    {
        erpc_free(ssl_client->psKey);
        ssl_client->psKey = NULL;
    }
    ssl_client->psKey = psKey;
    return (uint32_t)(uintptr_t)ssl_client->psKey;
}

uint32_t wifi_ssl_get_psKey(wifi_sslclient_context *ssl_client)
{
    return (uint32_t)(uintptr_t)ssl_client->psKey;
}

int wifi_start_ssl_client(wifi_sslclient_context *ssl_client, const char *host, uint32_t port, int timeout, const char *rootCABuff, const char *cli_cert, const char *cli_key, const char *pskIdent, const char *psKey)
{
    char buf[512];
    int ret = WIO_MBEDTLS_ERR_SSL_BAD_INPUT_DATA;
    uint32_t flags;
    int enable = 1;
    int keep_idle = 30;
    const int use_psk = rootCABuff == NULL && pskIdent != NULL && pskIdent[0] != '\0' && psKey != NULL && psKey[0] != '\0';

    if (ssl_client == NULL || host == NULL || host[0] == '\0' || port == 0 || port > 65535)
        return handle_error(ret);

    /* Reconnecting must also release a previous failed or established session. */
    wifi_ssl_init(ssl_client);
    if (rootCABuff == NULL && !use_psk)
    {
        ret = WIO_MBEDTLS_ERR_SSL_CA_CHAIN_REQUIRED;
        goto cleanup;
    }
    if (!wio_tls_ready())
    {
        ret = WIO_MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
        goto cleanup;
    }
    if (timeout <= 0)
        timeout = 30000;

    log_v("Starting socket");

    ssl_client->socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ssl_client->socket < 0)
    {
        log_e("ERROR opening socket");
        ret = WIO_MBEDTLS_ERR_NET_SOCKET_FAILED;
        goto cleanup;
    }

    ip_addr_t srv;
    if (netconn_gethostbyname(host, &srv) != ERR_OK)
    {
        ret = WIO_MBEDTLS_ERR_NET_UNKNOWN_HOST;
        goto cleanup;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = srv.addr;
    serv_addr.sin_port = htons(port);

    if (lwip_connect(ssl_client->socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0)
    {
        lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        lwip_setsockopt(ssl_client->socket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable));
        lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
        lwip_setsockopt(ssl_client->socket, IPPROTO_TCP, TCP_KEEPIDLE, &keep_idle, sizeof(keep_idle));
    }
    else
    {
        log_v("Connect to Server failed!");
        ret = WIO_MBEDTLS_ERR_NET_CONNECT_FAILED;
        goto cleanup;
    }

    if (fcntl(ssl_client->socket, F_SETFL, fcntl(ssl_client->socket, F_GETFL, 0) | O_NONBLOCK) < 0)
    {
        ret = WIO_MBEDTLS_ERR_NET_SOCKET_FAILED;
        goto cleanup;
    }
    log_v("Seeding the random number generator");

    ret = wio_mbedtls_ctr_drbg_seed(&ssl_client->drbg_ctx, wio_mbedtls_entropy_func,
                                &ssl_client->entropy_ctx,
                                (const unsigned char *)"wio-rtl8720-tls", 15);
    if (ret != 0)
        goto cleanup;

    log_v("Setting up the SSL/TLS structure...");

    if ((ret = wio_mbedtls_ssl_config_defaults(&ssl_client->ssl_conf,
                                           WIO_MBEDTLS_SSL_IS_CLIENT,
                                           WIO_MBEDTLS_SSL_TRANSPORT_STREAM,
                                           WIO_MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        goto cleanup;
    }

    wio_mbedtls_ssl_conf_min_tls_version(&ssl_client->ssl_conf, WIO_MBEDTLS_SSL_VERSION_TLS1_2);
    wio_mbedtls_ssl_conf_max_tls_version(&ssl_client->ssl_conf, WIO_MBEDTLS_SSL_VERSION_TLS1_2);
    if (!use_psk)
        wio_mbedtls_ssl_conf_ciphersuites(&ssl_client->ssl_conf, certificate_ciphersuites);
    wio_mbedtls_ssl_conf_rng(&ssl_client->ssl_conf, wio_mbedtls_ctr_drbg_random, &ssl_client->drbg_ctx);

    /* Certificate connections always authenticate the peer. PSK connections
     * authenticate through their key instead of an X.509 certificate.
     */

    if (rootCABuff != NULL)
    {
        log_v("Loading CA cert");
        // for (int i = 0; i < strlen(rootCABuff); i++)
        // {
        //     printf("%c", rootCABuff[i]);
        // }
        // printf("");
        ret = wio_mbedtls_x509_crt_parse(&ssl_client->ca_cert, (const unsigned char *)rootCABuff, strlen(rootCABuff) + 1);
        if (ret != 0)
        {
            if (ret > 0)
                ret = WIO_MBEDTLS_ERR_X509_INVALID_FORMAT;
            goto cleanup;
        }
        wio_mbedtls_ssl_conf_ca_chain(&ssl_client->ssl_conf, &ssl_client->ca_cert, NULL);
        wio_mbedtls_ssl_conf_authmode(&ssl_client->ssl_conf, WIO_MBEDTLS_SSL_VERIFY_REQUIRED);
    }
    else if (use_psk)
    {
        log_v("Setting up PSK");
        // convert PSK from hex to binary
        if ((strlen(psKey) & 1) != 0 || strlen(psKey) > 2 * WIO_MBEDTLS_PSK_MAX_LEN)
        {
            log_e("pre-shared key not valid hex or too long");
            ret = WIO_MBEDTLS_ERR_SSL_BAD_INPUT_DATA;
            goto cleanup;
        }
        unsigned char psk[WIO_MBEDTLS_PSK_MAX_LEN];
        size_t psk_len = strlen(psKey) / 2;
        for (size_t j = 0; j < psk_len * 2; j += 2)
        {
            char c = psKey[j];
            if (c >= '0' && c <= '9')
                c -= '0';
            else if (c >= 'A' && c <= 'F')
                c -= 'A' - 10;
            else if (c >= 'a' && c <= 'f')
                c -= 'a' - 10;
            else
            {
                ret = WIO_MBEDTLS_ERR_SSL_BAD_INPUT_DATA;
                goto cleanup;
            }
            psk[j / 2] = c << 4;
            c = psKey[j + 1];
            if (c >= '0' && c <= '9')
                c -= '0';
            else if (c >= 'A' && c <= 'F')
                c -= 'A' - 10;
            else if (c >= 'a' && c <= 'f')
                c -= 'a' - 10;
            else
            {
                ret = WIO_MBEDTLS_ERR_SSL_BAD_INPUT_DATA;
                goto cleanup;
            }
            psk[j / 2] |= c;
        }
        // set mbedtls config
        ret = wio_mbedtls_ssl_conf_psk(&ssl_client->ssl_conf, psk, psk_len,
                                   (const unsigned char *)pskIdent, strlen(pskIdent));
        if (ret != 0)
        {
            log_e("wio_mbedtls_ssl_conf_psk returned %d", ret);
            goto cleanup;
        }
        wio_mbedtls_ssl_conf_authmode(&ssl_client->ssl_conf, WIO_MBEDTLS_SSL_VERIFY_NONE);
    }
    if (cli_cert != NULL && cli_key != NULL)
    {
        log_v("Loading CRT cert");

        ret = wio_mbedtls_x509_crt_parse(&ssl_client->client_cert, (const unsigned char *)cli_cert, strlen(cli_cert) + 1);
        if (ret != 0)
        {
            if (ret > 0)
                ret = WIO_MBEDTLS_ERR_X509_INVALID_FORMAT;
            goto cleanup;
        }

        log_v("Loading private key");
        ret = wio_mbedtls_pk_parse_key(&ssl_client->client_key, (const unsigned char *)cli_key, strlen(cli_key) + 1, NULL, 0
#if WIO_MBEDTLS_VERSION_MAJOR >= 3
                                   , wio_mbedtls_ctr_drbg_random, &ssl_client->drbg_ctx
#endif
                                   );

        if (ret != 0)
        {
            goto cleanup;
        }

        if ((ret = wio_mbedtls_ssl_conf_own_cert(&ssl_client->ssl_conf, &ssl_client->client_cert, &ssl_client->client_key)) != 0)
            goto cleanup;
    }

    //log_v("Setting hostname :%s for TLS session...", host);

    if ((ret = wio_mbedtls_ssl_setup(&ssl_client->ssl_ctx, &ssl_client->ssl_conf)) != 0)
    {
        goto cleanup;
    }

    wio_mbedtls_ssl_set_bio(&ssl_client->ssl_ctx, &ssl_client->socket, wio_mbedtls_net_send, wio_mbedtls_net_recv, NULL);

    /* This sets SNI and the expected certificate name; do not bypass it. */
    if ((ret = wio_mbedtls_ssl_set_hostname(&ssl_client->ssl_ctx, host)) != 0)
    {
        goto cleanup;
    }

    log_v("Performing the SSL/TLS handshake...");
    unsigned long handshake_start_time = millis();
    unsigned long handshake_timeout = ssl_client->handshake_timeout;
    if (handshake_timeout == 0)
        handshake_timeout = (unsigned long)timeout;
    while ((ret = wio_mbedtls_ssl_handshake(&ssl_client->ssl_ctx)) != 0)
    {
        /* Fatal errors terminate the session. Retrying them hides the first error. */
        if (ret != WIO_MBEDTLS_ERR_SSL_WANT_READ && ret != WIO_MBEDTLS_ERR_SSL_WANT_WRITE)
            goto cleanup;
        if ((millis() - handshake_start_time) >= handshake_timeout)
        {
            ret = WIO_MBEDTLS_ERR_SSL_TIMEOUT;
            goto cleanup;
        }
        vTaskDelay(1);
    }

    log_v("Protocol is %s Ciphersuite is %s", wio_mbedtls_ssl_get_version(&ssl_client->ssl_ctx), wio_mbedtls_ssl_get_ciphersuite(&ssl_client->ssl_ctx));
    if (cli_cert != NULL && cli_key != NULL)
    {
        if ((ret = wio_mbedtls_ssl_get_record_expansion(&ssl_client->ssl_ctx)) >= 0)
        {
            log_v("Record expansion is %d", ret);
        }
        else
        {
            log_w("Record expansion is unknown (compression)");
        }
    }

    log_v("Verifying peer X.509 certificate...");

    if (rootCABuff != NULL && (flags = wio_mbedtls_ssl_get_verify_result(&ssl_client->ssl_ctx)) != 0)
    {
        bzero(buf, sizeof(buf));
        wio_mbedtls_x509_crt_verify_info(buf, sizeof(buf), "  ! ", flags);
        log_v("Failed to verify peer certificate! verification info: %s", buf);
        ret = WIO_MBEDTLS_ERR_X509_CERT_VERIFY_FAILED;
        goto cleanup;
    }
    else
    {
        if (rootCABuff != NULL)
        {
            log_v("Certificate verified.");
        }
    }

    /* ssl_conf retains pointers to these credentials for the session lifetime. */
    return ssl_client->socket;

cleanup:
    wifi_stop_ssl_socket(ssl_client, rootCABuff, cli_cert, cli_key);
    return handle_error(ret);
}

void wifi_stop_ssl_socket(wifi_sslclient_context *ssl_client, const char *rootCABuff, const char *cli_cert, const char *cli_key)
{
    log_v("Cleaning SSL connection.");
    (void)rootCABuff;
    (void)cli_cert;
    (void)cli_key;

    if (ssl_client == NULL)
        return;

    if (ssl_client->socket >= 0)
    {
        close(ssl_client->socket);
        ssl_client->socket = -1;
    }

    if (!ssl_client->tls_initialized)
        return;
    wio_mbedtls_ssl_free(&ssl_client->ssl_ctx);
    wio_mbedtls_ssl_config_free(&ssl_client->ssl_conf);
    wio_mbedtls_ctr_drbg_free(&ssl_client->drbg_ctx);
    wio_mbedtls_entropy_free(&ssl_client->entropy_ctx);
    wio_mbedtls_x509_crt_free(&ssl_client->ca_cert);
    wio_mbedtls_x509_crt_free(&ssl_client->client_cert);
    wio_mbedtls_pk_free(&ssl_client->client_key);
    ssl_client->tls_initialized = 0;
}

int wifi_data_to_read(wifi_sslclient_context *ssl_client)
{
    if (ssl_client == NULL || !ssl_client->tls_initialized || ssl_client->socket < 0)
        return handle_error(WIO_MBEDTLS_ERR_SSL_BAD_INPUT_DATA);
    //log_v("RET: %i",ret);   //for low level debug

    // res = wio_mbedtls_ssl_get_bytes_avail(&ssl_client->ssl_ctx);
    // log_v("ssl_ctx: %d res: %d", &ssl_client->ssl_ctx, res);
    //log_v("RES: %i",res);    //for low level debug
    // if (ret != WIO_MBEDTLS_ERR_SSL_WANT_READ && ret != WIO_MBEDTLS_ERR_SSL_WANT_WRITE && ret < 0)
    // {
    //     return handle_error(ret);
    // }
    int ret = 0;
    uint8_t has_backup_recvtimeout = 0;
    int backup_recv_timeout, recv_timeout;
    socklen_t len = sizeof(backup_recv_timeout);
    int retry = 20;

    // peek for 10ms
    ret = lwip_getsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &backup_recv_timeout, &len);
    if (ret >= 0)
    {
        recv_timeout = 100;
        ret = lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));
        if (ret >= 0)
        {
            has_backup_recvtimeout = 1;
        }
    }
    uint8_t dummy;
    memset(&dummy, 0, 1);
    ret = -1;
    while (retry-- && ret < 0)
    {
        ret = wio_mbedtls_ssl_read(&ssl_client->ssl_ctx, &dummy, 0);
        log_v("retry :%d ret", retry, ret);
        if (ret != WIO_MBEDTLS_ERR_SSL_WANT_READ && ret != WIO_MBEDTLS_ERR_SSL_WANT_WRITE)
            break;
        delay(10);
    }
    if (ret > 0)
    {
        log_v("%c", dummy);
    }

    if (has_backup_recvtimeout == 1)
    {
        // restore receiving timeout
        lwip_setsockopt(ssl_client->socket, SOL_SOCKET, SO_RCVTIMEO, &backup_recv_timeout, sizeof(recv_timeout));
    }

    return ret;
}

int wifi_send_ssl_data(wifi_sslclient_context *ssl_client, const uint8_t *data, uint16_t len)
{
    log_v("Writing HTTP request..."); //for low level debug
    int ret = -1;
    if (ssl_client == NULL || !ssl_client->tls_initialized || ssl_client->socket < 0 || (data == NULL && len != 0))
        return handle_error(WIO_MBEDTLS_ERR_SSL_BAD_INPUT_DATA);
    if (len == 0)
        return 0;
    unsigned long start = millis();
    unsigned long timeout = ssl_client->handshake_timeout ? ssl_client->handshake_timeout : 30000;

    while ((ret = wio_mbedtls_ssl_write(&ssl_client->ssl_ctx, data, len)) <= 0)
    {
        if (ret != WIO_MBEDTLS_ERR_SSL_WANT_READ && ret != WIO_MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            return handle_error(ret);
        }
        if ((millis() - start) >= timeout)
            return handle_error(WIO_MBEDTLS_ERR_SSL_TIMEOUT);
        vTaskDelay(1);
    }

    len = ret;
    //log_v("%d bytes written", len);  //for low level debug
    return ret;
}

int wifi_get_ssl_receive(wifi_sslclient_context *ssl_client, uint8_t *data, int length)
{
    if (ssl_client == NULL || !ssl_client->tls_initialized || ssl_client->socket < 0 || length < 0 || (data == NULL && length != 0))
        return handle_error(WIO_MBEDTLS_ERR_SSL_BAD_INPUT_DATA);
    //log_v( "Reading HTTP response...");   //for low level debug
    int ret = -1;

    ret = wio_mbedtls_ssl_read(&ssl_client->ssl_ctx, data, length);

    //log_v( "%d bytes read", ret);   //for low level debug
    return ret;
}

static bool parseHexNibble(char pb, uint8_t *res)
{
    if (pb >= '0' && pb <= '9')
    {
        *res = (uint8_t)(pb - '0');
        return true;
    }
    else if (pb >= 'a' && pb <= 'f')
    {
        *res = (uint8_t)(pb - 'a' + 10);
        return true;
    }
    else if (pb >= 'A' && pb <= 'F')
    {
        *res = (uint8_t)(pb - 'A' + 10);
        return true;
    }
    return false;
}

static unsigned char asciiLower(unsigned char ch)
{
    return ch >= 'A' && ch <= 'Z' ? ch + ('a' - 'A') : ch;
}

/* X.509 strings carry a length and need not have a terminating NUL. */
static bool matchName(const unsigned char *name, size_t nameLen, const char *domainName)
{
    if (name == NULL || nameLen == 0 || domainName == NULL || domainName[0] == '\0')
        return false;
    size_t domainLen = strlen(domainName);
    if (nameLen > 2 && name[0] == '*' && name[1] == '.')
    {
        const char *dot = strchr(domainName, '.');
        if (dot == NULL || dot == domainName)
            return false;
        domainName = dot;
        domainLen = strlen(dot);
        ++name;
        --nameLen;
    }
    if (nameLen != domainLen)
        return false;
    for (size_t i = 0; i < nameLen; ++i)
        if (name[i] == '\0' || asciiLower(name[i]) != asciiLower((unsigned char)domainName[i]))
            return false;
    return true;
}

// Verifies certificate provided by the peer to match specified SHA256 fingerprint
int wifi_verify_ssl_fingerprint(wifi_sslclient_context *ssl_client, const char *fp, const char *domain_name)
{
    if (ssl_client == NULL || !ssl_client->tls_initialized || fp == NULL)
        return false;
    // Convert hex string to byte array
    uint8_t fingerprint_local[32];
    int len = strlen(fp);
    int pos = 0;
    for (size_t i = 0; i < sizeof(fingerprint_local); ++i)
    {
        while (pos < len && ((fp[pos] == ' ') || (fp[pos] == ':')))
        {
            ++pos;
        }
        if (pos > len - 2)
        {
            log_v("pos:%d len:%d fingerprint too short", pos, len);
            return false;
        }
        uint8_t high, low;
        if (!parseHexNibble(fp[pos], &high) || !parseHexNibble(fp[pos + 1], &low))
        {
            log_v("pos:%d len:%d invalid hex sequence: %c%c", pos, len, fp[pos], fp[pos + 1]);
            return false;
        }
        pos += 2;
        fingerprint_local[i] = low | (high << 4);
    }

    // Get certificate provided by the peer
    const wio_mbedtls_x509_crt *crt = wio_mbedtls_ssl_get_peer_cert(&ssl_client->ssl_ctx);

    if (!crt)
    {
        log_v("could not fetch peer certificate");
        return false;
    }

    // Calculate certificate's SHA256 fingerprint
    uint8_t fingerprint_remote[32];
    wio_mbedtls_sha256_context sha256_ctx;
    wio_mbedtls_sha256_init(&sha256_ctx);
    wio_mbedtls_sha256_starts(&sha256_ctx, false);
    wio_mbedtls_sha256_update(&sha256_ctx, crt->raw.p, crt->raw.len);
    wio_mbedtls_sha256_finish(&sha256_ctx, fingerprint_remote);
    wio_mbedtls_sha256_free(&sha256_ctx);

    // Check if fingerprints match
    if (memcmp(fingerprint_local, fingerprint_remote, 32))
    {
        log_v("fingerprint doesn't match");
        return false;
    }

    // Additionally check if certificate has domain name if provided
    if (domain_name)
        return wifi_verify_ssl_dn(ssl_client, domain_name);
    else
        return true;
}

// Checks if peer certificate has specified domain in CN or SANs
int wifi_verify_ssl_dn(wifi_sslclient_context *ssl_client, const char *domain_name)
{
    log_v("domain name: '%s'", (domain_name) ? domain_name : "(null)");

    if (ssl_client == NULL || !ssl_client->tls_initialized || domain_name == NULL || domain_name[0] == '\0')
        return false;

    // Get certificate provided by the peer
    const wio_mbedtls_x509_crt *crt = wio_mbedtls_ssl_get_peer_cert(&ssl_client->ssl_ctx);
    if (crt == NULL)
        return false;

    // Check for domain name in SANs
    const wio_mbedtls_x509_sequence *san = &crt->subject_alt_names;
    bool has_dns_san = false;
    while (san != NULL)
    {
        if (san->buf.tag == (WIO_MBEDTLS_ASN1_CONTEXT_SPECIFIC | WIO_MBEDTLS_X509_SAN_DNS_NAME))
        {
            has_dns_san = true;
            if (matchName(san->buf.p, san->buf.len, domain_name))
                return true;
        }
        san = san->next;
    }
    if (has_dns_san)
        return false;

    // Check for domain name in CN
    const wio_mbedtls_asn1_named_data *common_name = &crt->subject;
    while (common_name != NULL)
    {
        // While iterating through DN objects, check for CN object
        if (!WIO_MBEDTLS_OID_CMP(WIO_MBEDTLS_OID_AT_CN, &common_name->oid))
        {
            if (matchName(common_name->val.p, common_name->val.len, domain_name))
                return true;
        }

        // Fetch next DN object
        common_name = common_name->next;
    }

    return false;
}
void wifi_ssl_strerror(int errnum, char *buffer, size_t buflen)
{
    wio_mbedtls_strerror(errnum, buffer, buflen);
}
