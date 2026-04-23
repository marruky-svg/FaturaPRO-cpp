#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include "Database.hpp"
#include "Statement.hpp"

class GestorMigracoes
{
    private:
    Database&   m_db;
    std::string m_caminho;

    //cria a tabela de controlo se não existir 
    // esta tabela guarda quais migrações já foram aplicadas
    // tem apenas uma coluna: versao (o nome do ficheiro SQL)
    void criarTabelaControle()
    {
        m_db.executar(
            "CREATE TABLE IF NOT EXISTS schema_migrations ("
            "versao TEXT PRIMARY KEY)"
        );
    }
    //verifica se uma migração específica já foi aplicada
    //faz SELECT na tabela de controle procurando o nome do ficheiro 
    //se passo() retorna true, há uma linha - logo já foi aplicada 
    bool jaAplicada(const std::string& versao)
    {
        Statement stmt(m_db.obterHandle(),
    "SELECT versao FROM schema_migrations WHERE versao = ?");
    stmt.vincularTexto(1,versao);
    return stmt.passo();
    }
    //lê o conteudo inteiro de um ficheiro SQL para uma string 
    //std::ifstream abre o ficheiro para leitura
    //std::istreambuf_iterator percorre o ficheiro byte a byte 
    //o resultado é todo o SQL dentro do ficheiro numa unica string
    std::string lerFicheiro(const std::string& caminho)
    {
        std::ifstream ficheiro(caminho);
        if(!ficheiro.is_open())
            throw std::runtime_error("Não foi possivel abrir: " + caminho);
        return std::string(
            std::istreambuf_iterator<char>(ficheiro),
            std::istreambuf_iterator<char>()
        );
    } 
    //registra na tabela de controle que uma migração foi aplicada
    //Usa prepared statement para segurança 
    void registrarMigracao(const std::string& versao)
    {
        Statement stmt(m_db.obterHandle(), "INSERT INTO schema_migrations (versao) VALUES(?)");
    }

    public:
    //construtor: receber referencia ao Database e o caminho da pasta migrations/
    //Referência pq nao queremos(e nem podemos) copiar o Database - queremos usar o mesmo
    GestorMigracoes(Database& db, const std::string& caminho)
    :m_db(db), m_caminho(caminho){}
    
    //Método principal: aplica todas as migrações pendentes 
    void aplicar()
    {
        //Passo 1: garantir que a tabela de controle existe 
        criarTabelaControle();

        //Passo 2: listar todos os ficheiros .sql da pasta migrations/
        //std::filesystem::directory_iterator percorre a pasta
        //Guardamos os caminhos num vector para depois ordenar 
        std::vector<std::filesystem::path> ficheiros;
        for(const auto& entrada : std::filesystem::directory_iterator(m_caminho))
        {
            //Filtra apenas os ficheiros com extensao .sql
            if(entrada.path().extension() == ".sql")
                ficheiros.push_back(entrada.path());
        }
        //Passo 3: ordenar por nome de ficheiro
        //Como os ficheiros começam com 001_, 002_, etc...
        //a ordem alfabética é a ordem correta de execução 
        std::sort(ficheiros.begin(), ficheiros.end());

        //Passo 4: para cada ficheiro, verificar se já foi aplicado
        //Se não foi: ler o SQL, executar, registrar 
        for(const auto& ficheiro : ficheiros)
        {
            //filename() retorna apenas o nome do ficheiro(ex: "001_criar_empresas.sql")
            //.string() converte de std::filesystem::path para std::string
            std::string versao = ficheiro.filename().string();
            if(!jaAplicada(versao))
            {
                //Lê o conteúdo SQL do ficheiro
                std::string sql = lerFicheiro(ficheiro.string());   
                
                //Executar o SQL(criar tabela, alterar schema, etc...)
                m_db.executar(sql);

                //Registra que esta migração foi aplicada
                registrarMigracao(versao);
            }
        }
    }
};

/*      FLUXO MENTAL

Aplicação inicia
    │
    ├─ Cria tabela schema_migrations (se não existir)
    │
    ├─ Lê pasta migrations/
    │   ├─ 001_criar_empresas.sql
    │   ├─ 002_criar_clientes.sql
    │   └─ 003_criar_produtos.sql
    │
    ├─ Verifica schema_migrations:
    │   ├─ 001 já aplicada? NÃO → executa SQL → regista
    │   ├─ 002 já aplicada? NÃO → executa SQL → regista
    │   └─ 003 já aplicada? NÃO → executa SQL → regista
    │
    └─ Próxima vez que iniciar:
        ├─ 001 já aplicada? SIM → ignora
        ├─ 002 já aplicada? SIM → ignora
        └─ 003 já aplicada? SIM → ignora


        EXEMPLO NO MAIN():
        
        #include "Database.h"   
        #include "GestorMigracoes.h"

        int main()
        {
            Database db("faturapro.db");
            GestorMigracoes migracoes(db, "migrations");
            migracoes.aplicar();
            return 0;
        }

*/