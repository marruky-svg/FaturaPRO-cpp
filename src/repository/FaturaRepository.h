#pragma once
#include "domain/Fatura.h"
#include "domain/LinhaFatura.h"
#include "util/Database.hpp"
#include "util/Statement.hpp"


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


            Statement stmt_linhas(m_db.obterHandle(), "INSERT INTO linhas_fatura(id_fatura,id_produto,descricao,quantidade,preco_unitario,desconto,taxa_iva,subtotal) VALUES(?,?,?,?,?,?,?,?)");
            for(auto& linha : fatura.getLinhasMut())
            {
                linha.setFaturaID(idGerado);
                stmt_linhas.vincularInteiro(1, linha.getFaturaID());
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
