#pragma once
#include "domain/Relatorio.h"
#include "util/Database.hpp"
#include "util/Statement.hpp"
#include <vector>

class RelatorioRepository
{
    private:
    Database& m_db;

    public:
    RelatorioRepository(Database& db)
    :m_db(db){}

    FaturacaoPorPeriodo faturacaoPorPeriodo(const std::string& dataInicio,  const std::string& dataFim)
    {
        Statement stmt(m_db.obterHandle(), "SELECT COUNT(*), SUM(subtotal), SUM(total_iva), SUM(total) FROM faturas WHERE estado = 'emitida' AND data_emissao >= ? AND data_emissao<= ?");
        stmt.vincularTexto(1, dataInicio);
        stmt.vincularTexto(2, dataFim);
        stmt.passo();

        FaturacaoPorPeriodo periodo;
        periodo.data_fim = dataFim;
        periodo.data_inicio = dataInicio;
        periodo.total_faturas = stmt.obterInteiro(0);
        periodo.total_subtotal = stmt.obterDecimal(1);
        periodo.total_iva = stmt.obterDecimal(2);
        periodo.total_geral = stmt.obterDecimal(3);
        
        return periodo;
    }

    std::vector<TopCliente> topClientes(int limite)
    {
        //JOIN -> junta cada fatura com o respectivo cliente
        //Para cada grupo, calcular o nº de faturas e total faturado
        //Ordena pelo total faturado, do maior para o menor(DESC)
        Statement stmt(m_db.obterHandle(), "SELECT clientes.id, clientes.nome, COUNT(faturas.id), SUM(faturas.total) FROM faturas JOIN clientes ON clientes.id = faturas.cliente_id WHERE faturas.estado = 'emitida' GROUP BY clientes.id ORDER BY SUM(faturas.total) DESC LIMIT ?");
        stmt.vincularInteiro(1, limite);
        std::vector<TopCliente> clientes;
        while(stmt.passo())
        {
            TopCliente cliente;
            cliente.id_cliente = stmt.obterInteiro(0);
            cliente.nome = stmt.obterTexto(1);
            cliente.total_faturas = stmt.obterInteiro(2);
            cliente.total_faturado = stmt.obterDecimal(3);
            clientes.push_back(cliente);
        }
        return clientes;
    }

    std::vector<TopProduto> topProduto(int limite)
    {
        Statement stmt(m_db.obterHandle(), "SELECT produtos.id, produtos.nome, COUNT(linhas_fatura.id), SUM(linhas_fatura.subtotal) FROM linhas_fatura JOIN produtos ON produtos.id = linhas_fatura.id_produto GROUP BY produtos.id ORDER BY(linhas_fatura.subtotal) DESC LIMIT ?");
        stmt.vincularInteiro(1,limite);
        std::vector<TopProduto> produtos;
        while(stmt.passo())
        {
            TopProduto produto;
            produto.id_produto = stmt.obterInteiro(0);
            produto.nome = stmt.obterTexto(1);
            produto.total_faturas = stmt.obterInteiro(2);
            produto.total_faturado = stmt.obterDecimal(3);
            produtos.push_back(produto);
        }

        return produtos;
    }
};                                         