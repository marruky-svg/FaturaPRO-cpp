#pragma once
#include "domain/LinhaFatura.h"
#include <string>
#include <vector>

enum class EstadoFatura
{
    Rascunho, 
    Emitida,
    Paga,
    Anulada
};

class Fatura
{
    private: 
    int m_id = 0;
    std::string m_numero; //FT 2026/000001
    int m_id_cliente = 0;
    std::string m_data_emissao;
    std::string m_data_vencimento;
    EstadoFatura m_estadoFatura = EstadoFatura::Rascunho;
    double m_subtotal = 0; // calculado no metodo subtotal() de linhaFatura
    double m_total_iva = 0;
    double m_total = 0;
    double m_desconto_global = 0;
    std::string m_observacao;
    std::string m_criada_em;
    std::vector<LinhaFatura> m_linhas;

    public:
    //CONSTRUCTOR
    Fatura(int id_cliente)
    :m_id_cliente(id_cliente){}
    
    //METODOS
    void adicionarLinha(const LinhaFatura& linha){m_linhas.push_back(linha);}
    //Atualizar subtotal/total/total_iva
    void recalcular()
    {
        double subtotal = 0;
        double iva = 0;
        for(auto& linha : m_linhas)
        {   
            subtotal += linha.calcularSubtotal();
            iva += linha.calcularIVA();
        }
        double fator = 1 - m_desconto_global/100;
        m_subtotal = subtotal * fator;
        m_total_iva = iva * fator;
        m_total = m_subtotal + m_total_iva;
    }

    //CONVERSOR
    std::string estadoToString() const 
    {
        switch(m_estadoFatura)
        {
            case EstadoFatura::Anulada: return "anulada";
            case EstadoFatura::Emitida: return "emitida";
            case EstadoFatura::Paga: return "paga";
            case EstadoFatura::Rascunho: return "rascunho";
        }
        return "rascunho";
    }

    EstadoFatura stringToEstado(const std::string& estado)
    {
        if(estado == "anulada") return EstadoFatura::Anulada;
        if(estado == "emitida") return EstadoFatura::Emitida;
        if(estado == "paga") return EstadoFatura::Paga;
        if(estado == "rascunho") return EstadoFatura::Rascunho;
        throw std::runtime_error("Estado inválido");
    }
    
    //GETTERS
    int getID() const {return m_id;}
    const std::string& getNumero() const {return m_numero;}
    int getID_Cliente() const {return m_id_cliente;}
    const std::string& getDataEmissao() const {return m_data_emissao;}
    const std::string& getDataVencimento() const {return m_data_vencimento;}
    EstadoFatura getEstadoFatura() const {return m_estadoFatura;}
    double getSubTotal() const {return m_subtotal;}
    double getTotal() const {return m_total;}
    double getTotalIVA() const {return m_total_iva;}
    double getDescontGlobal() const {return m_desconto_global;}
    const std::string& getObservacao() const {return m_observacao;}
    const std::string& getCriadaEm() const {return m_criada_em;}
    const std::vector<LinhaFatura>& getLinhas() const {return m_linhas;}
    std::vector<LinhaFatura>& getLinhasMut() {return m_linhas;}

    //SETTERS
    void setDescontoGlobal(double desconto) {m_desconto_global = desconto;}
    void setNumero(const std::string& numero) {m_numero = numero;}
    void setDataEmissao(const std::string& data) {m_data_emissao = data;}
    void setDataVencimento(const std::string& data) {m_data_vencimento = data;}
    void setEstadoFatura(EstadoFatura estado) {m_estadoFatura = estado;}
    void setObservacao (const std::string& observacao) {m_observacao = observacao;}
    void setID(int id) {m_id = id;}
    void setID_Cliente(int id_cliente) {m_id_cliente = id_cliente;}
    void setCriadaEm(const std::string& data) {m_criada_em = data;}
    void setSubtotal(double subtotal) {m_subtotal = subtotal;}
    void setTotalIVA(double totalIva) {m_total_iva = totalIva;}
    void setTotal(double total) {m_total = total;}
};