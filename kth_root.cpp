#include<iostream>
#include<boost/multiprecision/cpp_int.hpp>//for int1024_t
#include<boost/multiprecision/cpp_dec_float.hpp>//for cpp_dec_float_100
#include<string>
#include<vector>
#include<gmp.h>

using namespace std;
using namespace boost::multiprecision;

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

int1024_t string_to_number(string str_num)
{
    //cout<<"\nstr_num="<<str_num;
    int1024_t num=0,mul=1;
    for(int a=str_num.length()-1;a>=0;a--)
    {
        //cout<<" stoi="<<(string(1,str_num.at(a)));
        num=num+mul*stoi(string(1,str_num.at(a)));
        mul=mul*10;
    }
    return num;
}

int1024_t ssm(int1024_t x,int1024_t k,int1024_t m)//successive square multiplication
{
    //x^k mod m
    int1024_t start=k,rem,q=2;
    long int count=0;//,power=1;
    mpz_t mod_result,x_mpz,m_mpz,k_mpz;
    mpz_init_set_str(x_mpz,x.convert_to<string>().c_str(),10);
    mpz_init_set_str(m_mpz,m.convert_to<string>().c_str(),10);
    mpz_init_set_str(k_mpz,k.convert_to<string>().c_str(),10);
    mpz_init(mod_result);
    mpz_mod(mod_result,x_mpz,m_mpz);
    
    mpz_t mul_large;
    mpz_init_set_str(mul_large,"1",10);
    //vector<int1024_t> mod_result_vec;
    while(q!=0)
    {
        rem=start%2;
        q=start/2;
        start=q;
        if(count>0)
        {   
            mpz_t mul;
            mpz_init(mul);
            mpz_mul(mul,mod_result,mod_result);
            mpz_mod(mod_result,mul,m_mpz);
        }
        if(rem==1)
        {
            //cout<<"\nmr="<<mod_result;
            mpz_mul(mul_large,mul_large,mod_result);
            if(mpz_cmp(mul_large,m_mpz)>0)
            {   mpz_mod(mul_large,mul_large,m_mpz);}
        }
        count++;
    }
    mpz_t result;
    mpz_init(result);
    mpz_mod(result,mul_large,m_mpz);
    
    return string_to_number(mpz_get_str(NULL,10,result));
}

bool is_prime_FLT(int1024_t number)//primality test using fermat's little theorem. Should fail in 561
{
    //cout<<"\n\nnumber=="<<number;
    int x=2;
    int1024_t mod=ssm(x,number-1,number);
    if(mod==1)
    {   return true;}
    else 
    {   return false;}
}
bool is_prime_MRT(int1024_t num)//Miller Rabin test.
{
    //step 1
    if(num%2!=0)
    {
        if(is_prime_FLT(num))
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
            if(b0==1 || b0==num-1)
            {   return true;}
            else
            {   
                int1024_t ssm_result;
                do
                {
                    ssm_result=ssm(b0,2,num);
                    b0=ssm_result;
                    //cout<<"ssm="<<ssm_result<<endl;
                    if(ssm_result==num-1 || ssm_result==1)
                    {   break;}
                }
                while(true);
                if(ssm_result==num-1)
                {   return true;}
                else if(ssm_result==1)
                {   return false;}
                else
                {   return false;}
            }
        }
        else 
        {   return false;}
    }
    else
    {   return false;}
}

vector<vector<int1024_t>> get_prime_factors(int1024_t num)
{
    int1024_t mul=1,start=2,temp;
    int count=0;
    vector<vector<int1024_t>> pf_and_count_vec;
    while(mul!=num)
    {
        if(num%start==0)
        {
            vector<int1024_t> pf;
            pf.push_back(start);
            temp=start;
            count=0;
            while(num%temp==0)
            {
                count++;
                temp=temp*start;
                //cout<<"\ncheck3";
            }
            pf.push_back(count);
            mul=mul*temp/start;
            //cout<<"\nfactors= "<<pf[0]<<", "<<pf[1];
            pf_and_count_vec.push_back(pf);
            //cout<<"\ncheck1";
        }
        start++;
        while(!is_prime_MRT(start))
        {   start++;/*cout<<"\ncheck2";*/}
        //cout<<"\ncheck4="<<start;
    }
    return pf_and_count_vec;
}

int1024_t calc_phi(int1024_t m)
{
    vector<vector<int1024_t>> pf_and_count_vec=get_prime_factors(m);
    int1024_t mul=1;
    for(long long int a=0;a<pf_and_count_vec.size();a++)
    {   
        mul=mul*(pow(pf_and_count_vec.at(a).at(0),pf_and_count_vec.at(a).at(1).convert_to<long long int>())-pow(pf_and_count_vec.at(a).at(0),pf_and_count_vec.at(a).at(1).convert_to<long long int>()-1));
    }
    return mul;
}

int main()
{
    int1024_t x,k,a,m;
    cout<<"x^k=a (mod m) \nk: ";
    cin>>k;
    cout<<"a: ";
    cin>>a;
    cout<<"m: ";
    cin>>m;
    
    int1024_t phi_m=calc_phi(m);
    int1024_t u=inverse_euclid_my_version(k,phi_m);
    if(u==0)
    {   return 0;}
    else
    {
        int1024_t x=ssm(a,u,m);
        cout<<"\nx= "<<x<<"\n";
    }

    return 0;
}