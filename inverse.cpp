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
    while(small!=0)
    {
        temp=small;
        small=big%small;
        big=temp;
    }
    return temp;
}

long int inverse_brut_force(long int a,long int m)
{
    for (long int x = 1; x < m; x++)
    {
        if (((a%m) * (x%m)) % m == 1)
        {   return x;}
    }
}

long int inverse_euclid(long int a,long int m)
{
    long int m0 = m;
    long int y = 0, x = 1;
 
    if (m == 1)
        return 0;
 
    while (a > 1) {
        // q is quotient
        long int q = a / m;
        long int t = m;
 
        // m is remainder now, process same as
        // Euclid's algo
        m = a % m; a = t;
        t = y;
 
        // Update y and x
        y = x - q * y;
        x = t;
    }
 
    // Make x positive
    if (x < 0)
        x += m0;
 
    return x;
}

long int inverse_euclid_my_version(long int b,long int m)
{
    long int a1=1,a2=0,a3=m,b1=0,b2=1,b3=b,q;
    while(b3!=1)
    {
        q=a3/b3;
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
    cout<<"b*x=1*mod(m)";
    long int b,m;
    cout<<"\nb: ";
    cin>>b;
    cout<<"m: ";
    cin>>m;
    point1:
    cout<<"\n1. Brut force"<<"\n2. Extended euclidian algorithm (downloaded version)"<<"\n3. Extended euclidian algorithm (my version)";
    cout<<"\n\nSelect option: ";
    int option;
    cin>>option;
    if(option==1)
    {
        auto start = high_resolution_clock::now();
        if(gcd(b,m)==1)
        {
            cout<<"\nx= "<<inverse_brut_force(b,m);
        }
        else
        {
            cout<<"Since gcd of a and m is not 1, inverse cannot be found.";
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start); 
        cout<<"\n\nduration= "<<duration.count()<<" microseconds";
    }
    else if(option==2)
    {
        auto start = high_resolution_clock::now();
        //cout<<"\nx= "<<inverse_euclid(a,b);
        if(gcd(b,m)==1)
        {
            cout<<"\nx= "<<inverse_euclid(b,m);
        }
        else
        {
            cout<<"Since gcd of a and b is not 1, inverse cannot be found.";
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start); 
        cout<<"\n\nduration= "<<duration.count()<<" microseconds";
    }
    else if(option==3)
    {
        auto start = high_resolution_clock::now();
        //cout<<"\nx= "<<inverse_euclid(a,b);
        if(gcd(b,m)==1)
        {
            cout<<"\nx= "<<inverse_euclid_my_version(b,m);
        }
        else
        {
            cout<<"Since gcd of a and b is not 1, inverse cannot be found.";
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start); 
        cout<<"\n\nduration= "<<duration.count()<<" microseconds";
    }
    else
    {
        cout<<"\nWrond option!!";
        goto point1;
    }   
}
