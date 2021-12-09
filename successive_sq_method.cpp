#include<iostream>
#include<bitset>
#include<string>
#include<vector>
#include<math.h>
#include<boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;


/*string multiplyTwoNumbers(string num1, string num2) //string multiply
{ 
    if (num1 == "0" || num2 == "0") 
    { return "0"; } 
    string product(num1.size() + num2.size(), 0); 
    for (int i = num1.size() - 1; i >= 0; i--) 
    { 
        for (int j = num2.size() - 1; j >= 0; j--) 
        { 
            int n = (num1[i] - '0') * (num2[j] - '0') + product[i + j + 1]; 
            product[i + j + 1] = n % 10; product[i + j] += n / 10; 
            } 
    } 
    for (int i = 0; i < product.size(); i++) 
    { product[i] += '0'; } 
    if (product[0] == '0') 
    { return product.substr(1); } 
    return product; 
};*/

int main()
{
    int1024_t x,k,m;
    cout<<"x^k mod m";
    cout<<"\nx: ";
    cin>>x;
    cout<<"k: ";
    cin>>k;
    cout<<"m: ";
    cin>>m;

    //vector<long int> power_list;
    //vector<long int> mod_result_list;
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
            //mod_result_list.push_back(mod_result);
            //power_list.push_back(power);
            cout<<"\nmr="<<mod_result;//<<" p="<<power;
            mul_large=mul_large*mod_result;
            if(mul_large>m)
            {   mul_large=mul_large%m;}
        }
        count++;
    }
    cout<<"\n\nmul="<<mul_large;
    cout<<"\n\n"<<x<<"^"<<k<<" mod "<<m<<" = "<<mul_large%m;
}