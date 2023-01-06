![image](https://user-images.githubusercontent.com/107650627/209885689-0e2e0149-6e73-4c28-8702-164b2cca4263.png)

SafeBox is a Cryptography project aiming to encrypt and decrypt infinitely huge data in short time durations using Object Oriented Programming, and multithreading.

## User Guide
1. Put your data in the data directory in a .txt file.
2. Put your current directory in makefile.vars.
3. Enter the appropriate encryption parameters on the linux terminal. Including the number of shreds needed, the block size, the key file directory,.... etc. 

Example:
```
./bin/safebox --command import --input-file /home/omarelayat/Desktop/safebox/data/data.txt --output-file encrypted.txt --working-dir /home/omarelayat/Desktop/safebox/output/ --shreds 5 --block-size 1024 --key-file /home/omarelayat/Desktop/safebox/key_files/key.txt
```
4. Make sure to write the right path in input and working directory.
7. Run it in the terminal
8. Now you will have the encrypted files on the output-file directory. To decrypt the encrypted data, use the output of the encryption as an input for the decryption and re-run the terminal with the appropriate parameters.

***NOTE: Export Input file path is the output of the import file. Number of shreds, block size and path of key must be the same in encryption and decryption.***

Example:
```
./bin/safebox --command export --input-file encrypted.txt --output-file decrypted.txt --working-dir /home/omarelayat/Desktop/safebox/output --shreds 5 --block-size 1024 --key-file /home/omarelayat/Desktop/safebox/key_files/key.txt
```
9. Open the decrypted.txt. It should be exactly similar to the original input data.txt.
