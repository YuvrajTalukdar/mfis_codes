#include<iostream>
#include<chrono>

using namespace std;
using namespace std::chrono;

int gcd(long int a,long int b)
{
    long int temp,big,small;
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

int main()
{
    long int num1,num2;
    cout<<"First number: ";
    cin>>num1;
    cout<<"Second number: ";
    cin>>num2;
    auto start = high_resolution_clock::now();
    cout<<"GCD= "<<gcd(num1,num2);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    cout<<"\n\nduration= "<<duration.count()<<" microseconds";
}
