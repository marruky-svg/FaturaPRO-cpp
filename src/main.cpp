#include "util/Database.hpp"
#include "util/GestorMigracoes.hpp"
#include "util/Statement.hpp"
#include "domain/Cliente.h"
#include "domain/Produto.h"
#include "repository/ClienteRepository.h"
#include "repository/ProdutoRepository.h"
#include "service/ClienteService.h"
#include "service/ProdutoService.h"
#include "controller/ClienteController.h"
#include "controller/ProdutoController.h"
#include <iostream>
#include <httplib.h>

int main()
{
    Database db("faturapro.db");
    GestorMigracoes gm(db,"migrations");
    try
    {
       gm.aplicar();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    //Cliente
    ClienteRepository c_repo(db);
    ClienteService c_service(c_repo);
    ClienteController c_controller(c_service);

    //Produto
    ProdutoRepository p_repo(db);
    ProdutoService p_service(p_repo);
    ProdutoController p_controller(p_service);

    //Servidor
    httplib::Server servidor;

    //registrar rotas
    c_controller.registrarRotas(servidor);
    p_controller.registrarRotas(servidor);

    //Inicia o servidor - bloqueia aqui ate alguem parar (Ctrl + C)
    std::cout <<"Servidor a correr em http://localhost:8081"<<std::endl;
    servidor.listen("0.0.0.0", 8081);

}