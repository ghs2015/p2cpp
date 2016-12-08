//  Declarations for an AST calculator
//  From "flex & bison", fb3-1, by John Levine
//  Adapted by Brian Malloy
//  Further adapted by Zhang Xiang
#ifndef _AST_H_
#define _AST_H_
#include <iostream>
#include <string>
#include <vector>
#include <list>
extern void yyerror(const char*);
extern void yyerror(const char*, const char);

class Ast{

public:
	Ast(char n, int id, char rt) : nodetype(n), nodeid(id), returntype(rt) {}
	virtual ~Ast() {}
	char getNodetype() const { return nodetype; }
	void setNodetype(char nt)  { nodetype = nt; }
	int getNodeid() const { return nodeid;}
	char getReturntype() const { return returntype;}
	void setReturntype(char nrt) {returntype = nrt;}

	virtual Ast* getLeft() const { throw std::string("No Left"); }
	virtual Ast* getRight() const { throw std::string("No Right"); }
	virtual double getFloatNumber() const { throw std::string("No Float Number"); }
	virtual int getIntNumber() const { throw std::string("No Int Number"); }
	virtual std::string getName() const { throw std::string("No Name"); }
	virtual std::string getFunction() const { throw std::string("No Function"); }

	virtual void set_value(double v) {throw std::string("No setter");}
	virtual double get_value() const {throw std::string("No Value");}
	virtual std::list<Ast*> getVec() const {throw std::string("No Vector"); }
	virtual void push_back(Ast* ast) {throw std::string("No push_back()"); }
	virtual void push_front(Ast* ast) {throw std::string("No push_front()");}
	virtual void print_ast() { throw std::string("No print_ast()"); }
	virtual Ast* get_toPrint() { throw std::string("No toPrint"); }
	virtual Ast* get_suite() const { throw std::string("No suite");  }
	virtual void set_suite(Ast* ast) {throw std::string("No set_suite"); }

	virtual Ast* get_return() const {throw std::string("No return");}
	virtual void set_return(Ast* ast){throw std::string("No set_return");}
private:
	char nodetype;
	int nodeid;
	char returntype;
};

class AstNode : public Ast {
public:
	AstNode(char nodetype, int id, char rt, Ast* l, Ast* r) : Ast(nodetype, id, rt), left(l), right(r) {}
	virtual ~AstNode() {}
	virtual Ast* getLeft() const  { return left; }
	virtual Ast* getRight() const { return right; }

private:
	Ast *left;
	Ast *right;
};

class AstFloat : public Ast {
public:
	AstFloat(char nodetype, int id, char rt, double n) : Ast(nodetype, id, rt), number(n) {} 
	virtual ~AstFloat() {}
	virtual double getFloatNumber() const { return number; }
private:
	double number;
};

class AstInteger : public Ast {
public:
	AstInteger(char nodetype, int id, char rt, int n) : Ast(nodetype, id, rt), number(n) {} 
	virtual ~AstInteger() {}
	virtual int getIntNumber() const { return number; }
private:
	int number;
};

class AstName : public Ast {
public:
	AstName(char nodetype, int id, char rt, std::string n, double v) : Ast(nodetype,id,rt), name(n),value(v){}
	virtual ~AstName(){}
	virtual std::string getName() const { return name; }
	virtual void set_value(double v){value = v;}
	virtual double get_value() const {return value;}
private:
	std::string name;
	double value;
};
//!!Call node serves for updating the symbol table,nodetype='F'
class AstFunction : public Ast {
public:
	AstFunction(char nodetype, int id, char rt, std::string n, Ast* ast) : Ast(nodetype,id,rt), name(n), astSuite(ast){}
	virtual ~AstFunction(){}
	virtual std::string getName() const { return name; }
	virtual Ast* get_suite() const { return astSuite; }
	virtual void set_suite(Ast* ast) {astSuite=ast; }

private:
	std::string name;
	  //是一个一个往里放呢？还是一起往里放？一个一个吧。现在直接直接整块放suiteNode了。
	Ast* astSuite; //!!把赋值语句和打印语句都放在一起，这样按顺序执行就不会有XX影响
};

//!!Call node serves for revieval, nodetype='C'
class AstCall : public Ast {
public:
	AstCall(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	virtual ~AstCall(){}
	virtual std::string getName() const { return name; }	
private:
	std::string name;
};

//AstPrint 
class AstPrint : public Ast {
public:
	AstPrint(char nodetype,int id,char rt,std::string n,Ast* ast) : Ast(nodetype,id,rt), name(n),toPrint(ast) {}
	virtual ~AstPrint(){}
	virtual Ast* get_toPrint() {return toPrint;}
  //virtual void print_ast() { std::cout<<eval(toPrint)<<std:endl; }
	virtual std::string getName() const { return name; }
private:
	std::string name;
	Ast* toPrint;
};
//!! AstAss To validate
class AstAss: public Ast {
public:
	AstAss(char nodetype,int id,char rt, std::string n, Ast* la, Ast* ra):	Ast(nodetype,id,rt), name(n),l(la),r(ra) {}
	virtual ~AstAss(){}
	virtual Ast* getLeft() const  { return l; }
	virtual Ast* getRight() const { return r; }
	virtual std::string getName() const { return name; }
private:
	std::string name;
	Ast* l;
	Ast* r;
};

//AstSuite To do
	//store the stmts in a suite by a vector
class AstSuite : public Ast {
public:
	// AstSuite(char nodetype, int id, char rt, std::string n, std::vector<Ast*> v) : Ast(nodetype,id,rt), name(n), vec(v){}
	AstSuite(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	virtual ~AstSuite(){}
	virtual std::string getName() const { return name; }
	 
	virtual std::list<Ast*> getVec() const { return vec;}
	virtual void push_back(Ast* ast) {vec.push_back(ast); std::cout<<"Pushing once."<<std::endl;}
	virtual void push_front(Ast* ast) {vec.push_front(ast); std::cout<<"Pushing front once."<<std::endl;}
	virtual Ast* get_return() const {return astReturn;}
	virtual void set_return(Ast* ast){astReturn = ast;}
private:
	std::string name;
	Ast* astReturn;
	std::list<Ast*> vec;
};

//AstReturn, Name is reserved as "return"
class AstReturn : public Ast {
public:
	AstReturn(char nodetype, int id, char rt, std::string n, double v) : Ast(nodetype,id,rt), name(n),value(v){}
	virtual ~AstReturn(){}
	virtual std::string getName() const { return name; }
	virtual void set_value(double v){value = v;}
	virtual double get_value() const {return value;}
private:
	std::string name;
	double value;
};

//AstVoid To do, name reserved as void, or nodetype as 'O'
class AstVoid : public Ast {
public:
	AstVoid(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	virtual ~AstVoid(){}
	virtual std::string getName() const { return name; }
private:
	std::string name;
};

//!!AstNone 怎么用啊？！
class AstNone : public Ast{
public:
	AstNone(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	virtual ~AstNone(){}
	virtual std::string getName() const { return name; }
private:
	std::string name;
};

//!!这里可以统一用一个evalAST~1h
double evalFloat(Ast*);
int    evalInt(Ast*);
//！！要定义eval啊... 知道了啊。。。
Ast*   eval(Ast*);
Ast*   evalFunction(AstFunction*);//！！用Ast作为返回值，可以想存啥，就存啥！早咋不这么用呢？。。。
Ast*   evalSuite(Ast* ast);
Ast*   evalCall(Ast* ast);
Ast*   evalPrint(Ast* ast);
void   treeFree(Ast*); // delete and free an AST 
Ast*   deepcopy(Ast* ast);

std::ostream& operator <<(std::ostream& os, const Ast * ast);

#endif
