#include<iostream>
#include<boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

int1024_t ssm(int1024_t x,int1024_t k,int1024_t m)//successive square multiplication
{
    //x^k mod m
    int1024_t start=k,rem,q=2;
    long int count=0;//,power=1;
    int1024_t mod_result=x%m;
    int1024_t mul_large=1;
    while(q!=0)
    {
        rem=start%2;
        q=start/2;
        start=q;
        if(count>0)
        {   mod_result=(mod_result*mod_result)%m;}
        if(rem==1)
        {
            //cout<<"\nmr="<<mod_result;
            mul_large=mul_large*mod_result;
        }
        count++;
    }
    int1024_t result=mul_large%m;
    return result;
}

bool is_prime_FLT(int1024_t number)
{
    //cout<<"\n\nnumber=="<<number;
    int x=2;
    int1024_t mod=ssm(x,number-1,number);
    if(mod==1)
    {   return true;}
    else 
    {   return false;}
}

int main()
{
    int x;
    cout<<"Enter number: ";
    cin>>x;
    if(is_prime_FLT(x))
    {
        cout<<"\nPrime";
    }
    else 
    {
        cout<<"Not prime";
    }
    //cout<<"\n is prime= "<<is_prime_FLT(x);
}