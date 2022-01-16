#include<iostream>
#include<vector>
#include<chrono>
#include<boost/multiprecision/cpp_int.hpp>//for int1024_t
#include<boost/multiprecision/cpp_dec_float.hpp>//for cpp_dec_float_100

using namespace std;
using namespace boost::multiprecision;
using namespace std::chrono;

struct point
{   int1024_t x,y;};

int1024_t inverse_euclid_my_version(int1024_t b,int1024_t m)
{
    int1024_t a1=1,a2=0,a3=m,b1=0,b2=1,b3=b,q;
    while(b3!=1)
    {
        q=a3/b3;
        int1024_t t1=b1,t2=b2,t3=b3;
        b1=a1-q*b1;
        b2=a2-q*b2;
        b3=a3-q*b3;
        a1=t1;
        a2=t2;
        a3=t3;   
    }
    if(b2<0)
    {   return b2+m;}
    else
    {   return b2;}
}

int1024_t calc_lamda1(int1024_t x,int1024_t y,int1024_t mod)
{
    int1024_t numerator=(3*x*x+1)%mod;
    int1024_t denominator=(2*y)%mod;
    //cout<<"\nn="<<numerator<<" d="<<denominator<<endl;
    int1024_t inv_result=inverse_euclid_my_version(denominator,mod);
    return (inv_result*numerator)%mod;
}

int1024_t calc_lamda2(int1024_t prev_x,int1024_t prev_y,int1024_t x,int1024_t y,int1024_t mod)
{
    int1024_t numerator=(y-prev_y)%mod;
    int1024_t denominator=(x-prev_x)%mod;
    if(numerator<0)
    {   numerator=numerator+mod;}
    if(denominator<0)
    {   denominator=denominator+mod;}
    int1024_t inv_result=inverse_euclid_my_version(denominator,mod);
    return (inv_result*numerator)%mod;
}

int main()
{
    point p1,p2;
    cout<<"Enter the initial point: \nx: ";
    cin>>p1.x;
    cout<<"y: ";
    cin>>p1.y;
    p2.x=p1.x;
    p2.y=p1.y;
    int1024_t n,m;
    cout<<"mod: ";
    cin>>m;
    int1024_t lamda;
    point new_point;
    //for(int1024_t a=0;a<n;a++)
    int1024_t a=0;
    int1024_t first_x,first_y;
    while(true)
    {
        if((p1.x==p2.x) && (p1.y==p2.y || p1.y==-1*(p2.y-m)))
        {   lamda=calc_lamda1(p1.x,p1.y,m);}
        else
        {   lamda=calc_lamda2(p1.x,p1.y,p2.x,p2.y,m);}
        //point new_point;
        new_point.x=(lamda*lamda-p2.x-p1.x)%m;
        if(new_point.x<0)
        {   new_point.x=new_point.x+m;}
        new_point.y=(lamda*(p1.x-new_point.x)-p1.y)%m;
        if(new_point.y<0)
        {   new_point.y=new_point.y+m;}
        if(a==0)
        {   
            first_x=new_point.x;
            first_y=new_point.y;
        }
        cout<<"\nlamda: "<<lamda;
        cout<<" : ("<<new_point.x<<","<<new_point.y<<"), ";
        p2.x=new_point.x;
        p2.y=new_point.y;
        if(a!=0 && first_x==new_point.x && first_y==new_point.y)
        {   break;}
        a++;
    }
    return 0;
}