//
//  main.cpp
//  DelayQueue
//  Sample usage of DelayQueue
//
//  Created by Takuro Tamura on 2022/06/08.
//

#include <chrono>
#include <iostream>
#include <string>
#include "DelayQueue.hpp"

using std::chrono::system_clock;

void elapsed(const system_clock::time_point& start,
             const system_clock::time_point& current)
{
    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    auto current_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current.time_since_epoch()).count();
    std::cout << "elapsed " << current_ms - start_ms << " msec." << std::endl;
}

int main(int argc, const char * argv[]) {
/// 1. Sample of basic use
    std::cout << "######## Sample 1 ########" << std::endl;
    delay::DelayQueue<int> q1;
    system_clock::time_point start = std::chrono::system_clock::now();
    q1.offer(1, 6000);
    q1.offer(2, 2000);
    q1.offer(3, 4000);
    
    std::cout << q1.take() << std::endl;
    system_clock::time_point current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    std::cout << q1.take() << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    std::cout << q1.take() << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    std::cout << std::endl;
    
/// 2. Sample of using seconds as a time unit.
    std::cout << "######## Sample 2 ########" << std::endl;
    delay::DelayQueue<int> q2;
    start = std::chrono::system_clock::now();
    q2.offer(1, 6, delay::TimeUnit::Seconds);
    q2.offer(2, 2, delay::TimeUnit::Seconds);
    q2.offer(3, 4, delay::TimeUnit::Seconds);
    
    std::cout << q2.take() << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);

    std::cout << q2.take() << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    std::cout << q2.take() << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    std::cout << std::endl;
    
/// 3. Sample of putting pointers into DelayQueue.
    std::cout << "######## Sample 3 ########" << std::endl;
    delay::DelayQueue<int*> q3;
    int* a = new int(1);
    int* b = new int(2);
    int* c = new int(3);
    start = std::chrono::system_clock::now();
    q3.offer(a, 6000);
    q3.offer(b, 2000);
    q3.offer(c, 4000);
    
    int* p = q3.take();
    std::cout << *p << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    p = q3.take();
    std::cout << *p << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    p = q3.take();
    std::cout << *p << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    delete a; delete b; delete c;
    
    std::cout << std::endl;
    
/// 4. Sample of putting classes into DelayQueue.
    std::cout << "######## Sample 4 ########" << std::endl;
    delay::DelayQueue<std::string> q4;
    std::string str1("aaa");
    std::string str2("bbb");
    std::string str3("ccc");
    start = std::chrono::system_clock::now();
    q4.offer(std::move(str1), 6000);
    q4.offer(std::move(str2), 2000);
    q4.offer(std::move(str3), 4000);
    
    std::cout << q4.take() << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    std::cout << q4.take() << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    std::cout << q4.take() << std::endl;
    current = std::chrono::system_clock::now();
    elapsed(start, current);
    
    std::cout << std::endl;
    
    return 0;
}
