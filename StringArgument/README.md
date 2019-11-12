## StringArgument
Trusted functions get strings as arguments with char\* type. This uses [in, out, string] things!

### What does this program do?
Get up to 20 characters from stdin. One trusted function in enclave makes reverse string of it, and another trusted function in the same enclave blinds that string with asterisk(\*) except the first three characters. After all these processing, this program prints the processed string.  
Actually, very similar to ArrayArgument example.

### How to test
`./make.sh`  
... that's all! This program will be compiled as hardware mode.  
Enclave signing procedure uses one-step method. (just sign with my key file)  

Tested on Ubuntu 18.04.2 with kernel 5.0.0-32-generic
