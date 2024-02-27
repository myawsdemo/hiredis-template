### 安装依赖
```
sudo yum install -y git g++ openssl-devel
```
### 下载代码和依赖
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
### 下载demo代码
```
g++ -o redis-i redis-instance-tls.cpp -Wl,-Bstatic -lhiredis_ssl -lhiredis -Wl,-Bdynamic -lssl -lcrypto -ldl -lpthread
```