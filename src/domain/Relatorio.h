#pragma once 
#include <string>

struct FaturacaoPorPeriodo
{
    std::string data_inicio;
    std::string data_fim;
    int total_faturas = 0;
    double total_subtotal = 0;
    double total_iva = 0;
    double total_geral = 0;
};

struct TopCliente
{
    int id_cliente = 0;
    std::string nome;
    int total_faturas = 0;
    double total_faturado = 0;
};

struct TopProduto
{
    int id_produto = 0; 
    std::string nome;
    int quantidade_vendida = 0;
    double total_faturado = 0;
};