#pragma once
#include "repository/RelatorioRepository.h"

class RelatorioService
{
    private:
    RelatorioRepository& m_repo;

    public:
    RelatorioService(RelatorioRepository& repo)
    :m_repo(repo){}

    FaturacaoPorPeriodo faturacaoPorPeriodo(const std::string& dataInicio, const std::string& dataFim)
    {
        if(dataInicio.empty() || dataFim.empty())
            throw std::runtime_error("Data inválida");

        if(dataInicio > dataFim)
            throw std::runtime_error("Data do inicio maior que a final");
        
        return m_repo.faturacaoPorPeriodo(dataInicio,dataFim);
    }

    std::vector<TopCliente> topClientes(int limite)
    {
        if(limite < 1 || limite > 100)
            throw std::runtime_error("Limite inválido");
        
        return m_repo.topClientes(limite);
    }

    std::vector<TopProduto> topProdutos(int limite)
    {
        if(limite < 1 || limite > 100)
            throw std::runtime_error("Limite inválido");
        return m_repo.topProduto(limite);
    }

     std::vector<FaturaPorEstado> faturasPorEstado()
     {
        return m_repo.faturasPorEstado();
     }

    std::vector<IvaPorTaxa> ivaPorTaxa(const std::string& dataInicio,const std::string& dataFim)
    {
        if(dataInicio.empty() || dataFim.empty())
            throw std::runtime_error("Data inválida");

        if(dataInicio > dataFim)
            throw std::runtime_error("Data do inicio maior que a final");

        return m_repo.ivaPorTaxa(dataInicio, dataFim);
    }
};