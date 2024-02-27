#include "hiredis_cluster/hircluster.h"
#include "hiredis/hiredis_ssl.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

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
        std::cout << "SSL Context error: " << redisSSLContextGetError(ssl_error) << std::endl;
        exit(1);
    }

    struct timeval timeout = {5, 500000}; // 5.5s

    redisClusterContext *cc = redisClusterContextInit();
    redisClusterSetOptionAddNodes(cc, CLUSTER_NODE_TLS);
    redisClusterSetOptionConnectTimeout(cc, timeout);
    redisClusterSetOptionRouteUseSlots(cc);
    redisClusterSetOptionEnableSSL(cc, ssl);
    redisClusterSetOptionUsername(cc,);
    redisClusterSetOptionPassword(cc, CLUSTER_PASSWORD);
    redisClusterConnect2(cc);
    if (cc && cc->err) {
        std::cout << "Error: " << cc->errstr << std::endl;
        // handle error
        exit(-1);
    }

    int count = 0;
    while (count < 10000) {
        redisReply *reply = (redisReply *)redisClusterCommand(cc, "SET %s %d", "test1", count);
        std::cout << "SET: " << count << std::endl;
        if (cc && cc->err) {
            std::cout << "Error: " << cc->errstr << std::endl;
        }
        count++;
        freeReplyObject(reply);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    redisClusterFree(cc);
    redisFreeSSLContext(ssl);
    return 0;
}
