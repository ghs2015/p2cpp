/**
A symbol table leader is a singleton.
**/

#include <SymbolTable.h>
#include <string>

using namespace std;
class STLeader
{
	public:
		STLeader();
		~STLeader();
		//For the global visit to symbol table leader.
		static STLeader* get_stlptr(); //！！这里需要用指针吗？还是传对象的参考就行？试试指针。
		//In the runtime, when calling, get into a deeper symbol table.
		void add_st(SymbolTable& st); //！！这里用参考试试。
		//In the runtime, when leaving a scope, pop out the deepest symbol table. So I need a location recorder: current scope? Seems no.
		// static void pop_st();
		// static bool find_st();
		int find_symbol();
		Node* lookup_symbol();
	    //For navigating over scopes
	    void pop_scope();
		void push_scope();

	private:
		//Declare the only instance for the STLeader singleton
		static STLeader* stlptr;
		vector <SymbolTable> stv;
		//key: scope, value: stv
		map<int, SymbolTable> sts;
		//This is used to know the deepest symbol table in the list/vector, so that I can search.
		//e.g. stTables(currentScope).find(...)
		int currentScope;
};