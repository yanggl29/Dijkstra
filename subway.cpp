#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include "subway.h"
void SUBWAY::print_result(const vector<int> &result)
{
    string cur_line = lines[make_pair(result[result.size() - 1], result[result.size() - 2])];
    cout << ">>"
         << "搭乘" << cur_line << ':';
    for (int i = result.size() - 1; i >= 1; i--)
    {
        if (i > 0 && lines[make_pair(result[i], result[i - 1])] != cur_line)
        {
            cur_line = lines[make_pair(result[i], result[i - 1])];
            cout << stations[result[i]];
            cout << '\n'
                 << ">>"
                 << "换乘" << cur_line << ':';
        }
        cout << stations[result[i]] << ">>";
    }
    cout << stations[result[0]];
    cout << endl
         << ">>您已到达目的地" << endl;
}

SUBWAY::SUBWAY()
{
    n = 0;
    buildGraph();
}

pair<int, int> SUBWAY::findmin(vector<int> &vec, vector<bool> &sign)
{
    int min = __INT_MAX__, ans = -1;
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i] < min && sign[i] == false)
        {
            min = vec[i];
            ans = i;
        }
    }
    if (ans != -1)
        sign[ans] = true;
    pair<int, int> a = make_pair(min, ans);
    return a;
}

void SUBWAY::buildGraph()
{
    string starting_point, destination, line;
    int time, start_num, end_num;
    ifstream in("subway_data.txt");
    while (in.peek() != EOF)
    {
        in >> line >> starting_point >> destination >> time;
        auto start = find(stations.begin(), stations.end(), starting_point);
        if (start == stations.end())
        {
            stations.push_back(starting_point);
            n++;
        }
        auto end = find(stations.begin(), stations.end(), destination);
        if (end == stations.end())
        {
            stations.push_back(destination);
            n++;
        }
        start_num = find(stations.begin(), stations.end(), starting_point) - stations.begin();
        end_num = find(stations.begin(), stations.end(), destination) - stations.begin();
        lines[make_pair(start_num, end_num)] = line;
        lines[make_pair(end_num, start_num)] = line;

        if (adjGraph.size() > start_num)
        {
            adjGraph[start_num].push_back(make_pair(end_num, time));
        }
        else
        {
            vector<pair<int, int>> tmp;
            tmp.push_back(make_pair(end_num, time));
            adjGraph.push_back(tmp);
            tmp.clear();
        }

        if (adjGraph.size() > end_num)
        {
            adjGraph[end_num].push_back(make_pair(start_num, time));
        }
        else
        {
            vector<pair<int, int>> tmp;
            tmp.push_back(make_pair(start_num, time));
            adjGraph.push_back(tmp);
            tmp.clear();
        }
    }
    in.close();
}

pair<vector<int>, int> SUBWAY::GetShortestPath(int a, int b)
{
    vector<int> t;
    vector<bool> sign;
    vector<int> path, dis;
    for (int i = 0; i < adjGraph.size(); ++i)
    {
        sign.push_back(false);
        path.push_back(-1);
        dis.push_back(__INT_MAX__);
    }
    dis[a] = 0;
    pair<int, int> index = findmin(dis, sign);
    path[index.second] = -1;
    for (int i = 0; i < adjGraph.size(); ++i)
    {
        for (int j = 0; j < adjGraph[index.second].size(); ++j)
        {
            if (dis[adjGraph[index.second][j].first] > index.first + adjGraph[index.second][j].second)
            {
                path[adjGraph[index.second][j].first] = index.second;
                dis[adjGraph[index.second][j].first] = index.first + adjGraph[index.second][j].second;
            }
        }
        index = findmin(dis, sign);
    }
    int distance = dis[b];
    while (b != a && b != -1)
    {
        t.push_back(b);
        b = path[b];
    }
    t.push_back(b);
    string cur_line = lines[make_pair(t[t.size()-1],t[t.size()-2])];
    int cnt = 0;
    for(int i = t.size()-2; i > 0; --i){
        if(cur_line != lines[make_pair(t[i],t[i-1])])
        {
            cur_line = lines[make_pair(t[i],t[i-1])];
            cnt++;
        }
    }
    distance += 2*cnt;
    pair<vector<int>, int> ans = make_pair(t,distance);
    return ans;
}

void SUBWAY::DFS(int last_station, vector<bool> &sign, int start, int end, vector<int> &tmppath, vector<int> &result, int cnt)
{
    static int min = __INT_MAX__;
    sign[start] = true;
    tmppath.push_back(start);
    if (start == end)
    {
        if (min > cnt) min = cnt;
        result = tmppath;
    }
    else
    {
        for (int i = 0; i < adjGraph[start].size(); i++)
        {
            if (!sign[adjGraph[start][i].first])
            {
                if (lines[make_pair(last_station, start)] == lines[make_pair(start, adjGraph[start][i].first)])
                    DFS(start, sign, adjGraph[start][i].first, end, tmppath, result, cnt);
                else if(cnt+1 <= min)
                    DFS(start, sign, adjGraph[start][i].first, end, tmppath, result, cnt + 1);
            }
        }
    }
    sign[start] = false;
    tmppath.pop_back();
}

vector<int> SUBWAY::GetMinimumStation(int a, int b)
{
    vector<int> result;
    vector<bool> sign;
    vector<int> path;
    queue<pair<int, int>> tmp;
    for (int i = 0; i < n; ++i)
    {
        sign.push_back(false);
        path.push_back(-1);
    }
    for (int i = 0; i < adjGraph[a].size(); ++i)
    {
        tmp.push(make_pair(adjGraph[a][i].first, a));
    }
    sign[a] = true;
    while (!tmp.empty())
    {
        pair<int, int> t = tmp.front();
        tmp.pop();
        if (!sign[t.first])
        {
            path[t.first] = t.second;
            sign[t.first] = true;
            for (int j = 0; j < adjGraph[t.first].size(); ++j)
            {
                tmp.push(make_pair(adjGraph[t.first][j].first, t.first));
            }
        }
    }
    while (b != a && b != -1)
    {
        result.push_back(b);
        b = path[b];
    }
    result.push_back(b);
    return result;
}

bool SUBWAY::check_input(string starting_point, string destination)
{
    auto start = find(stations.begin(), stations.end(), starting_point);
    auto end = find(stations.begin(), stations.end(), destination);
    if (start == stations.end())
    {
        cout << "起始站不存在，请重新输入" << endl;
        return false;
    }
    if (end == stations.end())
    {
        cout << "终点站不存在，请重新输入" << endl;
        return false;
    }
    if (end == start)
    {
        cout << "您已处于目的地" << endl;
        return false;
    }
    return true;
}

void SUBWAY::FindShortestPathestPath(string starting_point, string destination)
{
    if (check_input(starting_point, destination))
    {
        auto start = find(stations.begin(), stations.end(), starting_point);
        auto end = find(stations.begin(), stations.end(), destination);
        pair<vector<int>, int> result = GetShortestPath(start - stations.begin(), end - stations.begin());
        print_result(result.first);
        cout << "预计花费时间：" << result.second << "min(s)\n";
    }
}

void SUBWAY::FindMinimumTransfer(string starting_point, string destination)
{
    if (check_input(starting_point, destination))
    {
        auto start = find(stations.begin(), stations.end(), starting_point);
        auto end = find(stations.begin(), stations.end(), destination);
        vector<bool> sign;
        for (int i = 0; i < n; i++)
        {
            sign.push_back(false);
        }
        vector<int> tmppath, transfer, result;
        sign[start - stations.begin()] = true;
        tmppath.push_back(start - stations.begin());
        for(int i = 0; i < adjGraph[start - stations.begin()].size(); i++)
        {
            DFS(start - stations.begin(), sign, adjGraph[start - stations.begin()][i].first, end - stations.begin(), tmppath, result, 0);
        }
        reverse(result.begin(), result.end());
        print_result(result);
    }
}

void SUBWAY::FindMinimumStation(string starting_point, string destination)
{
    if (check_input(starting_point, destination))
    {
        auto start = find(stations.begin(), stations.end(), starting_point);
        auto end = find(stations.begin(), stations.end(), destination);
        vector<int> result = GetMinimumStation(start - stations.begin(), end - stations.begin());
        print_result(result);
    }
}