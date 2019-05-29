#include<iostream>
using namespace std;
long fb(int n)
{
     if(n<=2)
     {
         return 1;
     }
     else
         return fb(n-1)+fb(n-2);
}
int main()
{
    int a;
    cin>>a;
    for(int i=1;;i++)
    {
        cout<<fb(i)<<"\t"<<i<<endl;
        system("pause");
    }
}