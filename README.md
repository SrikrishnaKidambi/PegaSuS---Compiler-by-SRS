# PegaSuS---Compiler-by-SRS
- This compiler is supported only in Linux machines. Follow the below steps to run the compiler

## Steps to install:

### Required packages:
- GNU-based RISCv Compiler:
``` 
sudo apt install gcc-riscv64-linux-gnu
```

- Qemulator installation:
```
sudo apt install qemu-user
```

- Clone the repo: 
``` git clone "
```

- In the root folder of the repo, run the following command to compile the project:
``` 
make 
```
- Then run the command to install the compiler in your linux machine
```
make install
```

- Follow the installation commands for the riscv gnu compiler and/or qemu compiler if they are reported missing on performing "make install".

- Type "pegasus --help" to see the flags provided and run the pegasus code.
