#include <string>
#include <map>
#include <iostream>

using namespace std;

class Node;

class SymbolTable
{
	public:
		SymbolTable(int arg_scope);
		~SymbolTable();		
		//void update_symbol(string name, string type, string value) ;
		void update_symbol(string name, *Node) ;
		// void print_table() ;
		bool find_symbol(string name) ; //search upwards to scope=0
		//Node* lookup_symbol (string name) ;
		//pair <string,string> lookup_symbol (string name) ;
		Node* lookup_symbol (string name) ;
		// string get_functionName();
		int get_scope();

	private:
		// SymbolTable* dad;
		//map <string, *Node> table;
		// map <string, pair <string, string>> table; //!!要在table里面存kind(variable or fun)，这样存不下了，考虑用*Node
		int scope;	
		map <string, *Node> table;
		// string functionName;
};
