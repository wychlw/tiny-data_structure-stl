
#ifndef MY_LIB
#define MY_LIB
#endif

#include <iostream>
#include <functional>
#include <utility>
#include "tree.hpp"

using namespace std;
using namespace my_lib;

#define ll long long int

#define DEBUG

#if defined(DEBUG)

void DBG_p()
{
}
template <typename T, typename... Args> void DBG_p(T head, Args... args)
{
    cout << " " << head;
    DBG_p(args...);
}
template <typename... Args> void DBG(Args... args)
{
    cout << "DBG: ";
    DBG_p(args...);
    cout << endl;
}

template <typename T> void DBGA(T arr)
{
    cout << "DBG: ";
    for (auto i : arr)
    {
        DBG_p('f',i.first,i.second,'b');
    }
    cout << endl;
}
#else
#define DBG(f, ...) void()
#endif

less<int> l;

int main()
{

    RBTree<int,int> t;
    t[1]=11;
    t.insert(2,12);
    t.get(3)=13;
    t.insert(0,10);
    t.insert(3,13);
    t.insert(-5,-15);
    t.insert(-3,-13);
    t.insert(-4,-14);
    t.insert(-2,-12);
    t.prt_tree();

    t.remove(1);
    t.remove(-3);
    t.prt_tree();

    cout<<t[3]<<' '<<t[0]<<' '<<t[-1]<<' '<<t[-2]<<endl;
    return 0;
}