#ifndef OPTIMIZER_H
#define OPTIMIZER_H

/*
#ifndef QUAD_DEFINED
#define QUAD_DEFINED
// Define the structure of IR code Quadruples

typedef struct{
	char op[20];
	char arg1[20];
	char arg2[20];
	char result[20];
} Quad;
#endif	
*/

#define IR_SIZE 10000	// Define the number of lines of IR code supported

// Original IR code as defined in parser.y and being used only as read-only
//extern Quad IR[];
extern int IR_idx;

// Optimized IR code being stored seperately in without modifying the original IR code
//extern Quad OPT_IR[];
extern int OPT_IR_idx;

// First pass - Algebraic Simplification that reads the entire new copy of IR code and performs algebraic simplification such as using identity or zero or strength-reduction rules for each quadruple. 
// This finally returns the number of simplifications made
int algebraic_simplification(void);
int constant_folding(void);
// Second pass - Copy Propagation that performs operations such as forward substitutes of copy sources and then remove dead copies. This function finally returns the total number of changes combining number of substitutions and removals.
int copy_propagation(int opt_level);
int dead_code_elimination(void);
// Third pass - Induction variable elimination 
int induction_variable_elimination(void);

// Constant propagation function api provided
int constant_propagation(void);
// CSE function api provided
int common_subexpression_elimination(void);
// Strength reduction function api
int strength_reduction(void);

int loop_invariant_code_motion(void); 
void print_opt_IR(void);	// Prints the optimised IR code
void print_original_IR(void);	// Prints the original IR code

#endif

