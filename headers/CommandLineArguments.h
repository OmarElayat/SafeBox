#ifndef COMMANDLINEARGUMENTS_H
#define COMMANDLINEARGUMENTS_H

#include <includes.h>
#include <defines.h>
#include <SafeBox.h>

class CommandLineArguments
{
    private:
        char input_file_name [COMMAND_LINE_ARGUMENT_MAX_SIZE];
        char output_file_name [COMMAND_LINE_ARGUMENT_MAX_SIZE];
        char working_dir [COMMAND_LINE_ARGUMENT_MAX_SIZE];
        char command [COMMAND_LINE_ARGUMENT_MAX_SIZE];
        char key_file [COMMAND_LINE_ARGUMENT_MAX_SIZE];
        uint16_t block_size;
        uint16_t shreds;
        char error_string[ERROR_STRING_MAX_SIZE];
        map <string,SafeBox *> commands;
    public:
        CommandLineArguments();
        bool parser(int argc,char ** argv);
        bool execute();
        char * getErrorString();
        ~CommandLineArguments();
};

#endif