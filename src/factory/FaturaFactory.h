#pragma once 
#include "domain/Fatura.h"
#include "domain/LinhaFatura.h"
#include "domain/Produto.h"

class FaturaFactory
{
    public:
    
    static LinhaFatura criarLinhaDeProduto(const Produto& produto, int quantidade)
    {
        return LinhaFatura(
            produto.getID(),
            produto.getNome(),  //snapshot 
            quantidade,
            produto.getPrecoUnitario(), //snapshot 
            produto.getTaxaIVA()    //snapshot
        );
    }
};