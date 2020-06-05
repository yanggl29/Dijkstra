#ifndef ___SUBWAY___
#define ___SUBWAY___
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>

using namespace std;

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

class SUBWAY
{
private:
    int n;                                                  //number of stations
    vector<vector<pair<int, int>>> adjGraph;                //adjacency list
    unordered_map<pair<int, int>, string, pair_hash> lines; //map the edge to the correspoing line
    vector<string> stations;                                //number the station
    pair<int, int> findmin(vector<int> &vec, vector<bool> &sign);
    pair<vector<int>, int> GetShortestPath(int a, int b);   //return the path(reversed) and cost of time
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
    void test()
    {
        string a, b;
        cin >> a >> b;
        auto start = find(stations.begin(), stations.end(), a);
        auto end = find(stations.begin(), stations.end(), b);
        cout << isoneline(start - stations.begin(), end - stations.begin()) << endl;
    }
};
#endif