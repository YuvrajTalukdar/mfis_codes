#include<fstream>
#include<boost/multiprecision/cpp_int.hpp>//for int1024_t
#include<boost/multiprecision/cpp_dec_float.hpp>//for cpp_dec_float_100
#include<string>
#include<random>

using namespace std;
using namespace boost::multiprecision;
typedef std::vector<char> char_array;

string encode_plaintext(string plain_text)//ok check
{
    string encoded_text;
    for(int a=0;a<plain_text.length();a++)
    {
        if(((int)plain_text.at(a))<10)
        {   encoded_text.append("00");}   
        else if(((int)plain_text.at(a))<100)
        {   encoded_text.append("0");}
        encoded_text.append(to_string((int)plain_text.at(a)));
    }
    return encoded_text;
}

string decode_encodedtext(string encoded_text)//ok check
{
    string decoded_text,temp="";
    int count=0,ascii_val;
    for(int a=0;a<encoded_text.length();a++)
    {
        temp.push_back((encoded_text.at(a)));
        count++;
        if(count==3)
        {
            ascii_val=stoi(temp);
            decoded_text.push_back((char(ascii_val)));
            count=0;
            temp="";
        }
    }
    return decoded_text;
}

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

int1024_t ssm(int1024_t x,int1024_t k,int1024_t m)//successive square multiplication
{
    //x^k mod m
    int1024_t start=k,rem,q=2;
    long int count=0;//,power=1;
    int1024_t mod_result=x%m;
    int1024_t mul_large=1;
    //vector<int1024_t> mod_result_vec;
    while(q!=0)
    {
        rem=start%2;
        q=start/2;
        start=q;
        if(count>0)
        {   mod_result=(mod_result*mod_result)%m;}
        if(rem==1)
        {
            //cout<<"\nmr="<<mod_result;

            mul_large=mul_large*mod_result;
            if(mul_large>m)
            {   mul_large=mul_large%m;}
        }
        count++;
    }
    int1024_t result=mul_large%m;
    return result;
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

bool is_prime_FLT(int1024_t number)//primality test using fermat's little theorem
{
    //cout<<"\n\nnumber=="<<number;
    int x=2;
    int1024_t mod=ssm(x,number-1,number);
    if(mod==1)
    {   return true;}
    else 
    {   return false;}
}

bool is_prime_MRT(int1024_t num)
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
                //cout<<"\nssm="<<ssm_result;
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

char_array charset()
{
    return char_array( 
    {'0','1','2','3','4',
    '5','6','7','8','9'
    });
};    
string random_string( size_t length, std::function<char(void)> rand_char )
{
    std::string str(length,0);
    std::generate_n( str.begin(), length, rand_char );
    return str;
}
int1024_t get_large_prime_number(int length,auto randchar)
{
    bool prime_found=false;
    int1024_t prime;
    while(!prime_found)
    {
        //prime=dice();
        prime=string_to_number(random_string(length,randchar));
        //prime_found=is_prime_FLT(prime);
        prime_found=is_prime_FLT(prime);
        if(prime_found)
        {   prime_found=is_prime_MRT(prime);}
    }
    return prime;
}

int main()
{
    int option;
    point1:
    cout<<"1. Generate key\n2. ElGamal Encryption\n3. ElGamal Decryption\nSelect: ";
    cin>>option;
    if(option==1)//ok check
    {
        //calculating the essential ElGamal variables
        int1024_t e1,e2,p,d;
        cout<<"\nKey length: ";
        int key_length;
        cin>>key_length;
        const auto ch_set = charset();  
        std::default_random_engine rng(std::random_device{}());
        std::uniform_int_distribution<> dist(0, ch_set.size()-1);
        auto randchar = [ ch_set,&dist,&rng ](){return ch_set[ dist(rng) ];};
        p=get_large_prime_number(key_length,randchar);
        e1=string_to_number(random_string(key_length,randchar));
        //d=get_large_prime_number(key_length);
        d=string_to_number(random_string(key_length,randchar));
        e2=ssm(e1,d,p);

        cout<<"\nPublic key:-";
        cout<<"\ne1= "<<e1;
        cout<<"\ne2= "<<e2;
        cout<<"\np= "<<p;
        cout<<"\n\nPrivate key:-";
        cout<<"\nd= "<<d;
        cout<<"\np= "<<p<<endl;
    }
    else if(option==2)//ok check
    {
        cout<<"\nPlain Text: ";
        string plain_text,encoded_text;
        getline(cin>>ws,plain_text);
        encoded_text=encode_plaintext(plain_text);
        cout<<"\nencoded text: "<<encoded_text;
        
        //rsa key generation 
        int1024_t e1,e2,p;
        //get public RSA key
        cout<<"\nEnter public key:-";
        cout<<"\ne1: ";
        cin>>e1;
        cout<<"e2: ";
        cin>>e2;
        cout<<"p: ";
        cin>>p;
        int r_size;
        cout<<"set r size: ";
        cin>>r_size;

        const auto ch_set = charset();  
        std::default_random_engine rng(std::random_device{}());
        std::uniform_int_distribution<> dist(0, ch_set.size()-1);
        auto randchar = [ ch_set,&dist,&rng ](){return ch_set[ dist(rng) ];};
        int1024_t r=string_to_number(random_string(r_size,randchar));
        cout<<"\nr: "<<r<<"\n\n";
        //spliting the encoded text in shunks and encrypting it
        int count=0,msg_shunk_size=p.convert_to<string>().length()-1;
        //vector<int1024_t> msg_shunk_vec;
        vector<int1024_t> encrypted_msg_chunk_vec_c2;
        int1024_t c1=ssm(e1,r,p);
        string shunk;
        int1024_t ssm_result;
        for(int a=0;a<encoded_text.length();a++)
        {
            shunk.push_back(encoded_text.at(a));
            count++;
            if(count==msg_shunk_size)
            {
                count=0;
                //msg_shunk_vec.push_back(string_to_number(shunk));
                ssm_result=ssm(e2,r,p);
                ssm_result=ssm_result*string_to_number(shunk);
                ssm_result=ssm_result%p;
                //cout<<"\nresult="<<ssm_result;
                encrypted_msg_chunk_vec_c2.push_back(ssm_result);
                shunk.clear();
            }
        }
        if(shunk.length()>0)
        {   
            //msg_shunk_vec.push_back(string_to_number(shunk));
            ssm_result=ssm(e2,r,p);
            ssm_result=ssm_result*string_to_number(shunk);
            ssm_result=ssm_result%p;
            //cout<<"\nresult="<<ssm_result;
            encrypted_msg_chunk_vec_c2.push_back(ssm_result);
        }
        ofstream file1("cipher_text_elgamal.txt",ios::out);
        file1<<"Cipher text: \n";
        cout<<"\nCipher text: \n";
        for(int a=0;a<encrypted_msg_chunk_vec_c2.size();a++)
        {
            file1<<c1<<","<<encrypted_msg_chunk_vec_c2.at(a)<<"\n";
            cout<<c1<<", "<<encrypted_msg_chunk_vec_c2.at(a)<<endl;
        }
        file1<<"end\n";
        cout<<"\n";
        file1.close();
    }
    else if(option==3)
    {
        cout<<"\nReading cipher_text.txt file...";
        ifstream file1("cipher_text_elgamal.txt",ios::in);
        string line;
        getline(file1,line);
        //vector<int1024_t> encrypted_msg_chunk_vec_c1;
        int1024_t c1=0;
        vector<int1024_t> encrypted_msg_chunk_vec_c2;
        int comma_position;
        string c1_str,c2_str;
        while(!file1.eof())
        {
            getline(file1,line);
            if(strcasestr(line.c_str(),"end"))
            {   break;}
            cout<<"\n"<<line;
            for(int a=0;a<line.length();a++)
            {
                if(line.at(a)==',')
                {   comma_position=a;break;}
            }
            c1_str="";c2_str="";
            c1_str.append(line.begin(),line.begin()+comma_position);
            c2_str.append(line.begin()+comma_position+1,line.end());
            //cout<<"\nc1= "<<c1_str<<" c2="<<c2_str;
            if(c1==0)
            {   c1=string_to_number(c1_str);}
            encrypted_msg_chunk_vec_c2.push_back(string_to_number(c2_str));
        }
        file1.close();
        
        int1024_t d,p;
        cout<<"\n\nEnter private key:-";
        cout<<"\nd: ";
        cin>>d;
        cout<<"p: ";
        cin>>p;
        vector<string> decrypted_msg_chunk_vec;
        string temp;
        int shunk_size=0;
        int1024_t c_power_d=ssm(c1,d,p);
        int1024_t c1_power_d_inv=inverse_euclid_my_version(c_power_d,p);
        for(int a=0;a<encrypted_msg_chunk_vec_c2.size();a++)
        {
            //cout<<"\nnum= "<<encrypted_msg_chunk_vec.at(a);
            temp=((c1_power_d_inv*encrypted_msg_chunk_vec_c2.at(a))%p).convert_to<string>();
            if(temp.length()>shunk_size)
            {   shunk_size=temp.length();}
            decrypted_msg_chunk_vec.push_back(temp);
            cout<<"\ndecrypted_shunk"<<a+1<<": "<<temp;
        }
        string encoded_text;
        int mod_shunk=shunk_size%3;
        for(int a=0;a<decrypted_msg_chunk_vec.size();a++)
        {
            if(shunk_size>decrypted_msg_chunk_vec.at(a).length())
            {
                if(a==decrypted_msg_chunk_vec.size()-1)//last shunk
                {
                    int remaining0=(encoded_text.length()+decrypted_msg_chunk_vec.at(a).length())%3;
                    if(remaining0!=0)
                    {   remaining0=3-remaining0;}
                    for(int b=0;b<remaining0;b++)
                    {   encoded_text.append("0");}
                }
                else//rest shunks
                {
                    int current_mod=decrypted_msg_chunk_vec.at(a).length()%3;
                    if(mod_shunk!=current_mod)
                    {
                        int remaining0;
                        if(mod_shunk<current_mod)
                        {   remaining0=(3-current_mod)+mod_shunk;}
                        else if(mod_shunk>current_mod)
                        {   remaining0=mod_shunk-current_mod;}
                        for(int b=0;b<remaining0;b++)
                        {   encoded_text.append("0");}
                    }   
                }
            }
            encoded_text.append(decrypted_msg_chunk_vec.at(a));
        }
        int mod=encoded_text.length()%3;
        if(mod!=0)
        {   
            for(int a=0;a<3-mod;a++)
            {   encoded_text.insert(encoded_text.begin(),'0');}
        }
        cout<<"\n\nEncoded Text: "<<encoded_text;
        cout<<"\nPlain Text: "<<decode_encodedtext(encoded_text)<<"\n\n";
    }
    else
    {   goto point1;}
}
