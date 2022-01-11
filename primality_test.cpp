#include<iostream>
#include<boost/multiprecision/cpp_int.hpp>//for int1024_t
#include<boost/multiprecision/cpp_dec_float.hpp>//for cpp_dec_float_100
#include<gmp.h>

using namespace boost::multiprecision;
using namespace std;

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
                cout<<"ssm="<<ssm_result<<endl;
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

bool is_prime_Solovay_Strassen(int1024_t num)//should fail in 91
{
    if(num%2==0)
    {   return false;} 
    else
    {
        int1024_t a=2;
        //lhs
        int1024_t lhs;
        if(num%8==1 || num%8==7)
        {   lhs=1;}
        else if(num%8==3 || num%8==5)
        {   lhs=-1;}
        //rhs
        int1024_t rhs=ssm(2,(num-1)/2,num);
        cout<<"\nlhs="<<lhs<<" rhs="<<rhs<<endl;
        if(rhs=num-1)
        {   rhs=-1;}
        if(lhs==rhs)
        {   return true;}
        else 
        {   return false;}
    }
}

int main()
{
    cout<<"\nEnter Number: ";
    int1024_t number;
    bool negative=false;
    cin>>number;
    if(number==2 || number==-2)
    {
        cout<<number<<" is prime\n\n";
        return 0;
    }
    if(number<0)
    {   negative=true;number=number*-1;}
    bool is_prime=true;
    point1:
    cout<<"\n1. fermat's little theorem test\n2. Miller Rabin test\n3. Solovay-Strassen's test\n4. Brut force\n\nOption: ";
    int option;
    cin>>option;
    if(option==1)
    {   is_prime=is_prime_FLT(number);}
    else if(option==2)
    {   
        is_prime=is_prime_FLT(number);
        if(is_prime)
        {   is_prime=is_prime_MRT(number);}
    }
    else if(option==3)
    {   is_prime=is_prime_Solovay_Strassen(number);}
    else if(option==4)
    {
        int1024_t root=sqrt(number);
        for(int1024_t a=3;a<=root;a+=2)
        {
            if(number%a==0)
            {   is_prime=false;break;}
        }
    }
    else
    {   cout<<"\nWrong option!!";goto point1;}
    if(negative)
    {   number=number*-1;}
    if(is_prime)
    {   cout<<number<<" is prime\n\n";}
    else
    {   cout<<number<<" is not prime\n\n";}

    return 0;
}
