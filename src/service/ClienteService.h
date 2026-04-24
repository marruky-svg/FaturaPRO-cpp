#pragma once
#include "domain/Cliente.h"
#include "repository/ClienteRepository.h"
#include "data_hora.h"
#include <string>
#include <vector>
#include <regex>

class ClienteService
{
    private:
    ClienteRepository& m_repo;
 
    //VALIDAÇÕES
    void validarNif(const std::string& nif)
    {
        if(nif.length() != 9)
            throw std::runtime_error("Nif com quantidade de digitos errada");
        
        for(char c : nif)
            if(!isdigit(c))
                throw std::runtime_error("Nif inválido");
        int soma = 0;
        for(size_t i = 0; i < nif.length() - 1;i++)
            soma += (nif[i] - '0') * (9 - i);
        if(soma % 11  == 0 || soma % 11 == 1)
        {
            if((nif[nif.length()- 1] - '0') == 0)
                return;
        }
        else
        {
            if((nif[nif.length()- 1] - '0') == 11 - (soma % 11))
                return;
        }
        throw std::runtime_error("Nif inválido");
    }

    void validarCodigoPostal(const std::string& codigo_postal)
    {
        if(codigo_postal.empty()) 
            return;
        std::regex padrao(R"(^\d{4}-\d{3}$)");
        if(!std::regex_match(codigo_postal,padrao))
            throw std::runtime_error("Código postal inválido (formato XXXX-XXX)");
    }

    void validarEmail(const std::string& email)
    {
        if (email.empty()) return;
        std::regex padrao(R"(^[^\s@]+@[^\s@]+\.[^\s@]+$)");
        if (!std::regex_match(email, padrao))
            throw std::runtime_error("Email invalido");
    }

    
   
    public:
    ClienteService(ClienteRepository& repo)
    :m_repo(repo){}

    void criarCliente(const Cliente& cliente)
    {
       validarNif(cliente.getNif());
       validarEmail(cliente.getEmail()); 
       validarCodigoPostal(cliente.getCodigoPostal());
        auto resultado = m_repo.buscarPorNif(cliente.getNif());
        if(resultado.has_value())
            throw std::runtime_error("Cliente já existente");
        m_repo.guardar(cliente);
    }

    Cliente obterCliente(int id)
    {
        auto resultado = m_repo.buscarPorId(id);
        if(!resultado.has_value())
            throw std::runtime_error("Cliente nao encontrado");
        return *resultado;
    }

    std::vector<Cliente> listarClientes(int pagina, int tamanho)
    {
        if(pagina < 1)
            throw std::runtime_error("Pagina tem de ser maior que 0");
        if(tamanho < 1 || tamanho > 100)
            throw std::runtime_error("Tamanho tem de estar entre 1 e 100");

        int offset = (pagina - 1) * tamanho; 
        return m_repo.buscarTodos(tamanho, offset);
    }

    void atualizarCliente(const Cliente& cliente)
    {
       validarNif(cliente.getNif());
       validarEmail(cliente.getEmail()); 
       validarCodigoPostal(cliente.getCodigoPostal());
        auto resultado = m_repo.buscarPorId(cliente.getID());
        if(!resultado.has_value())
            throw std::runtime_error("Cliente nao encontrado");
        m_repo.atualizar(cliente);
    }

    void removerCliente(int id)
    {
        auto resultado = m_repo.buscarPorId(id);
        if(!resultado.has_value())
            throw std::runtime_error("Cliente nao encontrado");
        m_repo.remover(id);    
    }
};