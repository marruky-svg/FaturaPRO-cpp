#include "util/Database.hpp"
#include "util/Statement.hpp"
#include "util/GestorMigracoes.hpp"

int main()
{
//Criar base de dados
Database db("faturapro.db");
db.executar("CREATE TABLE IF NOT EXISTS clientes("
    "id INTEGER PRIMARY KEY, "
    "nome TEXT NOT NULL, "
    "nif TEXT UNIQUE)");

GestorMigracoes migracoes(db, "migrations");
migracoes.aplicar();
//Inserir com prepared statement (seguro contra SQL injection)
Statement inserir(db.obterHandle(), "INSERT OR IGNORE INTO clientes(nome, nif) VALUES(?,?)");
inserir.vincularTexto(1, "Elder Souza");
inserir.vincularTexto(2,"295793503");
inserir.passo();

//Ler dados
Statement consulta(db.obterHandle(), "SELECT id, nome, nif FROM clientes");
while(consulta.passo())
{
    int id = consulta.obterInteiro(0);
    std::string nome = consulta.obterTexto(1);
    std::string nif = consulta.obterTexto(2);
}

}