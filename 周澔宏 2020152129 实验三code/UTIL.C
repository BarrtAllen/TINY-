/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"



/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { case IF:
    case THEN:
    case ELSE:
    case END:
    case REPEAT:
    case UNTIL:
    case READ:
    case WRITE:
	case T_TRUE:
	case T_FALSE:
	case OR:
	case AND:
	case NOT:
	case INT:
	case BOOL:
	case STRING:
	case DO:
	case WHILE: 
    case FLOAT:
    case DOUBLE:
       fprintf(listing,
         "reserved word: %s\n",tokenString);
		 break;
    case ASSIGN: fprintf(listing,":=\n"); break;

    case LT: fprintf(listing,"<\n"); break;
    case EQ: fprintf(listing,"=\n"); break;
	case GT: fprintf(listing,">\n"); break;
    case LTE: fprintf(listing,"<=\n"); break;
	case GTE: fprintf(listing,">=\n"); break;

    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case SEMI: fprintf(listing,";\n"); break;
	case COMMA: fprintf(listing,",\n"); break;
    case SQM: fprintf(listing,"\'\n"); break;
    case PLUS: fprintf(listing,"+\n"); break;
    case MINUS: fprintf(listing,"-\n"); break;
    case TIMES: fprintf(listing,"*\n"); break;
    case OVER: fprintf(listing,"/\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;
	case STR:
	  fprintf(listing,"STR,name= %s\n",tokenString);
	  break;
	case ERROR: {
	
			fprintf(listing,
				"ERROR %s :%s\n",errorMsg[errorCode],tokenString);

		
		}break;
   
	

    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
/*newStmtNodeï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Õ¼ä£¬ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã¶ï¿½Ó¦ï¿½Ä³ï¿½Ê¼Öµ*/
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;//STmtKÎªï¿½ï¿½ï¿½ï¿½Ä²ï¿½ï¿½ï¿½
    t->kind.stmt = kind;//kindï¿½æ´¢ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newDeclNode creates a new expression
 * node for syntax tree construction
 */
 
//todo 
TreeNode * newDeclNode(DeclKind kind)
{
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else {
    	/*
    	º¯Êý»á¶Ô"t"½øÐÐ³õÊ¼»¯£¬°üÀ¨½«×Ó½ÚµãºÍÐÖµÜ½Úµã¶¼ÉèÎªNULL£¬
		½«½ÚµãÀàÐÍ"nodekind"ÉèÎªDeclK£¨±íÊ¾ÉùÃ÷ÀàÐÍ£©£¬
		½«ÉùÃ÷ÀàÐÍ"kind"´æ´¢ÔÚ"kind.exp"ÖÐ£¬ÒÔ¼°½«ÐÐºÅ"lineno"´æ´¢ÔÚ"lineno"ÖÐ¡£
		*/ 
        for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = DeclK;
        t->kind.exp = kind;
        t->lineno = lineno;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 2;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    //Óï¾ä½Úµã 
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case RepeatK:
          fprintf(listing,"Repeat\n");
          break;
        case AssignK:
          fprintf(listing,"Assign to: %s\n",tree->attr.name);
          break;
        case ReadK:
          fprintf(listing,"Read: %s\n",tree->attr.name);
          break;
        case WriteK:
          fprintf(listing,"Write\n");
          break;
        case WhileK:
          fprintf(listing,"While\n");
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    //±í´ïÊ½½Úµã 
    { switch (tree->kind.exp) {
        case OpK:
        //²Ù×÷·û½Úµã
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
		//³£Á¿½Úµã
		 {
        //³£Á¿Öµ¼õÈ¥ÆäÕûÊý²¿·Ö´óÓÚ0£¬ÔòËµÃ÷ÊÇ¸¡µãÊý 
          if (tree->attr.val - (int)tree->attr.val > 0) {
            fprintf(listing, "Const: Float: %f\n", tree->attr.val);
          }
          else {
              fprintf(listing, "Const: Integer: %d\n", (int)tree->attr.val);
          }
          break;
        }
          
        case ConstStrK:
        //×Ö·û´®³£Á¿½Úµã 
          fprintf(listing, "Const: String: %s\n", tree->attr.name);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind == DeclK)
    {
        switch (tree->kind.decl) {
        case IntK:
            fprintf(listing, "Type: int\n");
            break;
        case BoolK:
            fprintf(listing, "Type: bool\n");
            break;
        case StringK:
            fprintf(listing, "Type: string\n");
            break;
        case FloatK:
            fprintf(listing, "Type: float\n");
            break;
        case DoubleK:
            fprintf(listing, "Type: double\n");
            break;
        default:
            fprintf(listing, "Unknown Declation kind\n");
            break;
        }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}





int isLegalChar(char c) {


	return (isalnum(c)||
			isspace(c)||
			c == '>'||
			c == '<' ||
			c == '=' ||
			c == ',' ||
			c ==';' ||
			c =='\'' ||
			c =='{'	||
			c =='}' ||
			c =='+' ||
			c =='-' ||
			c =='*' ||
			c =='/' ||
			c =='(' ||
			c ==')' 
			);


	}
