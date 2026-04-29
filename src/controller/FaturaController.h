#pragma once 
#include "domain/Fatura.h"
#include "domain/LinhaFatura.h"
#include "service/FaturaService.h"
#include "repository/FaturaRepository.h"
#include <nlohmann/json.hpp>
#include <httplib.h>
using json = nlohmann::json;


class FaturaController 
{
    private:
    FaturaService& m_faturaService;

    json linhaToJSON(const LinhaFatura& linha) const
    {
        json j; 
        j["id"] = linha.getID();
        j["id_fatura"] = linha.getFaturaID();
        j["id_produto"] = linha.getProdutoID();
        j["descricao"] = linha.getDescricao();
        j["quantidade"] = linha.getQuantidade();
        j["preco_unitario"] = linha.getPrecoUnitario();
        j["desconto"] = linha.getDesconto();
        j["taxa_iva"] = linha.getTaxaIVA();

        return j;
    }

    json faturaToJSON(const Fatura& fatura) const 
    {
        // CRIAR JSON
        json j;
        j["id"] = fatura.getID();
        j["numero"] = fatura.getNumero();
        j["id_cliente"] = fatura.getID_Cliente();
        j["data_emissao"] = fatura.getDataEmissao();
        j["data_vencimento"] = fatura.getDataVencimento();
        j["estado_fatura"] = fatura.estadoToString();
        j["subtotal"] = fatura.getSubTotal();
        j["total_iva"] = fatura.getTotalIVA();
        j["total"] = fatura.getTotal();
        j["desconto_global"] = fatura.getDescontGlobal();
        j["observacao"] = fatura.getObservacao();
        j["criada_em"] = fatura.getCriadaEm();

        json arrLinhas = json::array();
        for(const auto& linha : fatura.getLinhas())
        {
            arrLinhas.push_back(linhaToJSON(linha));
        }
        j["linhas"] = arrLinhas;

        return j;
    }

    LinhaFatura jsonToLinha(const json& j) const 
    {
        LinhaFatura linha(j["id_produto"],j["descricao"],j["quantidade"],j["preco_unitario"], j["taxa_iva"]);
        if(j.contains("desconto")) linha.setDesconto(j["desconto"]);
        if(j.contains("id_fatura")) linha.setFaturaID(j["id_fatura"]);
        if(j.contains("id")) linha.setID(j["id"]);

        return linha;
    }

   Fatura jsonToFatura(const json& j) const 
    {
        Fatura fatura(j["id_cliente"]);  // obrigatório
        
        if (j.contains("id")) fatura.setID(j["id"]);
        if (j.contains("numero")) fatura.setNumero(j["numero"]);
        if (j.contains("data_emissao")) fatura.setDataEmissao(j["data_emissao"]);
        if (j.contains("data_vencimento")) fatura.setDataVencimento(j["data_vencimento"]);
        if (j.contains("estado_fatura")) fatura.setEstadoFatura(fatura.stringToEstado(j["estado_fatura"]));
        if (j.contains("subtotal")) fatura.setSubtotal(j["subtotal"]);
        if (j.contains("total_iva")) fatura.setTotalIVA(j["total_iva"]);
        if (j.contains("total")) fatura.setTotal(j["total"]);
        if (j.contains("desconto_global")) fatura.setDescontoGlobal(j["desconto_global"]);
        if (j.contains("observacao")) fatura.setObservacao(j["observacao"]);
        if (j.contains("criada_em")) fatura.setCriadaEm(j["criada_em"]);
        
        if (j.contains("linhas"))
            for (const auto& linha : j["linhas"])
                fatura.adicionarLinha(jsonToLinha(linha));
        
        
        return fatura;
    }


    public: 

    void registrarRotas(httplib::Server& servidor)
    {
        servidor.Get("/api/v1/faturas", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                std::string paginaStr = req.get_param_value("pagina");
                std::string tamanhoStr = req.get_param_value("tamanho");

                int pagina = paginaStr.empty() ? 1 : std::stoi(paginaStr);
                int tamanho = tamanhoStr.empty() ? 20 : std::stoi(tamanhoStr);

                auto faturas = m_faturaService.listarTodos(pagina, tamanho);

                json arr = json::array();
                for(const auto& fatura : faturas)
                    arr.push_back(faturaToJSON(fatura));
                
                res.set_content(arr.dump(), "application/json");
                res.status = 200;
            }
            catch(const std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 500;
            }
        });

        servidor.Get(R"(/api/v1/faturas/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                int id_fatura = std::stoi(req.matches[1]);

                auto fatura = m_faturaService.obterFatura(id_fatura);
                json j = faturaToJSON(fatura);
                res.set_content(j.dump(), "application/json");
                res.status = 200;
            }
            catch(std::runtime_error& e)
            {
                json erro; 
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 404;
            }
        });

        servidor.Post("/api/v1/faturas", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                json j = json::parse(req.body);
                int id_cliente = j["id_cliente"];
                Fatura fatura = m_faturaService.criarRascunho(id_cliente);
                res.set_content(faturaToJSON(fatura).dump(), "application/json");
                res.status = 201;
            }
            catch(std::runtime_error& e)
            {
                json erro; 
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 400;
            }
        });

        servidor.Put(R"(/api/v1/faturas/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                int id_fatura = std::stoi(req.matches[1]); 
                json j = json::parse(req.body);
                Fatura fatura = jsonToFatura(j);
                fatura.setID(id_fatura);
                m_faturaService.atualizarRascunho(fatura);
                res.set_content(faturaToJSON(fatura).dump(), "application/json");
                res.status = 200;
            }
            catch(std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 404;
            }
        });

        servidor.Delete(R"(/api/v1/faturas/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try 
            {
                int id_fatura = std::stoi(req.matches[1]);
                m_faturaService.deletarRascunho(id_fatura);
                res.status = 204;
            }
            catch(std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 404;
            }
        });

        servidor.Post(R"(/api/v1/faturas/(\d+)/linhas)",[this](const httplib::Request& req, httplib::Response& res){
            try 
            {
                int id_fatura = std::stoi(req.matches[1]);
                json j = json::parse(req.body);
                int id_produto = j["id_produto"];
                int quantidade = j["quantidade"];

                Fatura fatura_atualizada = m_faturaService.adicionarLinha(id_fatura, id_produto, quantidade);

                res.set_content(faturaToJSON(fatura_atualizada).dump(), "application/json");
                res.status = 201;
            }
            catch(std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 400;
            }
        });

        servidor.Post(R"(/api/v1/faturas/(\d+)/emitir)", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                int id_fatura = std::stoi(req.matches[1]);
                m_faturaService.emitir(id_fatura);
                Fatura fatura = m_faturaService.obterFatura(id_fatura);
                res.set_content(faturaToJSON(fatura).dump(), "application/json");
                res.status = 200;
            }
            catch(std::runtime_error& e)
            {
                json erro; 
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 400;
            }
        });
    }
};