/**
A symbol table leader is a singleton.
**/
#ifndef _STL_H_
#define _STL_H_

#include <string>
#include <vector>
#include <map>
#include "SymbolTable.h"
// #include "ast.h"

class Ast;
using namespace std;

class STLeader
{
	public:
		//For the global visit to symbol table leader.
		static STLeader* get_stlptr(); //！！这里需要用指针吗？还是传对象的参考就行？试试指针。
		static void      free_stl( );
		
		// Symbol operations 
		int  find_symbol(string n);
		Ast* lookup_symbol(string n);
		void update_symbol(string name, Ast* n);

		//Symbol Table insertion and navigation operations
		void add_st(const SymbolTable& st); //！！这里用参考试试。
		//In the runtime, when leaving a scope, pop out the deepest symbol table. So I need a location recorder: current scope? Seems no.
	    void pop_scope();
	    //In the runtime, when calling, get into a deeper symbol table.
		void push_scope();
		int  get_scope();
		void set_scope(int s);

    private:
		//Declare the only instance for the STLeader singleton
		static STLeader* stlptr;

		vector<SymbolTable> stv; //所以我需要一个copy constructor？？
		//key: scope number, value: st
		map <int, SymbolTable> sts;
		//This is used to know the deepest symbol table in the list/vector, so that I can search.
		//e.g. stTables(currentScope).find(...)
		int currentScope;

};

#endif