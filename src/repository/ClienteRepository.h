#pragma once
#include "util/Database.hpp"
#include "util/Statement.hpp"
#include "domain/Cliente.h"
#include <optional>
#include <vector>

class ClienteRepository
{
    private:
    Database& m_db;

    public:
    ClienteRepository(Database& db)
    :m_db(db){}

    void guardar(const Cliente& cliente)
    {
        Statement stmt(m_db.obterHandle(),"INSERT INTO clientes(nif,nome,morada,codigo_postal,cidade,email,telefone,tipo,ativo) VALUES(?,?,?,?,?,?,?,?,?)");
        stmt.vincularTexto(1, cliente.getNif());
        stmt.vincularTexto(2, cliente.getNome());
        stmt.vincularTexto(3, cliente.getMorada());
        stmt.vincularTexto(4, cliente.getCodigoPostal());
        stmt.vincularTexto(5, cliente.getCidade());
        stmt.vincularTexto(6, cliente.getEmail());
        stmt.vincularTexto(7, cliente.getTelefone());
        stmt.vincularTexto(8, cliente.TipotoString());
        stmt.vincularInteiro(9, cliente.getAtivo());
        stmt.passo();
    }   

    std::optional<Cliente> buscarPorId(int id)
    {
        Statement stmt(m_db.obterHandle(), "SELECT id, nif, nome, morada, codigo_postal, cidade, email, telefone, tipo, ativo FROM clientes WHERE id = ?");
        stmt.vincularInteiro(1, id);
        if(stmt.passo())
        {
            Cliente cliente(stmt.obterTexto(2), stmt.obterTexto(1));
            cliente.setID(stmt.obterInteiro(0));
            cliente.setMorada(stmt.obterTexto(3));
            cliente.setCodigoPostal(stmt.obterTexto(4));
            cliente.setCidade(stmt.obterTexto(5));
            cliente.setEmail(stmt.obterTexto(6));
            cliente.setTelefone(stmt.obterTexto(7));
            cliente.setTipo(cliente.StringToTipo(stmt.obterTexto(8)));
            cliente.setAtivo(stmt.obterInteiro(9));
            return cliente;
        }
        return std::nullopt;
    }

    std::optional<Cliente> buscarPorNif(const std::string& nif)
    {
        Statement stmt (m_db.obterHandle(), "SELECT id, nif, nome, morada, codigo_postal, cidade, email, telefone, tipo, ativo FROM clientes WHERE nif = ?");
        stmt.vincularTexto(1, nif);
        if(stmt.passo())
        {
            Cliente cliente(stmt.obterTexto(2), stmt.obterTexto(1));
            cliente.setID(stmt.obterInteiro(0));
            cliente.setMorada(stmt.obterTexto(3));
            cliente.setCodigoPostal(stmt.obterTexto(4));
            cliente.setCidade(stmt.obterTexto(5));
            cliente.setEmail(stmt.obterTexto(6));
            cliente.setTelefone(stmt.obterTexto(7));
            cliente.setTipo(cliente.StringToTipo(stmt.obterTexto(8)));
            cliente.setAtivo(stmt.obterInteiro(9));
            return cliente;
        }
        return std::nullopt;
    }

    std::vector<Cliente> buscarTodos(int limit, int offset)
    {
        std::vector<Cliente> clientes;
        Statement stmt(m_db.obterHandle(), "SELECT id,nif,nome,morada,codigo_postal,cidade,email,telefone,tipo,ativo FROM clientes ORDER BY id LIMIT ? OFFSET ?");
        stmt.vincularInteiro(1,limit);
        stmt.vincularInteiro(2,offset);
        //ACABA QUANDO NAO TIVER MAIS LINHAS PARA LER
        while(stmt.passo())
        {
            //CRIANDO CLIENTE A CADA ITERACAO 
            Cliente cliente(stmt.obterTexto(2), stmt.obterTexto(1));
            cliente.setID(stmt.obterInteiro(0));
            cliente.setMorada(stmt.obterTexto(3));
            cliente.setCodigoPostal(stmt.obterTexto(4));
            cliente.setCidade(stmt.obterTexto(5));
            cliente.setEmail(stmt.obterTexto(6));
            cliente.setTelefone(stmt.obterTexto(7));
            cliente.setTipo(cliente.StringToTipo(stmt.obterTexto(8)));
            cliente.setAtivo(stmt.obterInteiro(9));
            //ADICIONANDO AO VECTOR
            clientes.push_back(cliente);
        }
        return clientes;
    }

    void atualizar(const Cliente& cliente)
    {
        Statement stmt(m_db.obterHandle(), "UPDATE clientes SET nif = ?, nome = ?, morada = ?, codigo_postal = ?, cidade = ?, email = ?, telefone = ?, tipo = ?, ativo = ? WHERE id = ?");
        stmt.vincularTexto(1, cliente.getNif());
        stmt.vincularTexto(2, cliente.getNome());
        stmt.vincularTexto(3, cliente.getMorada());
        stmt.vincularTexto(4, cliente.getCodigoPostal());
        stmt.vincularTexto(5, cliente.getCidade());
        stmt.vincularTexto(6, cliente.getEmail());
        stmt.vincularTexto(7, cliente.getTelefone());
        stmt.vincularTexto(8, cliente.TipotoString());
        stmt.vincularInteiro(9, cliente.getAtivo());
        stmt.vincularInteiro(10,cliente.getID());
        stmt.passo();
    }

    void remover(int id)
    {
        Statement stmt(m_db.obterHandle(), "UPDATE clientes SET ativo = false WHERE id = ?");
        
        stmt.vincularInteiro(1, id);
        stmt.passo();
    }
};