#pragma once
#include "domain/Fatura.h"
#include "domain/LinhaFatura.h"
#include "domain/Produto.h"
#include "factory/FaturaFactory.h"

class FaturaBuilder
{
    private:
    Fatura m_fatura;

    public:
    FaturaBuilder(int clienteID)
    :m_fatura(clienteID){}

    FaturaBuilder& adicionarProduto(const Produto& produto, int quantidade)
    {
        LinhaFatura linha = FaturaFactory::criarLinhaDeProduto(produto, quantidade);
        m_fatura.adicionarLinha(linha);
        return *this;
    }

    FaturaBuilder& comDesconto(double desconto)
    {
        m_fatura.setDescontoGlobal(desconto);
        return *this;
    }

    FaturaBuilder& comObservacoes(const std::string& obs)
    {
        m_fatura.setObservacao(obs);
        return *this;
    }

    Fatura construir()
    {
        m_fatura.recalcular();
        return m_fatura;
    }
};


/*
    FUNCIONAMENTO:
    - O builder vai ter um membro privado do tipo fatura,
    todos os metodos vao retornar o uma referencia ao proprio obj da classe,
    para fazer o encadeamento, apenas o ultimo retornará uma fatura.
*/