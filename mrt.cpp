//Miller Rabin Test
#include<iostream>
#include<math.h>
#include<boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
//#include <boost/multiprecision/detail/functions/pow.hpp>

using namespace boost::multiprecision;
using namespace std;

int1024_t ssm(int1024_t x,int1024_t k,int1024_t m)//successive square multiplication
{
    //x^k mod m
    int1024_t start=k,rem,q=2;
    long int count=0;//,power=1;
    int1024_t mod_result=x%m;
    int1024_t mul_large=1;
    //vector<int1024_t> mod_result_vec;
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
            if(mul_large>m)
            {   mul_large=mul_large%m;}
        }
        count++;
    }
    int1024_t result=mul_large%m;
    return result;
}

bool is_prime_MRT(int1024_t num)
{
    //step 1
    if(num%2!=0)
    {
        int1024_t power_result,n=num-1;
        cpp_dec_float_100 power=1;
        int1024_t q=1,q_old,power_old;
        do
        {
            power_result=pow(2,power).convert_to<int1024_t>();
            q_old=q;
            q=n/power_result;
            power_old=(int1024_t)power;
            power++;
        }
        while(n%power_result==0);
        //step 2
        int1024_t a=2;
        int1024_t b0=ssm(a,q_old,num);
        //step 3
        if(b0==1 || b0==-1)
        {   return true;}
        else
        {   
            int1024_t ssm_result;
            do
            {
                ssm_result=ssm(b0,2,num);
                b0=ssm_result;
                if(ssm_result!=num-1 || ssm_result!=num+1)
                {   break;}
            }
            while(true);
            if(ssm_result==num-1)
            {   return true;}
            else if(ssm_result==num+1)
            {   return false;}
            else
            {   return false;}
        }
    }
    else
    {   return false;}
}

int main()
{
    int1024_t num;
    cout<<"\nEnter number: ";
    cin>>num;
    if(is_prime_MRT(num))
    {   cout<<"\nIt is prime.";}
    else 
    {   cout<<"\nIt is not prime.";}
}