#include<iostream>
#include<vector>
#include<chrono>

using namespace std;
using namespace std::chrono;

int random_num_generator(int max)
{
    return rand()%max;
}

vector<int> selection_sort(vector<int> int_list)//best=n^2 worst=n^2
{
    int temp;
    for(int a=0;a<int_list.size();a++)
    {
        for(int b=0;b<int_list.size();b++)
        {
            if(int_list[a]<int_list[b])
            {
                temp=int_list[a];
                int_list[a]=int_list[b];
                int_list[b]=temp;
            }
        }
    }
    return int_list;
}

vector<int> bubble_sort(vector<int> int_list)//best=n worst=n^2
{
    int temp;
    for(int a=0;a<int_list.size()-1;a++)
    {    
        for (int b=0;b<int_list.size()-a-1;b++)
        {
            if(int_list[b]>int_list[b+1])
            {
                temp=int_list[b];
                int_list[b]=int_list[b+1];
                int_list[b+1]=temp;
            }
        }
    }
    return int_list;
}

vector<int> insertion_sort(vector<int> int_list)//best=n worst=n^2
{
    int temp,key,b;
    for(int a=0;a<int_list.size();a++)
    {
        key=int_list[a];
        b=a-1;
        while(b>=0 && int_list[b]>key)
        {
            int_list[b+1]=int_list[b];
            b=b-1;
        }
        int_list[b+1]=key;
    }
    return int_list;
}

int main()
{
    vector<int> int_list;
    int list_size;
    cout<<"\nEnter the number of integers you want to sort: ";
    cin>>list_size;
    for(int a=0;a<list_size;a++)
    {   
        int num=random_num_generator(list_size);
        cout<<num<<", ";
        int_list.push_back(num);
    }
    int option=-1;
    while(true)
    {
        cout<<"\n\n1. Selection Sort.\n2. Bubble sort.\n3. Insertion Sort";
        cout<<"\nSelect Sorting algorithm: ";
        cin>>option;
        if(option==1||option==2||option==3)
        {   break;}
        else
        {
            cout<<"\nWrong option!!";
        }
    }
    auto start = high_resolution_clock::now();
    vector<int> sorted_list;
    if(option==1)
    {   sorted_list=selection_sort(int_list);}
    else if(option==2)
    {   sorted_list=bubble_sort(int_list);}
    else if(option==3)
    {   sorted_list=insertion_sort(int_list);}
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    cout<<"\n\nduration= "<<duration.count()<<" microseconds";

    cout<<"\n\n\nSorted list:-";
    for(int a=0;a<sorted_list.size();a++)
    {
        cout<<sorted_list.at(a)<<", ";
    }
}
