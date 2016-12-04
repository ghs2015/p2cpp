#include "SymbolTable.h"

//constructor
SymbolTable:: SymbolTable(int arg_scope):scope(arg_scope){};
//descturctor
SymbolTable:: ~SymbolTable(){};

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

void SymbolTable::update_symbol(string name, Ast* nptr) 	
{
	table[name] = nptr;
	//this->print_table();
}

Ast* SymbolTable::lookup_symbol (string name) 
{
	return table[name];
}

int SymbolTable::get_scope(){
	return scope;
}
void SymbolTable::set_scope(int s){
	scope = s;
}
// void SymbolTable::print_table() 
// {
// 	for(map<string, Ast*>::const_iterator it = table.begin(); it != table.end(); ++it)
// 		{
// 			cout << it->first << " " << it->second.type << endl;
// 		}
// }