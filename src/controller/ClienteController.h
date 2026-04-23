#pragma once
#include "domain/Cliente.h"
#include "repository/ClienteRepository.h"
#include "service/ClienteService.h"
#include <nlohmann/json.hpp>
#include <httplib.h>
using json = nlohmann::json;

class ClienteController
{
    private:
    ClienteService& m_cs;

    json clienteToJSON (const Cliente& cliente) const 
    {
        //CRIAR JSON 
        json j; 
        j["id"] = cliente.getID();
        j["nome"] = cliente.getNome();
        j["nif"] = cliente.getNif();
        j["morada"] = cliente.getMorada();
        j["codigo_postal"] = cliente.getCodigoPostal();
        j["cidade"] = cliente.getCidade();
        j["email"] = cliente.getEmail();
        j["telefone"] = cliente.getTelefone();
        j["tipo"] = cliente.TipotoString();
        j["ativo"] = cliente.getAtivo();

        return j;
    }

    Cliente jsonToCliente(const json& j)
    {
        Cliente cliente(j["nome"],j["nif"]);
        cliente.setID(j["id"]);
        cliente.setMorada(j["morada"]);
        cliente.setCodigoPostal(j["codigo_postal"]);
        cliente.setCidade(j["cidade"]);
        cliente.setEmail(j["email"]);
        cliente.setTelefone(j["telefone"]);
        cliente.setTipo(cliente.StringToTipo(j["tipo"]));
        cliente.setAtivo(j["ativo"]);

        return cliente;
    } 

    public:
    ClienteController(ClienteService& cs)
    :m_cs(cs){}

    void registrarRotas(httplib::Server& servidor)
    {
        //LISTAR TODOS
        //[this] -> acede aos membros do controller (m_cs, clienteToJSON)
        servidor.Get("/api/v1/clientes", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                //Extrair query paramaters (string vazia se nao existirem)
                std::string paginaStr = req.get_param_value("pagina");
                std::string tamanhoStr = req.get_param_value("tamanho");

                //Aplicar defaults se vazios
                int pagina = paginaStr.empty() ? 1 : std::stoi(paginaStr);
                int tamanho = tamanhoStr.empty() ? 20 : std::stoi(tamanhoStr);

                //Passar ao service
                auto clientes = m_cs.listarClientes(pagina, tamanho);
                
                json arr = json::array();
                for(const auto& c : clientes)
                    arr.push_back(clienteToJSON(c));
                //arr.dump() -> converte o objeto JSON em string
                //"application/json" -> Content-Type da resposta, diz ao cliente HTTP que é tipo JSON, nao HTML nem texto simples
                res.set_content(arr.dump(), "application/json");
                //status code HTTP da resposta. 200 -> OK significa "pedido processado com sucesso".
                //É o status padrão para GETs bem sucedidos
                //Se nao definir, por defeito ele fica 200
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

        //OBTER POR ID
        servidor.Get(R"(/api/v1/clientes/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                //EXTRAIR id DA URL, O matches[1] CONTÉM O QUE FOI CAPTURADO PELO (\d+)
                int id = std::stoi(req.matches[1]);

                auto cliente = m_cs.obterCliente(id);
                json j = clienteToJSON(cliente);
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

        //CRIAR
        servidor.Post("/api/v1/clientes", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                json j = json::parse(req.body);
                Cliente cliente = jsonToCliente(j);
                m_cs.criarCliente(cliente);
                res.set_content(clienteToJSON(cliente).dump(), "application/json");
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

        //ATUALIZAR
        servidor.Put(R"(/api/v1/clientes/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                int id = std::stoi(req.matches[1]);
                json j = json::parse(req.body);
                Cliente cliente = jsonToCliente(j);
                cliente.setID(id);
                m_cs.atualizarCliente(cliente);
                res.set_content(clienteToJSON(cliente).dump(), "application/json");
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

        //DELETAR
        servidor.Delete(R"(/api/v1/clientes/(\d+))", [this](const httplib::Request& req, httplib::Response& res){
            try
            {
                int id = std::stoi(req.matches[1]);
                m_cs.removerCliente(id);
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