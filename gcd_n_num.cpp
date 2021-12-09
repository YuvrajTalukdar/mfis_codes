#include<iostream>
#include<string>
#include<chrono>
#include<set>
#include<math.h>

using namespace std;
using namespace std::chrono;

int random_num_generator(int max)
{
    return rand()%(max*2);
}

int gcd(int a,int b)
{
    int temp,big,small;
    if(a>b)
    {   
        big=a;
        small=b;
    }
    else 
    {
        big=b;
        small=a;
    }
    while(small>0)
    {
        temp=small;
        small=big%small;
        big=temp;
    }
    return big;
}

int find_gcd(set<int> coefficient_list)
{
    set<int>::iterator itr=coefficient_list.begin();
    int gcd_result=*coefficient_list.begin();
    itr++;
    for(itr;itr!=coefficient_list.end();itr++)
    {   gcd_result=gcd(gcd_result,*itr);}
    return gcd_result;
}

int main()
{
    int count,num;
    cout<<"Enter the no of elements: ";
    cin>>count;
    set<int> num_set;
    cout<<endl;
    while(num_set.size()!=count)
    {
        num=random_num_generator(count)*5;
        num_set.insert(num);
        //cout<<num<<", ";
    }
    auto start = high_resolution_clock::now();
    cout<<"\n\nGDC = "<<find_gcd(num_set);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    cout<<"\n\nduration= "<<duration.count()<<" microseconds";
}
 
