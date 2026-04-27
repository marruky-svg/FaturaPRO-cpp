#pragma once
#include "domain/Fatura.h"
#include "domain/LinhaFatura.h"
#include "util/Database.hpp"
#include "util/Statement.hpp"
#include <sstream>
#include <iomanip>

class FaturaRepository
{
    private: 
    Database& m_db;

    public:
    FaturaRepository(Database& db)
    :m_db(db){}

    void guardar(Fatura& fatura)
    {
        m_db.executar("BEGIN TRANSACTION");
        try
        {
            
            
            Statement stmt(m_db.obterHandle(), "INSERT INTO faturas(numero,cliente_id,data_emissao,data_vencimento,estado,subtotal,total_iva,total,desconto_global,observacoes,criada_em) VALUES(?,?,?,?,?,?,?,?,?,?,?)");
            
            stmt.vincularTexto(1, fatura.getNumero());
            stmt.vincularInteiro(2, fatura.getID_Cliente());
            stmt.vincularTexto(3, fatura.getDataEmissao());
            stmt.vincularTexto(4, fatura.getDataVencimento());
            stmt.vincularTexto(5, fatura.estadoToString());
            stmt.vincularDecimal(6, fatura.getSubTotal());
            stmt.vincularDecimal(7, fatura.getTotalIVA());
            stmt.vincularDecimal(8, fatura.getTotal());
            stmt.vincularDecimal(9,fatura.getDescontGlobal());
            stmt.vincularTexto(10, fatura.getObservacao());
            stmt.vincularTexto(11,fatura.getCriadaEm());
            stmt.passo();
            int idGerado = sqlite3_last_insert_rowid(m_db.obterHandle());
            fatura.setID(idGerado);


            
            for(auto& linha : fatura.getLinhasMut())
            {
                Statement stmt_linhas(m_db.obterHandle(), "INSERT INTO linhas_fatura(id_fatura,id_produto,descricao,quantidade,preco_unitario,desconto,taxa_iva,subtotal) VALUES(?,?,?,?,?,?,?,?)");
                stmt_linhas.vincularInteiro(1, idGerado);
                stmt_linhas.vincularInteiro(2, linha.getProdutoID());
                stmt_linhas.vincularTexto(3, linha.getDescricao());
                stmt_linhas.vincularInteiro(4,linha.getQuantidade());
                stmt_linhas.vincularDecimal(5, linha.getPrecoUnitario());
                stmt_linhas.vincularDecimal(6,linha.getDesconto());
                stmt_linhas.vincularDecimal(7,linha.getTaxaIVA());
                stmt_linhas.vincularDecimal(8, linha.calcularSubtotal());
                stmt_linhas.passo();
            }

            m_db.executar("COMMIT");
        }
        catch(...)
        {
            m_db.executar("ROLLBACK");
            throw;
        }
    }

    std::optional<Fatura> buscarPorID(int id)
    {
        Statement stmt(m_db.obterHandle(), "SELECT id, numero, cliente_id, data_emissao, data_vencimento, estado, subtotal, total_iva, total, desconto_global, observacoes, criada_em FROM faturas WHERE id = ?");
        stmt.vincularInteiro(1, id);
        
        if (!stmt.passo())
            return std::nullopt;
        
        Fatura fatura(stmt.obterInteiro(2));  
        fatura.setNumero(stmt.obterTexto(1));
        fatura.setDataEmissao(stmt.obterTexto(3));
        fatura.setDataVencimento(stmt.obterTexto(4));
        fatura.setEstadoFatura(fatura.stringToEstado(stmt.obterTexto(5)));
        fatura.setSubtotal(stmt.obterDecimal(6));        
        fatura.setTotalIVA(stmt.obterDecimal(7));         
        fatura.setTotal(stmt.obterDecimal(8));            
        fatura.setDescontoGlobal(stmt.obterDecimal(9));
        fatura.setObservacao(stmt.obterTexto(10));
        fatura.setCriadaEm(stmt.obterTexto(11));
        
        // Carregar linhas
        Statement stmt_linha(m_db.obterHandle(), "SELECT id, id_fatura, id_produto, descricao, quantidade, preco_unitario, desconto, taxa_iva, subtotal FROM linhas_fatura WHERE id_fatura = ?");
        stmt_linha.vincularInteiro(1, id);
        
        while (stmt_linha.passo())
        {
            LinhaFatura linha(
                stmt_linha.obterInteiro(2),  // produto_id
                stmt_linha.obterTexto(3),    // descricao
                stmt_linha.obterInteiro(4),  // quantidade
                stmt_linha.obterDecimal(5),  // preco_unitario
                stmt_linha.obterDecimal(7)   // taxa_iva
            );
            linha.setID(stmt_linha.obterInteiro(0));
            linha.setFaturaID(stmt_linha.obterInteiro(1));
            linha.setDesconto(stmt_linha.obterDecimal(6));
            fatura.adicionarLinha(linha);
        }
        return fatura;
    }

    std::vector<Fatura> buscarTodos(int limit, int offset)
    {
        std::vector<Fatura> faturas;
        Statement stmt(m_db.obterHandle(), "SELECT * FROM faturas ORDER BY id LIMIT ? OFFSET ?");
        stmt.vincularInteiro(1,limit);
        stmt.vincularInteiro(2, offset);

        while(stmt.passo())
        {
            //CRIANDO A FATURA
            Fatura fatura(stmt.obterInteiro(2)); 
            fatura.setID(stmt.obterInteiro(0)); 
            fatura.setNumero(stmt.obterTexto(1));
            fatura.setDataEmissao(stmt.obterTexto(3));
            fatura.setDataVencimento(stmt.obterTexto(4));
            fatura.setEstadoFatura(fatura.stringToEstado(stmt.obterTexto(5)));
            fatura.setSubtotal(stmt.obterDecimal(6));        
            fatura.setTotalIVA(stmt.obterDecimal(7));         
            fatura.setTotal(stmt.obterDecimal(8));            
            fatura.setDescontoGlobal(stmt.obterDecimal(9));
            fatura.setObservacao(stmt.obterTexto(10));
            fatura.setCriadaEm(stmt.obterTexto(11));

            //CRIANDO AS LINHAS DA FATURA
            Statement stmt_linha(m_db.obterHandle(), "SELECT id, id_fatura, id_produto, descricao, quantidade, preco_unitario, desconto, taxa_iva, subtotal FROM linhas_fatura WHERE id_fatura = ?");
            stmt_linha.vincularInteiro(1, fatura.getID());
            
            while(stmt_linha.passo())
            {
                LinhaFatura linha(
                stmt_linha.obterInteiro(2),  // produto_id
                stmt_linha.obterTexto(3),    // descricao
                stmt_linha.obterInteiro(4),  // quantidade
                stmt_linha.obterDecimal(5),  // preco_unitario
                stmt_linha.obterDecimal(7)   // taxa_iva
                );
                linha.setID(stmt_linha.obterInteiro(0));
                linha.setFaturaID(stmt_linha.obterInteiro(1));
                linha.setDesconto(stmt_linha.obterDecimal(6));
                fatura.adicionarLinha(linha);
            }
                //ADICIONANDO AS LINHAS À FATURAs
                faturas.push_back(fatura);
        }
        return faturas;
    }

    std::string obterProximoNumero(int ano)
    {
        std::string padrao = "FT " + std::to_string(ano) + "/%";

        Statement stmt(m_db.obterHandle(), "SELECT COUNT(*) FROM faturas WHERE numero LIKE ?");
        stmt.vincularTexto(1, padrao);
        // EXECUTAR O COUNT - ELE SEMPRE DEVOLVE UMA LINHA 
        stmt.passo();
        // LER O NUMERO DA FATURA
        int count = stmt.obterInteiro(0);

        // O PROXIMO PASSO E O SEGUINTE AO COUNT ATUAL
        int proximoNumero = count + 1;

        // Formatar a string final "FT 2026/00042"
        // std::setw(5) garante 5 caracteres de largura
        // std::setfill('0') preenche com zeros à esquerda
        std::stringstream ss;
        ss << "FT " << ano << "/"<< std::setw(5) << std::setfill('0') << proximoNumero;

        return ss.str();
    }

    void atualizar(Fatura& fatura)
    {
        m_db.executar("BEGIN TRANSACTION");

        try
        {
            // UPDATE DA FATURA 
            Statement stmt(m_db.obterHandle(), "UPDATE faturas SET cliente_id = ?, data_emissao = ?, data_vencimento = ?, estado = ?, subtotal = ?, total_iva = ?, total = ?, desconto_global = ?, observacoes = ? WHERE id = ?");
            stmt.vincularInteiro(1,fatura.getID_Cliente());
            stmt.vincularTexto(2, fatura.getDataEmissao());
            stmt.vincularTexto(3,fatura.getDataVencimento());
            stmt.vincularTexto(4, fatura.estadoToString());
            stmt.vincularDecimal(5,fatura.getSubTotal());
            stmt.vincularDecimal(6,fatura.getTotalIVA());
            stmt.vincularDecimal(7,fatura.getTotal());
            stmt.vincularDecimal(8, fatura.getDescontGlobal());
            stmt.vincularTexto(9, fatura.getObservacao());
            stmt.vincularInteiro(10, fatura.getID());
            stmt.passo();

            // DELETE NAS LINHAS ANTIGAS
            Statement stmt_del(m_db.obterHandle(), "DELETE FROM linhas_fatura WHERE id_fatura = ?");
            stmt_del.vincularInteiro(1, fatura.getID());
            stmt_del.passo();

            // INSERT DAS NOVAS LINHAS
            for(auto& linha : fatura.getLinhasMut())
            {
                Statement stmt_linhas(m_db.obterHandle(), "INSERT INTO linhas_fatura(id_fatura,id_produto,descricao,quantidade,preco_unitario,desconto,taxa_iva,subtotal) VALUES(?,?,?,?,?,?,?,?)");
                stmt_linhas.vincularInteiro(1, fatura.getID());
                stmt_linhas.vincularInteiro(2, linha.getProdutoID());
                stmt_linhas.vincularTexto(3, linha.getDescricao());
                stmt_linhas.vincularInteiro(4,linha.getQuantidade());
                stmt_linhas.vincularDecimal(5, linha.getPrecoUnitario());
                stmt_linhas.vincularDecimal(6,linha.getDesconto());
                stmt_linhas.vincularDecimal(7,linha.getTaxaIVA());
                stmt_linhas.vincularDecimal(8, linha.calcularSubtotal());
                stmt_linhas.passo();
            }

            m_db.executar("COMMIT");
        }
        catch(...)
        {
            m_db.executar("ROLLBACK");
            throw;
        }
    }
};
