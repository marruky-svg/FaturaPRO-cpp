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
        LinhaFatura linha = Fatra
    }
};