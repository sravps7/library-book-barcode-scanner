#include<iostream>
#include<stdio.h>
#include<vector>
#include<fstream>
#include<algorithm>
#include<string.h>
#include<functional>
#include<simplecpp>

using namespace std;
long long getNumFromString(string str)       ///generates numerical long long value of provided string
{

	long long  num=0;
	int size = str.length();
	for(int i=0;i<size;i++)
	{
		num= num*10+str[i]-'0';
	}

	return num;
}

vector<long long> getSortedBarcodesFromFile(string fileName)     /// load barcodes from file in vector
{

    ifstream file(fileName.c_str());      /// standard functions  accepts only cstandard strings
	string str;
	vector<long long> sorted;
	long long y;
	while(file)
	{
		getline(file,str);
		y = getNumFromString(str);
		if(y>0)
		{
			sorted.push_back(y);
		}
	}
	file.close();
	sort(sorted.begin(),sorted.end());
	return sorted;
}

string createOutputFileExctraName(string inputFileName)     /// generate name of Extra file
{
    inputFileName = inputFileName.substr(0,inputFileName.length()-4);     /// remove extension
    string outputFileName;
    outputFileName.append(inputFileName);
    outputFileName.append("Exctra.txt");             /// add new part
    return outputFileName;
}

string createOutputFileAbsentName(string inputFileName)     /// generate name of Absent file
{
   inputFileName = inputFileName.substr(0,inputFileName.length()-4);     /// remove extension
    string outputFileName;
    outputFileName.append(inputFileName);
    outputFileName.append("Absent.txt");           /// add new part
    return outputFileName;
}
