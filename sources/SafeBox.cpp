#include <SafeBox.h>

SafeBox::SafeBox ()
{

}
SafeBox::~SafeBox()
{

}
SafeBoxImport::SafeBoxImport (){

}
void SafeBoxImport::process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds){
        printf ("Import file\n");
        char outf [1024];
        char outiv [1024];
        char outq [1024];
        memset (outf,0,1024);
        memset (outiv,0,1024);
        memset (outq,0,1024);
        sprintf (outf,"%s/%s",working_dir,output_file);
        sprintf (outiv,"%s/%s.iv",working_dir,output_file);
        sprintf (outq,"%s/%s.queue",working_dir,output_file);
//creating an instance of shred manager to start creating the shreads and the threads
MultithreadedShredManager shredManager(outf, block_size, shreds,true);        
        FileSpooler fileSpooler (input_file,block_size,false);
        shredManager.encrypt(&fileSpooler,key_file,outiv,outq);

}
SafeBox * SafeBoxImport::clone (){
    return new SafeBoxImport();
}
SafeBoxImport::~SafeBoxImport(){

}

SafeBoxExport::SafeBoxExport (){

}
void SafeBoxExport::process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds){
        printf ("Export file\n");
        char inf [1024];
        char iniv [1024];
        char inq [1024];
        memset (inf,0,1024);
        memset (iniv,0,1024);
        memset (inq,0,1024);
        sprintf (inf,"%s/%s",working_dir,input_file);
        sprintf (iniv,"%s/%s.iv",working_dir,input_file);        
        sprintf (inq,"%s/%s.queue",working_dir,input_file);  
//creating an instance of shred manager to start creating the shreads and the threads
MultithreadedShredManager shredManager(inf, block_size, shreds,false);        
        FileSpooler fileSpooler (output_file,block_size,true);
        shredManager.decrypt(&fileSpooler,key_file,iniv,inq);
}
SafeBox * SafeBoxExport::clone (){
    return new SafeBoxExport();
}
SafeBoxExport::~SafeBoxExport(){
}
