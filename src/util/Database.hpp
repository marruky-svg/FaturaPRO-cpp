#pragma once
#include <sqlite3.h>
#include <string>
#include <stdexcept>

class Database
{
    private:
    sqlite3* m_db;
    
    public:
    Database(const std::string& db_name)
    {
        int value = sqlite3_open(db_name.c_str(), &m_db);
        if(SQLITE_OK != value)
        {
            auto errMsg = sqlite3_errmsg(m_db);
            std::string mensagem(errMsg);
            sqlite3_close(m_db);
            throw std::runtime_error("Problema na abertura da database, " + mensagem);

        }
    }
    //delete no assignment operator
    Database& operator=(const Database& other) = delete;
    //delete no copy constructor
    Database (const Database& other) = delete;
    ~Database(){sqlite3_close(m_db);}

    void executar(const std::string& sql)
    {
        //vai receber uma mensagem do sqlite3_exec se der erro, senao fica nullptr
        char* errMsg = nullptr;
        int result = sqlite3_exec(m_db,sql.c_str(),nullptr, nullptr,&errMsg);
        if (result != SQLITE_OK)
        {   
            //copia a mensagem de erro que é do sql para nos
            std::string mensagem(errMsg);
            sqlite3_free(errMsg);
            throw std::runtime_error("Problema na execução da database, " + mensagem);
        }
    }
    sqlite3* obterHandle(){return m_db;}
};