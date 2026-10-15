// TestCppSort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Benchmark.h"

const int arraySize = 1000000;
int array[arraySize];

void GenerateArray();
void VerifyResults(std::string name);
void BubbleSort();
void SelectionSort();
void InsertionSort();
void MergeSort();
std::vector<int> SubMergeSort(std::vector<int>, std::vector<int>);
void QuickSort();
std::vector<int> SubQuickSort(std::vector<int>);
void HeapSort();

int main()
{
    Benchmark::Init();
    Benchmark::LoadFile("Sorting", "1.0");

    //ALGORITHMS
    for (int i = 0; i < 1; i++)
    {
        GenerateArray();
        BubbleSort();
        VerifyResults("BubbleSort");

        GenerateArray();
        SelectionSort();
        VerifyResults("SelectionSort");

        GenerateArray();
        InsertionSort();
        VerifyResults("InsertionSort");

        GenerateArray();
        MergeSort();
        VerifyResults("MergeSort");

        GenerateArray();
        QuickSort();
        VerifyResults("QuickSort");

        GenerateArray();
        HeapSort();
        VerifyResults("HeapSort");
    }

    //Show Result
    /*for (int i = 0; i < arraySize; i++)
    {
        std::cout << std::to_string(i) << " : " << std::to_string(array[i]) << std::endl;
    }*/
    Benchmark::PrintResults();
    Benchmark::SaveResults("Sorting", "1.0");
}

void GenerateArray()
{
    srand(404);
    for (int i = 0; i < arraySize; i++)
    {
        array[i] = rand();
    }
}

void VerifyResults(std::string name)
{
    bool valid = true;
    for (int i = 1; i < arraySize; i++)
    {
        if (array[i] < array[i - 1])
        {
            std::cout << i << "\n";
            valid = false;
        }
    }
    if(valid) std::cout << name + " is VALID \n";
    else std::cout << name + " is BAD \n";
}

#pragma region BubbleSort
void BubbleSort()
{
    Benchmark::StartClock("BubbleSort");

    int temp;
    for (int i = 0; i < arraySize - 1; i++)
    {
        for (int j = 0; j < arraySize - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    Benchmark::StopClock("BubbleSort");
}

#pragma endregion

#pragma region SelectionSort
void SelectionSort()
{
    Benchmark::StartClock("SelectionSort");

    int temp;
    for (int i = 0; i < arraySize - 1; i++)
    {
        for (int j = i + 1; j < arraySize ; j++)
        {
            if (array[j] < array[i])
            {
                temp = array[j];
                array[j] = array[i];
                array[i] = temp;
            }
        }
    }

    Benchmark::StopClock("SelectionSort");
}

#pragma endregion

#pragma region InsertionSort
void InsertionSort()
{
    Benchmark::StartClock("InsertionSort");

    int index;
    int temp;
    for (int i = 0; i < arraySize - 1; i++)
    {
        index = i + 1;
        while (array[index] < array[index - 1] && index != 0)
        {
            temp = array[index];
            array[index] = array[index - 1];
            array[index - 1] = temp;
            index--;
        }
    }

    Benchmark::StopClock("InsertionSort");
}

#pragma endregion

#pragma region MergeSort
void MergeSort()
{
    std::vector<int> vArray(arraySize);
    for (int i = 0; i < arraySize - 1; i++)
    {
        vArray[i] = array[i];
    }

    Benchmark::StartClock("MergeSort");
    vArray = SubMergeSort(std::vector<int>(vArray.begin(), vArray.begin() + vArray.size() / 2), std::vector<int>(vArray.begin() + vArray.size() / 2, vArray.end()));
    Benchmark::StopClock("MergeSort");

    for (int i = 0; i < arraySize; i++)
    {
        array[i] = vArray[i];
    }
}

std::vector<int> SubMergeSort(std::vector<int> a, std::vector<int> b)
{
    if (a.size() > 1) a = SubMergeSort(std::vector<int>(a.begin(), a.begin() + a.size() / 2), std::vector<int>(a.begin() + a.size() / 2, a.end()));
    if (b.size() > 1) b = SubMergeSort(std::vector<int>(b.begin(), b.begin() + b.size() / 2), std::vector<int>(b.begin() + b.size() / 2, b.end()));
    int aIndex = 0;
    int bIndex = 0;
    std::vector<int> final;
    while (aIndex < a.size() || bIndex < b.size())
    {
        if (aIndex == a.size())
        {
            final.push_back(b[bIndex]);
            bIndex++;
            continue;
        }
        if (bIndex == b.size())
        {
            final.push_back(a[aIndex]);
            aIndex++;
            continue;
        }
        if (a[aIndex] < b[bIndex])
        {
            final.push_back(a[aIndex]);
            aIndex++;
        }
        else
        {
            final.push_back(b[bIndex]);
            bIndex++;
        }
    }
    return final;
}

#pragma endregion

#pragma region QuickSort
void QuickSort()
{
    std::vector<int> vArray(arraySize);
    for (int i = 0; i < arraySize - 1; i++)
    {
        vArray[i] = array[i];
    }

    Benchmark::StartClock("QuickSort");
    vArray = SubQuickSort(vArray);
    Benchmark::StopClock("QuickSort");

    for (int i = 0; i < arraySize; i++)
    {
        array[i] = vArray[i];
    }
}

std::vector<int> SubQuickSort(std::vector<int> arr)
{
    if (arr.size() <= 1) return arr;

    int selected = arr[0];

    std::vector<int> higher;
    std::vector<int> lower;

    int temp = 0;
    for (int i = 1; i < arr.size(); i++)
    {
        if (arr[i] < selected)
        {
            lower.push_back(arr[i]);
            continue;
        }
        higher.push_back(arr[i]);
    }

    lower = SubQuickSort(lower);
    higher = SubQuickSort(higher);

    arr.clear();
    arr.insert(arr.begin(), lower.begin(), lower.end());
    arr.push_back(selected);
    arr.insert(arr.end(), higher.begin(), higher.end());

    return arr;
}

#pragma endregion

#pragma region HeapSort
struct BinaryTree
{
    int root;
    BinaryTree* leftTree    { nullptr };
    BinaryTree* rightTree   { nullptr };

    void SetupNumber(int i)
    {
        if (i < root) SetupLeft(i);
        else SetupRight(i);
    }

    void SetupLeft(int i)
    {
        if (leftTree != nullptr)
        {
            leftTree->SetupNumber(i);
            return;
        }
        leftTree = new BinaryTree();
        leftTree->root = i;
    }

    void SetupRight(int i)
    {
        if (rightTree != nullptr)
        {
            rightTree->SetupNumber(i);
            return;
        }
        rightTree = new BinaryTree();
        rightTree->root = i;
    }

    int GetHighest()
    {
        int temp;
        if (rightTree != nullptr)
        {
            temp = rightTree->GetHighest();
            if (temp > INT32_MIN)
            {
                return temp;
            }
            delete(rightTree);
            rightTree = nullptr;
        }
        if (root != INT32_MIN)
        {
            temp = root;
            root = INT32_MIN;
            return temp;
        }
        else if (leftTree != nullptr)
        {
            temp = leftTree->GetHighest();
            if (temp > INT32_MIN)
            {
                return temp;
            }
            delete(leftTree);
            leftTree = nullptr;
        }
        return INT32_MIN;
    }

    void Delete()
    {
        if(leftTree != nullptr) leftTree->Delete();
        if(rightTree != nullptr) rightTree->Delete();
        delete(leftTree);
        delete(rightTree);
        leftTree = nullptr;
        rightTree = nullptr;
    }
};

void HeapSort()
{
    BinaryTree root;

    Benchmark::StartClock("HeapSort");
    root.root = array[0];
    for (int i = 1; i < arraySize; i++)
    {
        root.SetupNumber(array[i]);
    }
    
    for (int i = 0; i < arraySize; i++)
    {
        array[arraySize - 1 - i] = root.GetHighest();
    }

    root.Delete();
    Benchmark::StopClock("HeapSort");
}

#pragma endregion