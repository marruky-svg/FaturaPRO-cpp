#pragma once
#include <string>

enum class TipoProduto
{
    Produto,
    Servico
};


class Produto
{
    private:
    int m_id = 0;
    std::string m_sku;
    std::string m_nome;
    std::string m_descricao;
    double m_preco_unitario = 0.0;
    double m_taxa_iva = 0.0;
    int m_stock = 0;
    int m_stock_minimo = 5;
    TipoProduto m_tipo = TipoProduto::Produto;
    bool m_ativo = true;

    public:
    Produto(const std::string& sku, const std::string& nome)
    :m_sku(sku), m_nome(nome){}
    
    //GETTERS
    int getID() const {return m_id;}
    const std::string& getSKU() const {return m_sku;}
    const std::string& getNome() const {return m_nome;}
    const std::string& getDescricao() const {return m_descricao;}
    double getPrecoUnitario() const {return m_preco_unitario;}
    double getTaxaIVA() const {return m_taxa_iva;}
    int getStock() const {return m_stock;}
    int getStockMin() const {return m_stock_minimo;}
    const TipoProduto& getTipo() const {return m_tipo;}
    bool getAtivo() const {return m_ativo;}

    //SETTERS
    void setID(int id){m_id = id;}
    void setSKU(const std::string& sku) {m_sku = sku;}
    void setNome(const std::string& nome) {m_nome = nome;}
    void setDescricao(const std::string& descricao) {m_descricao = descricao;}
    void setPrecoUnitario(double preco) {m_preco_unitario = preco;}
    void setIVA(double iva) {m_taxa_iva = iva;} 
    void setStock(int stock) {m_stock = stock;}
    void setStockMin(int stockMin) {m_stock_minimo = stockMin;}
    void setTipo(TipoProduto tipo) {m_tipo = tipo;}
    void setAtivo(bool valor) {m_ativo = valor;}

    //CONVERSOR

      std::string TipotoString() const 
    {
        switch(m_tipo)
        {
            case TipoProduto::Produto:
                return "produto";
            case TipoProduto::Servico:
                return "servico";
        }
    }

    TipoProduto StringToTipo(const std::string& tipo) const 
    {
        if(tipo =="produto")
            return TipoProduto::Produto;
    
        if(tipo == "servico" || tipo =="serviço")
            return TipoProduto::Servico;
        return TipoProduto::Produto;
    }
};