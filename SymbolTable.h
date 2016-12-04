#include <string>
#include <map>
#include <iostream>
// #include "ast.h"
using namespace std;

class Ast;

class SymbolTable
{
	public:
		SymbolTable(int arg_scope);
		~SymbolTable();	
		//void update_symbol(string name, string type, string value) ;
		void update_symbol(string name, Ast* n) ;
		// void print_table() ;
		bool find_symbol(string name) ; //search upwards to scope=0
		//Ast* lookup_symbol (string name) ;
		//pair <string,string> lookup_symbol (string name) ;
		Ast* lookup_symbol (string name) ;
		// string get_functionName();
		int get_scope();
		void set_scope(int s);

	private:
		// map <string, pair <string, string>> table; //!!要在table里面存kind(variable or fun)，这样存不下了，考虑用*Ast
		int scope;	
		map <string, Ast*> table;
		// string functionName;
};
