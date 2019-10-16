#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main()
{
	string path;
	cout << "Input path" << endl;
	cin >> path;
	fstream local_file;
	local_file.open(path);
	string first;
	int second;
	local_file >> first;
	local_file >> second;
	cout << first << second;	
	local_file << "Hello World!" << endl;
	local_file.close();
	return 0;
}
