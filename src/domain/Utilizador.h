#pragma once
#include <string>

class Utilizador
{
    private:
    
    int m_id = 0;
    std::string m_nome;
    std::string m_email;
    std::string m_api_key;
    bool m_ativo = true;
    std::string m_criado_em;

    public: 

    Utilizador(const std::string& nome,const std::string& email, const std::string& api_key)
    :m_nome(nome), m_email(email), m_api_key(api_key){}

    // GETTERS
    int getID() const {return m_id;}
    const std::string& getNome() const {return m_nome;}
    const std::string& getEmail() const {return m_email;}
    const std::string& getApiKey() const {return m_api_key;}
    const std::string& getCriadoEm() const {return m_criado_em;}
    bool getAtivo() const {return m_ativo;}

    //SETTERS
    void setID(int id){m_id = id;}
    void setNome(const std::string& nome){m_nome = nome;}
    void setEmail(const std::string& email){m_email = email;}
    void setApiKey(const std::string& api_key) {m_api_key = api_key;}
    void setAtivo(bool estado) {m_ativo = estado;}
    void setCriadoEm(const std::string& criado_em) {m_criado_em = criado_em;}

};