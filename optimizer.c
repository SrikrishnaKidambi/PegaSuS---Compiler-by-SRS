#include "optimizer.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} Quad;

extern Quad IR[];
extern int IR_idx;

Quad OPT_IR[IR_SIZE];
int OPT_IR_idx = 0;

static int is_numeric(const char* s){
	if(!s || !*s){
		return 0;
	}
	char* end;
	strtod(s, &end);
	return (*end == '\0');
}

static int is_int_val(const char* s, int v){
	if(!is_numeric(s)){
		return 0;
	}

	double d = atof(s);
	return ((int) d == v) && (fabs(d - (int)d) < 1e-9);
}

static int pow2_exp(int v){
	if(v < 2){
		return -1;
	}
	int e = 0, x = v;
	while(x > 1){
		if(x & 1){
			return -1;
		}
		x >>= 1;
		e++;
	}
	return e;
}


static void make_copy_quad(Quad* q, const char* result, const char* src){
	strcpy(q->op, "=");
	strcpy(q->arg1, src);
	q->arg2[0] = '\0';
	strcpy(q->result, result);
}

static void make_binop_quad(Quad* q, const char* op, const char* a, const char* b, const char* result){
	strcpy(q->op, op);
	strcpy(q->arg1, a);
	strcpy(q->arg2, b);
	strcpy(q->result, result);
}

static int insert_opt_quad_at(int pos){
	if(OPT_IR_idx >= IR_SIZE){
		return 0;
	}
	memmove(&OPT_IR[pos + 1], &OPT_IR[pos], (OPT_IR_idx - pos) * sizeof(Quad));
	++OPT_IR_idx;
	return 1;
}

int algebraic_simplification(void)
{
	OPT_IR_idx = 0;
	int total = 0;
	
	// Iterate through the array of Quadruples
	for(int i = 0; i < IR_idx; i++){
		const Quad* s = &IR[i];
		Quad out;
		memset(&out, 0, sizeof(out));
		int fired = 0;

		// For addition operations
		if(strcmp(s->op, "+") == 0){
			if(is_int_val(s->arg2, 0)){
				make_copy_quad(&out, s->result, s->arg1);
				fired = 1;
			}
			else if(is_int_val(s->arg1, 0)){
				make_copy_quad(&out, s->result, s->arg2);
				fired = 1;
			}
		}
		// For subtraction operation
		else if(strcmp(s->op, "-") == 0){
			if(is_int_val(s->arg2, 0)){
				make_copy_quad(&out, s->result, s->arg1);
				fired = 1;
			}
			else if(s->arg1[0] && strcmp(s->arg1, s->arg2) == 0){
				make_copy_quad(&out, s->result, "0");
				fired = 1;
			}
		}
		// For multiplication operation
		else if(strcmp(s->op, "*") == 0){
			if(is_int_val(s->arg2, 0) || is_int_val(s->arg1, 0)){
				make_copy_quad(&out, s->result, "0");
				fired = 1;
			}
			else if(is_int_val(s->arg2, 1)){
				make_copy_quad(&out, s->result, s->arg1);
				fired = 1;
			}
			else if(is_int_val(s->arg1, 1)){
				make_copy_quad(&out, s->result, s->arg2);
				fired = 1;
			}
			// If x*2 then emit x + x
			else if(is_int_val(s->arg2, 2)){
				make_binop_quad(&out, "+", s->arg1, s->arg1, s->result);
				fired = 1;
			}
			else if(is_int_val(s->arg1, 2)){
				make_binop_quad(&out, "+", s->arg1, s->arg1, s->result);
				fired = 1;
			}
			// Now finally checking for something like x * (2^k) which can simply be converted into shifting operation
			else if(is_numeric(s->arg2)){
				int e = pow2_exp((int) atof(s->arg2));
				if(e >= 2){
					char es[12];
					sprintf(es, "%d", e);
					make_binop_quad(&out, "<<", s->arg1, es, s->result);
					fired = 1;
				}
			}
			else if(is_numeric(s->arg1)){
				int e = pow2_exp((int) atof(s->arg1));
                                if(e >= 2){
                                        char es[12];
                                        sprintf(es, "%d", e);
                                        make_binop_quad(&out, "<<", s->arg2, es, s->result);
                                        fired = 1;
                                }
			}
		}
		// Division operation involved
		else if(strcmp(s->op, "/") == 0){
			if(is_int_val(s->arg2, 1)){
				make_copy_quad(&out, s->result, s->arg1);
				fired = 1;
			}
			else if(s->arg1[0] && strcmp(s->arg1, s->arg2) == 0){
				make_copy_quad(&out, s->result, "1");
				fired = 1;
			}
			else if(is_numeric(s->arg2)){
				int e = pow2_exp((int) atof(s->arg2));
				if(e >= 1){
					char es[12];
					sprintf(es, "%d", e);
					make_binop_quad(&out, ">>", s->arg1, es, s->result);
					fired = 1;
				}
			}
		}
		// Modulo operation
		else if(strcmp(s->op, "%") == 0){
			if(is_int_val(s->arg2, 1)){
				make_copy_quad(&out, s->result, "0");
				fired = 1;
			}
		}
		// Exponentiation operation
		else if(strcmp(s->op, "^") == 0){
			if(is_int_val(s->arg2, 0)){
				make_copy_quad(&out, s->result, "1");
				fired = 1;
			}
			else if(is_int_val(s->arg2, 1)){
				make_copy_quad(&out, s->result, s->arg1);
				fired = 1;
			}
		}
		else if(strcmp(s->op, "!") == 0 && s->arg2[0] == '\0'){
			if(is_int_val(s->arg1, 0)){
				make_copy_quad(&out, s->result, "1");
				fired = 1;
			}
			else if(is_int_val(s->arg1, 1)){	
				make_copy_quad(&out, s->result, "0");
				fired = 1;
			}
		}
		OPT_IR[OPT_IR_idx++] = fired ? out : *s;
		if(fired) {
			total++;
		}
	}
	printf("[Algebraic Simplification] %d simplification(s) applied. OPT_IR has %d quad(s).\n", total, OPT_IR_idx);
	return total;
}
int constant_folding(void)
{
    OPT_IR_idx = 0;
    int total = 0;

    for(int i = 0; i < IR_idx; i++){
        const Quad* s = &IR[i];
        Quad out;
        memset(&out, 0, sizeof(out));
        int fired = 0;

        // just only fold the expressions where both the left and right arguments are constants(numericals)
        if((strcmp(s->op,"+") == 0 || strcmp(s->op,"-") == 0 ||
            strcmp(s->op,"*") == 0 || strcmp(s->op,"/") == 0 ||
            strcmp(s->op,"%") == 0 || strcmp(s->op,">") == 0 ||
            strcmp(s->op,"<") == 0 || strcmp(s->op,"==") == 0)
           && is_numeric(s->arg1) && is_numeric(s->arg2))
        {
            double a = atof(s->arg1);
            double b = atof(s->arg2);
            double r = 0;
            int valid = 1;

            if     (strcmp(s->op,"+")  == 0) r = a + b;
            else if(strcmp(s->op,"-")  == 0) r = a - b;
            else if(strcmp(s->op,"*")  == 0) r = a * b;
            else if(strcmp(s->op,"/")  == 0){
                if(b == 0){ valid = 0; }
                else r = a / b;
            }
            else if(strcmp(s->op,"%")  == 0){
                if((int)b == 0){ valid = 0; }
                else r = (int)a % (int)b;
            }
            else if(strcmp(s->op,">")  == 0) r = (a >  b);
            else if(strcmp(s->op,"<")  == 0) r = (a <  b);
            else if(strcmp(s->op,"==") == 0) r = (a == b);

            if(valid){
                char val[32];
                if(r == (int)r) sprintf(val, "%d", (int)r);
                else            sprintf(val, "%f", r);
                make_copy_quad(&out, s->result, val);
                fired = 1;
            }
        }

        OPT_IR[OPT_IR_idx++] = fired ? out : *s;
        if(fired) total++;
    }
    printf("[Constant Folding] %d fold(s) applied. OPT_IR has %d quad(s).\n",
           total, OPT_IR_idx);
    return total;
}
int dead_code_elimination(void)
{

    int total_removed = 0;
    int removed_this_round;

    // this functon  returns 1 if op is a side-effect operation 
    #define IS_SIDEEFFECT(op) \
        (strcmp(op,"call")==0   || strcmp(op,"return")==0  || \
         strcmp(op,"ifFalse")==0|| strcmp(op,"goto")==0    || \
         strcmp(op,"label")==0  || strcmp(op,"out")==0     || \
         strcmp(op,"in")==0     || strcmp(op,"param")==0   || \
         strcmp(op,"arg")==0    || strcmp(op,"func")==0    || \
         strcmp(op,"endfunc")==0|| strcmp(op,"entity")==0  || \
         strcmp(op,"end_entity")==0)

    do {
        removed_this_round = 0;

        // create the used buffer
        static char used[IR_SIZE][20];
        int used_cnt = 0;

        for(int i=0;i<OPT_IR_idx;i++){
            // record the use of first argument and push into the used buffer
            if(OPT_IR[i].arg1[0]){
                int found=0;
                for(int j=0;j<used_cnt;j++)
                    if(strcmp(used[j],OPT_IR[i].arg1)==0){found=1;break;}
                if(!found) strcpy(used[used_cnt++],OPT_IR[i].arg1);
            }
            // record the use of the second argument and push into used buffer
            if(OPT_IR[i].arg2[0]){
                int found=0;
                for(int j=0;j<used_cnt;j++)
                    if(strcmp(used[j],OPT_IR[i].arg2)==0){found=1;break;}
                if(!found) strcpy(used[used_cnt++],OPT_IR[i].arg2);
            }
           
            if(IS_SIDEEFFECT(OPT_IR[i].op)){
                if(OPT_IR[i].result[0]){
                    int found=0;
                    for(int j=0;j<used_cnt;j++)
                        if(strcmp(used[j],OPT_IR[i].result)==0){found=1;break;}
                    if(!found) strcpy(used[used_cnt++],OPT_IR[i].result);
                }
            }
        }

    
        Quad scratch[IR_SIZE];
        int  scratch_idx = 0;

        for(int i=0;i<OPT_IR_idx;i++){
            const char* res = OPT_IR[i].result;
            const char* op  = OPT_IR[i].op;

            // always force push the side -effects op's into the final ir table
            if(IS_SIDEEFFECT(op)){
                scratch[scratch_idx++]=OPT_IR[i];
                continue;
            }

            // keep quads that produce no named result (bare expr stmts)
            if(res[0]=='\0'){
                scratch[scratch_idx++]=OPT_IR[i];
                continue;
            }

            // keep in the final ir if the result is used somewhere in the above 
            int found=0;
            for(int j=0;j<used_cnt;j++)
                if(strcmp(used[j],res)==0){found=1;break;}

            if(found){
                scratch[scratch_idx++]=OPT_IR[i];
            } else {
                removed_this_round++;   //delete the variables/rows
            }
        }

        // copy the final ir to the original ir table 
        memcpy(OPT_IR,scratch,scratch_idx*sizeof(Quad));
        OPT_IR_idx = scratch_idx;
        total_removed += removed_this_round;

	//repeat the loop until complete dce is done
    } while(removed_this_round > 0);

    #undef IS_SIDEEFFECT

    printf("[Dead Code Elimination] %d dead quad(s) removed. OPT_IR has %d quad(s).\n",
           total_removed, OPT_IR_idx);
    return total_removed;
}

// Utility functions such as functions for printing the IR code

static void dump_quad_table(const char* title, const Quad* arr, int n){
	printf("\n%s (%d quad(s))\n", title, n);
	printf("%-5s  %-15s %-15s %-15s %-15s\n",
           "IDX","OP","ARG1","ARG2","RESULT");
	for(int i = 0; i < n; i++){
		printf("%-5d  %-15s %-15s %-15s %-15s\n", i, arr[i].op, arr[i].arg1, arr[i].arg2, arr[i].result);
	}
}
void print_opt_IR(void)
{
	dump_quad_table("Optimised IR code", OPT_IR, OPT_IR_idx);
}
void print_original_IR(void)
{
	dump_quad_table("Original IR code", IR, IR_idx);
}

