#pragma once
#include "repository/ClienteRepository.h"
#include "repository/FaturaRepository.h"
#include "repository/ProdutoRepository.h"
#include "data_hora.h"

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

};