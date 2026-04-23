CREATE TABLE IF NOT EXISTS empresas(
    id INTEGER PRIMARY KEY,
    nif TEXT NOT NULL UNIQUE, 
    nome TEXT NOT NULL,
    morada TEXT,
    codigo_postal TEXT,
    cidade TEXT, 
    telefone TEXT,
    email TEXT,
    iban TEXT,
    logo_path TEXT
);
