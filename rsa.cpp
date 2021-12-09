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
        if(b0==1 || b0==-1)
        {   return true;}
        else
        {   
            int1024_t ssm_result;
            do
            {
                ssm_result=ssm(b0,2,num);
                b0=ssm_result;
                if(ssm_result!=num-1 || ssm_result!=num+1)
                {   break;}
            }
            while(true);
            if(ssm_result==num-1)
            {   return true;}
            else if(ssm_result==num+1)
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
int1024_t get_large_prime_number(int length)
{
    /*unsigned long int min=100000,max=100000000;
    typedef boost::mt19937 RNGType;
    RNGType rng;
    boost::uniform_int<> one_to_six(min,max);
    boost::variate_generator< RNGType, boost::uniform_int<> >dice(rng, one_to_six);
    */
    const auto ch_set = charset();  
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, ch_set.size()-1);
    auto randchar = [ ch_set,&dist,&rng ](){return ch_set[ dist(rng) ];};   
    //std::cout<<random_string(length,randchar)<<std::endl;

    bool prime_found=false;
    int1024_t prime;
    while(!prime_found)
    {
        //prime=dice();
        prime=string_to_number(random_string(length,randchar));
        //prime_found=is_prime_FLT(prime);
        prime_found=is_prime_MRT(prime);
    }
    return prime;
}

int main()
{
    int option;
    point1:
    cout<<"1. Generate key\n2. RSA Encryption\n3. RSA Decryption\nSelect: ";
    cin>>option;
    if(option==1)//ok check
    {
        //calculating the essential RSA variables
        int1024_t p=47,q=37,n,phi_n,d,e=65537;
        cout<<"\nKey length: ";
        int key_length;
        cin>>key_length;
        p=get_large_prime_number(key_length);
        q=get_large_prime_number(key_length);
        cout<<"\np="<<p<<" q="<<q;
        n=p*q;
        phi_n=(p-1)*(q-1);
        d=inverse_euclid_my_version(e,phi_n);
        cout<<"\nPublic key:-";
        cout<<"\nn= "<<n;
        cout<<"\ne= "<<e;
        cout<<"\n\nPrivate key:-";
        cout<<"\nd= "<<d;
        cout<<"\nn= "<<n<<"\n\n";
    }
    else if(option==2)//ok check
    {
        cout<<"\nPlain Text: ";
        string plain_text,encoded_text;
        getline(cin>>ws,plain_text);
        encoded_text=encode_plaintext(plain_text);
        cout<<"\nencoded text: "<<encoded_text;
        
        //rsa key generation 
        int1024_t n,d,e=65537;
        //get public RSA key
        cout<<"\nEnter public key:-";
        cout<<"\nn: ";
        cin>>n;
        cout<<"e: ";
        cin>>e;
        //spliting the encoded text in shunks and encrypting it
        int count=0,msg_shunk_size=n.convert_to<string>().length()-1;
        //vector<int1024_t> msg_shunk_vec;
        vector<int1024_t> encrypted_msg_chunk_vec;
        string shunk;
        for(int a=0;a<encoded_text.length();a++)
        {
            shunk.push_back(encoded_text.at(a));
            count++;
            if(count==msg_shunk_size)
            {
                count=0;
                //msg_shunk_vec.push_back(string_to_number(shunk));
                encrypted_msg_chunk_vec.push_back(ssm(string_to_number(shunk),e,n));
                shunk.clear();
            }
        }
        if(shunk.length()>0)
        {   
            //msg_shunk_vec.push_back(string_to_number(shunk));
            encrypted_msg_chunk_vec.push_back(ssm(string_to_number(shunk),e,n));
        }
        ofstream file1("cipher_text.txt",ios::out);
        file1<<"Cipher text: \n";
        cout<<"\n\nCipher text: \n";
        for(int a=0;a<encrypted_msg_chunk_vec.size();a++)
        {
            file1<<encrypted_msg_chunk_vec.at(a)<<"\n";
            cout<<encrypted_msg_chunk_vec.at(a)<<endl;
        }
        file1<<"end\n";
        cout<<"\n";
        file1.close();
    }
    else if(option==3)
    {
        cout<<"\nReading cipher_text.txt file...";
        ifstream file1("cipher_text.txt",ios::in);
        string line;
        getline(file1,line);
        vector<int1024_t> encrypted_msg_chunk_vec;
        while(!file1.eof())
        {
            getline(file1,line);
            if(strcasestr(line.c_str(),"end"))
            {   break;}
            //cout<<"\n"<<line;
            encrypted_msg_chunk_vec.push_back(string_to_number(line));
        }
        file1.close();
        int1024_t d,n;
        cout<<"\nEnter private key:-";
        cout<<"\nd: ";
        cin>>d;
        cout<<"n: ";
        cin>>n;
        vector<string> decrypted_msg_chunk_vec;
        string temp;
        int shunk_size=0;
        for(int a=0;a<encrypted_msg_chunk_vec.size();a++)
        {
            //cout<<"\nnum= "<<encrypted_msg_chunk_vec.at(a);
            temp=ssm(encrypted_msg_chunk_vec.at(a),d,n).convert_to<string>();
            if(temp.length()>shunk_size)
            {   shunk_size=temp.length();}
            decrypted_msg_chunk_vec.push_back(temp);
            //cout<<"\nd="<<temp;
        }
        string encoded_text;
        for(int a=0;a<decrypted_msg_chunk_vec.size();a++)
        {
            if(a!=decrypted_msg_chunk_vec.size()-1)
            {
                if(decrypted_msg_chunk_vec.at(a).length()<shunk_size)
                {
                    for(int b=0;b<shunk_size-decrypted_msg_chunk_vec.at(a).length();b++)
                    {   encoded_text.append("0");}
                }
            }
            else
            {
                int remaining_char=(encoded_text.size())%3;
                remaining_char+=decrypted_msg_chunk_vec.at(a).size()%3;
                //cout<<"\net="<<encoded_text.size()<<" dc="<<decrypted_msg_chunk_vec.at(a).size()<<remaining_char;
                //cout<<"\ntext="<<decrypted_msg_chunk_vec.at(a);
                for(int b=0;b<remaining_char;b++)
                {   encoded_text.append("0");}
            }
            encoded_text.append(decrypted_msg_chunk_vec.at(a));
        }
        cout<<"\nEncoded Text: "<<encoded_text;
        cout<<"\nPlain Text: "<<decode_encodedtext(encoded_text)<<"\n\n";
    }
    else
    {   goto point1;}
}
