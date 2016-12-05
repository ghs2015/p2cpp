#include "SymbolTable.h"

//constructor and descturctor
SymbolTable:: SymbolTable(){}
SymbolTable:: SymbolTable(int arg_scope):scope(arg_scope){}
SymbolTable:: ~SymbolTable(){}
SymbolTable:: SymbolTable(const SymbolTable &obj){
	scope = obj.get_scope();
}
bool SymbolTable::find_symbol(string name) 
{
	bool Exist = false;
	std::map<string, Ast*>::iterator it;
	it = table.find(name);
	if (it != table.end()){
		Exist = true;
	}
	return Exist;
}

Ast* SymbolTable::lookup_symbol (string name) 
{
	return table[name];
}

void SymbolTable::update_symbol(string name, Ast* n) 	
{	
	table[name] = n;
}

int SymbolTable::get_scope() const{
	return scope;
}
void SymbolTable::set_scope(int s){
	scope = s;
}
