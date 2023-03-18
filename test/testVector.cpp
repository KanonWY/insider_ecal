#include <thread>
#include <iostream>
#include <atomic>
#include <chrono>
#include <vector>

std::atomic<bool> a{true};
std::atomic<bool> b{true};

constexpr static long long length = 1024 * 1024 * 200;

void ThreadfuncA()
{
    std::cout << "thread: => " << std::this_thread::get_id() << std::endl;
    std::vector<char> store;
    store.resize(length);
    while (a)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void ThreadfuncB()
{
    std::cout << "thread: => " << std::this_thread::get_id() << std::endl;
    std::vector<char> store;
    store.reserve(length);
    while (b)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

class ThreadHelper
{
public:
    ThreadHelper();
    ~ThreadHelper();

private:
    std::thread m_thread;
};

int main2()
{
    std::vector<std::thread> m;
    m.reserve(10);
    for (int i = 0; i < 3; ++i)
    {
        m.push_back(std::thread(ThreadfuncA));
    }

    for (int i = 0; i < 3; ++i)
    {
        std::thread t(ThreadfuncB);
        m.push_back(std::move(t));
    }

    int inputNumber = 0;
    while (true)
    {
        std::cout << "I am main thread please input Number" << std::endl;
        std::cin >> inputNumber;
        switch (inputNumber)
        {
            case 1: a = false; break;
            case 2: b = false; break;
            case 3: goto END;
            default: break;
        }
    }

END:
    for (auto& item : m)
    {
        if (item.joinable())
        {
            item.join();
        }
    }
    std::cout << "exit" << std::endl;
    return 0;
}

constexpr static long long SIZE = 1024 * 1024 * 200;

class Observer
{
public:
    Observer() { m_buf.resize(SIZE); }

    void clear()
    {
        m_buf.clear();
        m_buf.shrink_to_fit();
        std::cout << "m_buf cap = " << m_buf.capacity() << std::endl;
    }

private:
    std::vector<char> m_buf;
};

class ObserverPool
{
public:
    ObserverPool(int size = 10)
    {
        m_size = size;
        for (int i = 0; i < size; ++i)
        {
            Observer ob;
            m_obs.push_back(std::move(ob));
        }
    }

    void clear_all()
    {
        for(int i = 0; i< m_size; ++i)
        {
            m_obs[i].clear();
        }
    }

private:
    size_t m_size = 0;
    std::vector<Observer> m_obs;
};



int main()
{
    int a = 200;
    ObserverPool pool;
    std::cout << "随机输入清理" << std::endl;
    std::cin >> a;
    pool.clear_all();
    std::cout << "清理后的结果" << std::endl;
    std::cin >> a;
    return 0;
}



int main3()
{
    std::vector<char> m1(SIZE, 0);
    std::vector<char> m2(SIZE, 0);
    std::vector<char> m3(SIZE, 0);
    std::vector<char> m4(SIZE, 0);
    std::vector<char> m5(SIZE, 0);
    std::vector<char> m6(SIZE, 0);

    int a = 0;

    while (true)
    {
        std::cout << "please input number" << std::endl;
        std::cin >> a;
        switch (a)
        {
            case 1:
                m1.clear();
                m1.shrink_to_fit();
                break;
            case 2:
                m2.clear();
                m2.shrink_to_fit();
                break;
            case 3:
                m3.clear();
                m3.shrink_to_fit();
                break;
            case 4:
                m4.clear();
                m4.shrink_to_fit();
                break;
            case 5:
                m5.clear();
                m5.shrink_to_fit();
                break;
            case 6:
                m6.clear();
                m6.shrink_to_fit();
                break;
            case 10: exit(0);
            default: break;
        }
    }

    return 0;
}