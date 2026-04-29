#pragma once
#include "util/Statement.hpp"
#include "util/Database.hpp"
#include "domain/Produto.h"
#include <optional>
#include <vector>

class ProdutoRepository
{
    private:
    Database& m_db;

    public:
    //CONSTRUOR
    ProdutoRepository(Database& db)
    :m_db(db){}

    void guardar(Produto& produto)
    {
        Statement stmt(m_db.obterHandle(), "INSERT INTO produtos(sku,nome,descricao,preco_unitario,taxa_iva,stock,stock_minimo,tipo,ativo) VALUES(?,?,?,?,?,?,?,?,?)");
        stmt.vincularTexto(1,produto.getSKU());
        stmt.vincularTexto(2,produto.getNome());
        stmt.vincularTexto(3,produto.getDescricao());
        stmt.vincularDecimal(4,produto.getPrecoUnitario());
        stmt.vincularDecimal(5,produto.getTaxaIVA());
        stmt.vincularInteiro(6,produto.getStock());
        stmt.vincularInteiro(7,produto.getStockMin());
        stmt.vincularTexto(8, produto.TipotoString());
        stmt.vincularInteiro(9, produto.getAtivo());
        stmt.passo();
        int idGerado = sqlite3_last_insert_rowid(m_db.obterHandle());
        produto.setID(idGerado);
    }

    std::optional<Produto> buscarPorID(int id)
    {
        Statement stmt(m_db.obterHandle(), "SELECT id,sku,nome,descricao,preco_unitario,taxa_iva,stock,stock_minimo,tipo,ativo FROM produtos WHERE id = ?");
        stmt.vincularInteiro(1, id);
        if(stmt.passo())
        {
            Produto produto(stmt.obterTexto(1), stmt.obterTexto(2));
            produto.setID(stmt.obterInteiro(0));
            produto.setDescricao(stmt.obterTexto(3));
            produto.setPrecoUnitario(stmt.obterDecimal(4));
            produto.setIVA(stmt.obterDecimal(5));
            produto.setStock(stmt.obterInteiro(6));
            produto.setStockMin(stmt.obterInteiro(7));
            produto.setTipo(produto.StringToTipo(stmt.obterTexto(8)));
            produto.setAtivo(stmt.obterInteiro(9));

            return produto;
        }

        return std::nullopt;
    }

    std::optional<Produto> buscarPorSKU(const std::string& sku)
    {
        Statement stmt(m_db.obterHandle(), "SELECT id,sku,nome,descricao,preco_unitario,taxa_iva,stock,stock_minimo,tipo,ativo FROM produtos WHERE sku = ?");
        stmt.vincularTexto(1,sku);
            if(stmt.passo())
        {
            Produto produto(stmt.obterTexto(1), stmt.obterTexto(2));
            produto.setID(stmt.obterInteiro(0));
            produto.setDescricao(stmt.obterTexto(3));
            produto.setPrecoUnitario(stmt.obterDecimal(4));
            produto.setIVA(stmt.obterDecimal(5));
            produto.setStock(stmt.obterInteiro(6));
            produto.setStockMin(stmt.obterInteiro(7));
            produto.setTipo(produto.StringToTipo(stmt.obterTexto(8)));
            produto.setAtivo(stmt.obterInteiro(9));

            return produto;
        }

        return std::nullopt;
    }

    std::vector<Produto> buscarTodos(int limit, int offset)
    {
        Statement stmt(m_db.obterHandle(),"SELECT id,sku,nome,descricao,preco_unitario,taxa_iva,stock,stock_minimo,tipo,ativo FROM produtos ORDER BY id LIMIT ? OFFSET ? ");
        std::vector<Produto> produtos;

        stmt.vincularInteiro(1,limit);
        stmt.vincularInteiro(2,offset);

        while(stmt.passo())
        {
            Produto produto(stmt.obterTexto(1), stmt.obterTexto(2));
            produto.setID(stmt.obterInteiro(0));
            produto.setDescricao(stmt.obterTexto(3));
            produto.setPrecoUnitario(stmt.obterDecimal(4));
            produto.setIVA(stmt.obterDecimal(5));
            produto.setStock(stmt.obterInteiro(6));
            produto.setStockMin(stmt.obterInteiro(7));
            produto.setTipo(produto.StringToTipo(stmt.obterTexto(8)));
            produto.setAtivo(stmt.obterInteiro(9));
            produtos.push_back(produto);
        }
        return produtos;
    }

    void atualizar(const Produto& produto)
    {
        Statement stmt(m_db.obterHandle(), "UPDATE produtos SET sku = ?, nome = ?, descricao = ?, preco_unitario = ?, taxa_iva = ?, stock = ?, stock_minimo = ?, tipo = ?, ativo = ? WHERE id = ?");
        stmt.vincularTexto(1, produto.getSKU());
        stmt.vincularTexto(2, produto.getNome());
        stmt.vincularTexto(3, produto.getDescricao());
        stmt.vincularDecimal(4, produto.getPrecoUnitario());
        stmt.vincularDecimal(5, produto.getTaxaIVA());
        stmt.vincularInteiro(6, produto.getStock());
        stmt.vincularInteiro(7, produto.getStockMin());
        stmt.vincularTexto(8, produto.TipotoString());
        stmt.vincularInteiro(9, produto.getAtivo());
        stmt.vincularInteiro(10,produto.getID());
        stmt.passo();
    }

    void remover(int id)
    {
        Statement stmt(m_db.obterHandle(), "UPDATE produtos SET ativo = false WHERE id = ?");
        
        stmt.vincularInteiro(1, id);
        stmt.passo();
    }
};