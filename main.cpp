#include "subway.h"
int main()
{
    SUBWAY subway;
    int sel;
    cout << "1.路径最短" << endl
         << "2.换乘最少" << endl
         << "3.经过站点最少" << endl
         << ">>请输入线路搜索方式：";
    cin >> sel;
    if (sel != 1 && sel != 2 && sel != 3)
    {
        cout << "输入错误" << endl;
        return 0;
    }
    string a, b;
    cout << ">>请输入起始站：";
    cin >> a;
    cout << ">>请输入终点站：";
    cin >> b;
    cout << endl;
    switch (sel)
    {
    case 1:
        subway.FindShortestPathestPath(a, b);
        break;
    case 2:
        subway.FindMinimumTransfer(a, b);
        break;
    case 3:
        subway.FindMinimumStation(a, b);
        break;
    }
    return 0;
}