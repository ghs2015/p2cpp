// Companion source code for "flex & bison", published by O'Reilly
// helper functions for fb3-1
#  include <iostream>
#  include <stdlib.h>
#  include <math.h>
#  include "ast.h"
#  include "SymbolTable.h"
#  include "STLeader.h"

extern STLeader* stlptr;
extern char rt;
extern double v;

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
  //           v = stod(stptr->lookup_symbol(a->getName()).second); //！！你在外面能知道这个变量的类型吗？如果赋过值，我猜可以，后面论证下。
  //           break; //！！这里能直接用stptr吗？在parser里加了声明，不知道有效果木有。
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

Ast* evalFunction(Ast* astf){
	//！！这个函数君的人生目标就是运行一遍函数中的所有语句：
	//目前只包括赋值语句和打印语句。然后返回一个值。如果返回值为空，那就返回一个Ast空空屁。
	//不然就返回一个AstReturn.就用这个颜色一会吧，我打的字比较清楚。
	//$开始一个新的scope
	stlptr->push_scope();
	//$尸位素餐的代码；

	//我感觉还得加个赋值Node。AST家族不断发展壮大啊，威胁到地球和平了。。。
	//好像还要用递归，哇嘎嘎，行不行啊，少废话了，快写！挂科了一会。。。
	//$遍历astf->vec里的所有AST NODE，用eval()进行逐条执行。


	//$关闭当前scope

}
//！！这家伙好像一个皮包公司啊。。。
//！！怎样不用皮包+多条件判断？用polymophism？把返回类型等属性封装到类属性里，返回一个类的指针就行。
//！！后面儿有时间再重构吧，代码会简洁直观不少。
Ast* eval(Ast* ast){
	//for function node
	if(ast->getNodetype()=='F'){
		return evalFunction(ast);
	}
	//for assign node, 这家伙还需要用到符号表吧？？用就用吧，反正是全局的。局座。。。
	//你真是把这当弹幕了。。。。
	if(ast->getNodetype()=='A'){
		//人生目标：更新当前符号表
		//取得当前符号表.这两个动作似曾相识在parser里，待我去找来直接copy在这里！
		//更新当前符号表中的符号X（左边的哥们儿）的值为右边的哥们儿的值。
		//要不要给rt和v做个extern声明啊？？
		//$!!这里要注意加意外判断，比如左边是右值。
		rt = ast->getRight()->getReturntype();
		if (rt=='i'){v = eval(ast->getRight())->getIntNumber();}
		else if(rt=='f'){v = eval(ast->getRight())->getFloatNumber();}
		else { throw "What are you doing with the assign node?"; }
		ast->getLeft()->setReturntype(rt);
		ast->getLeft()->set_value(v);
		stlptr->update_symbol(ast->getLeft()->getName(), ast->getLeft());		
	}
	//for print node
	if(ast->getNodetype()=='P'){
		//！！需要operator overload啊
		std::cout<<eval(ast->get_toPrint())<<std::endl;
	}
	//for void node
	if(ast->getNodetype()=='0'){
		//啥也不干可以不？
	}
	//for int return type, char nodetype, int id, char rt, int n)
	if(ast->getReturntype()=='i'){
		return new AstInteger('I', ast->getNodeid(),'i', evalInt(ast) );
	}
	if(ast->getReturntype()=='f'){
		return new AstFloat('K', ast->getNodeid(), 'f', evalFloat(ast) );
	}
}