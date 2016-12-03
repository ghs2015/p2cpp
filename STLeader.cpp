#include <STLeader.h>
#include <iostream>

STLeader* STLeader::stlptr = 0;
//Constructor
STLeader::STLeader(){
}
//Destructor
STLeader::~STLeader(){
}
//Get the global pointer to the only instance of STLeader.
STLeader* STLeader::get_stlptr(){
	if (!stlptr){
		stlptr = new STLeader();
		stlptr->currentScope = 0;
	}
	return stlptr;
}

void STLeader:: add_st(SymbolTable& st){
	sts[st.get_scope()] = st;
}
//i means the deepest scope containing the symbol, if not, i is -1.
int STLeader::find_symbol(string name){
	int i = currentScope;
	while (i>=0){
		if (stv[i].find_symbol(name)){
			break;
		}
		else{
			--i;
		}
	}
	return i;
}

*Node STLeader::lookup_symbol(string name){
	return stv[find_symbol(name)].lookup_symbol(name);
}

void STLeader::pop_scope(){
	if (currentScope>=1){
		stv.pop_back(sts[currentScope]);
		currentScope-=1;
	}
}

void STLeader::push_scope(){
	currentScope+=1;
	stv.push_back(sts[currentScope]);
}

void STLeader::free_stl( ){	
	if(stlptr){ 
		delete stlptr;
		stlptr = 0;
		//std::cout<<"free_table() works!"<<std::endl;
	}
}

void STLeader::print_st(){
	for(map<int, SymbolTable> >::const_iterator it = table.begin(); it != table.end(); ++it)
		{
			cout << it->first << " " << it->second.get_scope() << "\n";
		}
}