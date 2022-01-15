#include<iostream>
#include<vector>
#include<chrono>
#include<algorithm>
#include<boost/multiprecision/cpp_int.hpp>//for int1024_t
#include<boost/multiprecision/cpp_dec_float.hpp>//for cpp_dec_float_100

using namespace std;
using namespace boost::multiprecision;
using namespace std::chrono;

struct eq_element
{
    int power;
    int1024_t coefficient;
};

struct point
{   int1024_t x,y;};

struct table_row
{
    int1024_t a_col;//x;
    int1024_t b_col;//y^2 mod m
    int1024_t c_col;//is QR
    vector<int1024_t> qr_indexes;
};

vector<int1024_t> binary_search(vector<table_row>& sorted_table,int1024_t& search_vaulue)
{
    long long int left=0,right=sorted_table.size()-1,mid;
    vector<int1024_t> qr_indexes;
    while(left <= right)
    {   
        mid=left+(right-left)/2;
        if(sorted_table.at(mid).b_col==search_vaulue)
        {   
            qr_indexes.push_back(sorted_table.at(mid).a_col);
            long long int index=mid;
            //look up 
            index--;
            while(index>=0)
            {
                if(sorted_table.at(index).b_col==search_vaulue)
                {   qr_indexes.push_back(sorted_table.at(index).a_col);}
                else if(sorted_table.at(index).b_col!=search_vaulue)
                {   break;}
                index--;
            }
            index=mid+1;
            //look down
            while(index<sorted_table.size())
            {
                if(sorted_table.at(index).b_col==search_vaulue)
                {   qr_indexes.push_back(sorted_table.at(index).a_col);}
                else if(sorted_table.at(index).b_col!=search_vaulue)
                {   break;}
                index++;
            }
            break;
        }
        else if(sorted_table.at(mid).b_col>search_vaulue)
        {   right=mid-1;}
        else if(sorted_table.at(mid).b_col<search_vaulue)
        {   left=mid+1;}
    }
    return qr_indexes;
}

int1024_t evualate_equation(vector<eq_element> equation,int1024_t x,int1024_t m)
{
    int1024_t result=0;
    for(unsigned long long int a=0;a<equation.size();a++)
    {   result=result+equation.at(a).coefficient*pow(x,equation.at(a).power);}
    return result%m;
}

bool row_comparator(table_row r1,table_row r2)
{   
    if(r1.b_col<r2.b_col)
    {   return true;}
    else
    {   return false;}
}

int main()
{
    cout<<"equation mod m\nm: ";
    int1024_t m;
    cin>>m;
    int term_count;
    cout<<"No of elements in ellipse equation: ";
    cin>>term_count;
    vector<eq_element> equation; 
    for(int a=0;a<term_count;a++)
    {
        eq_element element;
        cout<<"coefficient "<<a+1<<": ";
        cin>>element.coefficient;
        cout<<"power "<<a+1<<": ";
        cin>>element.power;
        equation.push_back(element);
    }
    auto start = high_resolution_clock::now();
    vector<table_row> table;
    vector<table_row> sorted_table;
    for(int1024_t a=0;a<m;a++)
    {
        table_row row;
        row.a_col=a;
        row.b_col=(a*a)%m;
        row.c_col=evualate_equation(equation,a,m);
        table.push_back(row);
        sorted_table.push_back(row);
    }
    sort(sorted_table.begin(),sorted_table.end(),row_comparator);

    vector<point> points_on_curve;
    for(unsigned long long int a=0;a<table.size();a++)
    {
        table.at(a).qr_indexes=binary_search(sorted_table,table.at(a).c_col);
        for(unsigned long long int b=0;b<table.at(a).qr_indexes.size();b++)
        {
            point p;
            p.x=table.at(a).a_col;
            p.y=table.at(a).qr_indexes.at(b);
            points_on_curve.push_back(p);
        }   
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    //print table
    cout<<"x | y^2 mod m | equation mod m | is QR | Column where QR is present\n";
    for(unsigned long long int a=0;a<table.size();a++)
    {
        cout<<table.at(a).a_col<<" | "<<table.at(a).b_col<<" | "<<table.at(a).c_col;
        if(table.at(a).qr_indexes.size()>0)
        {   cout<<" | Y |";}
        else
        {   cout<<" | N |";}
        for(unsigned long long int b=0;b<table.at(a).qr_indexes.size();b++)
        {
            cout<<table.at(a).qr_indexes.at(b)<<",";
        }
        cout<<endl;
    }
    cout<<"\nPoint on ellipse are:- \n";
    for(unsigned long long int a=0;a<points_on_curve.size();a++)
    {
        cout<<"("<<points_on_curve.at(a).x<<","<<points_on_curve.at(a).y<<"),";
    }
    cout<<"\n";
    cout<<"\n\nduration= "<<duration.count()<<" microseconds\n\n";
}