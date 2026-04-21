#include "optimizer.h"
#include "quad.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


extern Quad IR[];
extern int IR_idx;

Quad OPT_IR[IR_SIZE];
int OPT_IR_idx = 0;

//algebraic simplification
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
			// If x*2 then emit x + x // !! Removed as handled in strength reduction
			// else if(is_int_val(s->arg2, 2)){
			// 	make_binop_quad(&out, "+", s->arg1, s->arg1, s->result);
			// 	fired = 1;
			// }
			// else if(is_int_val(s->arg1, 2)){
			// 	make_binop_quad(&out, "+", s->arg1, s->arg1, s->result);
			// 	fired = 1;
			// }
			// Now finally checking for something like x * (2^k) which can simply be converted into shifting operation
			// !!! Removed as it is handled in strength reduction
            // else if(is_numeric(s->arg2)){
			// 	int e = pow2_exp((int) atof(s->arg2));
			// 	if(e >= 2){
			// 		char es[12];
			// 		sprintf(es, "%d", e);
			// 		make_binop_quad(&out, "<<", s->arg1, es, s->result);
			// 		fired = 1;
			// 	}
			// }
			// else if(is_numeric(s->arg1)){
			// 	int e = pow2_exp((int) atof(s->arg1));
            //                     if(e >= 2){
            //                             char es[12];
            //                             sprintf(es, "%d", e);
            //                             make_binop_quad(&out, "<<", s->arg2, es, s->result);
            //                             fired = 1;
            //                     }
			// }
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
            // !!! Removed as it is handled in strength reduction
			// else if(is_numeric(s->arg2)){
			// 	int e = pow2_exp((int) atof(s->arg2));
			// 	if(e >= 1){
			// 		char es[12];
			// 		sprintf(es, "%d", e);
			// 		make_binop_quad(&out, ">>", s->arg1, es, s->result);
			// 		fired = 1;
			// 	}
			// }
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
//constant folding code
int constant_folding(void)
{
	printf("Entered constant folding");
    Quad tmp[IR_SIZE];
    int tmp_n = OPT_IR_idx;
    memcpy(tmp, OPT_IR, tmp_n * sizeof(Quad));  // snapshot OPT_IR
    OPT_IR_idx = 0;
    int total = 0;

    for(int i = 0; i < tmp_n; i++){
        const Quad* s = &tmp[i];


		/* skip folding snapshot-tracked variables */
    	if(s->result[0] != '\0' && is_snapshot_tracked(s->result)){
    	    OPT_IR[OPT_IR_idx++] = *s;
    	    continue;
    	}
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
//dead code elimination code
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
     strcmp(op,"end_entity")==0 || strcmp(op,"[]=")== 0 || \
     strcmp(op,"array_init")==0 || strcmp(op,"[]") == 0)

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

	    if (strcmp(OPT_IR[i].op, "out") == 0 || strcmp(OPT_IR[i].op, "in") == 0) {
		    if (OPT_IR[i].result[0]) {
			int found = 0;
			for (int j = 0; j < used_cnt; j++)
				if (strcmp(used[j], OPT_IR[i].result) == 0) { found = 1; break; }
			if (!found) strcpy(used[used_cnt++], OPT_IR[i].result);
		    }
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

			/* never eliminate snapshot-tracked variables */
			if(res[0] != '\0' && is_snapshot_tracked(res)){
    		    scratch[scratch_idx++] = OPT_IR[i];
    		    continue;
    		}
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
// returns 1 if the op should never be moved (has side effects) 
static int is_sideeffect_op(const char* op){
    return (strcmp(op,"call")==0    || strcmp(op,"return")==0  ||
            strcmp(op,"ifFalse")==0 || strcmp(op,"goto")==0    ||
            strcmp(op,"label")==0   || strcmp(op,"out")==0     ||
            strcmp(op,"in")==0      || strcmp(op,"param")==0   ||
            strcmp(op,"arg")==0     || strcmp(op,"func")==0    ||
            strcmp(op,"endfunc")==0 || strcmp(op,"entity")==0  ||
            strcmp(op,"end_entity")==0);
}

// returns 1 if name is in the written[] set 
static int in_written(const char written[][20], int wcnt, const char* name){
    if(!name || !name[0]) return 0;
    for(int i=0;i<wcnt;i++)
        if(strcmp(written[i],name)==0) return 1;
    return 0;
}

int loop_invariant_code_motion(void)
{
    int total_hoisted = 0;
    int hoisted_this_round;

    do {
        hoisted_this_round = 0;

        
        int loop_start = -1;   // index of the label quad (top of loop) 
        int loop_end   = -1;   // index of the goto  quad (back edge)   
        char loop_label[20]   = "";
        char exit_label[20]   = "";
        //  i am scanning for the loop i want to from where to where the loop is extended in the ir it starts with Label and ends with goto 
        for(int i=0;i<OPT_IR_idx && loop_start==-1;i++){
           
            if(strcmp(OPT_IR[i].op,"label")!=0) continue;
            const char* lname = OPT_IR[i].result;

            for(int j=i+1;j<OPT_IR_idx;j++){
                if(strcmp(OPT_IR[j].op,"goto")==0 &&
                   strcmp(OPT_IR[j].result, lname)==0){
                    loop_start = i;
                    loop_end   = j;
                    strcpy(loop_label, lname);
                    break;
                }
            }

            if(loop_start == -1) continue;

            //find the ifFalse quad immediately after the label and its result is the exit label   
            if(loop_start+1 < OPT_IR_idx &&
               strcmp(OPT_IR[loop_start+1].op,"ifFalse")==0){
                strcpy(exit_label, OPT_IR[loop_start+1].result);
            }
        }

 
        if(loop_start == -1) break;
        int body_first = loop_start + 1;   
        int body_last  = loop_end   - 1;

        if(body_first > body_last){
            break;  
        }

        static char written[IR_SIZE][20];
        int wcnt = 0;
       // iam going through each and every quad in the loop body. Whatever is in the result column that is being copied to the written buffer

        for(int i=body_first; i<=body_last; i++){
            const char* res = OPT_IR[i].result;
            if(!res[0]) continue;
            int found=0;
            for(int k=0;k<wcnt;k++)
                if(strcmp(written[k],res)==0){found=1;break;}
            if(!found) strcpy(written[wcnt++], res);
        }

        static int inv_idx[IR_SIZE]; 
        int inv_cnt = 0;

        for(int i=body_first+1; i<=body_last; i++){
            const char* op  = OPT_IR[i].op;
            const char* a1  = OPT_IR[i].arg1;
            const char* a2  = OPT_IR[i].arg2;
            const char* res = OPT_IR[i].result;

            // side-effect ops 
            if(is_sideeffect_op(op)) continue;

            int write_count = 0;
            for(int k=body_first+1;k<=body_last;k++)
                if(strcmp(OPT_IR[k].result,res)==0) write_count++;
            if(write_count > 1) continue; 

            if(in_written(written, wcnt, a1)) continue;
            if(in_written(written, wcnt, a2)) continue;

            // record the index of the invarient quad
            inv_idx[inv_cnt++] = i;
        }

        if(inv_cnt == 0) break;  

        Quad newIR[IR_SIZE];
        int  newIR_idx = 0;

        // copy everything before the loop label
        for(int i=0;i<loop_start;i++)
            newIR[newIR_idx++] = OPT_IR[i];

        // insert the motioned quads BEFORE the loop label 
        for(int h=0;h<inv_cnt;h++)
            newIR[newIR_idx++] = OPT_IR[inv_idx[h]];

        for(int i=loop_start;i<=loop_end;i++){
            int skip=0;
            for(int h=0;h<inv_cnt;h++)
                if(inv_idx[h]==i){skip=1;break;}
            if(!skip) newIR[newIR_idx++]=OPT_IR[i];
        }

        // copy everthing after loop into the final IR table
        for(int i=loop_end+1;i<OPT_IR_idx;i++)
            newIR[newIR_idx++]=OPT_IR[i];

        //copy the final version of the IR
        memcpy(OPT_IR, newIR, newIR_idx*sizeof(Quad));
        OPT_IR_idx = newIR_idx;

        hoisted_this_round = inv_cnt;
        total_hoisted      += inv_cnt;

    } while(hoisted_this_round > 0);

    printf("[Loop Invariant Code Motion] %d quad(s) hoisted out of loop(s). OPT_IR has %d quad(s).\n",
           total_hoisted, OPT_IR_idx);
    return total_hoisted;
}

// Constant Propagation

#define CP_HASH_SIZE 1024
typedef struct{
	char name[20];
	char value[20];
	int active;
	int occupied;
} ConstEntry;

static ConstEntry const_map[CP_HASH_SIZE];

//hash function
static unsigned int hash_str(const char* s){
	unsigned int h = 5381;
	while(*s){
		h = ((h << 5) + h) + (unsigned char)(*s);
		s++;
	}
	return h & (CP_HASH_SIZE - 1);
}
//initiaze the hash table
static void const_map_init(void){
	memset(const_map,0,sizeof(const_map));
}

//find out the variables in loop and blacklist them
#define BLACKLIST_SIZE 256
static char blacklist[BLACKLIST_SIZE][20];
static int blacklist_cnt = 0;

static void blacklist_init(void){
    blacklist_cnt = 0;
}

static void blacklist_add(const char* name){
    if(!name || !name[0]) return;
    for(int i=0;i<blacklist_cnt;i++){
        if(strcmp(blacklist[i],name)==0) return; //already there
    }
    if(blacklist_cnt< BLACKLIST_SIZE){
        strcpy(blacklist[blacklist_cnt++],name);
    }
}

static int is_blacklisted(const char* name){
    for(int i = 0; i < blacklist_cnt; i++)
        if(strcmp(blacklist[i], name) == 0) return 1;
    return 0;
}


static void build_blacklist(const Quad* tmp, int tmp_n){
    blacklist_init();
    for(int i = 0; i < tmp_n; i++){
        //to find out the initial loop label
        if(strcmp(tmp[i].op, "label") != 0) continue;
        const char* lname = tmp[i].result;

        for(int j = i+1; j < tmp_n; j++){
            //confirm if loop by checking for goto to that label
            if(strcmp(tmp[j].op, "goto") == 0 &&
               strcmp(tmp[j].result, lname) == 0){
                // mark every result written inside this loop from i+1 to j
                for(int k = i+1; k < j; k++){
                    if(tmp[k].result[0] != '\0'){
                        blacklist_add(tmp[k].result);
                    }
                }
                break;
            }
        }
    }
}

//searching function in the constant table
static const char* const_lookup(const char* name){
	unsigned int h = hash_str(name);
	for(int i=0;i<CP_HASH_SIZE;i++){
		unsigned int slot = (h+i) & (CP_HASH_SIZE-1);
		if(!const_map[slot].occupied){
			return NULL; //empty slot so key not inserted here
		}
		if(const_map[slot].active && strcmp(const_map[slot].name,name)==0){
			return const_map[slot].value;
		}
	}
	return NULL;
}

//add entry to the constant table
static void const_set(const char* name,const char* value){
	unsigned int h = hash_str(name);
	for(int i=0;i<CP_HASH_SIZE;i++){
		unsigned int slot = (h+i) & (CP_HASH_SIZE -1 );
		if(const_map[slot].occupied && strcmp(const_map[slot].name,name)==0){
			strcpy(const_map[slot].value,value);
			const_map[slot].active=1;
			return;
		}
		if(!const_map[slot].occupied){
			strcpy(const_map[slot].name,name);
			strcpy(const_map[slot].value,value);
			const_map[slot].active=1;
			const_map[slot].occupied=1;
			return;
		}
	}
	fprintf(stderr,"[const_map] table size is full increase CP_HASH_SIZE.\n");
}

//kill the variable
static void const_kill(const char* name){
	unsigned int h = hash_str(name);
	for(int i=0;i < CP_HASH_SIZE;i++){
		unsigned int slot = (h + i) & (CP_HASH_SIZE -1);
		if(!const_map[slot].occupied){
			return; // not found
		}
		if(strcmp(const_map[slot].name,name)==0){
			const_map[slot].active=0; //kill
			return;
		}
	}
}

//print the constant table debugging
static void print_const_map(void){
    printf("\n%-6s  %-20s  %-20s  %-8s  %-8s\n",
           "SLOT", "NAME", "VALUE", "ACTIVE", "OCCUPIED");
    printf("%-6s  %-20s  %-20s  %-8s  %-8s\n",
           "----", "----", "-----", "------", "--------");
    for(int i = 0; i < CP_HASH_SIZE; i++){
        if(const_map[i].occupied){
            printf("%-6d  %-20s  %-20s  %-8s  %-8s\n",
                i,
                const_map[i].name,
                const_map[i].value,
                const_map[i].active   ? "yes" : "no",
                const_map[i].occupied ? "yes" : "no");
        }
    }
}
//check if it is arithmetic equation
int is_arith_quad(const char* op){
    return (
        strcmp(op, "+")  == 0 || strcmp(op, "-")  == 0 ||
        strcmp(op, "*")  == 0 || strcmp(op, "/")  == 0 ||
        strcmp(op, "%")  == 0 || strcmp(op, ">")  == 0 ||
        strcmp(op, "<")  == 0 || strcmp(op, "==") == 0 ||
        strcmp(op, "&&") == 0 || strcmp(op, "||") == 0 ||
        strcmp(op, "!")  == 0 || strcmp(op, "&")  == 0 ||
        strcmp(op, "|")  == 0 || strcmp(op, "[]") == 0 ||
        strcmp(op, "=")  == 0 || strcmp(op, "<<") == 0 ||
        strcmp(op, ">>") == 0
    );
}

int is_snapshot_tracked(const char* name) {
    for(int i = 0; i < OPT_IR_idx; i++) {
        if(strcmp(OPT_IR[i].op, "snapshot_track") == 0 &&
           strcmp(OPT_IR[i].arg1, name) == 0)
            return 1;
    }
    return 0;
}
//the major function that is exposed
int constant_propagation(void){
    Quad tmp[IR_SIZE];
    int tmp_n = OPT_IR_idx;
    memcpy(tmp, OPT_IR, tmp_n * sizeof(Quad));

    OPT_IR_idx = 0;
    const_map_init();

    //black list the loop variables
    build_blacklist(tmp,tmp_n);
    int total = 0;

    for(int i = 0; i < tmp_n; i++){
        Quad q = tmp[i];
        int changed = 0;
		if(q.result[0] != '\0' && is_snapshot_tracked(q.result)){
    	    OPT_IR[OPT_IR_idx++] = q;
    	    continue;
    	}
	if(strcmp(q.op, "func") == 0 || strcmp(q.op, "endfunc") == 0 || strcmp(q.op, "method") == 0 || strcmp(q.op, "constr") == 0 || strcmp(q.op, "end_constr") == 0){
		const_map_init();
	}
        if(is_arith_quad(q.op)){

            // Special case — [] op: only propagate into arg2, never arg1
            if(strcmp(q.op, "[]") == 0){
                if(q.arg2[0] != '\0' && !is_numeric(q.arg2)){
                    const char* v = const_lookup(q.arg2);
                    if(v){ strcpy(q.arg2, v); changed = 1; }
                }
            }
            else {
                // Step 1 — Propagate into arg1
                if(q.arg1[0] != '\0' && !is_numeric(q.arg1)){
                    const char* v = const_lookup(q.arg1);
                    if(v){
                        //printf("[CP] quad[%d]: arg1 '%s' → '%s'\n", i, q.arg1, v);
                        strcpy(q.arg1, v);
                        changed = 1;
                    }
                }

                // Step 2 — Propagate into arg2
                if(q.arg2[0] != '\0' && !is_numeric(q.arg2)){
                    const char* v = const_lookup(q.arg2);
                    if(v){
                        //printf("[CP] quad[%d]: arg2 '%s' → '%s'\n", i, q.arg2, v);
                        strcpy(q.arg2, v);
                        changed = 1;
                    }
                }

                // Step 3 — Constant folding: both args numeric now
                if(q.arg2[0] != '\0' && is_numeric(q.arg1) && is_numeric(q.arg2)){
                    double a  = atof(q.arg1);
                    double b  = atof(q.arg2);
                    double r  = 0;
                    int valid = 1;

                    if     (strcmp(q.op, "+")  == 0) r = a + b;
                    else if(strcmp(q.op, "-")  == 0) r = a - b;
                    else if(strcmp(q.op, "*")  == 0) r = a * b;
                    else if(strcmp(q.op, "/")  == 0){
                        if(b == 0){ valid = 0; fprintf(stderr, "[CP] divide by zero at quad[%d]\n", i); }
                        else r = a / b;
                    }
                    else if(strcmp(q.op, "%")  == 0){
                        if((int)b == 0){ valid = 0; }
                        else r = (int)a % (int)b;
                    }
                    else if(strcmp(q.op, ">")  == 0) r = (a >  b);
                    else if(strcmp(q.op, "<")  == 0) r = (a <  b);
                    else if(strcmp(q.op, "==") == 0) r = (a == b);
                    else valid = 0;   // <<, >>, &&, || not foldable here

                    if(valid){
                        char val[32];
                        if(fabs(r - (int)r) < 1e-9) sprintf(val, "%d", (int)r);
                        else                         sprintf(val, "%f", r);
                       // printf("[CF] quad[%d]: folding '%s %s %s' → '%s'\n",i, q.arg1, q.op, q.arg2, val);
                        make_copy_quad(&q, q.result, val);
                        changed = 1;
                    }
                }
            }

            // Step 4 — Update const_map based on what result is now
            if(q.result[0] != '\0'){
                if(strcmp(q.op, "=") == 0 && q.arg2[0] == '\0' && is_numeric(q.arg1)){
                    // only record if NOT a loop variable
                    if(!is_blacklisted(q.result)){
                        const_set(q.result, q.arg1);
                        //printf("[CP] quad[%d]: recording '%s' = '%s'\n", i, q.result, q.arg1);
                    } else {
                        //printf("[CP] quad[%d]: skipping loop var '%s'\n", i, q.result);
                    }
                } else {
                    const_kill(q.result);
                }
            }

        } // end is_arith

        // emit the quad — arith or control flow
        OPT_IR[OPT_IR_idx++] = q;
        if(changed) total++;

    } // end for — return is OUTSIDE the loop

    printf("\n[Constant Propagation] %d change(s) applied. OPT_IR has %d quad(s).\n",
           total, OPT_IR_idx);
    print_const_map();
    return total;
}

// Implementation of the common subexpression elimination
#define CSE_HASH_SIZE 1024

typedef struct CSEEntry {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
    int active;
    int occupied;
} CSEEntry;

static CSEEntry cse_map[CSE_HASH_SIZE];

//Hash function for cse that use all the fields combined op, arg1, arg2
static unsigned int hash_cse(const char* op,const char* arg1, const char* arg2){
    unsigned int h = 5381;
    //hash op
    for(const  char* s = op; *s ; s++) h = ((h<<5) + h ) + (unsigned char)(*s);
    h ^=0x9e3779b9; //to mix between fields to avoid collision
    //hash arg1
    for(const char* s = arg1; *s; s++) h = ((h<<5) + h) + (unsigned char)(*s);
    h ^=0x9e3779b9;
    //hash arg2
    for(const char* s = arg2; *s; s++) h = ((h << 5) + h) + (unsigned char)(*s);
    return h & (CSE_HASH_SIZE - 1);
}
//Initialze the cse map
static void cse_map_init(void){
    memset(cse_map,0,sizeof(cse_map));
}

//Lookup(op,arg1,arg2) - returns result or NULL
static const char* cse_lookup(const char* op, const char* arg1, const char* arg2){
    unsigned int h = hash_cse(op,arg1,arg2);
    for(int i=0;i<CSE_HASH_SIZE;i++){
        unsigned int slot = (h + i) & (CSE_HASH_SIZE -1);
        if(!cse_map[slot].occupied) return NULL; //empty so expression will not be here for sure
        if(cse_map[slot].active             &&
           strcmp(cse_map[slot].op,   op)   == 0 &&
           strcmp(cse_map[slot].arg1, arg1) == 0 &&
           strcmp(cse_map[slot].arg2, arg2) == 0){
            return cse_map[slot].result;
        }
    }
    return NULL;
}

//Lookup with commutative check for + * & | ==
static const char* cse_lookup_commutative(const char* op, const char* arg1,const char* arg2){
    const char* r = cse_lookup(op,arg1,arg2);
    if(r) return r;
    if(strcmp(op,"+")  == 0 || strcmp(op,"*")  == 0 ||
       strcmp(op,"&")  == 0 || strcmp(op,"|")  == 0 ||
       strcmp(op,"==") == 0 || strcmp(op,"&&") == 0 ||  
       strcmp(op,"||") == 0){                           
        r = cse_lookup(op, arg2, arg1);
    }
    return r;
}

// Add (op, arg1, arg2) -> result
static void cse_add(const char* op, const char* arg1, const char* arg2, const char* result){
    unsigned int h = hash_cse(op,arg1,arg2);
    for(int i=0;i<CSE_HASH_SIZE;i++){
        unsigned int slot = (h + i) & (CSE_HASH_SIZE -1);
        //update the result if it already exists
        if(cse_map[slot].occupied &&
           strcmp(cse_map[slot].op,   op)   == 0 &&
           strcmp(cse_map[slot].arg1, arg1) == 0 &&
           strcmp(cse_map[slot].arg2, arg2) == 0){
            strcpy(cse_map[slot].result, result);
            cse_map[slot].active = 1;
            return;
        }
        //insert into empty slot
        if(!cse_map[slot].occupied){
            strcpy(cse_map[slot].op,     op);
            strcpy(cse_map[slot].arg1,   arg1);
            strcpy(cse_map[slot].arg2,   arg2);
            strcpy(cse_map[slot].result, result);
            cse_map[slot].active   = 1;
            cse_map[slot].occupied = 1;
            return;
        }
    }
    fprintf(stderr, "[cse_map] table full! Increase CSE_HASH_SIZE.\n");
}

// Kill when arg1, arg2 or result changes
static void cse_kill(const char* name){
    if(!name || !name[0]) return;
    for(int i = 0; i < CSE_HASH_SIZE; i++){
        if(!cse_map[i].occupied || !cse_map[i].active) continue;
        if(strcmp(cse_map[i].arg1,   name) == 0 ||
           strcmp(cse_map[i].arg2,   name) == 0 ||
           strcmp(cse_map[i].result, name) == 0){
            cse_map[i].active = 0;  // soft delete — keep occupied=1
        }
    }
}

//print the cse optimizations done
static void print_cse_map(void){
    printf("\n%-6s  %-10s  %-15s  %-15s  %-15s  %-8s\n",
           "SLOT", "OP", "ARG1", "ARG2", "RESULT", "ACTIVE");
    printf("%-6s  %-10s  %-15s  %-15s  %-15s  %-8s\n",
           "----", "--", "----", "----", "------", "------");
    for(int i = 0; i < CSE_HASH_SIZE; i++){
        if(cse_map[i].occupied){
            printf("%-6d  %-10s  %-15s  %-15s  %-15s  %-8s\n",
                i,
                cse_map[i].op,
                cse_map[i].arg1,
                cse_map[i].arg2,
                cse_map[i].result,
                cse_map[i].active ? "yes" : "no");
        }
    }
}

//ops that are safe to use CSE
static int is_cse_candidate(const char* op){
    return (
        strcmp(op, "+")  == 0 || strcmp(op, "-")  == 0 ||
        strcmp(op, "*")  == 0 || strcmp(op, "/")  == 0 ||
        strcmp(op, "%")  == 0 || strcmp(op, ">")  == 0 ||
        strcmp(op, "<")  == 0 || strcmp(op, "==") == 0 ||
        strcmp(op, "&&") == 0 || strcmp(op, "||") == 0 ||
        strcmp(op, "!")  == 0 || strcmp(op, "&")  == 0 ||
        strcmp(op, "|")  == 0 || strcmp(op, "<<") == 0 ||
        strcmp(op, ">>") == 0
    );
}

//the main common subexpression elimination function
int common_subexpression_elimination(void){
    Quad tmp[IR_SIZE];
    int tmp_n = OPT_IR_idx;
    memcpy(tmp, OPT_IR, tmp_n * sizeof(Quad));

    OPT_IR_idx = 0;
    cse_map_init();
    int total = 0;

    for(int i = 0; i < tmp_n; i++){
        Quad q = tmp[i];
        int changed = 0;
	if(strcmp(q.op, "func") == 0 || strcmp(q.op, "endfunc") == 0 || strcmp(q.op, "method") == 0 || strcmp(q.op, "constr") == 0 || strcmp(q.op, "end_constr") == 0){
		cse_map_init();
	}

        if(is_cse_candidate(q.op)){
            char orig_op[20], orig_a1[20], orig_a2[20];
            strcpy(orig_op, q.op);
            strcpy(orig_a1, q.arg1);
            strcpy(orig_a2, q.arg2);

            const char* prev = cse_lookup_commutative(orig_op, orig_a1, orig_a2);

            if(prev){
                char old_result[20];
                strcpy(old_result, q.result);
                make_copy_quad(&q, old_result, prev);
                changed = 1;
                cse_add(orig_op, orig_a1, orig_a2, old_result);
                // no kill — keep (a+b) alive via new result
            }
            else{
                cse_kill(q.result);   // result being overwritten, kill stale entries
                cse_add(orig_op, orig_a1, orig_a2, q.result);
                // no kill again
            }
        }
        else{
            // control flow, copies, labels etc — just kill the result
            if(q.result[0] != '\0'){
                cse_kill(q.result);
            }
        }

        OPT_IR[OPT_IR_idx++] = q;
        if(changed) total++;
    }

    printf("[CSE] %d substitution(s) applied. OPT_IR has %d quad(s).\n", total, OPT_IR_idx);
    print_cse_map();
    return total;
}

// strength reduction code
int strength_reduction(void){
    Quad tmp[IR_SIZE];
    int tmp_n = OPT_IR_idx;
    memcpy(tmp,OPT_IR,tmp_n*sizeof(Quad));

    OPT_IR_idx = 0;
    int total = 0;

    for(int i=0;i<tmp_n;i++){
        Quad q = tmp[i];
        int changed = 0;
        //printf("[SR debug] quad[%d]: op='%s' arg1='%s' arg2='%s' result='%s'\n",i, q.op, q.arg1, q.arg2, q.result);
        // Multiplication to addition strength reduction
        if(strcmp(q.op,"*") == 0){
            //printf("[SR debug] MUL case: arg2='%s' is_numeric=%d\n",q.arg2, is_numeric(q.arg2));
            // x * 1 -> x (just if any additionally come after algebraic simplification)
            if(is_int_val(q.arg2,1)){
                make_copy_quad(&q, q.result,q.arg1);
                changed = 1;
            }
            else if(is_int_val(q.arg1,1)){
                make_copy_quad(&q, q.result, q.arg2);
                changed = 1;
            }
            // x * 2 -> x + x
            else if(is_int_val(q.arg1,2)){
                make_binop_quad(&q, "+", q.arg2, q.arg2, q.result);
                changed = 1;
            }
            else if(is_int_val(q.arg2,2)){
                make_binop_quad(&q, "+", q.arg1, q.arg1, q.result);
                changed = 1;
            }
            // x * 2^k -> x << k
            else if(is_numeric(q.arg2)){
                int val = (int)atof(q.arg2);
                int e = pow2_exp(val);
              //  printf("[SR debug] val=%d pow2_exp=%d\n", val, e);
                if(e >= 2){
                    char es[12];
                    sprintf(es, "%d", e);
                    make_binop_quad(&q, "<<", q.arg1, es, q.result);
                    changed = 1;
                }
            }
            else if(is_numeric(q.arg1)){
                int val = (int)atof(q.arg1);
                int e = pow2_exp(val);
                if(e>=2){
                    char es[12];
                    sprintf(es, "%d", e);
                    make_binop_quad(&q, "<<", q.arg2, es, q.result);
                    changed = 1;
                }
            }
        }
        //Division strength reduction
        else if(strcmp(q.op, "/") == 0){

            // x / 1  →  x
            if(is_int_val(q.arg2, 1)){
                make_copy_quad(&q, q.result, q.arg1);
                changed = 1;
            }
            // x / 2^k  →  x >> k  (k >= 1)
            else if(is_numeric(q.arg2)){
                int val = (int)atof(q.arg2);
                int e   = pow2_exp(val);
                if(e == -1 && val == 2) e = 1; // pow2_exp returns -1 for val<2
                if(e >= 1){
                    char es[12];
                    sprintf(es, "%d", e);
                    make_binop_quad(&q, ">>", q.arg1, es, q.result);
                    changed = 1;
                }
            }
        }

        // Modulo strength reduction
        else if(strcmp(q.op, "%") == 0){

            // x % 1  →  0
            if(is_int_val(q.arg2, 1)){
                make_copy_quad(&q, q.result, "0");
                changed = 1;
            }
            // x % 2^k  →  x & (2^k - 1)
            else if(is_numeric(q.arg2)){
                int val = (int)atof(q.arg2);
                // check val is power of 2
                if(val > 1 && (val & (val - 1)) == 0){
                    char mask[12];
                    sprintf(mask, "%d", val - 1);
                    make_binop_quad(&q, "&", q.arg1, mask, q.result);
                    changed = 1;
                }
            }
        }

        OPT_IR[OPT_IR_idx++] = q;
        if(changed) total++;
    }
    printf("[Strength Reduction] %d reduction(s) applied. OPT_IR has %d quad(s).\n",total, OPT_IR_idx);
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
int copy_propagation(int opt_level){
	int subs = 0, removed = 0;
	ct_clear();	// Starts with a clear copy table with copy_count = 0
	build_blacklist(OPT_IR, OPT_IR_idx);
	for(int i = 0; i < OPT_IR_idx; i++){
		Quad* q = &OPT_IR[i];
		// Skip if the current quad's operator is some structural operator
		if(q->result[0] != '\0' && is_snapshot_tracked(q->result)){
    	    OPT_IR[OPT_IR_idx++] = *q;
    	    continue;
    	}
		if(is_structural_op(q->op)){
			continue;
		}
		// if((strcmp(OPT_IR[i].op, "[]") == 0 || 
		//     strcmp(OPT_IR[i].op, "[]=") == 0) &&
		//     strcmp(OPT_IR[i].arg2, name) == 0){
		//     continue; 
		// }
		if (strcmp(q->op, "func") == 0 || strcmp(q->op, "endfunc") == 0 || strcmp(q->op, "method") == 0 || strcmp(q->op, "constr") == 0 || strcmp(q->op, "end_constr") == 0) {
		    ct_clear();
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
		    // Don't substitute array byte-offset — it must stay as the
		    // temp produced by the * (multiply) quad, not a raw index
		    int is_arr_op = (strcmp(q->op, "[]") == 0 || 
		                     strcmp(q->op, "[]=") == 0);
		    if(!is_arr_op){
		        const char* sub = ct_lookup(q->arg2);
		        if(strcmp(sub, q->arg2) != 0){
		            strncpy(q->arg2, sub, 19);
		            subs++;
		        }
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
			if(is_copy && !is_blacklisted(q->result)){
				ct_add(q->result, q->arg1);
			}
		}

	}

	// This part of the code handles the second pass over the IR code and is not used again or it is not a copy quad
	// Inorder to shrink the copy table after killing some pairs we use two-pointer based approach.
      if(opt_level>=2){
	int write = 0;
	for(int read = 0; read < OPT_IR_idx; read++){
		Quad* q = &OPT_IR[read];
		int dead = (strcmp(q->op,"=") == 0 && q->arg2[0]=='\0'
                    && q->result[0] && !is_used_after(q->result, read+1)
		    && !is_blacklisted(q->result));
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
      }
	printf("[CopyProp] %d substitution(s), %d dead copy(ies) removed.  "
           "OPT_IR has %d quad(s).\n", subs, removed, OPT_IR_idx);
	return subs + removed;
}


/* ==== Implementation of Induction Variable Elimination ==== */

#define MAX_LOOPS 64
#define MAX_BIVS 16
#define MAX_DIVS 64

typedef struct {
	int header;	// Stores the index of the label begin for the loop (in IR code)
	int footer; 	// index of goto begin in IR code
	int exit_test;	// Index of the condition check for the loops
	char label_begin[20];
	char label_end[20];
}LoopInfo;

typedef struct{
	char var[20];
	char step_str[20];
	double step;
	int update_idx;
	char op[4];
} BIV;

typedef struct{
	char result[20];
	char biv_name[20];
	char coeff_str[20];
	double coeff;
	int quad_idx;
	char div_op[4];
} DIV_entry;

static LoopInfo g_loops[MAX_LOOPS];
static int g_loop_count;
static BIV g_bivs[MAX_LOOPS][MAX_BIVS];
static int g_biv_count[MAX_LOOPS];
static DIV_entry g_divs[MAX_LOOPS][MAX_DIVS];
static int g_div_count[MAX_LOOPS];
static int g_live_var_cnt = 0;

// Generates the a new replacement variable for the replacement of the induction variable
static void ive_new_var(char* buf){
	sprintf(buf, "iv%d", g_live_var_cnt++);
}

// Perform Induction variable Elimination
int induction_variable_elimination(void){
	g_loop_count = 0;
	int total = 0;

	// Start with detecting the loops by observing the back edges (a goto pointing to a label back of the current index
	for(int i = 0; i < OPT_IR_idx && g_loop_count < MAX_LOOPS; i++){
		if(strcmp(OPT_IR[i].op, "goto") != 0){
			continue;
		}

		const char* target = OPT_IR[i].result;

		// Search for an earlier label with the same name
		int hdr = -1;
		for(int j = 0; j < i; j++){
			if(strcmp(OPT_IR[j].op, "label") == 0 && strcmp(OPT_IR[j].result, target) == 0){
				hdr = j;
				break;
			}
		}
		if(hdr < 0){
			continue;
		}
		LoopInfo* lp = &g_loops[g_loop_count];
		lp->header = hdr;
		lp->footer = i;
		strncpy(lp->label_begin, target, 19);
		lp->exit_test = -1;
		lp->label_end[0] = '\0';
		g_biv_count[g_loop_count] = 0;
        	g_div_count[g_loop_count] = 0;

		for(int j = hdr; j <= i; j++){
			if(strcmp(OPT_IR[j].op, "ifFalse") == 0){
				lp->exit_test = j;
				strncpy(lp->label_end, OPT_IR[j].result,19);
				break;
			}
		}
		g_loop_count++;
	}
	if(g_loop_count == 0){
		printf("[IVE] No loops detected.\n");
		return 0;
	}

	// Handling the nested loops by first processing the inner loops and avoid processing considering the inner loop again when considering the outer loops
	for(int a = 0; a < g_loop_count - 1; a++){
		for(int b = a + 1; b < g_loop_count; b++){
			int size_a = g_loops[a].footer - g_loops[a].header;
			int size_b = g_loops[b].footer - g_loops[b].header;
			if(size_a > size_b){
				LoopInfo tmp = g_loops[a];
				g_loops[a] = g_loops[b];	
				g_loops[b] = tmp;
		
				int tc = g_biv_count[a];
				g_biv_count[a] = g_biv_count[b];
				g_biv_count[b] = tc;
				tc = g_div_count[a];
				g_div_count[a] = g_div_count[b];
				g_div_count[b] = tc;
			}
		}
	}

	for(int li = 0; li < g_loop_count; li++){
		LoopInfo* lp = &g_loops[li];
		int body_start = lp->header + 1;
		int body_end = lp->footer - 1;
		if(body_start > body_end){
			continue;
		}

		// Collect BIVs that is (i = i +/- c)
		// Collect BIVs that is (i = i +/- c)
		int bc = 0;
		for(int i = body_start; i <= body_end && bc < MAX_BIVS; i++){
			int in_inner = 0;
			for(int x = 0; x < g_loop_count; x++){
				if(x == li){
					continue;
				}
				int size_x = g_loops[x].footer - g_loops[x].header;
				int size_li = g_loops[li].footer - g_loops[li].header;
				if(size_x >= size_li){
					continue;
				}
				if(i > g_loops[x].header && i <= g_loops[x].footer){
					in_inner = 1;
					i = g_loops[x].footer;
					break;
				}
			}
			if(in_inner){
				continue;
			}
		    	Quad* q = &OPT_IR[i];
		    	if((strcmp(q->op, "+") == 0 || strcmp(q->op, "-") == 0) && q->result[0] && is_numeric(q->arg2))
		    	{
				if(strcmp(q->arg1, q->result) == 0){
			    	// direct  i = i + c
			    	BIV* b = &g_bivs[li][bc++];
			    	strncpy(b->var, q->result, 19);
			    	strncpy(b->step_str, q->arg2, 19);
			    	strncpy(b->op, q->op, 3);
			    	b->step = (strcmp(q->op,"-")==0) ? -atof(q->arg2) : atof(q->arg2);
			    	b->update_idx = i;
				} else {
			    		// t = i + c  then  i = t  (two-quad pattern)
					for(int k = i+1; k <= body_end; k++){
						Quad* nq = &OPT_IR[k];
						if(strcmp(nq->op,"=")==0 && nq->arg2[0]=='\0' &&
						strcmp(nq->arg1, q->result)==0 &&
						strcmp(nq->result, q->arg1)==0)
						{
						BIV* b = &g_bivs[li][bc++];
						strncpy(b->var, nq->result, 19);
						strncpy(b->step_str, q->arg2, 19);
						strncpy(b->op, q->op, 3);
						b->step = (strcmp(q->op,"-")==0) ? -atof(q->arg2) : atof(q->arg2);
						b->update_idx = k;
						break;
						}
					}
				}
		    	}
		}
		g_biv_count[li] = bc;
		if(bc == 0) continue;

		// Collecting DIVs
		int dc = 0;
		for(int i = body_start; i <= body_end && dc < MAX_DIVS; i++){
			/*
			for(int x = 0; x < g_loop_count; x++){
				if(x == li){
					continue;
				}
				if(i > g_loops[x].header && i <= g_loops[x].footer){
					in_inner = 1;
					i = g_loops[x].footer;
					break;
				}
			}
			if(in_inner){
				continue;
			}*/
			Quad* q = &OPT_IR[i];
			if(strcmp(q->op, "*") != 0){
				continue;
			}
			if(!q->result[0]){
				continue;
			}

			for(int b = 0; b < bc; b++){
				const char* bname = g_bivs[li][b].var;
				char coeff[20] = "";
				int match = 0;

				if(strcmp(q->arg1,bname)==0 && is_numeric(q->arg2)){
				    strncpy(coeff, q->arg2, 19); match = 1;
				} else if(strcmp(q->arg2,bname)==0 && is_numeric(q->arg1)){
				    strncpy(coeff, q->arg1, 19); match = 1;
				}
				if(!match) continue;

				// scan forward for copy-to-named-var:  named_var = temp
				const char* real_result = q->result;
				int real_idx = i;
				for(int k = i+1; k <= body_end; k++){
				    Quad* nq = &OPT_IR[k];
				    if(strcmp(nq->op,"=")==0 && nq->arg2[0]=='\0' &&
				       strcmp(nq->arg1, q->result)==0)
				    {
					real_result = nq->result;
					real_idx = k;
					break;
				    }
				}

				// skip if real_result is itself a BIV (that is the increment quad, not a DIV)
				int is_biv = 0;
				for(int bb = 0; bb < bc; bb++)
				    if(strcmp(real_result, g_bivs[li][bb].var)==0){ is_biv=1; break; }
				if(is_biv) continue;
				
				int write_count = 0;
				for(int w = body_start; w <= body_end; w++){
					if(strcmp(OPT_IR[w].result, real_result) == 0){
						write_count++;
					}
				}
				if(write_count > 1){
					continue;
				}

				DIV_entry* d = &g_divs[li][dc++];
				strncpy(d->result,    real_result, 19);
				strncpy(d->biv_name,  bname,       19);
				strncpy(d->coeff_str, coeff,        19);
				d->coeff = atof(coeff);
				d->quad_idx = real_idx;
				strncpy(d->div_op, q->op, 3);
				break;
			    }
		}
		g_div_count[li] = dc;
		if(dc == 0) continue;

		// convert each of the DIV into a addition based updation
		for(int di = 0; di < dc; di++){
			DIV_entry* dv = &g_divs[li][di];

			BIV* bv = NULL;
			for(int b = 0; b < bc; b++){
				if(strcmp(g_bivs[li][b].var, dv->biv_name) == 0){
					bv = &g_bivs[li][b];
					break;
				}
			}
			if(!bv){
				continue;
			}
			char j[20];
			ive_new_var(j);

			// Insert the newly generated ive before the header
			int ins = lp->header;	// Slot to opened before the label
			if(!insert_opt_quad_at(ins)){
				continue;
			}

			for(int x = 0; x < g_loop_count; x++){
				if(g_loops[x].header >= ins){
					g_loops[x].header++;
				}
				if(g_loops[x].footer >= ins){
					g_loops[x].footer++;
				}
				if(g_loops[x].exit_test >= ins){
					g_loops[x].exit_test++;
				}
			}
			for (int b = 0; b < bc; b++)
				if (g_bivs[li][b].update_idx >= ins) g_bivs[li][b].update_idx++;
			for (int dj = di; dj < dc; dj++)
				if (g_divs[li][dj].quad_idx >= ins) g_divs[li][dj].quad_idx++;
			for (int b = 0; b < bc; b++)
				if (strcmp(g_bivs[li][b].var, dv->biv_name)==0)
				    { bv = &g_bivs[li][b]; break; }

			Quad* init_q = &OPT_IR[ins];
			/*strcpy(init_q->op, dv->div_op);
			strcpy(init_q->arg1, bv->var);
			strcpy(init_q->arg2, dv->coeff_str);
			strcpy(init_q->result, j);*/
			char biv_init_str[20] = "0";	// set the default value to 0 which is the case if no biv is found
			for(int k = 0; k < lp->header; k++){
				if(strcmp(OPT_IR[k].op, "=") == 0 &&
					OPT_IR[k].arg2[0] == '\0' &&
					strcmp(OPT_IR[k].result, bv->var) == 0 &&
					is_numeric(OPT_IR[k].arg1))
				{
					strcpy(biv_init_str, OPT_IR[k].arg1);
				}
			}

			double biv_init = atof(biv_init_str);
			double div_init = biv_init * dv->coeff;

			char div_init_str[24];
			if(div_init == (long)div_init){
				sprintf(div_init_str,"%ld", (long)div_init);
			}
			else{
				sprintf(div_init_str, "%f", div_init);
			}

			init_q = &OPT_IR[ins];
			strcpy(init_q->op, "=");
			strcpy(init_q->arg1, div_init_str);
			init_q->arg2[0] = '\0';
			strcpy(init_q->result, j); 

			// Insert the update statements for the generated ive variables
			double inc_val = bv->step * dv->coeff;
			char inc_str[24];
			if(inc_val == (long)inc_val) {
				sprintf(inc_str, "%ld", (long)inc_val);
			}
			else{
				sprintf(inc_str, "%f",  inc_val);
			}

			int inc_pos = bv->update_idx + 1;
			if(!insert_opt_quad_at(inc_pos)) {
				continue;
			}
			for(int x = 0; x < g_loop_count; x++){
				if(g_loops[x].footer >= inc_pos){
					g_loops[x].footer++;
				}
				if(g_loops[x].exit_test >= inc_pos){
					g_loops[x].exit_test++;
				}
			}
			for(int dj = di; dj < dc; dj++){
				if (g_divs[li][dj].quad_idx >= inc_pos){
				       	g_divs[li][dj].quad_idx++;
				}
			}
			Quad* inc_q = &OPT_IR[inc_pos];
			strcpy(inc_q->op, "+");
			strcpy(inc_q->arg1, j);
			strcpy(inc_q->arg2, inc_str);
			strcpy(inc_q->result, j);

			make_copy_quad(&OPT_IR[dv->quad_idx], dv->result, j);
			total++;
		}
	}
	printf("[IVE] %d induction variable(s) eliminated. OPT_IR has %d quads.\n", total, OPT_IR_idx);
	return total;
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

