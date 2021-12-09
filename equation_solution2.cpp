#include<iostream>
#include<chrono>
#include<vector>

using namespace std;
using namespace std::chrono;

vector<long int> big_list,small_list;

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
        cout<<"\ns="<<small<<" b="<<big;
        small_list.push_back(small);
        big_list.push_back(big);
        temp=small;
        small=big%small;
        big=temp;
    }
    return temp;
}

struct data
{
    long int val;
    int data_type;
    //1= add,2=sub,3=(,4=),5=val
};

vector<data> prep_data(long int a,long int b)
{
    vector<data> data_tape;
    data d1;
    d1.data_type=5;
    d1.val=a;
    data_tape.push_back(d1);
    
    //data d2;
    //d2.data_type=2;
    //data_tape.push_back(d2);
    
    data d3;
    d3.data_type=6;
    d3.val=(a/b)*(-1);
    data_tape.push_back(d3);

    data d4;
    d4.data_type=3;
    data_tape.push_back(d4);

    data d5;
    d5.data_type=5;
    d5.val=b;
    data_tape.push_back(d5);

    data d6;
    d6.data_type=4;
    data_tape.push_back(d6);

    return data_tape;
}

void print_tape(vector<data> data_tape)
{
    cout<<"\n";
    for(int c=0;c<data_tape.size();c++)
    {
        if(data_tape.at(c).data_type==1)
        {   cout<<"+";}
        else if(data_tape.at(c).data_type==2)
        {   cout<<"-";}
        else if(data_tape.at(c).data_type==3)
        {   cout<<"(";}
        else if(data_tape.at(c).data_type==4)
        {   cout<<")";}
        else if(data_tape.at(c).data_type>=5)
        {   cout<<data_tape.at(c).val;}
    }
}

long int calc_u(long int m,long int a)
{
    vector<data> data_tape;
    //get initial tape
    data_tape=prep_data(big_list[big_list.size()-2],small_list[small_list.size()-2]);
    //convert all in the form of m and a
    for(int c=0;c<data_tape.size();c++)
    {
        if(data_tape.at(c).data_type==5)
        {
            if(data_tape.at(c).val!=m && data_tape.at(c).val!=a)
            {   
                int b;
                for(b=small_list.size()-1;b>=0;b--)
                {
                    if(small_list.at(b)==data_tape.at(c).val)
                    {   break;}
                }
                b--;
                print_tape(data_tape);
                //cout<<"\nval="<<data_tape.at(c).val;
                //cout<<"\nb="<<big_list.at(b)<<" s="<<small_list.at(b)<<" c="<<c;
                vector<data> data_tape_temp=prep_data(big_list.at(b),small_list.at(b));
                data_tape.erase(data_tape.begin()+c);
                data_tape.insert(data_tape.begin()+c,data_tape_temp.begin(),data_tape_temp.end());
                c--;
                //int gh;cin>>gh;
            }
        }
    }
    print_tape(data_tape);
    //get a0 m0 count and sign
    long int a0=0,m0=0;
    vector<long int> mul_stack,bracket_stack;
    for(int c=0;c<data_tape.size();c++)
    {
        if(data_tape.at(c).data_type==3)
        {   bracket_stack.push_back(3);}
        else if(data_tape.at(c).data_type==4)
        {   
            bracket_stack.pop_back();
            mul_stack.pop_back();
        }
        else if(data_tape.at(c).data_type==5)
        {
            long int a0_temp=1,m0_temp=1;
            for(int b=0;b<mul_stack.size();b++)
            {
                if(data_tape.at(c).val==a)
                {   a0_temp=a0_temp*mul_stack.at(b);}
                else if(data_tape.at(c).val==m)
                {   m0_temp=m0_temp*mul_stack.at(b);}
            }
            if(data_tape.at(c).val==a)
            {   a0+=a0_temp;}
            else if(data_tape.at(c).val==m)
            {   m0+=m0_temp;}
        }
        else if(data_tape.at(c).data_type==6)
        {   mul_stack.push_back(data_tape.at(c).val);}
    }
    long int u0=a0%m;
    if(u0<0)
    {   u0+=m;}
    cout<<"\n\nm="<<m0<<" a="<<a0<<" u0="<<u0;
    
    return u0;
}

int main()
{
    cout<<"a*x=c*mod(m)";
    long int a,c,m;
    cout<<"\na: ";
    cin>>a;
    cout<<"c: ";
    cin>>c;
    cout<<"m: ";
    cin>>m;
    //bool sol_avail;
    auto start = high_resolution_clock::now();
    long int g=gcd(a,m);
    if(c%g==0)
    {   
        long int u0=calc_u(m,a);
        long int x0=u0*c/g;
        cout<<"\n\nSolutions:-";
        for(int k=0;k<g;k++)
        {
            x0=x0+m*k/g;
            cout<<"\nx"<<k<<"= "<<x0;
        }
    }
    else
    {   cout<<"\nSolution not available!!";}
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    cout<<"\n\nduration= "<<duration.count()<<" microseconds";

    return 0;
}
