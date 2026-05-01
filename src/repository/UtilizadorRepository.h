#pragma once
#include "domain/Utilizador.h"
#include "util/Database.hpp"
#include "util/Statement.hpp"
#include <optional>

class UtilizadorRepository
{
    private:
    Database& m_db;
    
    public:
    UtilizadorRepository(Database& db)
    :m_db(db){}

    void guardar(Utilizador& utilizador)
    {
        Statement stmt(m_db.obterHandle(), "INSERT INTO utilizadores(nome, email, api_key, ativo, criado_em) VALUES(?,?,?,?,?)");
        
        stmt.vincularTexto(1,utilizador.getNome());
        stmt.vincularTexto(2, utilizador.getEmail());
        stmt.vincularTexto(3, utilizador.getApiKey());
        stmt.vincularInteiro(4, utilizador.getAtivo());
        stmt.vincularTexto(5,utilizador.getCriadoEm());

        stmt.passo();
        int idGerado = sqlite3_last_insert_rowid(m_db.obterHandle());
        utilizador.setID(idGerado);

    }

    std::optional<Utilizador> buscarPorApiKey(const std::string& apiKey)
    {
        Statement stmt(m_db.obterHandle(), "SELECT id, nome, email, api_key, ativo, criado_em FROM utilizadores WHERE api_key = ?");

        stmt.vincularTexto(1, apiKey);
        if(!stmt.passo())
            return std::nullopt;
        
        Utilizador utilizador(stmt.obterTexto(1), stmt.obterTexto(2), stmt.obterTexto(3));
        utilizador.setID(stmt.obterInteiro(0));
        utilizador.setAtivo(stmt.obterInteiro(4));
        utilizador.setCriadoEm(stmt.obterTexto(5));

        return utilizador;
    }

    std::optional<Utilizador> buscarPorEmail(const std::string& email)
    {
        Statement stmt(m_db.obterHandle(), "SELECT id, nome, email, api_key, ativo, criado_em FROM utilizadores WHERE email = ?");
        stmt.vincularTexto(1, email);

        if(!stmt.passo())
            return std::nullopt;

        Utilizador utilizador(stmt.obterTexto(1), stmt.obterTexto(2), stmt.obterTexto(3));
        utilizador.setID(stmt.obterInteiro(0));
        utilizador.setAtivo(stmt.obterInteiro(4));
        utilizador.setCriadoEm(stmt.obterTexto(5));

        return utilizador;
        
    }
};