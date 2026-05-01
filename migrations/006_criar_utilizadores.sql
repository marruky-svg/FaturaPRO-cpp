CREATE TABLE IF NOT EXISTS utilizadores(
    id INTEGER PRIMARY KEY,
    nome TEXT NOT NULL,
    email TEXT NOT NULL UNIQUE,
    api_key TEXT NOT NULL UNIQUE,
    ativo BOOLEAN DEFAULT true,
    criado_em TEXT
    );