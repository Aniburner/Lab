#include<iostream>
#include<fstream>
#include<string>

using namespace std;

typedef struct stuInfo{
	string stuName;
	int Age;
}data;
typedef struct node{
	data info;
	struct node *next=NULL;
}Node, *ListPtr;

ListPtr CreateList();
void PrintList(ListPtr);
void InsertList(ListPtr&);

string path;

int main()
{
	ListPtr ListHead=NULL;
	for(;;)
	{
		cout << "1. Create List" << endl;
		cout << "2. Show List" << endl;
		cout << "3. Insert List" << endl;
		cout << "4. Quit" << endl;
		char command;
		cin >> command;
		switch(command)
		{
			case '1':ListHead=CreateList();break;
			case '2':PrintList(ListHead);break;
			case '3':InsertList(ListHead);break;
			case '4':return 0;
		}
	}
	return 0;
}

ListPtr InsertNode(string first, int second,ListPtr ListHead)
{
	ListPtr node= new Node;
	node->info.stuName=first;
	node->info.Age=second;
	node->next=ListHead;
	return node;
}
ListPtr CreateList()
{
	cout << "Input the file path" << endl;
	cin >> path;
	ifstream read_file;
	read_file.open(path);
	ListPtr ListHead=NULL;
	string first;
	int second;
	//cout << "before for loop" << endl;
	for(;;)
	{
		if(read_file.eof())break;
		getline(read_file,first);
		//cout << "read first: " << first	<< endl;
		if(read_file.eof())break;
		read_file >> second;
		read_file.get();
		//cout << "read second: " << second<< endl;
		if(read_file.eof())break;
		ListHead=InsertNode(first,second,ListHead);
	}
	read_file.close();
	return ListHead;
}
void PrintList(ListPtr ListHead)
{
	ListPtr current_ptr=ListHead;
	cout << "name                |age" << endl;
	for(;current_ptr!=NULL;current_ptr=current_ptr->next)
	{
		current_ptr->info.stuName.append(20 - current_ptr->info.stuName.size(), ' ');
		cout << current_ptr->info.stuName << "|" << current_ptr->info.Age << endl;
	}	
}
void InsertList(ListPtr &ListHead)
{
	ListPtr node=new Node;
	cout << "Input student name" << endl;
	cin.ignore(1024,'\n');
	getline(cin,node->info.stuName);
	cout << "Input age" << endl;
	cin >> node->info.Age;
	node->next=ListHead;
	ListHead=node;
	ofstream write_file;
	write_file.open(path,ios::app);
	write_file << node->info.stuName << endl;
	write_file << node->info.Age << endl;
	cout << "there should be written" << endl;
	write_file.close();
}



