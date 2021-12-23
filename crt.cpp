#include<iostream>

using namespace std;

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
        int temp=1;
        while(((int)M_arr[a]*temp)%(int)m[a]!=1)
        {   temp++;}
        M_inv[a]=temp;
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
