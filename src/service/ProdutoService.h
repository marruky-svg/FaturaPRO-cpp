#pragma once
#include "domain/Produto.h"
#include "repository/ProdutoRepository.h"
#include <string>
#include <vector>

class ProdutoService
{
    private: 
    ProdutoRepository& m_repo; 

    void validarProduto(const Produto& produto)
    {
        //VALIDAR SKU
        if(produto.getSKU() == "")
            throw std::runtime_error("SKU vazio");
        
        //VALIDAR PRECO_UNITARIO 
        if(produto.getPrecoUnitario() <= 0)
            throw std::runtime_error("Preço inválido");
        
        //VAlIDAR IVA
        if(produto.getTaxaIVA() != 0 && produto.getTaxaIVA() !=6 && produto.getTaxaIVA() != 13 && produto.getTaxaIVA() != 23)
            throw std::runtime_error("IVA inválido");
    }
    
    public:
    ProdutoService(ProdutoRepository& repo)
    :m_repo(repo){}

    void criarProduto(Produto& produto)
    {
        validarProduto(produto);
        //VERIFICAR SKU DUPLICADO
        auto resultado = m_repo.buscarPorSKU(produto.getSKU());
        if(resultado.has_value())
            throw std::runtime_error("SKU duplicado");
        
            m_repo.guardar(produto);
    }
    Produto obterProduto(int id)
    {
        auto resultado = m_repo.buscarPorID(id);
        if(!resultado.has_value())
            throw std::runtime_error("ID nao encontrado");
        return *resultado;
    }

    std::vector<Produto> listarProdutos(int pagina, int tamanho)
    {
        if(pagina < 1)
            throw std::runtime_error("Pagina tem de ser maior que 0");
        if(tamanho < 1 || tamanho > 100)
            throw std::runtime_error("Tamanho tem de estar entre 1 e 100");

        int offset = (pagina - 1) * tamanho; 
        return m_repo.buscarTodos(tamanho,offset);
    }

    void atualizarProduto(const Produto& produto)
    {
        validarProduto(produto);
        auto resultado = m_repo.buscarPorID(produto.getID());
        if(!resultado.has_value())
            throw std::runtime_error("Produto nao encontrado");
        m_repo.atualizar(produto);
    }

    void removerProduto(int id)
    {
        auto resultado = m_repo.buscarPorID(id);
        if(!resultado.has_value())
            throw std::runtime_error("Produto nao encontrado");
        m_repo.remover(id);
    }
};