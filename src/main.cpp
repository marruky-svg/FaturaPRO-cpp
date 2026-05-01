#include "util/Database.hpp"
#include "util/GestorMigracoes.hpp"
#include "repository/ClienteRepository.h"
#include "repository/ProdutoRepository.h"
#include "repository/FaturaRepository.h"
#include "repository/RelatorioRepository.h"
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


        // Services
        ClienteService c_service(c_repo);
        ProdutoService p_service(p_repo);
        FaturaService f_service(f_repo, c_repo, p_repo);  // 3 repositories
        RelatorioService r_service(r_repo);

        // Controllers
        ClienteController c_controller(c_service);
        ProdutoController p_controller(p_service);
        FaturaController f_controller(f_service);
        RelatorioController r_controller(r_service);

        // Servidor
        httplib::Server servidor;
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