#include <iostream>
#include <hiredis/hiredis.h>
#include <hiredis/hiredis_ssl.h>

int main() {
    /* An Hiredis SSL context. It holds SSL configuration and can be reused across
    * many contexts.
    */
    redisContext* c = nullptr;
    SSL_CTX* ssl_context = nullptr;

    /* An error variable to indicate what went wrong, if the context fails to
    * initialize.
    */
    redisSSLContextError ssl_error;

    /* Initialize global OpenSSL state.
    *
    * You should call this only once when your app initializes, and only if
    * you don't explicitly or implicitly initialize OpenSSL it elsewhere.
    */
    redisInitOpenSSL();

    /* Create SSL context */
    ssl_context = redisCreateSSLContext(
        NULL, 
        "/etc/ssl/certs", 
        NULL, 
        NULL,
        NULL, &ssl_error);

    if(ssl_context == NULL || ssl_error != REDIS_SSL_CTX_NONE) {
        std::cerr << "Error initiating SSL/TLS connection." << std::endl;
        exit(1);
    }

    /* Create Redis context and establish connection */
    c = redisConnect("clustercfg.redis-cluster.eaxl5m.use1.cache.amazonaws.com", 6379);
    if (c == NULL || c->err) {
        std::cerr << "Error Redis context and establish connection." << std::endl;
    }

    /* Negotiate SSL/TLS */
    if (redisInitiateSSLWithContext(c, ssl_context) != REDIS_OK) {
        /* Handle error, in c->err / c->errstr */
        std::cerr << "Error Negotiate SSL/TLS." << std::endl;
    }

    return 0;
}
