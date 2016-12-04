// Companion source code for "flex & bison", published by O'Reilly
// helper functions for fb3-1
#  include <iostream>
#  include <stdlib.h>
#  include <math.h>
#  include "ast.h"
#  include "SymbolTable.h"
#  include "STLeader.h"

extern STLeader* stlptr;

//TODO!! Add zero exception 这里的两个switch怪怪的，好像有问题啊！！
double evalFloat(Ast *a){
  double v = 0;
  double l = 0;
  double r = 0;
  //SymbolTable* stptr = SymbolTable::get_table();
  switch( a->getNodetype() ) {
    case '^':;
    case '+':;
    case '-':;
    case '*':;
    case '%':;
    case 'd':;
    case '/':
      if ((a->getLeft()->getReturntype()=='i')&&(a->getRight()->getReturntype()=='i')){
	    l = evalInt(a->getLeft());
	    r = evalInt(a->getRight());
      }
      else if (a->getLeft()->getReturntype()=='i'){
        l = evalInt(a->getLeft());
        r = evalFloat(a->getRight());
      }
      else if (a->getRight()->getReturntype()=='i'){
        l = evalFloat(a->getLeft());
        r = evalInt(a->getRight());
      }
      else {
        l = evalFloat(a->getLeft());
        r = evalFloat(a->getRight());        
      }
      break;    
    case 'M':
    case 'N':
    case 'K': break;
  }
  switch( a->getNodetype() ) {
    case '^': if (l==0&&r<0) { throw "Division by zero condition!"; }
              v = pow(l,r); 
              break;
    case 'K': v = a->getFloatNumber(); break;
    // case 'N': if (!stptr->find_symbol(a->getName())) { throw "NameError: the name is not defined"; }
    //           v = stod(stptr->lookup_symbol(a->getName()).second); 
    //           break; 
	case 'N': if(stlptr->find_symbol(a->getName)==-1) {throw "NameError: the name is not defined"; }
            v = (stlptr->lookup_symbol(a->getName))->get_value();  

  //case 'I': v = a->getIntNumber(); break;
    case '+': v = l + r; break;
    case '-': v = l - r; break;
    case '*': v = l * r; break;
    case '/': if (r==0) { throw "Division by zero condition!"; }
              v = l / r; 
              break;
    case 'd': if (r==0) { throw "Division by zero condition!"; }
              v = floor(l / r);
              break;

    case '%': if (r==0) { throw "Division by zero condition!"; }
              v = l - r*floor(l/r); 
              break;
 
    case 'M': v = -evalFloat(a->getLeft()); 
              break;  
    default: std::cout << "internal error: bad node in evalFloat"
                << a->getNodetype() << std::endl;;
  }
  return v;  

}

//TODO!! Add zero exception
int evalInt(Ast *a){
  //SymbolTable* stptr = SymbolTable::get_table();
  int v = 0;
  switch( a->getNodetype() ) {
  case '^': if (evalInt(a->getLeft())==0&&evalInt(a->getRight())<0) { throw "Division by zero condition!"; }
            v = pow(evalInt(a->getLeft()),evalInt(a->getRight())); break;
  //case 'K': v = a->getFloatNumber(); break;
  case 'I': v = a->getIntNumber(); break;
  // case 'N': if (!stptr->find_symbol(a->getName())) { throw "NameError: the name is not defined"; }
  //           v = stod(stptr->lookup_symbol(a->getName()).second); //！！你在外面能知道这个变量的类型吗？如果赋过值，我猜可以，后面论证下。
  //           break; //！！这里能直接用stptr吗？在parser里加了声明，不知道有效果木有。
  case 'N': if(stlptr->find_symbol(a->getName)==-1) {throw "NameError: the name is not defined"; }
            v = (int)(stlptr->lookup_symbol(a->getName))->get_value();
            break;

  case '+': v = evalInt(a->getLeft()) + evalInt(a->getRight()); break;
  case '-': v = evalInt(a->getLeft()) - evalInt(a->getRight()); break;
  case '*': v = evalInt(a->getLeft()) * evalInt(a->getRight()); break;
  case 'd': ;
  case '/': if (evalInt(a->getRight())==0) { throw "Division by zero condition!"; }
            else if (evalInt(a->getLeft()) * evalInt(a->getRight()) >= 0){
            	v = evalInt(a->getLeft()) / evalInt(a->getRight()); break;
            }
            else if (evalInt(a->getLeft()) * evalInt(a->getRight()) < 0){
            	v = floor(1.0 * evalInt(a->getLeft()) / evalInt(a->getRight())); break;
            }
  case '%': if (evalInt(a->getRight())==0) { throw "Division by zero condition!"; }
            else if (evalInt(a->getLeft()) * evalInt(a->getRight()) >= 0){
            	v = evalInt(a->getLeft()) % evalInt(a->getRight()); break;
            }
            else if (evalInt(a->getLeft()) * evalInt(a->getRight()) < 0){
            	v = evalInt(a->getLeft()) - evalInt(a->getRight())*floor(1.0*evalInt(a->getLeft())/evalInt(a->getRight())); break;
            }  
  case 'M': v = -evalInt(a->getLeft()); break;  
  default: std::cout << "internal error: bad node in evalInt"
                << a->getNodetype() << std::endl;;
  }
  return v;
}


void treeFree(Ast *a) {
  switch(a->getNodetype()) {
   // two subtrees
  case '+':
  case '-':
  case '*':
  case '/':
  case 'd':
  case '%':
  case '^':
    treeFree(a->getRight());

   // one subtrees
  case 'M':
    treeFree(a->getLeft());

   //no subtree
  //float
  case 'K':
  //int
  case 'I':
  //variable
  case 'N':
  	delete a;
  	break;
  //function
  case 'F':
  	delete a;
  default: std::cout << "internal error: bad node in treeFree()"
                << a->getNodetype() << std::endl;;
  }
}