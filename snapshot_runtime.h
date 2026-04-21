#ifndef SNAPSHOT_RUNTIME_H
#define SNAPSHOT_RUNTIME_H

void __snapshot_init(int n_vars);
void __snapshot_register_var(int* addr, const char* name);
void __snapshot_capture(void);
void __snapshot_rewind(int n);
void __snapshot_cleanup(void);

#endif