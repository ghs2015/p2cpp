// Companion source code for "flex & bison", published by O'Reilly
// helper functions for fb3-1
#  include <iostream>
#  include <stdlib.h>
#  include <math.h>
#  include <list>
#  include "ast.h"
#  include "SymbolTable.h"
#  include "STLeader.h"

extern STLeader* stlptr;
extern char rt;
extern double v;
extern char nt;

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
  case 'N': if(stlptr->find_symbol(a->getName())==-1) {throw "NameError: the name is not defined"; }
  v = (stlptr->lookup_symbol(a->getName()))->get_value();
  break;

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
  //           v = stod(stptr->lookup_symbol(a->getName()).second); //
    case 'N': if(stlptr->find_symbol(a->getName())==-1) {throw "NameError: the name is not defined"; }
    v = (int)(stlptr->lookup_symbol(a->getName()))->get_value();
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
//For calling a function
Ast* evalCall ( Ast* astCall) 
{
  Ast* astReturn;
  std::list<Ast*> :: iterator it ;
  std::list<Ast*> vec; 
  // std::cout<<"astCall->getName():    "<<astCall->getName()<<std::endl;
  // std::cout<<"stlptr ->find_symbol(astCall->getName()):     "<<stlptr ->find_symbol(astCall->getName())<<std::endl;
  // std::cout<<"vector size:     "<<stlptr -> lookup_symbol(astCall->getName()) ->get_suite()->getVec().size()<<std::endl;
  if (stlptr ->find_symbol(astCall->getName())<0){ throw std::string("The function is not defined!"); }
  vec = stlptr -> lookup_symbol(astCall->getName()) ->get_suite()->getVec(); 
     //开始一个新的scope
  stlptr -> push_scope();
	//根据函数名，查表得到要执行的函数的AstFunction,进而得到其suiteNode的vector，
	//然后eval(asts in the vec)，还要记得搞到returnNode。
	// stlptr -> lookup_symbol(astCall->getName()) is a AstFunction* 

	//遍历astf->vec里的所有AST NODE，用eval()进行逐条执行。
     //！！应该在这里考虑returnNode,执行到returnNode就停止。
  it = vec.begin();
  while(it!=vec.end()&&((*it)->getNodetype()!='R'))
  {
    // std::cout<<"IN evalCall(Ast* astCall)"<<std::endl;
    // std::cout<<"Current Node type is "<<(*it)->getNodetype()<<std::endl;
    eval(*it);
    ++it;
  }
	//这时函数体已经运行完，returnNode应该也已经生成并且存在当前的符号表里了。
	//"return"! ！！注意，这个需要在parser里面完成。TODO@parser
  //astReturn = stlptr->lookup_symbol("return");


	//关闭当前scope
  stlptr->pop_scope();
  //return astReturn;
  return (new AstVoid('0', astCall->getNodeid(), astCall->getReturntype(), astCall->getName()));
}

//for def a function
Ast* evalFunction(Ast* ast){
	//！！TODO涉及了好多指针操作啊，没有memory leakage怎么好意思呢？？
	//更新当前scope的符号表里的function node
  std::cout << "IN evalFunction()" << std::endl;;
  stlptr->update_symbol(ast->getName(), ast);
  return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), ast->getName()));
}

Ast* evalSuite(Ast* ast){
	return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), ast->getName()));
}


Ast* eval(Ast* ast){
  // std:: cout<< "IN eval( )"<< std::endl;
  // std:: cout<< "node type:    "<< ast->getNodetype()      << std::endl;
  // std:: cout<< "return type:    "<<   ast->getReturntype()   << std::endl;
	//for call function node
  if(ast->getNodetype()=='C'){
    // std:: cout<< "evalCall(ast);"<< std::endl;
    return evalCall(ast);
  }
	//for def function node
  if(ast->getNodetype()=='F'){
    // std:: cout<< "evalFunction(ast);"<< std::endl;
    return evalFunction(ast);
  }
   
    if(ast->getNodetype()=='N'){
      // std::cout<<"getNodetype()=='N' "<<std::endl;
      if(stlptr->find_symbol(ast->getName())<0){
        throw "The name is not defined.";
      }
      else {
        return ast;
        // std::cout<<"Stored Nodetype() is:  "<<stlptr->lookup_symbol(ast->getName())->getNodetype()<<std::endl;
        // std::cout<<"Stored value() is:  "<<stlptr->lookup_symbol(ast->getName())->get_value()<<std::endl;
                //return eval(stlptr->lookup_symbol(ast->getName())); //这里表里存的应该是（变量名：ASTF OR ASTI啊）
        // rt = stlptr->lookup_symbol(ast->getName())->getReturntype();
        // v = stlptr->lookup_symbol(ast->getName())->get_value();
      }
    }


  if(ast->getNodetype()=='A'){
	
    rt = ast->getRight()->getReturntype();
    // std::cout<<"ast->getLeft())->getReturntype is: "<<ast->getLeft()->getReturntype()<<std::endl;
    // std::cout<<"ast->getLeft()->getNT is: "<<ast->getLeft()->getNodetype()<<std::endl;
    // std::cout<<"ast->getRight()->getReturntype is: "<<rt<<std::endl;
    // std::cout<<"ast->getRight()->getNT is: "<<ast->getRight()->getNodetype()<<std::endl;

    if (rt=='i')  {
      v = eval(ast->getRight())->getIntNumber();
          }
    else if(rt=='f')   {
      v = eval(ast->getRight())->getFloatNumber();
    }
    else { throw "What are you doing with the assign node?"; }
    ast->getLeft()->setReturntype(rt);
    ast->getLeft()->set_value(v);
    stlptr->update_symbol(ast->getLeft()->getName(), ast->getLeft());
    // std:: cout<< "return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), ast->getName()));"<< std::endl;
    return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), "ass"));
  }
	//for print node
  if(ast->getNodetype()=='P'){
		//需要operator overload啊,已经加了，安。。。
    std::cout<<eval(ast->get_toPrint())<<std::endl;
    // std:: cout<< "ast->getNodetype()=='P'"<< std::endl;
    return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(),"PPP"));
  }
	//for void node
  if(ast->getNodetype()=='0'){
    return ast;
  }
	//for int return type, char nodetype, int id, char rt, int n)
      //!!看起来是对于AstNode进行运算的，检查
  if(ast->getReturntype()=='i'){
    // std:: cout<< "return new AstInteger('I', ast->getNodeid(),'i', evalInt(ast) );"<< std::endl;
    return new AstInteger('I', ast->getNodeid(),'i', evalInt(ast) );
  }
  if(ast->getReturntype()=='f'){
    // std:: cout<< "return new AstFloat('K', ast->getNodeid(), 'f', evalFloat(ast) );"<< std::endl;
    return new AstFloat('K', ast->getNodeid(), 'f', evalFloat(ast) );
  }
  else 
    {return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), ast->getName()));}
}

//operator overload is coming
std::ostream& operator <<(std::ostream& os, const Ast * ast)
{
	// std:: cout<< "IN std::ostream& operator <<(std::ostream& os, const Ast * ast)"<< std::endl;
  std:: cout<< "heheda: ";
	switch (ast->getNodetype()) {
		case 'I': 
                          os << (ast->getIntNumber());
                          break;
    case 'K':
    os << (ast->getFloatNumber());
    break;
		//！！理论上来说，evaluate之后不会出现N型了吧？
    case 'N':
			// os<< (ast->get_value());
    // os<<"Variable name is:  " <<ast->getName();
    os<< stlptr->lookup_symbol(ast->getName())->get_value();
                  // os<< (ast->getName());
    break;
    case '0':
    break;
		//！！看起来目前不用考虑打印其他类型的东东。
    default: os<< "Nodetype doesn't match any case.";
  }
  return os;
}

Ast* deepcopy(Ast* a){
  Ast* ast;
  switch (a->getNodetype())
  {
    //AstInteger
    case 'I': {
      ast = new AstInteger(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getIntNumber());
      break;
    }
    //AstFloat
    case 'K': {
     ast = new AstFloat(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getFloatNumber());
     break;
   }
    //AstName
   case 'N': {
    ast = new AstName(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getName(),a->get_value());

    break;
  }
    //AstVoid
  case '0': {
    ast = new AstVoid(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getName());
    break;
  }
    //AstCall
  case 'C': {
    ast = new AstCall(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getName());
    break;
  }
    //AstFunction
  case 'F': {
    ast = new AstFunction(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getName(), deepcopy(a->get_suite()));
    break;
  }
    //AstPrint
  case 'P': {
    ast = new AstPrint(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getName(), deepcopy(a->get_toPrint()));
    break;
  }
    //AstAss
  case 'A':{
    ast = new AstAss(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getName(),deepcopy(a->getLeft()),deepcopy(a->getRight()));
    break;
  }
    //AstSuite
    //!!To consider the return node?
  case 'S':{
    ast = new AstSuite(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getName());
    // std::vector<Ast*> v;
    std::list<Ast*>:: iterator it = a->getVec().begin();
    while (it!=a->getVec().end()){
      ast->push_back(deepcopy(*it));
      ++it;
    }
    
    break;
  }
    //AstReturn
  case 'R':{
    ast = new AstReturn(a->getNodetype(), a->getNodeid(),a->getReturntype(),a->getName(),a->get_value());
    break;
  }

    //astNode
  case '+':
  case '-':
  case '*':
  case '/':
  case 'd':
  case '%':
  case '^':{
    ast = new AstNode(a->getNodetype(), a->getNodeid(),a->getReturntype(),deepcopy(a->getLeft()),deepcopy(a->getRight()));
    break;
  }
  case 'M':{
    ast = new AstNode(a->getNodetype(), a->getNodeid(),a->getReturntype(),deepcopy(a->getLeft()),0);
    break;
  }

}
return ast;


}
