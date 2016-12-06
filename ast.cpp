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
//For calling a function
Ast* evalCall(Ast* astCall){
	//！！这个函数君的人生目标就是运行一遍函数中的所有语句：
	//目前只包括赋值语句和打印语句。然后返回一个值。如果返回值为空，那就返回一个Ast空空屁。
	//不然就返回一个AstReturn.就用这个颜色一会吧，我打的字比较清楚。
	//发现一个问题，怎样区分函数的定义和调用呢？def fun用新的node？call fun用callNode?
	//所以，有一种新的“对象”时，就可以定义一个新的类，把和她相关的乱七八糟的东西放在一起。
	//如果她和别的类有共同点，就可以用继承，多态来优化代码。加上上面的封装，就是OO三大特征了！
	//这样也方便自顶向下的设计程序。实践增强理解啊！理论是实践的抽象，但并不是实践。
	//开始一个新的scope
	Ast* astReturn;
	std::vector<Ast*> :: iterator it ;
	std::vector<Ast*> vec;	
	stlptr -> push_scope();
	//根据函数名，查表得到要执行的函数的AstFunction,进而得到其suiteNode的vector，
	//然后eval(asts in the vec)，还要记得搞到returnNode。
	// stlptr -> lookup_symbol(astCall->getName()) is a AstFunction* 
	//这里需要加一个查询语句。
	if (!stlptr ->find_symbol(astCall->getName())){ throw std::string("The function is not defined!"); }
	vec = stlptr -> lookup_symbol(astCall->getName()) ->get_suite()->getVec();	
	//遍历astf->vec里的所有AST NODE，用eval()进行逐条执行。
	it = vec.begin();
	while(it!=vec.end()){
		eval(*it);
		++it;
	}
	//这时函数体已经运行完，returnNode应该也已经生成并且存在当前的符号表里了。取一下，谢谢！
	//他的名字叫"return"! ！！注意，这个需要在parser里面完成。TODO@parser
	astReturn = stlptr->lookup_symbol("return");
	//我感觉还得加个赋值Node。AST家族不断发展壮大啊，威胁到地球和平了。。。
	//好像还要用递归，哇嘎嘎，行不行啊，少废话了，快写！挂科了一会。。。
	
	//关闭当前scope
	stlptr->pop_scope();
	return astReturn;
}

//for def a function
//这哥们儿的目标是把一个函数的函数体suite存进它里面，那我还得有个evalSuite啊。。。
Ast* evalFunction(Ast* ast){
	//function 和suite好像不在一个scope里面啊。所以呢，先存function进去，然后，然后就没有然后了。。。
	//那怎么存语句进这个functionNode呢？要在evalFunction之前，把astSuite做好，然后存在astFunction里就行了。
	//这个要在建立AST的时候做，在AST建好后，astSuite也应该。。。应该什么。。。不对了。。。
	//这样办吧，TODO@parser，在完成一个astSuite的生成的时候，把她存到astFunction里。
	//！！TODO涉及了好多指针操作啊，没有memory leakage怎么好意思呢？？
	//更新当前scope的符号表里的function node
	stlptr->update_symbol(ast->getName(), ast);
	return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), ast->getName()));
}

Ast* evalSuite(Ast* ast){
	return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), ast->getName()));
}

//！！这家伙好像一个皮包公司啊。。。
//！！怎样不用皮包+多条件判断？用polymophism？把返回类型等属性封装到类属性里，返回一个类的指针就行。
//！！后面儿有时间再重构吧，代码会简洁直观不少。
Ast* eval(Ast* ast){
       std:: cout<< "IN eval( )"<< std::endl;
       std:: cout<< "node type:    "<< ast->getNodetype()      << std::endl;
       std:: cout<< "return type:    "<<   ast->getReturntype()   << std::endl;
	//for call function node
	if(ast->getNodetype()=='C'){
		std:: cout<< "evalCall(ast);"<< std::endl;
		return evalCall(ast);
	}
	//for def function node
	if(ast->getNodetype()=='F'){
		std:: cout<< "evalFunction(ast);"<< std::endl;
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
		if (rt=='i')          {v = eval(ast->getRight())->getIntNumber();}
		else if(rt=='f')   {v = eval(ast->getRight())->getFloatNumber();}
		else                 { throw "What are you doing with the assign node?"; }
		ast->getLeft()->setReturntype(rt);
		ast->getLeft()->set_value(v);
		stlptr->update_symbol(ast->getLeft()->getName(), ast->getLeft());
		std:: cout<< "return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), ast->getName()));"<< std::endl;
		return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), "ass"));
	}
	//for print node
	if(ast->getNodetype()=='P'){
		//需要operator overload啊,已经加了，安。。。
		std::cout<<eval(ast->get_toPrint())<<std::endl;
		std:: cout<< "ast->getNodetype()=='P'"<< std::endl;
		return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(),"PPP"));
	}
	//for void node
	if(ast->getNodetype()=='0'){
		//啥也不干可以不？原物奉还！空空屁来，空空屁回。
		return ast;
	}
	//for int return type, char nodetype, int id, char rt, int n)
      //!!看起来是对于AstNode进行运算的，检查
	if(ast->getReturntype()=='i'){
		std:: cout<< "return new AstInteger('I', ast->getNodeid(),'i', evalInt(ast) );"<< std::endl;
		return new AstInteger('I', ast->getNodeid(),'i', evalInt(ast) );
	}
	if(ast->getReturntype()=='f'){
		std:: cout<< "return new AstFloat('K', ast->getNodeid(), 'f', evalFloat(ast) );"<< std::endl;
		return new AstFloat('K', ast->getNodeid(), 'f', evalFloat(ast) );
	}
	else 
		{return (new AstVoid('0', ast->getNodeid(), ast->getReturntype(), ast->getName()));}
}

//operator overload is coming
std::ostream& operator <<(std::ostream& os, const Ast * ast)
{
	std:: cout<< "IN std::ostream& operator <<(std::ostream& os, const Ast * ast)"<< std::endl;
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
                  os<< stlptr->lookup_symbol(ast->getName())->get_value();
                  // os<< (ast->getName());
			break;
		case '0':
			break;
		//！！看起来目前不用考虑打印其他类型的东东。
	}
	return os;
}
