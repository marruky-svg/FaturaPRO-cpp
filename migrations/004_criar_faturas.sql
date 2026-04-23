CREATE TABLE IF NOT EXISTS faturas(
    id INTEGER PRIMARY KEY,
    numero TEXT NOT NULL UNIQUE CHECK(length(numero) <=20),
    cliente_id INTEGER NOT NULL REFERENCES clientes(id),
    data_emissao DATE,
    data_vencimento DATE,
    estado TEXT CHECK(estado IN ('rascunho', 'emitida','paga','anulada')),
    subtotal REAL -- total sem IVA
    total_iva REAL, -- total de IVA
    total REAL, -- total com IVA
    desconto_global REAL DEFAULT 0,
    observacoes TEXT, 
    criada_em TIMESTAMP --data de criação 
)