#include<iostream>
#include<string>
#include<chrono>
#include<set>
#include<vector>
#include<math.h>
#include<thread>

using namespace std;
using namespace std::chrono;

vector<int> ans_from_thread;

int random_num_generator(int max)
{
    return rand()%(max*2);
}

int gcd(int a,int b)
{
    int temp,big,small;
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
    while(small>0)
    {
        temp=small;
        small=big%small;
        big=temp;
    }
    return big;
}

int find_gcd(vector<int> coefficient_list)
{
    int gcd_result=coefficient_list[0];
    for(int a=1;a<coefficient_list.size();a++)
    {   gcd_result=gcd(gcd_result,coefficient_list[a]);}
    ans_from_thread.push_back(gcd_result);
    return gcd_result;
}

int main()
{
    int count,num,thread_count;
    cout<<"Enter the no of elements: ";
    cin>>count;
    set<int> num_set;
    cout<<endl;
    while(num_set.size()!=count)
    {
        num=random_num_generator(count)*2;
        num_set.insert(num);
        //cout<<num<<", ";
    }
    thread_count=std::thread::hardware_concurrency();
    cout<<"\nRecommended no of threads to use: "<<thread_count-1;
    cout<<"\nSet the no of thread: ";
    cin>>thread_count;
    //spliting the data
    vector<vector<int>> splited_data;
    set<int>::iterator itr;
    itr=num_set.begin();
    int data_per_thread=count/thread_count,counter=0;
    for(int a=0;a<thread_count;a++)
    {
        vector<int> num_vec;
        counter=0;
        for(itr;itr!=num_set.end();itr++)
        {   
            num_vec.push_back(*itr);
            counter++;
            if(counter==data_per_thread)
            {   break;}
        }
        splited_data.push_back(num_vec);
    }
    //starting the threads
    auto start = high_resolution_clock::now();
    vector<thread*> thread_vec;
    for(int a=0;a<thread_count;a++)
    {
        thread* worker_thread=new thread(find_gcd,splited_data.at(a));
        thread_vec.push_back(worker_thread);
    }
    for(int a=0;a<thread_count;a++)
    {
        thread_vec.at(a)->join();
        delete thread_vec.at(a);
    }
    thread_vec.clear();
    cout<<"\n\nGDC = "<<find_gcd(ans_from_thread);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    cout<<"\n\nduration= "<<duration.count()<<" microseconds";
}
 
