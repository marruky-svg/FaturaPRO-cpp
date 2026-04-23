#include <gtest/gtest.h>
#include <memory>
#include <domain/Cliente.h>
#include "util/Database.hpp"
#include "util/GestorMigracoes.hpp"
#include "repository/ClienteRepository.h"
#include "service/ClienteService.h"

class ClienteServiceTest: public::testing::Test
{
    protected: 
    std::unique_ptr<Database> m_db;
    std::unique_ptr<ClienteRepository> m_repo;
    std::unique_ptr<ClienteService> m_cs;

    void SetUp() override
    {
        m_db = std::make_unique<Database>(":memory:");
        GestorMigracoes gm(*m_db, "../migrations");
        gm.aplicar();
        m_repo = std::make_unique<ClienteRepository> (*m_db);
        m_cs = std::make_unique<ClienteService> (*m_repo);
    }
};
//NIF COM 9 DIGITOS MAS COM CHECK DIGITE INCORRETO
TEST_F(ClienteServiceTest, CriarClienteComNifInvalido)
{
    Cliente c("João", "111111111");
    EXPECT_THROW(m_cs->criarCliente(c), std::runtime_error);

}
//TESTE VÁLIDO
TEST_F(ClienteServiceTest, CriarClienteValido)
{
    Cliente cliente("Elder", "295793503");
    m_cs->criarCliente(cliente);
}
//TESTE COM NIF DUPLICADO
TEST_F(ClienteServiceTest,CriarClientesComNifDuplicado)
{
    Cliente c1("Elder", "295793503");
    m_cs->criarCliente(c1);

    Cliente c2("Outro Nome", "295793503");
    EXPECT_THROW(m_cs->criarCliente(c2), std::runtime_error);
}

//OBTER CLIENTE INVÁLIDO
TEST_F(ClienteServiceTest, ObterClienteInexistente)
{
    EXPECT_THROW(m_cs->obterCliente(999), std::runtime_error);
}

//TESTAR EMAIL VÁLIDO
TEST_F(ClienteServiceTest, CriarClienteComEmailInvalido)
{
    Cliente c("Elder", "295793503");
    c.setEmail("semarroba.pt");
    EXPECT_THROW(m_cs->criarCliente(c), std::runtime_error);
}