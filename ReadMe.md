### 安装依赖
```
sudo yum install -y git g++ openssl-devel
sudo dnf install -y libevent-devel
```
### 下载Hiredis代码和依赖
```
git clone https://github.com/redis/hiredis.git
cd hiredis
make USE_SSL=1
sudo make install
```
### 拷贝包到/usr/lib64
```
cp hiredis_ssl.a hiredis.a /usr/lib64
```
### 下载demo代码 在根目录下执行以下代码
```
g++ -o redis-i redis-instance-tls.cpp -Wl,-Bstatic -lhiredis_ssl -lhiredis -Wl,-Bdynamic -lssl -lcrypto -ldl -lpthread
```
### 执行文件
```
./redis-i
```
示例输出如下：
```
Start connecting
Authentication successful
SET operation successful
GET operation successful. Value: westhouse-value
connect end
```

## 当需要链接集群模式的Redis时需要使用Hiredis Cluster，步骤如下：

### 下载Hiredis Cluster代码
```
git clone https://github.com/Nordix/hiredis-cluster.git
cd hiredis-cluster/examples/using_make
./build.sh
cd install/usr/local/lib
cp libhiredis_cluster.a /usr/lib64
cp libhiredis_cluster_ssl.a /usr/lib64
```
### 执行打包命令
```
g++ -o redis-c redis-cluster-tls.cpp -Wl,-Bstatic -lhiredis_cluster -lhiredis_cluster_ssl -lhiredis -lhiredis_ssl -Wl,-Bdynamic -lssl -lcrypto -ldl -lpthread
```
### 执行文件
```
./redis-c
```
示例输出如下：
```
SET operation successful
GET operation successful. Value: westhouse-value
```
