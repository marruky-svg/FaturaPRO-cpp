/*para queries que retornam dados (SELECT) ou comandos com parâmetros dinâmicos (INSERT com valores do utilizador), 
precisas de prepared statements. 
São mais seguros (previnem SQL injection) e mais eficientes (o SQLite compila o SQL uma vez e reutiliza).
*/
#pragma once
#include <sqlite3.h>
#include <string>
#include <stdexcept>

class Statement 
{
    private:
    sqlite3_stmt* m_stmt;

    public:
    //Cópia proibida - mesmo do Database
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    //Construtor vai compilar o sql
    Statement(sqlite3* db, const std::string& sql)
    {
        //sqlite3_prepare_v2 compila o SQL e guarda o resultado em m_stmt
        //Parametros:
        //db        - a conexão (vem do Database)
        //sql       - o SQL com placeholders ?
        //-1        - diz ao SQLite para calcular o tamanho da string sozinho
        //&m_stmt   - onde guardar o statement compilado
        //nullptr   - nao precisamos do ponteiro para o SQL nao consumido
        int resultado = sqlite3_prepare_v2(db, sql.c_str(), -1, &m_stmt, nullptr);
        if(resultado != SQLITE_OK)
        {
            //sqlite3_errmsg usa a conexao, nao o statement
            std::string mensagem(sqlite3_errmsg(db));
            throw std::runtime_error("Erro ao preparar SQL: " + mensagem);
        }
    }
    //liberta o statement compilado 
    ~Statement(){sqlite3_finalize(m_stmt);}

    //======================
    //        BIND
    //======================
    
    //Bind: liga uma string ao placeholder na posicao index
    //Os indices comecam em 1, nao em 0
    //SQLITE_TRANSIENT diz ao SQLite para copiar a string -
    //assim nao importa se a std::string original é destruída depois
    void vincularTexto(int index, const std::string& valor)
    {
        int resultado = sqlite3_bind_text(m_stmt, index, valor.c_str(), -1, SQLITE_TRANSIENT);
        if(resultado != SQLITE_OK)
            throw std::runtime_error("Erro ao vincular texto");
    }
    //Bind: liga um inteiro ao placeholder na posição index 
    void vincularInteiro(int index, int valor)
    {
        int resultado = sqlite3_bind_int(m_stmt, index, valor);
        if(resultado != SQLITE_OK)
            throw std::runtime_error("Erro ao vincular inteiro");
    }
    //Bind:liga um double ao placeholder na posicao index
    void vincularDecimal(int index, double valor)
    {
        int resultado = sqlite3_bind_double(m_stmt, index, valor);
        if(resultado != SQLITE_OK)
            throw std::runtime_error("Erro ao vincular decimal");
    }

    //======================
    //        STEP
    //======================

    //Step: executa o statement
    //Retorna true se ha uma linha de resultado para ler(SQLITE_ROW)
    //Retorn false se terminou (SQLITE_DONE)
    //Lança excecao para qlqr outro resultado(erro)
    
    bool passo()
    {
        int resultado = sqlite3_step(m_stmt);
        if(resultado == SQLITE_ROW)
            return true;
        if(resultado == SQLITE_DONE)
            return false;
        throw std::runtime_error("Erro ao executar o statement");
    }

    //======================
    //       COLUNA
    //======================

    //Column: lê uma string da coluna na posição index
    //ATENÇÂO: aqui os indices começam em 0, diferente do bind que começa em 1
    //sqlite3_column_text retorn const unsigned char* 
    //O reinterpret_cas converte para const char* que o std::string aceita
    std::string obterTexto(int index)
    {
        const unsigned char* texto = sqlite3_column_text(m_stmt, index);
        if(texto == nullptr)
            return "";
        return std::string(reinterpret_cast<const char*>(texto));
    }

    //Column: lê um inteiro da coluna 
    int obterInteiro(int index){return sqlite3_column_int(m_stmt, index);}

    //Column: lê um double da coluna
    double obterDecimal(int index){return sqlite3_column_double(m_stmt, index);}
    
};