/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode * stmt_sequence(void);  /* 锟斤拷锟斤拷锟斤拷锟?*/
static TreeNode * statement(void);      /* 锟斤拷锟?*/
static TreeNode * if_stmt(void);        /* if锟斤拷锟?*/
static TreeNode * repeat_stmt(void);    /* 循锟斤拷锟斤拷锟?*/ 
static TreeNode * assign_stmt(void);    /* 锟斤拷值锟斤拷锟?*/
static TreeNode * read_stmt(void);      /* 锟斤拷锟斤拷锟斤拷锟?*/
static TreeNode * write_stmt(void);     /* 锟斤拷锟斤拷锟斤拷 */

static TreeNode * exp(void);            /* 锟斤拷锟斤拷式 */
static TreeNode * simple_exp(void);     /* 锟津单憋拷锟斤拷式 */
static TreeNode * term(void);           /* 锟剿凤拷锟斤拷 */
static TreeNode * factor(void);         /* 锟斤拷锟斤拷 */

//todo
static TreeNode * while_stmt(void);     /* 锟斤拷锟斤拷锟斤拷 */ 
static TreeNode * judges(void);     /*锟斤拷锟斤拷锟斤拷锟斤拷*/
static TreeNode * judge(void);
static TreeNode * int_judge(void);    /*int锟斤拷锟斤拷*/
static TreeNode * bool_judge(void);   /*bool锟斤拷锟斤拷*/
static TreeNode * string_judge(void); /*string锟斤拷锟斤拷*/
static TreeNode * float_judge(void);  /*float锟斤拷锟斤拷*/
static TreeNode * double_judge(void); /*double锟斤拷锟斤拷*/
static TreeNode * varlist(void);        /*锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?/

/* 锟斤拷示锟斤拷锟斤拷锟斤拷息*/
static void syntaxError(char * message)
{ fprintf(listing,"\n>>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}
/* 锟叫断碉拷前执锟叫的诧拷锟斤拷锟角凤拷锟斤拷锟斤拷锟斤拷锟斤拷锟絫oken锟斤拷同锟斤拷锟斤拷同锟斤拷锟斤拷锟斤拷锟揭伙拷锟斤拷锟斤拷*/
static void match(TokenType expected)
{ if (token == expected) token = getToken();
  else {
    syntaxError("unexpected token -> ");
    printToken(token,tokenString);
    fprintf(listing,"      ");
  }
}
TreeNode* judges(void)
{
    TreeNode* t = judge();
    TreeNode* p = t;
    while ((token == INT) || (token == BOOL) ||
        (token == STRING) || (token == FLOAT) || (token == DOUBLE))
    {
        TreeNode* q;
        q = judge();
        if (q != NULL)
        {
            p->sibling = q;
            p = q;
        }
    }
    if (t == NULL)
    {
        t = stmt_sequence();//锟斤拷取锟斤拷锟斤拷锟斤拷械锟斤拷锓拷锟斤拷诘锟?
    }
    else {
    	//锟斤拷锟斤拷锟斤拷锟斤拷械锟斤拷锓拷锟斤拷诘锟斤拷锟斤拷拥锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟街拷锟?
        p->sibling = stmt_sequence();
    }
    return t;
}

TreeNode* judge(void)
{
    TreeNode* t = NULL;
    switch (token) {
    case INT:
        t = int_judge();
        break;
    case BOOL:
        t = bool_judge();
        break;
    case STRING:
        t = string_judge();
        break;
    case FLOAT:
        t = float_judge();
        break;
    case DOUBLE:
        t = double_judge();
        break;
    } 
    return t;
}


TreeNode* int_judge(void)
{
    TreeNode* t = newDeclNode(IntK);
    match(INT);
    if (t != NULL) {
        t->child[0] = varlist();
        t->child[0]->type = Integer;
    }
    return t;
}

TreeNode* bool_judge(void)
{
    TreeNode* t = newDeclNode(BoolK);
    match(BOOL);
    if (t != NULL) {
        t->child[0] = varlist();
        t->child[0]->type = Boolean;
    }
    return t;
}

TreeNode* string_judge(void)
{
    TreeNode* t = newDeclNode(StringK);
    match(STRING);
    if (t != NULL) {
        t->child[0] = varlist();
        t->child[0]->type = String;
    }
    return t;
}

TreeNode* float_judge(void)
{
    TreeNode* t = newDeclNode(FloatK);
    match(FLOAT);
    if (t != NULL) {
        t->child[0] = varlist();
        t->child[0]->type = Float;
    }
    return t;
}

TreeNode* double_judge(void)
{
    TreeNode* t = newDeclNode(DoubleK);
    match(DOUBLE);
    if (t != NULL) {
        t->child[0] = varlist();
        t->child[0]->type = Double;
    }
    return t;
}

TreeNode* varlist(void)
{
    TreeNode* t = NULL;
    if (token == ID) {
    // 锟斤拷锟斤拷一锟斤拷锟斤拷示锟斤拷识锟斤拷锟斤拷锟斤法锟斤拷锟节点，锟斤拷锟斤拷锟斤拷识锟斤拷锟斤拷锟斤拷锟街存储锟节节碉拷锟絘ttr.name锟斤拷
        t = newExpNode(IdK);
        t->attr.name = copyString(tokenString);
        match(ID);
        if (token == COMMA) {
    //锟斤拷锟斤拷锟絭arlist()锟斤拷锟斤拷锟斤拷取锟斤拷锟斤拷谋锟斤拷锟斤拷斜锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷拥锟斤拷锟角帮拷诘锟斤拷child[0]锟斤拷
            match(COMMA);
            t->child[0] = varlist();
            t->child[0]->type = t->type;
        }
        else if (token == SEMI) {
    //锟斤拷示锟斤拷锟斤拷锟叫憋拷锟侥斤拷锟斤拷锟斤拷锟斤拷
            match(SEMI);
        }
    }
    else {
    //锟斤拷锟斤拷锓拷锟斤拷锟斤拷锟较拷锟斤拷锟斤拷锟斤拷曰锟饺★拷锟揭伙拷锟絫oken
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
    }
    return t;
}

/* 锟斤拷锟斤拷匹锟斤拷锟斤拷战锟斤拷stmt_sequence */
TreeNode * stmt_sequence(void)
{ TreeNode * t = statement();/*锟斤拷为锟斤拷锟斤拷锟秸帮拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷一锟斤拷statement*/
  TreeNode * p = t;
  while ((token!=ENDFILE) && (token!=END) &&
         (token!=ELSE) && (token!=UNTIL)&&(token!=WHILE))/* 锟斤拷锟斤拷锟诫不锟斤拷识锟斤拷锟斤拷锟斤拷薪锟斤拷锟斤拷募呛锟?*/ 
  { TreeNode * q;
    match(SEMI);
    q = statement();/*锟斤拷锟叫讹拷锟角凤拷锟叫猴拷锟斤拷statement锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟揭伙拷锟絪tatement锟斤拷锟街碉拷*/
    if (q!=NULL) {
      if (t==NULL) t = p = q;
      else /* now p cannot be NULL either */
      { p->sibling = q;/*锟斤拷锟斤拷锟街弟癸拷系*/
        p = q;
      }
    }
  }
  return t;
}
/* 锟斤拷锟斤拷匹锟斤拷statement锟斤拷锟秸斤拷锟?*/
TreeNode * statement(void)/*锟斤拷锟斤拷tiny锟侥凤拷锟叫碉拷stmt锟斤拷锟斤拷锟捷碉拷一锟斤拷token锟斤拷锟缴讹拷应锟斤拷statement*/
{ TreeNode * t = NULL;
  switch (token) {
    case IF : t = if_stmt(); break;/*锟饺猴拷锟揭边猴拷锟斤拷锟斤拷锟狡硷拷为锟斤拷应锟斤拷锟斤拷式锟揭诧拷锟斤拷*/
    case REPEAT : t = repeat_stmt(); break;
    case ID : t = assign_stmt(); break;
    case READ : t = read_stmt(); break;
    case WRITE : t = write_stmt(); break;
    case DO:t = while_stmt(); break;
    default : syntaxError("unexpected token -> ");
              printToken(token,tokenString);
              token = getToken();
              break;
  } /* end case */
  return t;
}
/* 锟斤拷锟斤拷匹锟斤拷if_stmt锟斤拷锟秸斤拷锟?*/
TreeNode * if_stmt(void)
{ TreeNode * t = newStmtNode(IfK);/*锟饺癸拷锟斤拷一锟斤拷statement锟斤拷锟?/
  match(IF);              /*锟劫癸拷锟斤拷if_stmt锟斤拷要锟斤拷锟斤拷锟斤拷锟接斤拷悖拷直锟轿猠xp锟斤拷锟斤拷锟斤拷stat-seq*/
  if (t!=NULL) t->child[0] = exp();
  match(THEN);
  if (t!=NULL) t->child[1] = stmt_sequence();
  if (token==ELSE) {
    match(ELSE);
    if (t!=NULL) t->child[2] = stmt_sequence();
  }
  match(END);
  return t;
}
/* 锟斤拷锟斤拷匹锟斤拷repeat_stmt锟斤拷锟秸斤拷锟?*/
TreeNode * repeat_stmt(void)
{ TreeNode * t = newStmtNode(RepeatK);/*锟饺癸拷锟斤拷一锟斤拷statement锟斤拷锟?/
  match(REPEAT); /*锟劫癸拷锟斤拷if_stmt锟斤拷要锟斤拷锟斤拷锟斤拷锟接斤拷悖拷直锟轿猻tat-seq锟斤拷exp*/
  if (t!=NULL) t->child[0] = stmt_sequence();
  match(UNTIL);
  if (t!=NULL) t->child[1] = exp();
  return t;
}
/* 锟斤拷锟斤拷匹锟斤拷assign_stmt锟斤拷锟秸斤拷锟?*/
TreeNode * assign_stmt(void)
{ TreeNode * t = newStmtNode(AssignK);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(ASSIGN);
  if (t!=NULL) t->child[0] = exp();
  return t;
}
/* 锟斤拷锟斤拷匹锟斤拷read_stmt锟斤拷锟秸斤拷锟?*/
TreeNode * read_stmt(void)
{ TreeNode * t = newStmtNode(ReadK);
  match(READ);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  // while (token == COMMA)
  // {
  // 	TreeNode *t1 = newStmtNode(ReadK);
  // 	match(COMMA);
  // 	t1->attr.name = copyString(tokenString);
  // 	t->child[0] = t1;
  // 	t = t1;
  //  match(ID);
  //  if (token == SEMI)
  //  {
  //   match(SEMI);
  //  }
  // }
  return t;
}

/* 锟斤拷锟斤拷匹锟斤拷write_stmt锟斤拷锟秸斤拷锟?*/
TreeNode * write_stmt(void)
{ TreeNode * t = newStmtNode(WriteK);
  match(WRITE);
  if (t!=NULL) t->child[0] = exp();
  return t;
}

TreeNode* while_stmt(void)
{
    TreeNode* t = newStmtNode(WhileK);
    match(DO);
    if (t != NULL) t->child[0] = stmt_sequence();
    match(WHILE);
    if (t != NULL) t->child[1] = exp();
    return t;
}


/* 锟斤拷锟斤拷匹锟斤拷exp锟斤拷锟秸斤拷锟?*/
TreeNode * exp(void)
{ TreeNode * t = simple_exp();
/*锟斤拷为锟斤拷锟斤拷式锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷simple_exp锟斤拷锟缴ｏ拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷一锟斤拷锟劫革拷锟斤拷锟角凤拷锟斤拷cop锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷一锟斤拷*/
  if ((token==LT)||(token==EQ)|| (token == LTE)|| (token == GTE)) {
    TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
    }
    match(token);
    if (t!=NULL)
      t->child[1] = simple_exp();
  }
  return t;
}
/* 锟斤拷锟斤拷匹锟斤拷simple_stmt锟斤拷锟秸斤拷锟?*/
TreeNode * simple_exp(void)
{ TreeNode * t = term();
/*锟斤拷锟斤拷锟斤拷一锟斤拷term锟斤拷锟节革拷锟斤拷+锟斤拷-锟斤拷锟斤拷锟斤拷锟斤拷term锟斤拷锟斤拷a+b-c*/
  while ((token==PLUS)||(token==MINUS))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      t->child[1] = term();
    }
  }
  return t;
}
/* 锟斤拷锟斤拷匹锟斤拷term锟斤拷锟秸斤拷锟?*/
TreeNode * term(void)
{ TreeNode * t = factor();
/*锟斤拷锟斤拷锟斤拷一锟斤拷factor锟斤拷锟劫革拷锟斤拷锟角凤拷锟斤拷*锟斤拷/锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷factor*/
  while ((token==TIMES)||(token==OVER))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      p->child[1] = factor();
    }
  }
  return t;
}
/* 锟斤拷锟斤拷匹锟斤拷factor锟秸斤拷锟?*/
TreeNode * factor(void)
{ TreeNode * t = NULL;
  switch (token) {
      /*锟秸斤拷锟斤拷锟狡ワ拷锟斤拷锟斤拷帧锟斤拷锟斤拷锟斤拷锟斤拷址锟斤拷锟*/
    case NUM :
      t = newExpNode(ConstK);
      if ((t!=NULL) && (token==NUM))
        t->attr.val = atof(tokenString);
      match(NUM);
      break;
    case ID :
      t = newExpNode(IdK);
      if ((t!=NULL) && (token==ID))
        t->attr.name = copyString(tokenString);
      match(ID);
      break;
    case STR:
      t = newExpNode(ConstStrK);
      if ((t != NULL) && (token == STR))
        t->attr.name = copyString(tokenString);
      match(STR);
      break;
    case LPAREN :
      match(LPAREN);
      t = exp();
      match(RPAREN);
      break;
    default:
      syntaxError("unexpected token -> ");
      printToken(token,tokenString);
      token = getToken();
      break;
    }
  return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly 
 * constructed syntax tree
 */
TreeNode * parse(void)
{ TreeNode * t;
  token = getToken();
  t = judges();
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}