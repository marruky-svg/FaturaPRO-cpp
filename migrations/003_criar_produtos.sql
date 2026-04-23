CREATE TABLE IF NOT EXISTS produtos(
    id INTEGER PRIMARY KEY,
    sku TEXT NOT NULL UNIQUE CHECK(length(sku) <= 50),
    nome TEXT NOT NULL CHECK(length(nome) <= 255),
    descricao TEXT,
    preco_unitario REAL NOT NULL,
    taxa_iva REAL NOT NULL,
    stock INTEGER DEFAULT 0,
    stock_minimo INTEGER DEFAULT 5,
    tipo TEXT CHECK(tipo IN ('produto', 'servico')),
    ativo BOOLEAN DEFAULT TRUE
);