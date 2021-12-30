//Pholig Hellman algorithm
#include<iostream>
#include<boost/multiprecision/cpp_int.hpp>//for int1024_t
#include<boost/multiprecision/cpp_dec_float.hpp>//for cpp_dec_float_100
#include<gmp.h>
#include<string>

using namespace std;
using namespace boost::multiprecision;

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

//successive square multiplication
int1024_t ssm(int1024_t x,int1024_t k,int1024_t m)
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

//prime checker
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

vector<int1024_t> big_list,small_list;
int1024_t gcd(int1024_t a,int1024_t b)
{
    int1024_t temp,big,small;
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
        //cout<<"\ns="<<small<<" b="<<big;
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
    int1024_t val;
    int1024_t data_type;
    //1= add,2=sub,3=(,4=),5=val
};
vector<data> prep_data(int1024_t a,int1024_t b)
{
    vector<data> data_tape;
    data d1;
    d1.data_type=5;
    d1.val=a;
    data_tape.push_back(d1);
    
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
int1024_t calc_u(int1024_t m,int1024_t a)
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
                //print_tape(data_tape);
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
    //print_tape(data_tape);
    //get a0 m0 count and sign
    int1024_t a0=0,m0=0;
    vector<int1024_t> mul_stack,bracket_stack;
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
            int1024_t a0_temp=1,m0_temp=1;
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
    int1024_t u0=a0%m;
    if(u0<0)
    {   u0+=m;}
    //cout<<"\n\nm="<<m0<<" a="<<a0<<" u0="<<u0;
    
    return u0;
}
int1024_t eq_sol(int1024_t a,int1024_t c,int1024_t m)
{
    //cout<<"a*x=c*mod(m)";
    //bool sol_avail;
    int1024_t g=gcd(a,m);
    if(c%g==0)
    {   
        int1024_t u0=calc_u(m,a);
        int1024_t x0=u0*c/g;
        //cout<<"\n\nSolutions:-";
        for(int k=0;k<g;k++)
        {
            x0=x0+m*k/g;
            //cout<<"\nx"<<k<<"= "<<x0;
        }
        return x0;
    }
    else
    {   return 0;}
}

struct crt_equation
{   int1024_t a=0,m;};
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
int1024_t crt(vector<crt_equation> crt_eq_vec)
{
    int1024_t M_arr[crt_eq_vec.size()],M_inv[crt_eq_vec.size()],M=1;
    for(int a=0;a<crt_eq_vec.size();a++)
    {
        M=crt_eq_vec.at(a).m*M;
        cout<<"\neq"<<a<<": X= "<<crt_eq_vec.at(a).a<<" mod "<<crt_eq_vec.at(a).m;
    }
    //cout<<"\n\nM= "<<M<<endl;
    for(int a=0;a<crt_eq_vec.size();a++)
    {
        M_arr[a]=M/crt_eq_vec[a].m;
        //cout<<", M"<<a+1<<"="<<M_arr[a];
        M_inv[a]=inverse_euclid_my_version(M_arr[a],crt_eq_vec.at(a).m);
    }
    /*cout<<endl;
    for(int a=0;a<crt_eq_vec.size();a++)
    {
        cout<<", M_inv"<<a+1<<"="<<M_inv[a];
    }*/
    int1024_t x=0;
    for(int a=0;a<crt_eq_vec.size();a++)
    {
        x=x+crt_eq_vec[a].a*M_arr[a]*M_inv[a];
    }
    return x%M;
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

/*int1024_t calc_beta(int1024_t old_beta,int1024_t alpha,int1024_t x,int1024_t p)
{
    if(x<0)
    {
        int1024_t next_beta=eq_sol(pow(alpha,(-1*x).convert_to<long long int>()),old_beta,p)%p;
        cout<<"\n check1=="<<next_beta<<" pow="<<pow(alpha,(-1*x).convert_to<long long int>());
        return next_beta;
    }
    else
    {
        int1024_t next_beta=(pow(alpha,x.convert_to<long long int>())*old_beta)%p;
        cout<<"\n check2=="<<next_beta;
        return next_beta;
    }
}*/
int1024_t calc_beta2(int1024_t prev_beta,int1024_t alpha,int1024_t prev_x,int1024_t prev_q,int1024_t p)
{
    if(prev_x<0)
    {
        int1024_t ssm_result=ssm(alpha,-1*prev_x*prev_q,p);
        int1024_t next_beta=eq_sol(ssm_result,prev_beta,p)%p;
        //cout<<"\n check1=="<<next_beta;
        return next_beta;
    }   
    else
    {
        int1024_t ssm_result=ssm(alpha,(prev_x*prev_q),p);
        int1024_t next_beta=(ssm_result*prev_beta)%p;
        //cout<<"\n check2=="<<next_beta;
        return next_beta;
    } 
}

int main()
{
    int1024_t alpha,beta,p;
    cout<<"beta=(alpha^x) mod p";
    cout<<"\nbeta: ";
    cin>>beta;
    cout<<"alpha: ";
    cin>>alpha;
    cout<<"p: ";
    cin>>p;
    vector<int1024_t> x_vec;
    int1024_t next_beta,q;
    vector<vector<int1024_t>> pf_and_count_vec=get_prime_factors(p-1);
    vector<crt_equation> crt_eq_vec;
    for(int a=0;a<pf_and_count_vec.size();a++)
    {
        cout<<"\nq="<<pf_and_count_vec[a][0]<<" count="<<pf_and_count_vec[a][1];
        x_vec.clear();
        int1024_t rhs=ssm(alpha,(p-1)/pf_and_count_vec.at(a)[0],p);
        if(p-rhs==1)
        {   rhs=-1;}
        for(int b=0;b<pf_and_count_vec[a][1];b++)
        {
            //get the beta
            q=pow(pf_and_count_vec.at(a)[0],b+1);
            //cout<<"\nx: "<<b;
            if(b>0)
            {
                //next_beta=calc_beta(next_beta,alpha,-1*x_vec.at(b-1),p);
                next_beta=calc_beta2(next_beta,alpha,-1*x_vec.at(b-1),q/(pf_and_count_vec[a][0]*pf_and_count_vec[a][0]),p);
            }
            else
            {   next_beta=beta;}
            //cout<<" beta"<<b<<": "<<next_beta;
               
            int1024_t lhs=ssm(next_beta,(p-1)/q,p);
            if(p-lhs==1)
            {   lhs=-1;}
            //cout<<" lhs="<<lhs;
            if(lhs==rhs)
            {   x_vec.push_back(1);}
            else if(lhs==1)
            {   x_vec.push_back(0);}
            else//brut borce is required to find x
            {
                //cout<<"\nlhs="<<lhs<<" rhs="<<rhs<<" q="<<q<<" p="<<p;
                int1024_t new_rhs=rhs,x=2;
                while(lhs!=new_rhs)
                {
                    new_rhs=ssm(rhs,x,p);
                    x++;
                    if(new_rhs==p-1)
                    {   new_rhs=-1;}
                    //cout<<"\nrhs="<<new_rhs;
                }
                x_vec.push_back(x-1);
            }
            cout<<"\nq"<<b<<": "<<q<<" x"<<b<<": "<<x_vec.at(x_vec.size()-1);
        }
        crt_equation crt_eq;
        crt_eq.m=pow(pf_and_count_vec.at(a)[0],pf_and_count_vec.at(a)[1].convert_to<long long int>());
        for(int b=0;b<x_vec.size();b++)
        {
            crt_eq.a=crt_eq.a+pow(pf_and_count_vec.at(a)[0],b)*x_vec.at(b);
        }
        cout<<"\nA="<<crt_eq.a<<", M="<<crt_eq.m<<"\n";
        crt_eq_vec.push_back(crt_eq);
    }
    int1024_t sol=crt(crt_eq_vec);
    cout<<"\n\nX: "<<sol<<endl;

    return 0;
}