
#ifndef MY_LIB
#define MY_LIB
#endif

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "vector.cpp"
#include "list_old.cpp"

using std::cin;
using std::cout;
using std::endl;

int test_vector();
int test_list();


int main(){
    return test_list();
}

int test_vector()
{

    MY_LIB::vector<std::string> v;
    //v.pop_back();
    v.push_back("hello");
    cout << 1 << endl;
    v.emplace_back();
    cout << 2 << endl;
    v.push_back("world!");
    cout << 3 << endl;
    for (MY_LIB::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it << endl;
    }
    v.push_back("test");
    for (MY_LIB::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it << endl;
    }
    v.pop_back();
    for (MY_LIB::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it << endl;
    }
    return 0;
}

int test_list(){
    MY_LIB::list<int> l;

    l.push_back(1);
    l.push_front(2);
    l.emplace_back(3);
    l.emplace_front(4);

    for (MY_LIB::list<int>::iterator it=l.begin();it!=l.end();it++) {
        cout<<*it<<endl;
    }

    l.pop_back();
    l.push_front(72);
    l.push_front(73);
    l.pop_front();

    for (MY_LIB::list<int>::iterator it=l.begin();it!=l.end();it++) {
        cout<<*it<<endl;
    }

    //MY_LIB::list<int>::iterator it_tmp=l.begin();
    //it_tmp++;
    //l.insert(it_tmp,99);
    //cout<<*it_tmp<<endl;
    //it_tmp--;
    //cout<<*it_tmp<<endl;

    cout<<"ll"<<endl;

    return 0;
}