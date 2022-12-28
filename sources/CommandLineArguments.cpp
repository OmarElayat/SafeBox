#include <CommandLineArguments.h>


CommandLineArguments::CommandLineArguments()
{
    memset(input_file_name,0,COMMAND_LINE_ARGUMENT_MAX_SIZE);
    memset(output_file_name,0,COMMAND_LINE_ARGUMENT_MAX_SIZE);
    memset(working_dir,0,COMMAND_LINE_ARGUMENT_MAX_SIZE);
    memset(command,0,COMMAND_LINE_ARGUMENT_MAX_SIZE);
    memset(key_file,0,COMMAND_LINE_ARGUMENT_MAX_SIZE);
    memset(error_string,0,ERROR_STRING_MAX_SIZE);
    block_size = 1024;
    shreds = 10;
    commands["import"] = new SafeBoxImport(); 
    commands["export"] = new SafeBoxExport(); 
}
bool CommandLineArguments::parser(int argc,char ** argv)
{
    if ( argc %2 == 0 || argc < 2) return false;
    else
    {
        for ( uint8_t i = 1 ; i < argc ; i +=2)
        {
            if ( strcmp(argv[i],"--input-file") == 0)
                strncpy(input_file_name,argv[i+1],COMMAND_LINE_ARGUMENT_MAX_SIZE-1);
            else if ( strcmp(argv[i],"--output-file") == 0)
                strncpy(output_file_name,argv[i+1],COMMAND_LINE_ARGUMENT_MAX_SIZE-1);
            else if ( strcmp(argv[i],"--command") == 0)
                strncpy(command,argv[i+1],COMMAND_LINE_ARGUMENT_MAX_SIZE-1);
            else if ( strcmp(argv[i],"--working-dir") == 0)
                strncpy(working_dir,argv[i+1],COMMAND_LINE_ARGUMENT_MAX_SIZE-1);
            else if ( strcmp(argv[i],"--key-file") == 0)
                strncpy(key_file,argv[i+1],COMMAND_LINE_ARGUMENT_MAX_SIZE-1);
            else if ( strcmp(argv[i],"--block-size") == 0)
                block_size =atoi(argv[i+1]);
            else if ( strcmp(argv[i],"--shreds") == 0)
                shreds =atoi(argv[i+1]);
            else
            {
                sprintf(error_string,"undefined parameter: %s\n",argv[i]);
                return false;
            }
        }
        if ( strcmp (input_file_name,"") == 0 || strcmp (output_file_name,"") == 0 || 
            strcmp (command,"") == 0 || strcmp (working_dir,"") == 0 ||
            strcmp (key_file,"") == 0 || block_size == 0 || shreds == 0 ) return false;
        else return true;
    }
}
bool CommandLineArguments::execute()
{
    if ( commands[command] )
    {
        SafeBox * sb = commands[command]->clone();
        sb->process (input_file_name,
                    output_file_name,
                    working_dir,
                    key_file,
                    block_size,
                    shreds);
        delete (sb);
        return true;
    }
    else 
    {
        sprintf(error_string,"undefined command: %s\n",command);
        return false;
    }
}
char * CommandLineArguments::getErrorString()
{
    return error_string;
}
CommandLineArguments::~CommandLineArguments()
{
    
}
