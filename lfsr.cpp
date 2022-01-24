#include<iostream>

using namespace std;

bool check_equal_state(int* begin,int* end,int size)
{
    bool equal=true;
    for(int a=0;a<size;a++)
    {
        if(*(begin+a)!=*(end+a))
        {   
            equal=false;
            break;
        }
    }
    return equal;
}

int main()
{
    int no_of_registers;
    cout<<"Enter the no of registers: ";
    cin>>no_of_registers;
    int register_array[no_of_registers],start[no_of_registers];
    for(int a=0;a<no_of_registers;a++)
    {
        cout<<"\nx^"<<a<<": ";
        cin>>register_array[a];
        start[a]=register_array[a];
    }
    int temp,a=0;
    do
    {
        //printing the iteration
        cout<<"\n"<<a<<". ";
        for(int b=no_of_registers-1;b>=0;b--)
        {
            cout<<register_array[b]<<",";
        }
        cout<<" -->"<<register_array[0];
        for(int b=0;b<no_of_registers;b++)
        {
            if(b==0)
            {
                temp=register_array[b];
            }
            else
            {
                register_array[b-1]=register_array[b];
            }
        }
        register_array[no_of_registers-1]=register_array[no_of_registers-1]^temp;
        a++;
    }
    while(!check_equal_state(start,register_array,no_of_registers));
    //printing the iteration
    cout<<"\n"<<a<<". ";
    for(int b=no_of_registers-1;b>=0;b--)
    {
        cout<<register_array[b]<<",";
    }
    cout<<" -->"<<register_array[0];
    cout<<"\nPeriod="<<a;

    return 0;
} 
