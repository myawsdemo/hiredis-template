#include <iostream>
#include <hiredis/hiredis.h>
#include <hiredis/hiredis_ssl.h>

int main() {
    /* An Hiredis SSL context. It holds SSL configuration and can be reused across
    * many contexts.
    */
    redisSSLContext *ssl_context;

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

    std::cout << "Start connecting" << std::endl;
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
    redisContext* c = redisConnect("master.xxxx.cache.amazonaws.com", 6379);
    if (c == NULL || c->err) {
        std::cerr << "Error Redis context and establish connection." << std::endl;
    }

    /* Negotiate SSL/TLS */
    if (redisInitiateSSLWithContext(c, ssl_context) != REDIS_OK) {
        /* Handle error, in c->err / c->errstr */
        std::cerr << "Error Negotiate SSL/TLS." << std::endl;
    }

    // 设置用户名和密码
    const char* username = "westhouse";
    const char* password = "westhouse12345678";

    // 验证用户名密码
    redisReply* auth_reply = (redisReply*)redisCommand(c, "AUTH %s %s", username, password);
    if (auth_reply == NULL || auth_reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "Authentication failed: " << c->errstr << std::endl;
        freeReplyObject(auth_reply);
        redisFree(c);
        exit(1);
    }
    freeReplyObject(auth_reply);

    std::cout << "Authentication successful" << std::endl;

    // 设置键值
    const char* key = "westhouse-key";
    const char* value = "westhouse-value";

    redisReply* set_reply = (redisReply*)redisCommand(c, "SET %s %s", key, value);
    if (set_reply == NULL || set_reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "SET command failed: " << c->errstr << std::endl;
        freeReplyObject(set_reply);
        redisFree(c);
        exit(1);
    }
    freeReplyObject(set_reply);

    std::cout << "SET operation successful" << std::endl;

    // 获取键值
    redisReply* get_reply = (redisReply*)redisCommand(c, "GET %s", key);
    if (get_reply == NULL || get_reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "GET command failed: " << c->errstr << std::endl;
        freeReplyObject(get_reply);
        redisFree(c);
        exit(1);
    }

    std::cout << "GET operation successful. Value: " << get_reply->str << std::endl;

    freeReplyObject(get_reply);
    redisFree(c);

    std::cout << "connect end" << std::endl;

    return 0;
}
