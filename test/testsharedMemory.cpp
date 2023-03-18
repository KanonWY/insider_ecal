#include <cstdlib>
#include <iostream>
#include <random>
#include <stack>
#include <string>
#include <time.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <tuple>

std::stack<std::tuple<std::string, int, void*>> m_stack;

constexpr static int length = 1024 * 1024 * 200;

struct Data_test
{
    char data[length];
};

void create_shared_memory()
{
    srand(time(0));
    std::string fileName = "/shm";
    fileName.append(std::to_string(rand()));
    int shm_fd = shm_open(fileName.c_str(), O_RDWR | O_CREAT | O_EXCL, 0777);
    if ((shm_fd == -1) && (errno == EEXIST))
    {
        shm_fd = ::shm_open(fileName.c_str(), O_RDWR, 0777);
    }
    ::ftruncate(shm_fd, sizeof(Data_test));
    void* address = ::mmap(NULL, sizeof(Data_test), PROT_READ | PROT_WRITE,
                           MAP_SHARED, shm_fd, 0);
    memset(address, 0, sizeof(Data_test));
    // std::tuple<std::string, int, void*> tmp(fileName, shm_fd, address);
    std::cout << "mmap " << fileName.c_str() << std::endl;
    m_stack.emplace(fileName, shm_fd, address);
}

void UnInitMMap(int shm_fd, void* address, size_t size)
{
    munmap(address, size);
    close(shm_fd);
}

void pop_top()
{
    if (!m_stack.empty())
    {
        auto item = m_stack.top();
        m_stack.pop();
        auto fileName = std::get<0>(item);
        auto shm_fd = std::get<1>(item);
        auto address = std::get<2>(item);
        UnInitMMap(shm_fd, address, sizeof(Data_test));
        std::cout << "munmap " << fileName.c_str() << std::endl;
    }
}


int main()
{
    int inputNumber = 0;

    while (true)
    {
        std::cout << "please input number: " << std::endl;
        std::cin >> inputNumber;
        switch (inputNumber)
        {
            case 1: create_shared_memory(); break;
            case 2: pop_top(); break;
            case 3: std::cout << "退出" << std::endl; exit(0); 
            default: break;
        }
    }
    return 0;
}
