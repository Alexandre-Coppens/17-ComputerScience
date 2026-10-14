#pragma once
#include <map>
#include <string>
#include <vector>
#include <cstdint>

struct BenchmarkTest
{
	std::string name;
	double  high;
	double  low;
	double  average;
	int64_t  current;

	std::vector<double> timeList;
};

class Benchmark
{
private:
	static std::map<std::string, BenchmarkTest> benchmarkLists;
	static std::vector<std::vector<BenchmarkTest>> oldBenchmarkLists;
	static int64_t frequency;
	static int64_t endClock;
	static double average;

public:
	Benchmark();
	~Benchmark();

	static void Init();

	static void StartClock(std::string _name);
	static void StopClock(std::string _name);

	static void PrintResults();
	static void SaveResults(std::string _appName, std::string _version);
	static void LoadFile(std::string _appName, std::string _version);
};