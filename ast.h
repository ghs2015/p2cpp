//  Declarations for an AST calculator
//  From "flex & bison", fb3-1, by John Levine
//  Adapted by Brian Malloy
//  Further adapted by Zhang Xiang
#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <vector>
extern void yyerror(const char*);
extern void yyerror(const char*, const char);

class Ast{

	public:
	  Ast(char n, int id, char rt) : nodetype(n), nodeid(id), returntype(rt) {}
	  virtual ~Ast() {}
	  char getNodetype() const { return nodetype; }
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
	  virtual std::vector<Ast*> getVec() const {throw std::string("No Vector"); }
	  virtual void push_back(Ast* ast) {throw std::string("No push_back()"); }
	  virtual void print_ast() { throw std::string("No print_ast()"); }
	  virtual Ast* get_toPrint() { throw std::string("No toPrint"); }
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

class AstFunction : public Ast {
	public:
	  AstFunction(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	  virtual ~AstFunction(){}
	  virtual std::string getName() const { return name; }
	  virtual std::vector<Ast*> getVec() const {return vec; }
	  virtual void push_back(Ast* ast) {vec.push_back(ast);}
	private:
	  std::string name;
	  //是一个一个往里放呢？还是一起往里放？一个一个吧。
	  std::vector<Ast*> vec; //!!把赋值语句和打印语句都放在一起，这样按顺序执行就不会有XX影响
};

class AstPrint : public Ast {
    public:
	  AstPrint(char nodetype,int id,char rt,std::string n,Ast* ast) : Ast(nodetype,id,rt), name(n),toPrint(ast) {}
	  virtual ~AstPrint(){}
	  //!!这要对<<进行操作符重载啊。。。~0.5h
	  //！！还是再eval()里面print吧
	  //virtual void print_ast() { std::cout<<eval(toPrint)<<std:endl; }
	private:
	  std::string name;
	  Ast* toPrint;
};
//!! To validate
class AstAss: public Ast {
    public:
	  AstAss(char nodetype,int id,char rt,\
	  	       std::string n, Ast* la, Ast* ra): \
	                Ast(nodetype,id,rt), name(n),l(la),r(ra) {}
	  virtual ~AstAss(){}
	  virtual Ast* getLeft() const  { return l; }
	  virtual Ast* getRight() const { return r; }
	private:
	  std::string name;
	  Ast* l;
	  Ast* r;
};


//!!To do
class AstSuite : public Ast {
	public:
	  AstSuite(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	  virtual ~AstSuite(){}
	  virtual std::string getSuite() const { return name; }
	private:
	  std::string name;
};
//!!To do
class AstReturn : public Ast {
	public:
	  AstReturn(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	  virtual ~AstReturn(){}
	  virtual std::string getReturn() const { return name; }
	private:
	  std::string name;
};
//!!To do
class AstCall : public Ast {
	public:
	  AstCall(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	  virtual ~AstCall(){}
	  virtual std::string getCall() const { return name; }
	private:
	  std::string name;
};

//!!To do
class AstVoid : public Ast {
	public:
	  AstVoid(char nodetype, int id, char rt, std::string n) : Ast(nodetype,id,rt), name(n){}
	  virtual ~AstVoid(){}
	  virtual std::string getVoid() const { return name; }
	private:
	  std::string name;
};

//!!这里可以统一用一个evalAST~1h
double evalFloat(Ast*);
int evalInt(Ast*);
//！！要定义eval啊
Ast* eval(Ast*);
Ast* evalFunction(AstFunction*);//！！用Ast作为返回值，可以想存啥，就存啥！早咋不这么用呢？。。。
void treeFree(Ast*); // delete and free an AST 





#endif
// functionNode,
// suiteNode, suite 是什么神？
// returnNode, 
// NoneNode,
// callNode
// voidNode