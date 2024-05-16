#include <bits/stdc++.h>
#include <vector>
#include <string>
using namespace std;

int main()
{
    string s="bismillah";
    for(int i=0;i<s.size();i++){
        int x=s[i];
        x+=12;
        x*=11;
        cout<<x<<", ";
    }
}