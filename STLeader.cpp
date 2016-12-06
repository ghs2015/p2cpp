#include "STLeader.h"
#include <iostream>

//??Significance 
STLeader* STLeader::stlptr = 0;

//Get the global pointer to the only instance of STLeader.
STLeader* STLeader::get_stlptr(){
	if (!stlptr){
		stlptr = new STLeader;	
		stlptr->set_scope(-1);
		stlptr->push_scope();
	}
	return stlptr;
}

void STLeader::set_scope(int s){
	currentScope = s;
}

//i means the deepest scope containing the symbol, if not, i is -1.
int STLeader::find_symbol(string name){
	int i = currentScope;
	SymbolTable* stptr;
	while (i>=0){
		stptr = &stv[i];
		if (stptr->find_symbol(name)){
			break;
		}
		else{
			--i;
		}
	}
	return i;
}
//Return the pointer associated with the name in the symbol table
Ast* STLeader::lookup_symbol(string name){
	Ast* ast;
	ast = stv[find_symbol(name)].lookup_symbol(name);
	return ast;
}

void STLeader::update_symbol(string name, Ast* n){
	 // sts[currentScope].update_symbol(name, n);
	stv[currentScope].update_symbol(name, n);
}

void STLeader::pop_scope(){
	if (currentScope >= 1){
		stv.pop_back();
		currentScope-=1;
	}
}

void STLeader::push_scope(){
	currentScope+=1;
	SymbolTable st(currentScope);
	stv.push_back(st);
}

int STLeader::get_scope(){
	return currentScope;
}

void STLeader::free_stl( ){	
	if(stlptr){ 
		delete stlptr;
		stlptr = 0;
	}
}

