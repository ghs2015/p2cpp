#ifndef _ST_H_
#define _ST_H_

#include <string>
#include <map>
#include <iostream>
// #include "ast.h"
using namespace std;

class Ast;

class SymbolTable
{
	public:
		//constructors
		SymbolTable();
		SymbolTable(int arg_scope);
		~SymbolTable();	
		SymbolTable(const SymbolTable &obj);
		//find, look up, and update
		bool find_symbol(string name) ; //search upwards to scope=0
		Ast* lookup_symbol (string name) ;
		void update_symbol(string name, Ast* n) ;
		//Scope operation
		int get_scope () const;
		void set_scope(int s);

	private:
		int scope;	
		map <string, Ast*> table;		
};

#endif