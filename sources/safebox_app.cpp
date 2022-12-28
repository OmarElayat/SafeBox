#include <CommandLineArguments.h>
#include <Lottery.h>


int main(int argc, char **argv)
{    
    time_t start = time(NULL);
    CommandLineArguments cmlarg;
    if (cmlarg.parser(argc,argv))
    {
        if (!cmlarg.execute())
        {
            if ( strcmp(cmlarg.getErrorString(),"") != 0 )
                printf ("%s\n",cmlarg.getErrorString());
        }        
    }
    else
    {
        if ( strcmp(cmlarg.getErrorString(),"") != 0 )
            printf ("%s",cmlarg.getErrorString());
        printf ("usage: %s --command <import/export> --input-file <file-name> --output-file <file-name> --working_dir <working directory full path> --key-file <Full path to file containing the AES key> --block-size <size of a block> --shreds <# of shreds>\n",argv[0]);
        exit(1);
   }
    time_t end = time(NULL);
    printf ("Total time: %lu\n",end-start);
    
 
    return 0;
}