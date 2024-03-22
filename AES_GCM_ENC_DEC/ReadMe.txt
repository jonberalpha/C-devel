# Requirements
libssl-dev

# Compilation
make

# Remove binary and object file
make clean

# Program Parameters
# ./crypto e|d input output
# argv[0] = ./crypto
# argv[1] = e or d
# argv[2] = input
# argv[3] = output

# Program execution in terminal
# ./crypto e text.txt enc_text.txt
# Password: 12345
# ./crypto d enc_text.txt dec_text.txt
# Password: 12345
