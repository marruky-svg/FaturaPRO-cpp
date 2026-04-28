#pragma once
#include "repository/ClienteRepository.h"
#include "repository/FaturaRepository.h"
#include "repository/ProdutoRepository.h"
#include "data_hora.h"
#include "factory/FaturaFactory.h"
class FaturaService
{
    private:
    // PARA PERSISTENCIA
    FaturaRepository& m_faturaRepo;
    // VALIDAR EXISTENCIA CLIENTE
    ClienteRepository& m_clienteRepo;
    // PARA SNAPSHOTS E ATUALIZACAO DE STOCK 
    ProdutoRepository& m_produtoRepo;

    public:
    FaturaService(FaturaRepository& faturaRepo, ClienteRepository& clienteRepo, ProdutoRepository& produtoRepo)
    :m_faturaRepo(faturaRepo), m_clienteRepo(clienteRepo), m_produtoRepo(produtoRepo){}

    Fatura criarRascunho(int clienteID)
    {
        // CASO NAO ACHE O CLIENTE/ NAO EXISTA
        if(!m_clienteRepo.buscarPorId(clienteID))
          throw std::runtime_error("Cliente não existente");
        
          // CRIAR FATURA COM NOME ID DO CLIENTE
        Fatura fatura(clienteID);
        fatura.setCriadaEm(dataAtual());

        // PERSISTIR COM FATURA_REPOSITORY
        m_faturaRepo.guardar(fatura);

        return fatura;
    }

    Fatura adicionarLinha(int faturaId, int produtoId, int quantidade)
    {
        if(quantidade <= 0)
            throw std::runtime_error("Quantidade inválida");
       
        auto procuraFatura = m_faturaRepo.buscarPorID(faturaId);

        if(!procuraFatura.has_value())
            throw std::runtime_error("Fatura não existente");
        
        Fatura fatura = *procuraFatura;
        if(fatura.getEstadoFatura() != EstadoFatura::Rascunho)
            throw std::runtime_error("Fatura não disponível para alterações");

        auto procuraProduto = m_produtoRepo.buscarPorID(produtoId);
        
        if(!procuraProduto.has_value())
             throw std::runtime_error("Produto não existente");
        
        Produto produto = *procuraProduto;
        
        if(produto.getStock() < quantidade)
            throw std::runtime_error("Stock insuficiente");
            
        fatura.adicionarLinha(FaturaFactory::criarLinhaDeProduto(produto, quantidade));
        fatura.recalcular();
        
        m_faturaRepo.atualizar(fatura);
        return fatura;
    }
};