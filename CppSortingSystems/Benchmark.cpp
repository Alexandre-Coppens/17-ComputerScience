#include "Benchmark.h"
#include "Windows.h"
#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, BenchmarkTest>			Benchmark::benchmarkLists;
std::vector<std::vector<BenchmarkTest>> Benchmark::oldBenchmarkLists;
int64_t Benchmark::frequency;
int64_t Benchmark::endClock;
double Benchmark::average;

Benchmark::Benchmark()
{
}

Benchmark::~Benchmark()
{
}

void Benchmark::Init()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	frequency = li.QuadPart;
}

void Benchmark::StartClock(std::string _name)
{
	if (benchmarkLists.find(_name) == benchmarkLists.end())
	{
		benchmarkLists[_name].name = _name;
		benchmarkLists[_name].high = DBL_MIN;
		benchmarkLists[_name].low = DBL_MAX;
	}
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	benchmarkLists[_name].current = li.QuadPart;
}

void Benchmark::StopClock(std::string _name)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	endClock = li.QuadPart;
	benchmarkLists[_name].timeList.push_back((float)(endClock - benchmarkLists[_name].current) / frequency);
	std::cout << "||Benchmark for: " + _name + " || Results: " + std::to_string(benchmarkLists[_name].timeList[benchmarkLists[_name].timeList.size() - 1]) + " seconds." << std::endl;
	average = 0;
	for (double time : benchmarkLists[_name].timeList)
	{
		average += time;
		if (time < benchmarkLists[_name].low) benchmarkLists[_name].low = time;
		if (time > benchmarkLists[_name].high) benchmarkLists[_name].high = time;
	}
	benchmarkLists[_name].average = average / benchmarkLists[_name].timeList.size();
}

void Benchmark::PrintResults()
{
	std::cout << std::endl;
	std::cout << "=-=-=-=-=-= || BENCHMARK RESULTS || =-=-=-=-=-=" << std::endl;
	std::cout << std::endl;
	for (auto &b : benchmarkLists)
	{
		std::cout << "=-=-=-=-=-= || " + b.second.name + " || =-=-=-=-=-=" << std::endl;
		std::cout << "- AVERAGE : " + std::to_string(b.second.average) << std::endl;
		std::cout << "- HIGHEST : " + std::to_string(b.second.high) << std::endl;
		std::cout << "- LOWEST :  " + std::to_string(b.second.low) << std::endl;
		std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
		std::cout << "                ." << std::endl;
	}
	std::cout << std::endl;
}

void Benchmark::SaveResults(std::string _appName, std::string _version)
{
	if (benchmarkLists.empty()) return;

	std::cout << "Saving Data & Recalculating Benchmarks results...\n";
	//Save at last pos of oldBenchmarkLists
	oldBenchmarkLists.push_back(std::vector<BenchmarkTest>());
	int currentIndice = oldBenchmarkLists.size() - 1;
	for (auto& b : benchmarkLists)
	{
		oldBenchmarkLists[currentIndice].push_back(b.second);
		b.second = BenchmarkTest();
		b.second.name = b.first;
		b.second.high = DBL_MIN;
		b.second.low = DBL_MAX;
	}
	//Rewrite all benchmarkLists with global results
	BenchmarkTest stamp;
	for (int i = 0; i < oldBenchmarkLists.size(); i++)
	{
		for (int j = 0; j < oldBenchmarkLists[i].size(); j++)
		{
			stamp.name		= oldBenchmarkLists[i][j].name;
			stamp.average	= oldBenchmarkLists[i][j].average;
			stamp.low		= oldBenchmarkLists[i][j].low;
			stamp.high		= oldBenchmarkLists[i][j].high;
		}
		benchmarkLists[stamp.name].low   = min(benchmarkLists[stamp.name].low, stamp.low);
		benchmarkLists[stamp.name].high = max(benchmarkLists[stamp.name].high, stamp.high);
		benchmarkLists[stamp.name].timeList.push_back(stamp.average );
	}
	double average = 0;
	for (auto& b : benchmarkLists)
	{
		for (double time : b.second.timeList)
		{
			average += time;
		}
		b.second.average =  average / b.second.timeList.size();
	}

	std::cout << "Writing results on file\n";
	//Writing on file
	std::fstream fout;
	fout.open("temp.csv", std::ios::out | std::ios::app);
	fout << _appName << ",v" << _version << "\n";
	fout << "\n";
	//Save here the average of all tests
	fout << "RESULTS\n";
	fout << "Benchmark Name, Average, Min, Max\n";
	for (auto& b : benchmarkLists)
	{
		//						  Name					  ||  	Average				||	Min						 ||	 Max
		fout << "-" << b.second.name << "," << b.second.average << "s," << b.second.low << "s," << b.second.high << "s," << "\n";
	}
	fout << "\n";
	//Save here all the test already done
	fout << "TIMES\n";
	fout << "Test ID, Benchmark Name, Average, Min, Max\n";
	for (int i = 0; i < oldBenchmarkLists.size(); i++)
	{
		for (BenchmarkTest& b : oldBenchmarkLists[i])
		{
			//						  ID   ||	Name			 ||  	  Average		   ||		Min		 ||	 Max
			fout << "-" << i << "," << b.name << "," << b.average << "s," << b.low << "s," << b.high << "s," << "\n";
		}
	}

	fout.close();
	remove(std::string(_appName + ".v" + _version + ".csv").c_str());
	rename("temp.csv", std::string(_appName + ".v" + _version + ".csv").c_str());

	std::cout << "Results SAVED!\n";
}

void Benchmark::LoadFile(std::string _appName, std::string _version)
{
	std::fstream file;

	file.open(std::string(_appName + ".v" + _version + ".csv").c_str(), std::ios::in);

	std::string name;
	double high, low, average;
	int64_t curent;
	std::vector<double> timeList;
	
	std::vector<std::string> row;
	std::string line, word, temp;

	bool timesReached = false;
	int currentOldBenchmark = -1;
	BenchmarkTest tempBenchmark;

	while (getline(file, line))
	{
		row.clear();

		std::stringstream s(line);

		while (getline(s, word, ','))
		{
			row.push_back(word);
		}

		if (!row.empty())
		{
			//Check if TIMES is reached (where the old timers are stocked
			if (row[0] == "TIMES") timesReached = true;
			if (row[0][0] == '-' && timesReached)
			{
				row[0] = row[0].substr(1, row[0].size() - 1);
				if (stoi(row[0]) != currentOldBenchmark)
				{
					currentOldBenchmark = stoi(row[0]);
					oldBenchmarkLists.push_back(std::vector<BenchmarkTest>());
				}
				tempBenchmark.name = row[1];
				tempBenchmark.average = stod(row[2]);
				tempBenchmark.low = stod(row[3]);
				tempBenchmark.high = stod(row[4]);
				oldBenchmarkLists[oldBenchmarkLists.size() - 1].push_back(tempBenchmark);
			}
		}
	}

	file.close();
}
