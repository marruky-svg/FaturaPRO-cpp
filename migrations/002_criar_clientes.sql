CREATE TABLE IF NOT EXISTS clientes(
    id INTEGER  PRIMARY KEY,
    nif TEXT NOT NULL UNIQUE CHECK(length(nif) = 9), 
    nome TEXT NOT NULL,
    morada TEXT,
    codigo_postal TEXT,
    cidade TEXT,
    email TEXT,
    telefone TEXT,
    tipo TEXT CHECK(tipo IN ('particular', 'empresa')),
    ativo BOOLEAN DEFAULT TRUE,
    criado_em TIMESTAMP,
    atualizado_em TIMESTAMP 

);