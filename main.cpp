#include <iostream>
#include "SymbolTable.h"
#include "STLeader.h"
#include "ast.h"
extern int yyparse();

int main() {
  try {
    //std::cout << "> "; 
    int returnCode = yyparse();    
    std::cout << std::endl;
    return returnCode;
  }
  catch (const std::string& msg) {
    std::cout << msg << std::endl; 
  }
  catch (const char* msg) {
    std::cout << msg << std::endl; 
  }
  catch (...) {
    std::cout << "Oops, need someone behind the plate" << std::endl; 
  }
  return 0;
}
