#pragma once
#include "repository/UtilizadorRepository.h"
#include <string>

class Autenticador
{
    private:
    UtilizadorRepository& m_repo;

    public:
    
    Autenticador(UtilizadorRepository& repo)
    : m_repo(repo){}

    bool validar(const std::string& apiKey)
    {
        if(apiKey.empty()) return false;

        auto procuraUtilizador = m_repo.buscarPorApiKey(apiKey);
        if(!procuraUtilizador.has_value()) return false;

        return procuraUtilizador->getAtivo();
    }

    std::string gerarApiKey()
    {
        const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::string key;
        for(int i = 0; i < 32; i++)
            key += chars[rand() % chars.size()];
        return key;
    }
};