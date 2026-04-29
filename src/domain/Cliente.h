#pragma once
#include <string>
#include <any>

enum class TipoCliente
{
    Particular,
    Empresa
};

class Cliente
{
    private:
    
    int             m_id = 0;
    std::string     m_nif;
    std::string     m_nome;
    std::string     m_morada;
    std::string     m_codigo_postal;
    std::string     m_cidade;
    std::string     m_email;
    std::string     m_telefone;
    TipoCliente     m_tipo = TipoCliente::Particular;
    bool            m_ativo = true;
    std::string     m_criado_em; //dd/mm/aaaa
    std::string     m_atualizado_em;//dd/mm/aaaa

    public:

    Cliente(const std::string& nome, const std::string& nif)
    :m_nome(nome), m_nif(nif){}

    //GETTERS
    const int getID() const {return m_id;}
    const std::string& getNif() const {return m_nif;}
    const std::string& getNome() const {return m_nome;}
    const std::string& getMorada() const {return m_morada;}
    const std::string& getCodigoPostal() const {return m_codigo_postal;}
    const std::string& getCidade() const {return m_cidade;}
    const std::string& getEmail() const {return m_email;}
    const std::string& getTelefone() const {return m_telefone;}
    const TipoCliente getTipo() const {return m_tipo;}
    const std::string& getCriado_em() const {return m_criado_em;}
    const std::string& getAtualizado_em() const {return m_atualizado_em;}
    bool getAtivo() const  {return m_ativo;} 

    //SETTERS
    void setID(int id)                          {m_id = id;}
    void setNif(const std::string& nif)         {m_nif = nif;}
    void setNome(const std::string& nome)       {m_nome = nome;}
    void setMorada(const std::string& morada)   {m_morada = morada;}
    void setCodigoPostal(const std::string& cp) {m_codigo_postal = cp;}
    void setCidade(const std::string& cidade)   {m_cidade = cidade;}
    void setEmail(const std::string& email)     {m_email = email;}
    void setTelefone(const std::string& tele)   {m_telefone = tele;}
    void setTipo(TipoCliente tipo)              {m_tipo = tipo;}
    void setCriado_em(const std::string& c)     {m_criado_em = c;}
    void setAtualizado_em(const std::string& a) {m_atualizado_em = a;}
    void setAtivo(int ativo)
    {
        if(ativo == 1)
            m_ativo = true;
        else
            m_ativo = false;
    }
    //CONVERSOR
    std::string TipotoString() const 
    {
        switch(m_tipo)
        {
            case TipoCliente::Empresa:
                return "empresa";
            case TipoCliente::Particular:
                return "particular";
        }
    }

    TipoCliente StringToTipo(const std::string& tipo) const
    {
        if (tipo == "empresa")    return TipoCliente::Empresa;
        if (tipo == "particular") return TipoCliente::Particular;
        return TipoCliente::Particular;
    }
 

};
