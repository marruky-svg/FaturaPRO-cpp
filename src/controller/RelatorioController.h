#pragma once
#include "service/RelatorioService.h"
#include <nlohmann/json.hpp>
#include <httplib.h>
using json = nlohmann::json;

class RelatorioController
{
    private:
    RelatorioService& m_relatorioService;

    json faturacaoPorPeriodoToJSON(FaturacaoPorPeriodo f) const 
    {
        json j;
        j["data_inicio"] = f.data_inicio;
        j["data_fim"] = f.data_fim;
        j["total_faturas"] = f.total_faturas;
        j["total_subtotal"] = f.total_subtotal;
        j["total_iva"] = f.total_iva;
        j["total_geral"] = f.total_geral;

        return j;
    }

    json topClientesToJSON(TopCliente c) const 
    {
        json j;
        j["id_cliente"] = c.id_cliente;
        j["nome"] = c.nome;
        j["total_faturas"] = c.total_faturas;
        j["total_faturado"] = c.total_faturado;

        return j;
    }

    json topProdutosToJSON(TopProduto p) const 
    {
        json j; 
        j["id_produto"] = p.id_produto;
        j["nome"] = p.nome;
        j["quantidade_vendida"] = p.quantidade_vendida;
        j["total_faturado"] = p.total_faturado;

        return j;
    }

    json faturaPorEstadoToJSON(FaturaPorEstado f) const 
    {
        json j; 
        j["estado"] = f.estado;
        j["total"] = f.total;

        return j;
    }

    json ivaPorTaxaToJSON(IvaPorTaxa taxa) const 
    {
        json j;
        j["taxa"] = taxa.taxa;
        j["total_subtotal"] = taxa.total_subtotal;
        j["total_iva"] = taxa.total_iva;

        return j;
    }

    public:
    RelatorioController(RelatorioService& rs)
    :m_relatorioService(rs){}

    void registrarRotas(httplib::Server servidor)
    {
        servidor.Get("/api/v1/relatorios/faturacao", [this](const httplib::Request& req, httplib::Response& res){
            try 
            {
                std::string dataInicio = req.get_param_value("data_inicio");
                std::string dataFim = req.get_param_value("data_fim");

                res.set_content(faturacaoPorPeriodoToJSON(m_relatorioService.faturacaoPorPeriodo(dataInicio, dataFim)).dump(), "application/json");
                res.status = 200;
            }
            catch(std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 500;
            }
        });
        servidor.Get("/api/v1/relatorios/top-clientes", [this](const httplib::Request& req, httplib::Response& res){
            try 
            {
                std::string limiteStr = req.get_param_value("limite");
                int limite = limiteStr.empty() ? 5 : std::stoi(limiteStr);
                json arr = json::array();
                auto topClientes = m_relatorioService.topClientes(limite);
                for(const auto& topCliente : topClientes)
                    arr.push_back(topClientesToJSON(topCliente));
                res.set_content(arr.dump(), "application/json");
                res.status = 200;
            }
            catch(std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 500;
            }
        });

        servidor.Get("/api/v1/relatorios/top-produtos", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                std::string limiteStr = req.get_param_value("limite");
                int limite = limiteStr.empty() ? 5 : std::stoi(limiteStr);
                json arr = json::array();
                auto topProdutos = m_relatorioService.topProdutos(limite);
                for(const auto& topProduto : topProdutos)
                    arr.push_back(topProdutosToJSON(topProduto));
                res.set_content(arr.dump(), "application/json");
                res.status = 200;
            }
            catch(std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 500;
            }
        });

        servidor.Get("/api/v1/relatorios/faturas-por-estado", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                json arr = json::array();
                auto faturas = m_relatorioService.faturasPorEstado();
                for(const auto& fatura : faturas)
                    arr.push_back(faturaPorEstadoToJSON(fatura));
                res.set_content(arr.dump(), "application/json");
                res.status = 200;
            }
            catch(std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 500;
            }
        });

        servidor.Get("/api/v1/relatorios/iva-por-taxa", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                std::string dataInicio = req.get_param_value("data_inicio");
                std::string dataFim = req.get_param_value("data_fim"); 
                json arr = json::array();
                auto taxas = m_relatorioService.ivaPorTaxa(dataInicio, dataFim);
                for(const auto& taxa : taxas)
                    arr.push_back(ivaPorTaxaToJSON(taxa));
                res.set_content(arr.dump(), "application/json");
                res.status = 200;
            }
            catch(std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 500;
            }
        });
    }
   

    
};  