#include <iostream>
#include <vector>


constexpr static long long size = 1600 * 1024 * 1024;


int main()
{
    std::vector<char> m_vec(size, '0');
    std::cout << "m_vec size = " << m_vec.size() << std::endl;
    std::cout << "m_vec cap = " << m_vec.capacity() << std::endl;
    m_vec.clear();
    std::cout << "m_vec size = " << m_vec.size() << std::endl;
    std::cout << "m_vec cap = " << m_vec.capacity() << std::endl;
    return 0;
}

