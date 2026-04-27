CREATE TABLE IF NOT EXISTS linhas_fatura(
    id INTEGER PRIMARY KEY,
    id_fatura INTEGER NOT NULL REFERENCES faturas(id),
    id_produto INTEGER NOT NULL REFERENCES produtos(id),
    descricao TEXT,
    quantidade INTEGER NOT NULL, 
    preco_unitario REAL NOT NULL,
    desconto REAL DEFAULT 0,
    taxa_iva REAL NOT NULL,
    subtotal REAL NOT NULL
)