#pragma once
#include "domain/Produto.h"
#include "repository/ProdutoRepository.h"
#include "service/ProdutoService.h"
#include <nlohmann/json.hpp>
#include <httplib.h>
using json = nlohmann::json;

class ProdutoController
{
    private:
    ProdutoService& m_ps;

    json produtoToJSON(const Produto& produto)
    {
        json j; 
        j["id"] = produto.getID();
        j["sku"] = produto.getSKU();
        j["nome"] = produto.getNome();
        j["descricao"] = produto.getDescricao();
        j["preco_unitario"] = produto.getPrecoUnitario();
        j["taxa_iva"] = produto.getTaxaIVA();
        j["stock"] = produto.getStock();
        j["stock_minimo"] = produto.getStockMin();
        j["tipo"] = produto.TipotoString();
        j["ativo"] = produto.getAtivo();

        return j;
    }

    Produto jsonToProduto(const json& j)
    {
        Produto produto(j["sku"],j["nome"]);
        produto.setID(j["id"]);
        produto.setDescricao(j["descricao"]);
        produto.setPrecoUnitario(j["preco_unitario"]);
        produto.setIVA(j["taxa_iva"]);
        produto.setStock(j["stock"]);
        produto.setStockMin(j["stock_minimo"]);
        produto.setTipo(produto.StringToTipo(j["tipo"]));
        produto.setAtivo(j["ativo"]);

        return produto;
    }

    public:

    ProdutoController(ProdutoService& ps)
    :m_ps(ps){}

    void registrarRotas(httplib::Server& servidor)
    {
        //LISTAR TODOS 
        servidor.Get("/api/v1/produtos", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                //Extrair query paramaters (string vazia se nao existirem)
                std::string paginaStr = req.get_param_value("pagina");
                std::string tamanhoStr = req.get_param_value("tamanho");

                //Aplicar defaults se vazios
                int pagina = paginaStr.empty() ? 1 : std::stoi(paginaStr);
                int tamanho = tamanhoStr.empty() ? 20 : std::stoi(tamanhoStr);
                
                std::vector<Produto> produtos;
                produtos = m_ps.listarProdutos(pagina, tamanho);
                json arr = json::array();
                for(const auto& produto : produtos)
                    arr.push_back(produtoToJSON(produto));
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

        servidor.Get(R"(/api/v1/produtos/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                int id  = std::stoi(req.matches[1]);
                auto produto = m_ps.obterProduto(id);
                json j = produtoToJSON(produto);
                res.set_content(j.dump(), "application/json");
                res.status = 200;
            }
            catch(const std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 404;
            }
        });

        servidor.Post("/api/v1/produtos", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                json j = json::parse(req.body);
                auto produto = jsonToProduto(j);
                m_ps.criarProduto(produto);
                res.set_content(produtoToJSON(produto).dump(), "application/json");
                res.status = 201;
            }
            catch(const std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 400;
            }
        });

        servidor.Put(R"(/api/v1/produtos/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                int id = std::stoi(req.matches[1]);
                json j = json::parse(req.body);
                Produto produto = jsonToProduto(j);
                produto.setID(id);
                m_ps.atualizarProduto(produto);
                res.set_content(produtoToJSON(produto).dump(), "application/json");
                res.status = 200;
            }
            catch(const std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 404;
            }
        });

        servidor.Delete(R"(/api/v1/produtos/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                int id = std::stoi(req.matches[1]);
                m_ps.removerProduto(id);
                res.status = 204;
            }
            catch(const std::runtime_error& e)
            {
                json erro;
                erro["erro"] = e.what();
                res.set_content(erro.dump(), "application/json");
                res.status = 404;
            }
        });
    }
};