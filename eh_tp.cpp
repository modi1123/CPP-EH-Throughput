#include <iostream>
#include <chrono>

// Creates a recursive stack with dtors to call and try/catch to be examined
// before finally catching it in main. Compile with cl /O2 /EHsc eh_tp.cpp

int count = 0;

struct A
{
    int i;
    A(int i) : i(i) {}
    ~A()
    {
        i++;
    }
};


__declspec(noinline)
void recursive(int level)
{
    A a1(count++);
    A a2(count++);
    A a3(count++);
    
    if (level)
    {
        try
        {
            return recursive(level - 1);
        }
        catch(float)
        {
            std::cout << "This is here so the runtime examines this catch, never entered" << std::endl;
        }
    }   
    else {
        throw 1;
    }
    
    return;
}

void main()
{
    using namespace std::chrono;
    volatile int catchcount = 0;
    auto t1 = high_resolution_clock::now();
    for (int i = 0; i < 50000; i++)
    {
        try
        {
            recursive(100);
        }
        catch(...)
        {
            catchcount++;
        }
    }
    
    auto t2 = high_resolution_clock::now();
    
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  std::cout << "Took " << time_span.count() << " seconds.";
  std::cout << std::endl;
}