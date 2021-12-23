#include<iostream>

using namespace std;

long int inverse_euclid_my_version(long int b,long int m)
{
    long int a1=1,a2=0,a3=m,b1=0,b2=1,b3=b,q;
    while(b3!=1)
    {   
        q=a3/b3;
        //cout<<"\na3="<<a3<<" b3="<<b3<<" a2="<<a2<<" b2="<<b2<<" a1="<<a1<<" b1="<<b1<<" q="<<q;
        long int t1=b1,t2=b2,t3=b3;
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

int main()
{
    int no_of_eq;
    cout<<"no of eq: ";
    cin>>no_of_eq;
    long long int a_arr[no_of_eq],m[no_of_eq],M_arr[no_of_eq],M_inv[no_of_eq],M=1;
    for(int a=0;a<no_of_eq;a++)
    {
        cout<<"\na"<<a+1<<": ";
        cin>>a_arr[a];
        cout<<"m"<<a+1<<": ";
        cin>>m[a];
        M=m[a]*M;
    }
    cout<<"\n\nM= "<<M<<endl;
    for(int a=0;a<no_of_eq;a++)
    {
        M_arr[a]=M/m[a];
        cout<<", M"<<a+1<<"="<<M_arr[a];
        M_inv[a]=inverse_euclid_my_version(M_arr[a],m[a]);
    }
    cout<<endl;
    for(int a=0;a<no_of_eq;a++)
    {
        cout<<", M_inv"<<a+1<<"="<<M_inv[a];
    }
    int x=0;
    for(int a=0;a<no_of_eq;a++)
    {
        x=x+a_arr[a]*M_arr[a]*M_inv[a];
    }
    cout<<"\nx= "<<x%(int)M<<endl;
}
