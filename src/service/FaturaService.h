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

    void emitir(int faturaId)
    {
        auto procuraFatura = m_faturaRepo.buscarPorID(faturaId);
        if(!procuraFatura.has_value())
            throw std::runtime_error("Fatura não existente");
        
        Fatura fatura = *procuraFatura;
        if(fatura.getEstadoFatura() != EstadoFatura::Rascunho)
            throw std::runtime_error("Fatura não disponível para alteração");
        
        if(fatura.getLinhas().empty())
            throw std::runtime_error("Fatura sem linha de produto");
        
        for(const auto& linha : fatura.getLinhas())
        {    
            auto produtoPtr = m_produtoRepo.buscarPorID(linha.getProdutoID());
            if(!produtoPtr.has_value())
                throw std::runtime_error("Produto não existe");
            
             auto produto = *produtoPtr;
             if(produto.getStock() < linha.getQuantidade())
                throw std::runtime_error("Stock insuficiente");
        }
            
        int ano = obterAnoAtual();
        fatura.setNumero(m_faturaRepo.obterProximoNumero(ano));
        
        fatura.setDataEmissao(dataAtual());
        fatura.setDataVencimento(dataMaisDias(30));
        fatura.setEstadoFatura(EstadoFatura::Emitida);
        
        for(const auto& linha : fatura.getLinhas())
        {
            auto produtoPtr = m_produtoRepo.buscarPorID(linha.getProdutoID());
            auto produto = *produtoPtr;
            produto.setStock(produto.getStock() - linha.getQuantidade());
            m_produtoRepo.atualizar(produto);
        }

        m_faturaRepo.atualizar(fatura);
    }
};