#include "util/Database.hpp"
#include "util/GestorMigracoes.hpp"
#include "repository/ClienteRepository.h"
#include "repository/ProdutoRepository.h"
#include "repository/FaturaRepository.h"
#include "repository/UtilizadorRepository.h"
#include "repository/RelatorioRepository.h"
#include "auth/Autenticador.h"
#include "service/ClienteService.h"
#include "service/ProdutoService.h"
#include "service/FaturaService.h"
#include "service/RelatorioService.h"
#include "controller/ClienteController.h"
#include "controller/ProdutoController.h"
#include "controller/FaturaController.h"
#include "controller/RelatorioController.h"
#include <httplib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main()
{
    try
    {
        //INICIALIZA COM TIMESTAMP ATUAL
        srand(time(nullptr));

        Database db("faturapro.db");
        GestorMigracoes gm(db, "migrations");
        gm.aplicar();

        // Repositories
        ClienteRepository c_repo(db);
        ProdutoRepository p_repo(db);
        FaturaRepository f_repo(db);
        RelatorioRepository r_repo(db);
        UtilizadorRepository u_repo(db);


        // Services & Autenticador
        ClienteService c_service(c_repo);
        ProdutoService p_service(p_repo);
        FaturaService f_service(f_repo, c_repo, p_repo);  // 3 repositories
        RelatorioService r_service(r_repo);
        Autenticador autenticador (u_repo);

        // Controllers
        ClienteController c_controller(c_service);
        ProdutoController p_controller(p_service);
        FaturaController f_controller(f_service);
        RelatorioController r_controller(r_service);

        // Servidor
        httplib::Server servidor;

        //Middleware de autenticacao - corre antes de qualquer rota
        servidor.set_pre_routing_handler([&autenticador](const httplib::Request& req, httplib::Response& res){
            std::string apiKey = req.get_header_value("X-API-Key");

            if(!autenticador.validar(apiKey))
            {
                json erro; 
                erro["erro"] = "Nao autorizado";
                res.set_content(erro.dump(), "application/json");
                res.status = 401;
                return httplib::Server::HandlerResponse::Handled;
            }

            return httplib::Server::HandlerResponse::Unhandled;
        });

        //Registrar rotas
        c_controller.registrarRotas(servidor);
        p_controller.registrarRotas(servidor);
        f_controller.registrarRotas(servidor);
        r_controller.registrarRotas(servidor);

        std::cout << "Servidor a correr em http://localhost:8081" << std::endl;
        servidor.listen("0.0.0.0", 8081);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}