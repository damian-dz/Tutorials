#include <cstdlib>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;
    std::string processName;

    Timer(const std::string& name)
    {
        start = std::chrono::high_resolution_clock::now();
        processName = "\"" + name + "\" ";
    }

    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "The process " << processName << "took " << duration.count() << " s.\n";
    }
};

class MyMallocClass
{
public:
    MyMallocClass(size_t length)
    {
        m_pData = reinterpret_cast<double*>(std::malloc(length * sizeof(double)));
        m_length = m_pData == nullptr ? 0 : length;
    }

    ~MyMallocClass()
    {
        if (m_pData != nullptr) {
            std::free(m_pData);
        }
    }

    void operator=(const MyMallocClass& other)
    {
        if (m_pData != nullptr && other.m_pData != nullptr && m_length != other.m_length) {
            double* pTemp = reinterpret_cast<double*>(std::realloc(m_pData, other.m_length * sizeof(double)));
            if (pTemp != nullptr) {
                m_pData = pTemp;
                m_length = other.m_length;
            }
        }
        if (m_pData != nullptr && other.m_pData != nullptr) {
            std::memcpy(m_pData, other.m_pData, m_length * sizeof(double));
        }
    }

private:
    double* m_pData;
    size_t m_length;
};

class MyNewClass
{
public:
    MyNewClass(size_t length)
    {
        m_pData = new double[length];
        m_length = m_pData == nullptr ? 0 : length;
    }

    ~MyNewClass()
    {
        if (m_pData != nullptr) {
            delete[] m_pData;
        }
    }

    void operator=(const MyNewClass& other)
    {
        if (m_pData != nullptr && other.m_pData != nullptr && m_length != other.m_length) {
            double* pTemp = new double[other.m_length];
            if (pTemp != nullptr) {
                // std::memcpy(pTemp, m_pData, std::min(m_length, other.m_length) * sizeof(double));
                delete[] m_pData;
                m_pData = pTemp;
                m_length = other.m_length;
            }
        }
        if (m_pData != nullptr && other.m_pData != nullptr) {
            std::memcpy(m_pData, other.m_pData, m_length * sizeof(double));
        }
    }

private:
    double* m_pData;
    size_t m_length;
};

std::chrono::duration<float> testMalloc(const std::vector<int>& lengths)
{
    Timer t("malloc");
    size_t numInstances = lengths.size();
    for (size_t i = 0; i < numInstances; ++i) {
        MyMallocClass myMalloc(lengths[i]);
    }
    return std::chrono::high_resolution_clock::now() - t.start;
}

std::chrono::duration<float> testNew(const std::vector<int>& lengths)
{
    Timer t("new");
    size_t numInstances = lengths.size();
    for (size_t i = 0; i < numInstances; ++i) {
        MyNewClass myNew(lengths[i]);
    }
    return std::chrono::high_resolution_clock::now() - t.start;
}

std::chrono::duration<float> testRealloc(const std::vector<int>& lengths)
{
    Timer t("realloc");
    size_t numInstances = lengths.size();
    MyMallocClass myMalloc(10);
    for (size_t i = 0; i < numInstances; ++i) {
        MyMallocClass myOtherMalloc(lengths[i]);
        myMalloc = myOtherMalloc;
    }
    return std::chrono::high_resolution_clock::now() - t.start;
}

std::chrono::duration<float> testDeleteNew(const std::vector<int>& lengths)
{
    Timer t("delete-new");
    size_t numInstances = lengths.size();
    MyNewClass myNew(10);
    for (size_t i = 0; i < numInstances; ++i) {
        MyNewClass myOtherNew(lengths[i]);
        myNew = myOtherNew;
    }
    return std::chrono::high_resolution_clock::now() - t.start;
}

int main()
{
    constexpr int minLength = 1000;
    constexpr int maxLength = 100000;
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_int_distribution<int> dis(minLength, maxLength);
    constexpr int numInstances = 10000;

    std::vector<int> lengths;
    for (int i = 0; i < numInstances; ++i) {
        lengths.push_back(dis(gen));
    }

    constexpr int numRuns = 12;

    float sumT1 = 0;
    float sumT2 = 0;

    for (int i = 0; i < numRuns; ++i) {
        auto t1 = testRealloc(lengths);
        auto t2 = testDeleteNew(lengths);

        if (t1 / t2 < 1) {
            std::cout << "realloc " << (t2 / t1) << " x faster" << std::endl;
        } else if (t2 / t1 < 1) {
            std::cout << "delete-new " << (t1 / t2) << " x faster" << std::endl;
        }

        if (i > 2) {
            sumT1 += t1.count();
            sumT2 += t2.count();
        }
        std::cout << std::endl;
    }

    if (sumT1 / sumT2 < 1) {
        std::cout << "realloc " << (sumT2 / sumT1) << " x faster" << std::endl;
    } else if (sumT2 / sumT1 < 1) {
        std::cout << "delete-new " << (sumT1 / sumT2) << " x faster" << std::endl;
    }
}