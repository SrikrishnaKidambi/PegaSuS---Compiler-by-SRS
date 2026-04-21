#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SNAPSHOT_VARS   32
#define MAX_SNAPSHOT_STATES 1024

typedef struct {
    int*  addr;
    char  name[64];
} SnapVar;

typedef struct {
    int values[MAX_SNAPSHOT_VARS];
} SnapState;

static SnapVar    snap_vars[MAX_SNAPSHOT_VARS];
static int        snap_var_count = 0;
static SnapState  snap_history[MAX_SNAPSHOT_STATES];
static int        snap_history_count = 0;
static int        snap_active = 0;

// Forward declaration
void __snapshot_capture(void);

void __snapshot_init(int n_vars){
    snap_var_count = 0;
    snap_history_count = 0;
    snap_active = 1;
    (void)n_vars;
}

void __snapshot_register_var(int* addr, const char* name){
    if(snap_var_count >= MAX_SNAPSHOT_VARS) return;
    snap_vars[snap_var_count].addr = addr;
    strncpy(snap_vars[snap_var_count].name, name, 63);
    snap_vars[snap_var_count].name[63] = '\0';
    snap_var_count++;
    // Take initial snapshot
    __snapshot_capture();
}

void __snapshot_capture(void){
    if(!snap_active) return;
    if(snap_history_count >= MAX_SNAPSHOT_STATES) return;
    SnapState* s = &snap_history[snap_history_count++];
    for(int i = 0; i < snap_var_count; i++){
        s->values[i] = *snap_vars[i].addr;
    }
}

void __snapshot_rewind(int n){
    if(!snap_active){
        fprintf(stderr, "[Snapshot] rewind called outside snapshot block\n");
        return;
    }
    int target = snap_history_count - 1 - n;
    if(target < 0){
        fprintf(stderr, "[Snapshot] rewind(%d): only %d state(s) available\n",
                n, snap_history_count);
        return;
    }
    SnapState* s = &snap_history[target];
    printf("[Snapshot] Rewinding %d step(s) to state %d:\n", n, target);
    for(int i = 0; i < snap_var_count; i++){
        printf("  %s = %d  (was %d)\n",
               snap_vars[i].name,
               s->values[i],
               *snap_vars[i].addr);
        *snap_vars[i].addr = s->values[i];
    }
    snap_history_count = target + 1;
}

void __snapshot_cleanup(void){
    snap_active = 0;
    snap_var_count = 0;
    snap_history_count = 0;
}