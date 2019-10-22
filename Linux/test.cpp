#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
using namespace std;

int main()
{
	char buf[10];
	memset(buf,0,10);
	ifstream infile("in");
	ofstream outfile("out");
	if(!infile || !outfile)
	{
		cerr << "ERROR: file r/w wrong!\n";
		return -1;
	}
	for(;;)
	{
		memset(buf,0,10);
		infile >> buf;
		outfile << buf;
		if(infile.eof())
			break;
	}
	return 0;
}
