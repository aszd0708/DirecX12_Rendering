// TestProject_Console.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <chrono>
#include <cassert>
#include <memory>
#include <vector>
#include "CpuMemoryPool.h"

using namespace std;

class TestObject : public IMemoryBlockHanlde
{
public:
    TestObject(int index) : _index(index) {};
    ~TestObject() {};
    INT64 _index;
};

UINT MAX_COUNT = 100000;

void CheckSTL()
{
    std::cout << "Using STL\n\n";
    vector<TestObject*> vec;
    {
        std::cout << "Allocate\n";
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < MAX_COUNT; ++i)
        {
            TestObject* obj = new TestObject(i);
            vec.emplace_back(obj);
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Done!\n";
        std::cout << "Total Time : " << elapsed << "\n\n";
    }

    {
        std::cout << "Release\n";
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < MAX_COUNT; ++i)
        {
            TestObject* obj = vec[i];
            delete obj;
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Done!\n";
        std::cout << "Total Time : " << elapsed << "\n\n";

        start = std::chrono::steady_clock::now();
        vec.shrink_to_fit();

        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Pool Relese Done!\n";
        std::cout << "Total Time : " << elapsed << "\n";
    }
}

void CheckSTL_Random()
{
    std::cout << "Using STL Random\n\n";
    vector<TestObject*> vec;
    {
        std::cout << "Allocate\n";
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < MAX_COUNT; ++i)
        {
            TestObject* obj = new TestObject(i);
            vec.emplace_back(obj);
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Done!\n";
        std::cout << "Total Time : " << elapsed << "\n\n";
    }

    {
        std::cout << "Release\n";
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < MAX_COUNT; ++i)
        {
            int index = rand() % vec.size();
            TestObject* obj = vec[index];
            delete obj;
            vec.erase(vec.begin() + index);
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Done!" << "\n";
        std::cout << "Total Time : " << elapsed << "\n\n";

        start = std::chrono::steady_clock::now();
        vec.shrink_to_fit();

        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Pool Relese Done!\n";
        std::cout << "Total Time : " << elapsed << "\n";
    }
}

void CheckMemoryPool()
{
    std::cout << "Using Memory Pool\n\n";
    CpuMemoryPool* memPool = new CpuMemoryPool(eBlockSize::BYTE_64,0);
    MemoryList* memList = new MemoryList(memPool);

    {
        std::cout << "Allocate\n";
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < MAX_COUNT; ++i)
        {
            TestObject* obj = nullptr;
            memPool->GetMemory(&obj, i);
            obj->_index = i;
            memList->Add(obj->GetMemoryHandler());
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Done!\n";
        std::cout << "Total Time : " << elapsed << "\n";
    }

    std::cout << "\n";

    std::cout << "Release\n";

    {
        auto start = std::chrono::steady_clock::now();

        int successCount = 0;
        int failCount = 0;
        for (int i = 0; i < MAX_COUNT; ++i)
        {
            MemoryBlock block;
            bool isSuccess = memList->GetMemoryBlock(i, block);
            if (isSuccess)
            {
                successCount++;
                TestObject* obj = nullptr;
                memPool->GetObject(block, &obj);
                memPool->ReleaseMemory(obj);
            }
            else
            {
                failCount++;
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Objects Release Done!\n";
        std::cout << "Total Time : " << elapsed << "\n";

        std::cout << "\n";
        start = std::chrono::steady_clock::now();
        delete memPool;
        delete memList;

        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Pool Relese Done!\n";
        std::cout << "Total Time : " << elapsed << "\n";
    }
}

void CheckMemoryPool_Random()
{
    std::cout << "Using Memory Pool Random\n\n";
    CpuMemoryPool* memPool = new CpuMemoryPool(eBlockSize::BYTE_64,0);
    MemoryList* memList = new MemoryList(memPool);

    {
        std::cout << "Allocate\n";
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < MAX_COUNT; ++i)
        {
            TestObject* obj = nullptr;
            memPool->GetMemory(&obj, i);
            obj->_index = i;
            memList->Add(obj->GetMemoryHandler());
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Done!\n";
        std::cout << "Total Time : " << elapsed << "\n";
    }

    std::cout << "\n";

    std::cout << "Release\n";

    {
        auto start = std::chrono::steady_clock::now();

        int successCount = 0;
        int failCount = 0;
        for (int i = 0; i < MAX_COUNT; ++i)
        {
            MemoryBlock block;
            int index = rand() % memList->GetCount();
            bool isSuccess = memList->GetMemoryBlock(index, block);
            if (isSuccess)
            {
                TestObject* obj = nullptr;
                bool getSuccess = memPool->GetObject(block, &obj);
                if (getSuccess)
                {
                    memPool->ReleaseMemory(obj);
                    memList->RemoveAtUnordered(index);
                    successCount++;
                }
                else
                {
                    failCount++;
                }
            }
            else
            {
                failCount++;
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Objects Release Done!\n";
        std::cout << "Total Time : " << elapsed << "\n";

        std::cout << "\n";
        start = std::chrono::steady_clock::now();
        delete memPool;
        delete memList;

        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Pool Relese Done!\n";
        std::cout << "Total Time : " << elapsed << "\n";
    }
}

int main()
{
    std::cout << "Class Size : " << sizeof(TestObject) << "Byte" << "\n\n";
    std::cout << "Total Count : " << MAX_COUNT << "\n\n";

    //CheckSTL();
    CheckMemoryPool_Random();

    std::cout << "\n\n";
}