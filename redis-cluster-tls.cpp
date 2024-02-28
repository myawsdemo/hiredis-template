#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <hiredis/hiredis.h>
#include <hiredis/hiredis_ssl.h>
#include <hiredis_cluster/hircluster.h>
#include <hiredis_cluster/hircluster_ssl.h>

#define CLUSTER_NODE_TLS "clustercfg.redis-cluster.eaxl5m.use1.cache.amazonaws.com:6379"
#define CLUSTER_USERNAME "westhouse"
#define CLUSTER_PASSWORD "westhouse12345678"

int main() {
    redisSSLContext *ssl;
    redisSSLContextError ssl_error;

    redisInitOpenSSL();
    ssl = redisCreateSSLContext(nullptr, "/etc/ssl/certs", nullptr, nullptr,
                                nullptr, &ssl_error);
    if (!ssl) {
        std::cerr << "SSL Context error: " << redisSSLContextGetError(ssl_error) << std::endl;
        exit(1);
    }

    struct timeval timeout = {5, 500000}; // 5.5s

    redisClusterContext *cc = redisClusterContextInit();
    redisClusterSetOptionAddNodes(cc, CLUSTER_NODE_TLS);
    redisClusterSetOptionConnectTimeout(cc, timeout);
    redisClusterSetOptionRouteUseSlots(cc);
    redisClusterSetOptionEnableSSL(cc, ssl);
    redisClusterSetOptionUsername(cc, CLUSTER_USERNAME);
    redisClusterSetOptionPassword(cc, CLUSTER_PASSWORD);
    redisClusterConnect2(cc);
    if (cc && cc->err) {
        std::cout << "Error: " << cc->errstr << std::endl;
        // handle error
        exit(-1);
    }

    const char* key = "westhouse-key";
    const char* value = "westhouse-value";

    redisReply *set_reply = (redisReply *)redisClusterCommand(cc, "SET %s %s", key, value);
    if (cc && cc->err) {
         std::cerr << "SET command failed: " << cc->errstr << std::endl;
    }
    freeReplyObject(set_reply);
    std::cout << "SET operation successful" << std::endl;

    redisReply *get_reply = (redisReply *)redisClusterCommand(cc, "GET %s", key);

    if (get_reply == NULL || get_reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "GET command failed: " << cc->errstr << std::endl;
    }
    std::cout << "GET operation successful. Value: " << get_reply->str << std::endl;
    freeReplyObject(get_reply);

    redisClusterFree(cc);
    redisFreeSSLContext(ssl);
    return 0;
}
