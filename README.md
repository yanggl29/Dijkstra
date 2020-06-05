<<<<<<< HEAD
<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
    MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>
# <center>**基于Dijkstra的广州地铁查询程序**</center>
&emsp;&emsp;**摘要：本实验主要基于Dijkstra算法实现了广州地铁线路查询以及提供线路优化。根据广州地铁线路图建立双向有权图，然后根据Dijkstra算法提供最短路径为其最基本功能。除此之外，我们还在如何提供最少换乘方案上进行了一些探讨，并提供了两种可行方案。**

## **1.引言**

&emsp;&emsp;本实验提供了三种线路方案，第一种是最短路径，第二种是最少换乘，第三种最少站台。第一种方案主要基于Dijkstra算法，Dijkstra是单源最短路算法，不能处理带负边权的情况，用邻接矩阵或邻接表存图。另外，有另一种算法，SPFA算法可以处理带有负边权的情况，考虑到实际中广州地铁并不会出现负边权的情况，所以本次实验采用Dijkstra算法来进行实现。第二种方案我们提供了两种解决方法，由于时间关系，我们采取了其中比较容易实现的方法。第三种方案采用了BFS算法进行实现。

## **2.解决方法**

### **2.1数据结构设计**

&emsp;&emsp;根据实际需求，我们首先设计了图的结构。这里首先采用了建立类SUBWAY来进行实现，其数据成员有整数n，用来表示图中的节点个数；二维向量adjGraph，用以储存广州地铁线路图，以邻接表形式；向量stations，用以建立站点与整数类型之间的一个映射；除了以上数据成员，还声明了若干函数用于实现解决方法。数据结构程序语言描述如下：
```cpp
class SUBWAY
{
  private:
    int n;                                                  //number of stations
    vector<vector<pair<int, int>>> adjGraph;                //adjacency list
    unordered_map<pair<int, int>, string, pair_hash> lines; //map the edge to the correspoing line
    vector<string> stations;                                //number the station
    pair<int, int> findmin(vector<int> &vec, vector<bool> &sign);
    pair<vector<int>, int> GetShortestPath(int a, int b);   //return the path(reversed)
    vector<int> GetMinimumStation(int a, int b); //return the path(reversed)
    void buildGraph();
    bool isoneline(int a, int b);
    bool check_input(string starting_point, string destination);
    void print_result(const vector<int> &result);
    void DFS(int last_station, vector<bool> &sign, int start, int end, vector<int> &tmppath, vector<int> &result, int cnt);
  public:
    SUBWAY();
    void FindShortestPathestPath(string starting_point, string destination);
    void USEDFS(int start, int end);
    void FindMinimumTransfer(string starting_point, string destination);
    void FindMinimumStation(string starting_point, string destination);
};
```

### **2.2算法描述**

&emsp;&emsp;第一种方案的提供采用了Dijkstra算法，算法是由Dijkstra于1959年提出的，主要用于解决单源起点问题，是目前求解最短路径最完备广泛的算法之一$^{[2]}$，所以该算法十分适用于该方案的实现。该算法的具体实现方法如下：设最短路径的终点存放在集合S中，在初始状态下，只有一个元素点$V_0$储存在集合S中；以后每选择出总路径最短且不在集合中的一个点，计算出出一条路径（$V_0$,...,$V_k$）后，就将$V_k$填加到集合S中，直到所有点都加到集合中为止。这样就可以得到起点到所有点之间的最短路径，从而实现该方案的输出。

&emsp;&emsp;第二种方案的提供采用了DFS算法。在本实验中最初的版本，我们使用DFS算法来计算出从起始点到终点站的所有路线，然后从这些路径中选择出换乘次数最少的路线。但在实现之后，发现程序从输出结果需要近半分钟，且需要占据大量的空间，因此我们对其进行了相应的优化。我们在使用DFS算法计算所有路径的同时，也计算当前路线所需要的换乘数，一旦发现该路线的换乘数比我们当前所计算出来的换乘数最少的路线的换乘数大，则不再继续遍历当前路线（具体实现图4所示），如此，我们就可以大幅度地降低时间复杂度。如此同时，我们只会记录换乘数最少的路径而不是所有路径，故空间复杂度也得到了相应的降低。但我们在实现时未考虑到站点数以及距离，因此该程序中的所规划出来的路线并不一定是换乘次数最少的路线中经过站点最少或者路径最短的一条，需要作进一步的修改。

&emsp;&emsp;此外，第二种方案我们还想了另一种基于Dijkstra的实现方法，具体如下：在最短路径中，换乘时换乘站在图中只有一个点，而在对图进行改进后，将换乘时的站分裂开（不同的线路若相交则相交的点在两条线路中都存在，且有一条单独的边连结这两个相同站台的点），然后将分裂开的点之间的边的权值设为遍历全部边的权值之和，保证了若要换乘，则换乘代价极大，从而减少换乘次数。与直接判断是否换乘而添加换乘代价相比，改进后的图的顶点数更多，也可以重复访问交通枢纽的站点（前提是在不同线路中未全部访问完），这样就可以避免直接使用Dijkstra算法时出现无法再次通过交通枢纽的情况。经过与其他同学的讨论，该方法是可行的，但由于时间原因，我们没有实现。

&emsp;&emsp;第三种方案的提供是基于BFS算法来实现的，但我们在实现时未考虑到换乘次数，因此该程序中的所规划出来的路线并不一定是经过站点数最少的路线中换乘次数最少的一条，需要作进一步的修改。

### **2.3实验数据**

&emsp;&emsp;实验数据来源于广州地铁官网$^{[3]}$以及百度地图$^{[4]}$，我们通过调查每个站点之间的预计时间以及换乘信息，将信息记录下来，并用于见图，信息来源如图2.3：

<center><img src="pic\map.png" width="700" hegiht="560" align=center /></center>
<center>图2.3</center>

### **2.4实验结果与测试**

&emsp;&emsp;根据百度地图输出结果与测试结果进行比较以验证程序的正确性；方案一测试如图2.4.1与图2.4.2

<center><img src="pic\test1.png" width="440" hegiht="180" align=center /></center>
<center>图2.4.1</center>
<center><img src="pic\testg1.png" width="440" hegiht="220" align=center /></center>
<center>图2.4.2</center>

&emsp;&emsp;方案二测试如图2.4.3与图2.4.4，**由于考虑不周，我们最终没有返回所需要的预计时间，但路线结果是没有问题的。**



<center><img src="pic\test2.png" width="800" hegiht="200" align=center /></center>
<center>图2.4.3</center>
<center><img src="pic\testg2.png" width="440" hegiht="500" align=center /></center>
<center>图2.4.4</center>

&emsp;&emsp;方案三测试，由于网上没有找到途经最少站点的的方案，于是只能通过人工测试进行，如图2.4.5：

<center><img src="pic\test3.png" width="800" hegiht="380" align=center /></center>
<center>图2.4.5</center>

&emsp;&emsp;通过与图2.3的线路进行比较，可以看到该结果确实是途经站点最少的方案，所以该功能没有问题。

&emsp;&emsp;经过以上测试可以看到程序的正确性得到了说明，但没有经过严谨的数学证明，所以这是一大不足。

## **3.程序使用说明**

&emsp;&emsp;在Linux环境下使用以下命令，然后根据提示输入信息：

```
$ make
```

&emsp;&emsp;在Windows10环境下，需要输入以下命令然后根据提示输入即可：

```
> g++ main.cpp subway.cpp -o subway
> ./subway
```

## **4.结语**

&emsp;&emsp;在本实验中，我们简单地实现了一个广州地铁查询系统，该系统通过利用Dijksta算法、BFS算法和DFS算法分别规划出了所经过路径最短的路线、经过站台数最少的路线以及换乘次数最少的路线，经过验证后发现，程序能够比较正确地得到我们所需要的结果，但仍然存在下列的问题需要作进一步的改善：

&emsp;&emsp;1.在第二种方案的提供中我们想了一个算法但没有去实现，而是选择了另一种效率更低的方法，在未进行任何优化的情况下该算法需要花费将近1分钟才可以得到结果，而在经过一些优化以减少不必要的运算后，时间可以缩短到1秒之内；

&emsp;&emsp;2.由于时间原因，本来计划使用QT制造一个用户界面的计划也没有实现，是一个不小的遗憾，导致现在还需要使用命令行来进行测试与程序使用，界面不够美观；

&emsp;&emsp;3.由于数据收集于广州地铁以及百度地图，数据没有精确到小数点，从而导致到后面会有些许的误差，预计时间会有一点出入；

&emsp;&emsp;4.在规划经过站台数最少的路线的时候，我们利用了BFS算法来进行规划，在测试的过程中，我们发现我们仅仅关注了所经过站台数最少这一特点，没有关注换乘次数这一特点。也就是说，虽然程序所规划的路线的确是经过站点数最少的路线，但并不一定是经过站点数最少的路线中换乘次数最少的一条，所以该算法并不具备现实意义，可以改用DFS算法来进行规划；

&emsp;&emsp;5.在规划换乘次数最少的路线时，我们同样地只关注了换乘次数这一特点。也就是说，虽然程序所规划的路线的确是换乘次数最少的路线，但并不一定是换乘次数最少的路线中经过站点最少或者路径最短的一条，所以该算法并不具备现实意义，需要稍作修改。




## **参考文献**
[1] 《数据结构与算法实验实践教程》  乔海燕、蒋爱军、高集荣、刘晓铭著

[2] 石伟力. 大气污染源管理系统的设计与开发[D]. 南宁: 广西大学, 2015

[3] [广州地铁](http://ydyc.gzmtr.cn:19090/pcindex.html#/) http://ydyc.gzmtr.cn:19090/pcindex.html#/

[4] [百度地图](https://map.baidu.com/@12626516,2626080,13z) https://map.baidu.com/@12626516,2626080,13z
=======
# Dijkstra
>>>>>>> 4faa0071abc0f005335c44c69e1750b51d9718fb
