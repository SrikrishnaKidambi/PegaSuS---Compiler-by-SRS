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

// Next Pass of Optimization - Copy Propagation (has 2 sub passes - one for propagating the copied variable, another one for removing the dead code created due to propagating the copy variable

#define MAX_COPIES 256

typedef struct{
	char from[20];
	char to[20];
	int live;
}CopyEntry;

static CopyEntry copy_table[MAX_COPIES];
static int copy_count;

// Clears the entire copy table - called at the start of the process to ensure clean table
static void ct_clear(void){
	copy_count = 0;
	memset(copy_table, 0, sizeof(copy_table));
}

// Function used for handling the addition of new copy pairs into the copy table. This is called when we see some IR code like x = y then we need to replace x with y
// Hence x -> from and y -> to.
// Now we will be checking if there is space in the copy table to add a new copy pair and if yes add it.
static void ct_add(const char* from, const char* to){
	for(int i = 0; i < copy_count; i++){
		if(copy_table[i].live && strcmp(copy_table[i].from, from) == 0){
			strncpy(copy_table[i].to, to, 19);
			return;
		}
	}
	if(copy_count < MAX_COPIES){
		strncpy(copy_table[copy_count].from, from , 19);
		strncpy(copy_table[copy_count].to, to, 19);
		copy_table[copy_count++].live = 1;
	}
}

// This is called for every operand that is each argument in each quad in the generated IR code
// If we find that the current variable in the IR code is having some copy source by iterating through the copy table then we simply return the source variable name
// If there is no match then we return the same variable name itself indicating no substitution
static const char* ct_lookup(const char* name){
	for(int i = 0; i < copy_count; i++){
		if(copy_table[i].live && strcmp(copy_table[i].from, name) == 0){
			return copy_table[i].to;
		}
	}
	return name;
}

// This is the function that kills a copy pair i.e., invalidating a copy pair when either of from or to variables of a copy pair are updated
static void ct_kill(const char* name){
	for(int i = 0; i < copy_count; i++){
		if(copy_table[i].live && 
				(strcmp(copy_table[i].from, name) == 0 ||
				strcmp(copy_table[i].to, name) == 0)){
			copy_table[i].live = 0;
		}
	}
}

// This is a helper function for scanning if the variable is going to be used again in the future or not for deciding if the copy pair is dead (if not used again)
static int is_used_after(const char* name, int from){
	for(int i = from; i < OPT_IR_idx; i++){
		if(strcmp(OPT_IR[i].arg1, name) == 0 || 
				strcmp(OPT_IR[i].arg2, name) == 0){
			return 1;
		}
	}
	return 0;
}


// This is a helper function that checks if the operator is something like a structural operator that is representing a structure but not something like holding a value
// If the quad's operator matches with any of these then we skip that quad.
static int is_structural_op(const char* op){
	static const char* structural[] = {
		"label", "goto", "func", "endfunc", "entity", "end_entity", "method", "end_method", "constr", "end_constr", "param", NULL
	};
	for(int i = 0; structural[i]; i++){
		if(strcmp(op, structural[i]) == 0){
			return 1;
		}
	}
	return 0;
}

// Major function that is responsible for applying the optimization techinque - Copy Propagation. 
int copy_propagation(void){
	int subs = 0, removed = 0;
	ct_clear();	// Starts with a clear copy table with copy_count = 0
	for(int i = 0; i < OPT_IR_idx; i++){
		Quad* q = &OPT_IR[i];
		// Skip if the current quad's operator is some structural operator
		if(is_structural_op(q->op)){
			continue;
		}

		// Check if either arg1 or arg2 of the current quad is having some copy source and if yes the function returns the copy source. 
		// If the returned copy source is same as the original variable name then no substitution is needed.
		if(q->arg1[0]){
			const char* sub = ct_lookup(q->arg1);
			if(strcmp(sub, q->arg1) != 0){
				strncpy(q->arg1, sub, 19);
				subs++;
			}
		}
		if(q->arg2[0]){
			const char* sub = ct_lookup(q->arg2);
			if(strcmp(sub, q->arg2) != 0){
				strncpy(q->arg2, sub, 19);
				subs++;
			}
		}

		// Checks if the current quad is a copy or not and if yes
		// Firstly we will kill all the copy pairs that are dependent on the result variable of the quad (as those values are stale and need to be added again)
		int is_copy = (strcmp(q->op, "=") == 0 && q->arg2[0] == '\0' &&
				q->result[0] && q->arg1[0]);
		// Firstly kill the copy pair accoriding to the matchings and then add. If reversed then we will be adding and 
		// then deleting the same pair that has been added 
		if(q->result[0]){
			ct_kill(q->result);
			if(is_copy){
				ct_add(q->result, q->arg1);
			}
		}
	}

	// This part of the code handles the second pass over the IR code and is not used again or it is not a copy quad
	// Inorder to shrink the copy table after killing some pairs we use two-pointer based approach.
	int write = 0;
	for(int read = 0; read < OPT_IR_idx; read++){
		Quad* q = &OPT_IR[read];
		int dead = (strcmp(q->op,"=") == 0 && q->arg2[0]=='\0'
                    && q->result[0] && !is_used_after(q->result, read+1));
		if(dead){
			removed++;
			continue;
		}
		if(write != read){
			OPT_IR[write] = OPT_IR[read];
		}
		write++;
	}
	OPT_IR_idx = write;
	printf("[CopyProp] %d substitution(s), %d dead copy(ies) removed.  "
           "OPT_IR has %d quad(s).\n", subs, removed, OPT_IR_idx);
	return subs + removed;
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

