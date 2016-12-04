// Generated by transforming |cwd:///parser/2.7.2.y| on 2016-10-15 at 17:11:42 +0000
%{
  #include <iostream>
  #include <cmath>
  #include <math.h>
  #include <cassert>
  #include "STLeader.h"
  // #include "SymbolTable.h"
  #include "ast.h"
  #include <iomanip>
  
	//class Ast;
    int nodeid = 0; //Give each node a unique id 
    char rt; //Return type
    char nt; //Node type
    double v; //value of the variable

    int yylex (void);
    extern int yylineno;
    extern char *yytext;
    void yyerror (char const *);
    //!!??
    STLeader* stlptr = STLeader::get_stlptr();
    // stlptr->set_scope(0);
    // SymbolTable st(0);
    // stlptr->add_st(st);
    %}

//ZX+:Define types for the yylval
    %union {
    	Ast* ast;
	//double d;
    	int i;
    }

// 2 tokens, INTEGER and FLOAT X+
//%token <d> FLOATNUMBER
//%token <i> INTEGER

// 83 tokens, in alphabetical order:
    %token AMPEREQUAL AMPERSAND AND AS ASSERT AT BACKQUOTE BAR BREAK
    %token CIRCUMFLEX CIRCUMFLEXEQUAL CLASS COLON COMMA CONTINUE DEDENT
    %token DEF DEL DOT DOUBLESLASH DOUBLESLASHEQUAL 
    %token DOUBLESTAR DOUBLESTAREQUAL ELIF ELSE ENDMARKER EQEQUAL
    %token EQUAL EXCEPT EXEC FINALLY FOR FROM GLOBAL GREATER GREATEREQUAL GRLT
    %token IF IMPORT IN INDENT IS LAMBDA LBRACE LEFTSHIFT LEFTSHIFTEQUAL LESS
    %token LESSEQUAL LPAR LSQB MINEQUAL MINUS NAME NEWLINE NOT NOTEQUAL 
    %token OR PASS PERCENT PERCENTEQUAL PLUS PLUSEQUAL PRINT RAISE 
    %token RBRACE RETURN RIGHTSHIFT RIGHTSHIFTEQUAL RPAR RSQB 
    %token SEMI SLASH SLASHEQUAL STAR STAREQUAL
    %token STRING TILDE TRY VBAREQUAL WHILE WITH 
    %token FLOATNUMBER INTEGER YIELD plus_STRING
    %start start

//declare the types of tokens involved in the calculation. ZX+
//print_stmt
    %type <i> pick_unop pick_PLUS_MINUS pick_multop augassign 
    %type <ast> pick_yield_expr_testlist pick_yield_expr_testlist_comp opt_yield_test testlist_comp testlist
    %type <ast> opt_test test or_test and_test not_test comparison 
    %type <ast> expr xor_expr and_expr shift_expr arith_expr 
    %type <ast> term factor power atom listmaker dictorsetmaker 
    %type <ast> expr_stmt star_EQUAL testlist1 plus_STRING
    %type <ast> yield_expr lambdef opt_listmaker opt_dictorsetmaker 
    %%

    start
    : file_input
    | single_input
    ;
//single_input // Used in: start
//	: NEWLINE
//	| simple_stmt
//	| compound_stmt NEWLINE
//	;

single_input // Used in: start
: NEWLINE
| stmt 
;
file_input // Used in: start
: star_NEWLINE_stmt ENDMARKER
;
pick_NEWLINE_stmt // Used in: star_NEWLINE_stmt
: NEWLINE
| stmt
;
star_NEWLINE_stmt // Used in: file_input, star_NEWLINE_stmt
: pick_NEWLINE_stmt star_NEWLINE_stmt
| %empty
;
decorator // Used in: decorators
: AT dotted_name LPAR opt_arglist RPAR NEWLINE
| AT dotted_name NEWLINE
;
opt_arglist // Used in: decorator, trailer
: arglist
| %empty
;
decorators // Used in: decorators, decorated
: decorator decorators
| decorator
;
decorated // Used in: compound_stmt
: decorators classdef
| decorators funcdef
;
funcdef // Used in: decorated, compound_stmt
: DEF NAME parameters COLON suite
;
parameters // Used in: funcdef
: LPAR varargslist RPAR 
| LPAR RPAR 
;
varargslist // Used in: parameters, old_lambdef, lambdef
: star_fpdef_COMMA pick_STAR_DOUBLESTAR
| fpdef opt_EQUAL_test star_COMMA_fpdef
;
opt_EQUAL_test // Used in: varargslist, star_fpdef_COMMA, star_COMMA_fpdef
: EQUAL test
| %empty
;
star_fpdef_COMMA // Used in: varargslist, star_fpdef_COMMA
: fpdef opt_EQUAL_test COMMA star_fpdef_COMMA
| %empty
;
opt_DOUBLESTAR_NAME // Used in: pick_STAR_DOUBLESTAR
: COMMA DOUBLESTAR NAME
| %empty
;
pick_STAR_DOUBLESTAR // Used in: varargslist
: STAR NAME opt_DOUBLESTAR_NAME
| DOUBLESTAR NAME
;
fpdef // Used in: varargslist, star_fpdef_COMMA, fplist, star_fpdef_notest, star_COMMA_fpdef
: NAME
| LPAR fplist RPAR 
;
fplist // Used in: fpdef
: fpdef star_fpdef_notest
;
stmt // Used in: pick_NEWLINE_stmt, plus_stmt
: simple_stmt 
| compound_stmt
;
simple_stmt // Used in: single_input, stmt, suite
: small_stmt small_stmt_STAR_OR_SEMI NEWLINE
;
small_stmt // Used in: simple_stmt, small_stmt_STAR_OR_SEMI
: expr_stmt 
| print_stmt
| del_stmt
| pass_stmt
| flow_stmt
| import_stmt
| global_stmt
| exec_stmt
| assert_stmt
;
expr_stmt // Used in: small_stmt
: testlist augassign pick_yield_expr_testlist 
{

	if ($1->getNodetype()=='N')
	{
		//!!Build a ASTNODE for the RHS. ~40min.
		rt=($1->getReturntype()=='i'&&$3->getReturntype()=='i'?'i':'f');
		switch($2)
		{
			case PLUSEQUAL: nt='+';        break;
			case MINEQUAL: nt='-';         break;
			case STAREQUAL: nt='*';        break;
			case SLASHEQUAL: nt='/';       break;
			case PERCENTEQUAL: nt='%';     break;
			case DOUBLESLASHEQUAL: nt='d'; break;
		}

		Ast* rhsAst = new AstNode(nt, nodeid++,rt,$1,$3);
		if (rt=='i')        { v = evalInt(rhsAst);    }
		else if(rt == 'f')	{ v = evalFloat(rhsAst); }
		else { throw "NameError: the name is not defined"; }
		//因为这个变量在这种情况下一定存在，所以更新下就好。
		stlptr->lookup_symbol($1->getName())->setReturntype(rt);
		stlptr->lookup_symbol($1->getName())->set_value(v);		
	}
	else if($1->getNodetype()=='K'||$1->getNodetype()=='I')
	{
		throw "SyntaxError: can't assign to literal";
	}			   



}
| testlist star_EQUAL
{    
		//std::cout<<"In testlist star_EQUAL"<<std::endl;
		//if ($2->getReturntype()=='u') {std::cout<<"HAHAHAHAHA ~!"<<std::endl;}
		// SymbolTable* stptr = SymbolTable::get_table();
		if($1->getNodetype()=='N')  //Handle the exception.
		//update_symbol_info(string name, string type, string value);
		{
			rt = $2->getReturntype();

			if (rt=='i')        { v =evalInt($2);    }
			else if(rt == 'f')	{ v = evalFloat($2); }
			else { throw "NameError: the name is not defined"; }

			$1->setReturntype(rt);
			$1->set_value(v);
						
		}
		else if($1->getNodetype()=='K'||$1->getNodetype()=='I')
		{
			throw "SyntaxError: can't assign to literal";
		}
	}	
pick_yield_expr_testlist // Used in: expr_stmt, star_EQUAL
: yield_expr
| testlist {$$ = $1;}
;
star_EQUAL // Used in: expr_stmt, star_EQUAL
: EQUAL pick_yield_expr_testlist star_EQUAL 
{
	if ($2->getNodetype()=='N'&&$2->getReturntype()=='u')
	{			
		$2 = $3;
		$$ = $2;
	}
	else 
	{
		$$ = $2;
	}		
}
| %empty 
{    
	//!!TO PROCESS
	$$ = new AstName('N', nodeid++, 'u', yytext, 0); 
	std::cout<<"%empty: "<< yytext <<std::endl;
}
;
augassign // Used in: expr_stmt
: PLUSEQUAL {$$ = PLUSEQUAL;}
| MINEQUAL {$$ = MINEQUAL;}
| STAREQUAL {$$ = STAREQUAL;}
| SLASHEQUAL {$$ = SLASHEQUAL;}
| PERCENTEQUAL {$$ = PERCENTEQUAL;}
| AMPEREQUAL {$$ = AMPEREQUAL;}
| VBAREQUAL {$$ = VBAREQUAL;}
| CIRCUMFLEXEQUAL {$$ = CIRCUMFLEXEQUAL;}
| LEFTSHIFTEQUAL {$$ = LEFTSHIFTEQUAL;}
| RIGHTSHIFTEQUAL {$$ = RIGHTSHIFTEQUAL;}
| DOUBLESTAREQUAL {$$ = DOUBLESTAREQUAL;}
| DOUBLESLASHEQUAL {$$ = DOUBLESLASHEQUAL;}
;
print_stmt // Used in: small_stmt
: PRINT opt_test
{
	if ($2->getNodetype()=='N'&&$2->getReturntype()=='u') {throw "NameError: the name is not defined";}
    	//std::cout<<$2<<std::endl;
	if ($2->getReturntype()=='i'){
    		//std::cout << "evalInt($2)" << std::endl;
		std::cout << ">>> " << evalInt($2) << std::endl;
	}
	else if ($2->getReturntype()=='f'){
    		//std::cout << "evalFloat($2)" << std::endl;
		std::cout << ">>> " <<std::defaultfloat << std::setprecision(12) << evalFloat($2) << std::endl;
	}

	treeFree($2);    	
	//!!后面再考虑这个内存管理的问题
	//SymbolTable::free_table();
    	//std::cout << ">>> ";
}
| PRINT RIGHTSHIFT test opt_test_2
;
opt_test // Used in: print_stmt
: test star_COMMA_test {$$ = $1;}
| %empty {}
;
opt_test_2 // Used in: print_stmt
: plus_COMMA_test
| %empty
;
del_stmt // Used in: small_stmt
: DEL exprlist
;
pass_stmt // Used in: small_stmt
: PASS
;
flow_stmt // Used in: small_stmt
: break_stmt
| continue_stmt
| return_stmt
| raise_stmt
| yield_stmt
;
break_stmt // Used in: flow_stmt
: BREAK
;
continue_stmt // Used in: flow_stmt
: CONTINUE
;
return_stmt // Used in: flow_stmt
: RETURN testlist
| RETURN
;
yield_stmt // Used in: flow_stmt
: yield_expr
;
raise_stmt // Used in: flow_stmt
: RAISE test opt_test_3
| RAISE
;
opt_COMMA_test // Used in: opt_test_3, exec_stmt
: COMMA test
| %empty
;
opt_test_3 // Used in: raise_stmt
: COMMA test opt_COMMA_test
| %empty
;
import_stmt // Used in: small_stmt
: import_name
| import_from
;
import_name // Used in: import_stmt
: IMPORT dotted_as_names
;
import_from // Used in: import_stmt
: FROM star_DOT dotted_name IMPORT pick_STAR_import
| FROM plus_DOT IMPORT pick_STAR_import
;
star_DOT // Used in: import_from, star_DOT
: DOT star_DOT
| %empty
;
plus_DOT // Used in: import_from, plus_DOT
: DOT plus_DOT
| DOT
;
pick_STAR_import // Used in: import_from
: STAR
| LPAR import_as_names RPAR
| import_as_names
;
import_as_name // Used in: import_as_names, star_COMMA_import_as_name
: NAME AS NAME
| NAME
;
dotted_as_name // Used in: dotted_as_names
: dotted_name AS NAME
| dotted_name
;
import_as_names // Used in: pick_STAR_import
: import_as_name star_COMMA_import_as_name
;
dotted_as_names // Used in: import_name, dotted_as_names
: dotted_as_name
| dotted_as_names COMMA dotted_as_name
;
dotted_name // Used in: decorator, import_from, dotted_as_name, dotted_name
: NAME
| dotted_name DOT NAME
;
global_stmt // Used in: small_stmt, global_stmt
: global_stmt COMMA NAME
| GLOBAL NAME
;
exec_stmt // Used in: small_stmt
: EXEC expr IN test opt_COMMA_test
| EXEC expr
;
assert_stmt // Used in: small_stmt
: ASSERT test COMMA test
| ASSERT test
;
compound_stmt // Used in: single_input, stmt
: if_stmt
| while_stmt
| for_stmt
| try_stmt
| with_stmt
| funcdef
| classdef
| decorated
;
if_stmt // Used in: compound_stmt
: IF test COLON suite star_ELIF ELSE COLON suite
| IF test COLON suite star_ELIF
;
star_ELIF // Used in: if_stmt, star_ELIF
: ELIF test COLON suite star_ELIF
| %empty
;
while_stmt // Used in: compound_stmt
: WHILE test COLON suite ELSE COLON suite
| WHILE test COLON suite
;
for_stmt // Used in: compound_stmt
: FOR exprlist IN testlist COLON suite ELSE COLON suite
| FOR exprlist IN testlist COLON suite
;
try_stmt // Used in: compound_stmt
: TRY COLON suite plus_except opt_ELSE opt_FINALLY
| TRY COLON suite FINALLY COLON suite
;
plus_except // Used in: try_stmt, plus_except
: except_clause COLON suite plus_except
| except_clause COLON suite
;
opt_ELSE // Used in: try_stmt
: ELSE COLON suite
| %empty
;
opt_FINALLY // Used in: try_stmt
: FINALLY COLON suite
| %empty
;
with_stmt // Used in: compound_stmt
: WITH with_item star_COMMA_with_item COLON suite
;
star_COMMA_with_item // Used in: with_stmt, star_COMMA_with_item
: COMMA with_item star_COMMA_with_item
| %empty
;
with_item // Used in: with_stmt, star_COMMA_with_item
: test AS expr
| test
;
except_clause // Used in: plus_except
: EXCEPT test opt_AS_COMMA
| EXCEPT
;
pick_AS_COMMA // Used in: opt_AS_COMMA
: AS
| COMMA
;
opt_AS_COMMA // Used in: except_clause
: pick_AS_COMMA test
| %empty
;
suite // Used in: funcdef, if_stmt, star_ELIF, while_stmt, for_stmt, 
      // try_stmt, plus_except, opt_ELSE, opt_FINALLY, with_stmt, classdef
: simple_stmt
| NEWLINE INDENT plus_stmt DEDENT
;
plus_stmt // Used in: suite, plus_stmt
: stmt plus_stmt
| stmt
;
testlist_safe // Used in: list_for
: old_test plus_COMMA_old_test
| old_test
;
old_test // Used in: testlist_safe, old_lambdef, list_if, comp_if, plus_COMMA_old_test
: or_test
| old_lambdef
;
old_lambdef // Used in: old_test
: LAMBDA varargslist COLON old_test
| LAMBDA COLON old_test
;
test // Used in: opt_EQUAL_test, print_stmt, opt_test, raise_stmt, 
     // opt_COMMA_test, opt_test_3, exec_stmt, assert_stmt, if_stmt, 
     // star_ELIF, while_stmt, with_item, except_clause, opt_AS_COMMA, 
     // opt_IF_ELSE, listmaker, testlist_comp, lambdef, subscript, 
     // opt_test_only, sliceop, testlist, dictorsetmaker, argument, 
     // testlist1, star_COMMA_test, star_test_COLON_test,
     // plus_COMMA_test, dictarg, listarg
: or_test opt_IF_ELSE {$$ = $1;}
| lambdef
;
opt_IF_ELSE // Used in: test
: IF or_test ELSE test
| %empty
;
or_test // Used in: old_test, test, opt_IF_ELSE, or_test, comp_for
: and_test {$$ = $1;}
| or_test OR and_test
;
and_test // Used in: or_test, and_test
: not_test {$$ = $1;}
| and_test AND not_test
;
not_test // Used in: and_test, not_test
	: NOT not_test {$$ = $2;} //!!TO MODIFY
	| comparison {$$ = $1;}
	;
comparison // Used in: not_test, comparison
: expr {$$ = $1;}
| comparison comp_op expr
;
comp_op // Used in: comparison
: LESS
| GREATER
| EQEQUAL
| GREATEREQUAL
| LESSEQUAL
| GRLT
| NOTEQUAL
| IN
| NOT IN
| IS
| IS NOT
;
expr // Used in: exec_stmt, with_item, comparison, expr, 
     // exprlist, star_COMMA_expr
: xor_expr {$$ = $1;}
| expr BAR xor_expr
;
xor_expr // Used in: expr, xor_expr
: and_expr {$$ = $1;}
| xor_expr CIRCUMFLEX and_expr
;
and_expr // Used in: xor_expr, and_expr
: shift_expr {$$ = $1;}
| and_expr AMPERSAND shift_expr
;
shift_expr // Used in: and_expr, shift_expr
: arith_expr
{
	$$ = $1;
}
| shift_expr pick_LEFTSHIFT_RIGHTSHIFT arith_expr
;
pick_LEFTSHIFT_RIGHTSHIFT // Used in: shift_expr
: LEFTSHIFT
| RIGHTSHIFT
;
arith_expr // Used in: shift_expr, arith_expr
: term {$$ = $1;}
| arith_expr pick_PLUS_MINUS term
{
	rt=($1->getReturntype()=='i'&&$3->getReturntype()=='i'?'i':'f');
	switch($2)
	{
			//case PLUS: $$ = $1 + $3; break;
			//case MINUS: $$ = $1 - $3; break;
		case PLUS:  $$ = new AstNode('+', nodeid++, rt, $1, $3); 
		break;
		case MINUS: $$ = new AstNode('-', nodeid++, rt,$1, $3); 
		break;
	}
}
;
pick_PLUS_MINUS // Used in: arith_expr
: PLUS {$$ = PLUS;}
| MINUS {$$ = MINUS;}
;
term // Used in: arith_expr, term
: factor {$$ = $1;}
	| term pick_multop factor //TO ADD AST VERSION!!
	{
		rt=($1->getReturntype()=='i'&&$3->getReturntype()=='i'?'i':'f');
		switch($2)
		{
			case STAR:        $$ = new AstNode('*', nodeid++, rt,$1,$3); 
			break;
			case SLASH:	      $$ = new AstNode('/', nodeid++, rt,$1,$3); 
			break;
			case PERCENT:     $$ = new AstNode('%', nodeid++, rt,$1,$3); 
			break;
			case DOUBLESLASH: $$ = new AstNode('d', nodeid++, rt,$1,$3); 
			break;
		}		
		// switch($2)
		// {
		// 	case STAR: $$ = $1 * $3; break;
		// 	case SLASH:            
		// 	           if($3==0) {std::cout<<"ZeroDivisionError"<<std::endl; $$=0; break;}
		// 	           else if ($1*$3>=0) {$$=$1/$3; break;}
		// 	           else if ($1*$3<0) {$$=floor(1.0*$1/$3); break;}
		// 	case PERCENT: 
		//                if($3==0) {std::cout<<"ZeroDivisionError"<<std::endl; $$=0; break;}
		// 	           else if ($1*$3>=0) {$$=$1%$3; break;}
		// 	           else if ($1<0) {$$ = $1 - $3*floor(1.0*$1/$3); break;}
		// 	           else if ($3<0) {$$ = $1 - $3*floor(1.0*$1/$3); break;}
		// 	case DOUBLESLASH: 
		// 			   if($3==0) {std::cout<<"ZeroDivisionError"<<std::endl; $$=0; break;}
		// 	           else if ($1*$3>=0) {$$=$1/$3; break;}
		// 	           else if ($1*$3<0) {$$=floor(1.0*$1/$3); break;}
		// }
	}
	;
pick_multop // Used in: term
: STAR {$$ = STAR;}
| SLASH {$$ = SLASH;}
| PERCENT {$$ = PERCENT;}
| DOUBLESLASH {$$ = DOUBLESLASH;}
;
factor // Used in: term, factor, power
: pick_unop factor 
{
	switch($1)
	{
		case PLUS: $$ = $2; break;
			//case MINUS: $$ = -$2; break; //TO ADD AST!!
		case MINUS: {
				//std::cout<<"return type is:  "<<$2->getReturntype()<<std::endl;
				//std::cout<<"node type is:  "<<$2->getNodetype()<<std::endl;
			rt = $2->getReturntype(); $$ = new AstNode('M', nodeid++, rt, $2, NULL); 
			break;
		}
			//case TILDE: $$ = (-$2) - 1; break; //TO ADD AST!!
	}
}
| power {$$ = $1;}
;
pick_unop // Used in: factor
: PLUS {$$ = PLUS;}
| MINUS {$$ = MINUS;}
| TILDE {$$ = TILDE;}
;
power // Used in: factor
: atom star_trailer DOUBLESTAR factor 
{
	rt = ($1->getReturntype()=='i'&&$4->getReturntype()=='i'?'i':'f');
	if(rt=='i'&&evalInt($4)<0){
		rt = 'f';
			//std::cout<<evalInt($4)<<std::endl;
	}
		//std::cout<<rt<<std::endl;
	$$ = new AstNode('^', nodeid++, rt,$1,$4);
}
| atom star_trailer
;
star_trailer // Used in: power, star_trailer
: trailer star_trailer
| %empty
;
atom // Used in: power //TO ADD AST VERSION.!!
: LPAR opt_yield_test RPAR {$$ = $2;}
	| LSQB opt_listmaker RSQB {$$ = $2;} //!!TO MODIFY
	| LBRACE opt_dictorsetmaker RBRACE {$$ = $2;} //!!TO MODIFY
	| BACKQUOTE testlist1 BACKQUOTE {$$ = $2;} //!!TO MODIFY
	| NAME { 
		//！！应该在这里做查询符号，而不是上面，因为那时候已经有了	。。。
		STLeader* stlptr = STLeader::get_stlptr();
		if((stlptr->find_symbol(yytext))>=0)
		{
		       	  //!!这里可能需要用deep copy~0.5h 返回一个新Node.现场来一个。
			rt = (stlptr->lookup_symbol(yytext))->getReturntype();
			v =  (stlptr->lookup_symbol(yytext))->get_value();
			$$ = new AstName('N', nodeid++, rt, yytext, v);
		}
		else
		{
			$$ = new AstName('N', nodeid++, 'u', yytext, 0); }
		}		     		      
		| FLOATNUMBER {$$ = new AstFloat('K', nodeid++, 'f', atof(yytext)); } 
		| INTEGER {$$ = new AstInteger('I', nodeid++, 'i', atoi(yytext)); } 
	| plus_STRING {$$ = $1;} //!!TO MODIFY
	;
pick_yield_expr_testlist_comp // Used in: opt_yield_test
: yield_expr
| testlist_comp {$$ = $1;}
;
opt_yield_test // Used in: atom
: pick_yield_expr_testlist_comp {$$ = $1;}
| %empty {}
;
opt_listmaker // Used in: atom
: listmaker
| %empty {}
;
opt_dictorsetmaker // Used in: atom
: dictorsetmaker
| %empty {}
;
// plus_STRING // Used in: atom, plus_STRING
// 	: STRING plus_STRING {$$=$2;} //!!TO MODIFY
// 	| STRING
// 	;
listmaker // Used in: opt_listmaker
: test list_for
| test star_COMMA_test
;
testlist_comp // Used in: pick_yield_expr_testlist_comp
: test comp_for
| test star_COMMA_test {$$ = $1;}
;
lambdef // Used in: test
	: LAMBDA varargslist COLON test {$$=$4;} //!!TO MODIFY
	| LAMBDA COLON test {$$=$3;} //!!TO MODIFY
	;
trailer // Used in: star_trailer
: LPAR opt_arglist RPAR 
| LSQB subscriptlist RSQB
| DOT NAME
;
subscriptlist // Used in: trailer
: subscript star_COMMA_subscript
;
subscript // Used in: subscriptlist, star_COMMA_subscript
: DOT DOT DOT
| test
| opt_test_only COLON opt_test_only opt_sliceop
;
opt_test_only // Used in: subscript
: test 
| %empty
;
opt_sliceop // Used in: subscript
: sliceop
| %empty
;
sliceop // Used in: opt_sliceop
: COLON test
| COLON
;
exprlist // Used in: del_stmt, for_stmt, list_for, comp_for
: expr star_COMMA_expr
;
testlist // Used in: expr_stmt, pick_yield_expr_testlist, 
         // return_stmt, for_stmt, opt_testlist, yield_expr
: test star_COMMA_test {$$ = $1;}
;
dictorsetmaker // Used in: opt_dictorsetmaker
: test COLON test pick_comp_for
| test pick_for_test
;
pick_comp_for // Used in: dictorsetmaker
: comp_for
| star_test_COLON_test
;
pick_for_test // Used in: dictorsetmaker
: comp_for
| star_COMMA_test
;
classdef // Used in: decorated, compound_stmt
: CLASS NAME LPAR opt_testlist RPAR COLON suite 
| CLASS NAME COLON suite
;
opt_testlist // Used in: classdef
: testlist
| %empty
;
arglist // Used in: opt_arglist, arglist
: argument COMMA arglist
| argument COMMA
| argument
| listarg COMMA arglist_postlist
| listarg
| dictarg
;
argument // Used in: arglist, arglist_postlist
: test opt_comp_for
| test EQUAL test
;
opt_comp_for // Used in: argument
: comp_for
| %empty
;
list_iter // Used in: list_for, list_if
: list_for
| list_if
;
list_for // Used in: listmaker, list_iter
: FOR exprlist IN testlist_safe list_iter
| FOR exprlist IN testlist_safe
;
list_if // Used in: list_iter
: IF old_test list_iter
| IF old_test
;
comp_iter // Used in: comp_for, comp_if
: comp_for
| comp_if
;
comp_for // Used in: testlist_comp, pick_comp_for, pick_for_test, 
         // opt_comp_for, comp_iter
: FOR exprlist IN or_test comp_iter
| FOR exprlist IN or_test
;
comp_if // Used in: comp_iter
: IF old_test comp_iter
| IF old_test
;
testlist1 // Used in: atom, testlist1
: test
| testlist1 COMMA test
;
// encoding_decl // Used in: start
// 	: NAME
// 	;
yield_expr // Used in: pick_yield_expr_testlist, yield_stmt, 
//            // pick_yield_expr_testlist_comp
 	: YIELD testlist {$$ = $2;} //TO MODIFY
// 	| YIELD 
 	;
star_fpdef_notest // Used in: fplist, star_fpdef_notest
: COMMA fpdef star_fpdef_notest
| COMMA
| %empty
;
star_COMMA_expr // Used in: exprlist, star_COMMA_expr
: COMMA expr star_COMMA_expr
| COMMA
| %empty
;
star_COMMA_fpdef // Used in: varargslist, star_COMMA_fpdef
: COMMA fpdef opt_EQUAL_test star_COMMA_fpdef
| COMMA
| %empty
;
star_COMMA_test // Used in: opt_test, listmaker, testlist_comp, testlist, pick_for_test, star_COMMA_test
: COMMA test star_COMMA_test
| COMMA
| %empty
;
star_test_COLON_test // Used in: pick_comp_for, star_test_COLON_test
: COMMA test COLON test star_test_COLON_test
| COMMA
| %empty
;
star_COMMA_subscript // Used in: subscriptlist, star_COMMA_subscript
: COMMA subscript star_COMMA_subscript
| COMMA
| %empty
;
star_COMMA_import_as_name // Used in: import_as_names, 
                          // star_COMMA_import_as_name
: COMMA import_as_name star_COMMA_import_as_name
| COMMA
| %empty
;
plus_COMMA_test // Used in: opt_test_2, plus_COMMA_test
: COMMA test plus_COMMA_test
| COMMA test COMMA
| COMMA test
;
plus_COMMA_old_test // Used in: testlist_safe, plus_COMMA_old_test
: COMMA old_test plus_COMMA_old_test
| COMMA old_test COMMA
| COMMA old_test
;
dictarg // Used in: arglist, arglist_postlist
: DOUBLESTAR test
;
listarg // Used in: arglist
: STAR test
;
arglist_postlist // Used in: arglist, arglist_postlist
: argument COMMA arglist_postlist
| dictarg
| argument
;
small_stmt_STAR_OR_SEMI // Used in: simple_stmt, small_stmt_STAR_OR_SEMI
: SEMI small_stmt small_stmt_STAR_OR_SEMI
| SEMI
| %empty
;

%%

#include <stdio.h>
void yyerror (char const *s) {
	fprintf (stderr, "%d: %s with [%s]\n", yylineno, s, yytext);
}
