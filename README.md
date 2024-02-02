## 描述
使用邻接表和floyd算法来实现基本要求

可以自动的进行结点或路径的增减，只要点击地图即可

l 设计大学的校园平面图，所含景点不少于10个，以图中顶点表示校内各景点，存放景点名称、代号、简介等信息；以边表示路径，存放路径长度等相关信息。
l 为来访客人提供图中任意景点相关信息的查询。
l 为来访客人提供图中任意景点的问路查询，即查询任意两相景点之间的一条最短的简单路径。
l 求校园图的关节点和桥
l 提供图中任意景点问路查询，即求任意两个景点之间的所有路径。
l 提供校园图中多个景点的最佳访问路线查询，即求途经这多个景点的最佳路径。
l 增删改查景点或路径

![image.png](https://cdn.nlark.com/yuque/0/2024/png/38890709/1706899381885-89dad3a4-56ef-4fed-99af-045b8f31d343.png#averageHue=%23b0e3ac&clientId=u3b3c9fea-3526-4&from=paste&height=633&id=u14e832b2&originHeight=791&originWidth=1259&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=470980&status=done&style=none&taskId=u62c65ae1-fad6-4f7f-8ae9-203005c68e0&title=&width=1007.2)
![image.png](https://cdn.nlark.com/yuque/0/2024/png/38890709/1706899418905-6bb0c149-a54a-4fe4-9039-04c0c4c7a8a5.png#averageHue=%23b0e3ac&clientId=u3b3c9fea-3526-4&from=paste&height=626&id=u85d75588&originHeight=782&originWidth=1249&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=468708&status=done&style=none&taskId=ufbb34783-7111-4ec2-88fb-3944d205ab1&title=&width=999.2)
![image.png](https://cdn.nlark.com/yuque/0/2024/png/38890709/1706899457204-ac2117ea-f9cf-4ff9-b69d-561c5f9a992e.png#averageHue=%23b0e3ac&clientId=u3b3c9fea-3526-4&from=paste&height=631&id=u34e189dc&originHeight=789&originWidth=1252&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=463825&status=done&style=none&taskId=u28b2171a-2f21-4587-a6e6-057fd1828a6&title=&width=1001.6)




## 依赖
本机Qt6
存在qt creator或VS2022或VSCode都可

示例VSCode（MinGW生成器）
将代码拉取到本地
在CMakeLists.txt同级目录下
打开终端
```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make.exe
```
将edge.txt和point.txt放入exe可执行文件同级目录下
![image.png](https://cdn.nlark.com/yuque/0/2024/png/38890709/1706899838794-a8df1393-4281-45a8-8399-41eda1a582e9.png#averageHue=%23fbfafa&clientId=u3b3c9fea-3526-4&from=paste&height=433&id=u473d1175&originHeight=541&originWidth=963&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=53252&status=done&style=none&taskId=u798520a3-78ae-4bfa-a9c5-a231dcf339a&title=&width=770.4)
即可出现示例
![image.png](https://cdn.nlark.com/yuque/0/2024/png/38890709/1706899904147-0ce8ee1b-3436-4046-9419-4fdc2b20b4cf.png#averageHue=%23322e27&clientId=u3b3c9fea-3526-4&from=paste&height=58&id=u095947f9&originHeight=72&originWidth=470&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=8800&status=done&style=none&taskId=ue540b597-d442-4ebb-ba3b-fa6580652d6&title=&width=376)
来运行

