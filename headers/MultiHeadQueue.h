#ifndef MULTIHEADQUEUE_H
#define MULTIHEADQUEUE_H

#include <includes.h>
#include <defines.h>
#include <vector>
#include <thread>
#include <fstream>

using namespace std;

template <typename T>
class MultiHeadQueue: private std::vector<T>{
    
    private:
        mutex mtx;

    public:
void encrypt(CryptoPP::byte * key, CryptoPP::byte * iv, T * buffer, long sz)
{
    std::string ciphertext;
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( buffer ), sz );
    stfEncryptor.MessageEnd();
    memcpy (buffer,ciphertext.c_str(),ciphertext.size());
    buffer = (T*) buffer;
}
void decrypt(CryptoPP::byte * key, CryptoPP::byte * iv, T * buffer, long sz)
{
    std::string decryptedtext;
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( buffer ), sz );
    stfDecryptor.MessageEnd();
    sz = (uint16_t) decryptedtext.size();
    memcpy (buffer,decryptedtext.c_str(),sz);
    buffer = (T*) buffer;
}

        MultiHeadQueue ():vector<T>(){
        }
        void enqueue(T & t)
        {
            mtx.lock();
            vector<T>::push_back(t);
            mtx.unlock();
        }
        bool dequeue(T & t,std::function <bool (T&,T&)> check)
        {
            mtx.lock();
            T e;
            for (auto x = vector<T>::begin() ;x != vector<T>::end();x++)
            {
            if(check(t,x[0])){          //check function is a lampda function that returns true if the two passed blocks are from the same shred
            t=x[0];
            cout<<"check"<<endl;
            vector<T>::erase(x);        //removing the element from the queue
            mtx.unlock();
            return false;
            }
}
            mtx.unlock();
            return true;
        }


 void dump (char * filename,char * p_key_file,char * p_iv_file)
        {
            mtx.lock();
            ofstream f;
            f.open(filename,ios::out);
            T * buffer = (T *) calloc(vector<T>::size(),sizeof(T));
            buffer = vector<T>::data(); 
            long buffer_size = vector<T>::size()*sizeof(T);

            //reading key and IV and encrypting
            ifstream f1, f2;
	        byte *key = new byte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	        byte *iv = new byte[AES::BLOCKSIZE];
            f1.open(p_key_file,ios::in);
	        f1.read(reinterpret_cast<char*>(key),CryptoPP::AES::DEFAULT_KEYLENGTH);
	        f2.open(p_iv_file,ios::in);
	        if (f2.is_open())
	            f2.read(reinterpret_cast<char*>(iv),AES::BLOCKSIZE); 
            //encrypt(key, iv, buffer, buffer_size);
            //Buffer is encrypted and ready to be dumped 
                if ( f.is_open()){ 
                    cout<<"open"<<endl;
               f.write(reinterpret_cast<char*> (buffer), buffer_size);   //casting the buffer into const char pointer to write it in the queue file
                cout<<"wrote"<<endl;
            }
            f.close();
            f2.close();
            f1.close();
            mtx.unlock();
        }
        void load (char * filename,char * p_key_file,char * p_iv_file)
        {
            //filename has the que encrypted
            mtx.lock();
            ifstream f;
            f.open(filename,ios::in);
            if ( f.is_open())
            {   // decrypt first and then read into the buffer
                //reading queue
                f.seekg(0,f.end);
                long sz = f.tellg();
                f.seekg(0,f.beg);
                T * buffer = (T *) calloc(sz/sizeof(T),sizeof(T));
                f.read (reinterpret_cast<char*>(buffer),sz);
                f.close();
                //reading key and IV and decrypting
                ifstream f1, f2;
	            byte *key = new byte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	            byte *iv = new byte[AES::BLOCKSIZE];
	            f1.open(p_key_file,ios::in);
	            f1.read(reinterpret_cast<char*>(key),CryptoPP::AES::DEFAULT_KEYLENGTH);
	            f2.open(p_iv_file, ios::in);
	            if (f2.is_open())
		            f2.read(reinterpret_cast<char*>(iv),AES::BLOCKSIZE);
                //decrypt(key, iv, buffer, sz);
                sz/=sizeof(T);
                for ( int i = 0 ; i < sz ; i++)
                    vector<T>::push_back(buffer[i]);
            mtx.unlock();
            }

        }
        ~MultiHeadQueue(){}
};



#endif
