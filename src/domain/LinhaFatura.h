#pragma once
#include <string>



class LinhaFatura
{
    private:
    int m_id = 0;
    int m_fatura_id = 0;
    int m_produto_id = 0;
    std::string m_descricao;
    int m_quantidade;
    double m_preco_unitario;
    double m_desconto = 0; //porcentagem de 0-100, default 0
    double m_taxa_iva;

    public:
    LinhaFatura(int produto_id, const std::string& descricao, int qtd, double preco_unitario, double taxa_iva)
    :m_produto_id(produto_id), m_descricao(descricao), m_quantidade(qtd), m_preco_unitario(preco_unitario), m_taxa_iva(taxa_iva){}

    //GETTERS
    int getID() const {return m_id;}
    int getFaturaID() const {return m_fatura_id;}
    int getProdutoID() const {return m_produto_id;}
    const std::string& getDescricao() const {return m_descricao;}
    int getQuantidade() const {return m_quantidade;}
    double getPrecoUnitario() const {return m_preco_unitario;}
    double getDesconto() const {return m_desconto;}
    double getTaxaIVA() const {return m_taxa_iva;}


    //SETTERS
    void setQuantidade(int qtd){m_quantidade = qtd;}
    void setDesconto(double desconto){m_desconto = desconto;}
    void setFaturaID(int id) {m_fatura_id = id;}
    void setID(int id) {m_id = id;}
    //METODOS 
    double calcularSubtotal() const 
    {
        return ((m_preco_unitario * m_quantidade) * (1 - m_desconto/100));
    }

    double calcularIVA() const 
    {
        return (calcularSubtotal() * m_taxa_iva/100);
    }
};