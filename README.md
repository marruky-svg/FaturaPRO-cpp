# FaturaPro

![C++](https://img.shields.io/badge/C++-17-blue)
![CMake](https://img.shields.io/badge/CMake-3.20+-green)
![SQLite](https://img.shields.io/badge/SQLite-3-orange)
![License](https://img.shields.io/badge/license-MIT-blue)

Enterprise invoicing system in C++17 with REST API, API key authentication, and aggregated reporting. Demonstrates layered architecture, design patterns, and transactional SQLite persistence.

## Features

- Full CRUD for Clients, Products, and Invoices
- Sequential fiscal invoice numbering (FT YYYY/00001)
- Atomic stock decrement via SQL transactions
- 5 aggregated reports (revenue, top clients, top products, invoices by status, VAT by rate)
- API key authentication with auto bootstrap
- Versioned schema with automatic migrations
- Portuguese NIF validation with check digit algorithm
- Unit tests with Google Test

## Tech Stack

| Layer | Technology |
|-------|-----------|
| Language | C++17 |
| Build | CMake 3.20+ with FetchContent |
| HTTP Server | cpp-httplib |
| JSON | nlohmann/json |
| Database | SQLite3 |
| Testing | Google Test |

## Quick Start

### Prerequisites

- macOS or Linux
- CMake 3.20+
- SQLite3 (`brew install sqlite3` on macOS)
- nlohmann/json (`brew install nlohmann-json` on macOS)

### Compile and run

```bash
git clone https://github.com/marruky-svg/FaturaPRO-cpp.git
cd FaturaPRO-cpp
mkdir build && cd build
cmake ..
make
cd ..
./build/programa
```

On first run, an admin user is created with a unique API key. Copy the key — you'll need it for all requests.

### Testing

```bash
# Replace YOUR_KEY with the key printed in the console
curl http://localhost:8081/api/v1/clientes -H "X-API-Key: YOUR_KEY"
```

## Architecture

HTTP Server (port 8081)
                    │
                    ▼
            Middleware (Authenticator) ──> Verifies X-API-Key
                    │
                    ▼
                Controllers ──> REST endpoints + JSON
                    │
                    ▼
                 Services ──> Business logic
                    │
                    ▼
               Repositories ──> Database access
                    │
                    ▼
                 SQLite3 ──> Persistence

## REST Endpoints

All endpoints require the `X-API-Key` header.

> **Note:** API endpoints use Portuguese terminology to match the fiscal domain (`faturas` = invoices, `clientes` = clients, `produtos` = products).

### Clients

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/v1/clientes` | Paginated list |
| GET | `/api/v1/clientes/{id}` | Get by ID |
| POST | `/api/v1/clientes` | Create |
| PUT | `/api/v1/clientes/{id}` | Update |
| DELETE | `/api/v1/clientes/{id}` | Soft delete |

### Products

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/v1/produtos` | Paginated list |
| GET | `/api/v1/produtos/{id}` | Get by ID |
| POST | `/api/v1/produtos` | Create |
| PUT | `/api/v1/produtos/{id}` | Update |
| DELETE | `/api/v1/produtos/{id}` | Soft delete |

### Invoices

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/v1/faturas` | Paginated list |
| GET | `/api/v1/faturas/{id}` | Get by ID |
| POST | `/api/v1/faturas` | Create draft |
| PUT | `/api/v1/faturas/{id}` | Update draft |
| DELETE | `/api/v1/faturas/{id}` | Delete draft |
| POST | `/api/v1/faturas/{id}/linhas` | Add line item |
| POST | `/api/v1/faturas/{id}/emitir` | Issue invoice |

### Reports

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/v1/relatorios/faturacao` | Total revenue by period |
| GET | `/api/v1/relatorios/top-clientes` | Top clients by revenue |
| GET | `/api/v1/relatorios/top-produtos` | Top products by quantity sold |
| GET | `/api/v1/relatorios/faturas-por-estado` | Invoice count by status |
| GET | `/api/v1/relatorios/iva-por-taxa` | VAT aggregated by rate |

## Technical Decisions

**Layered architecture (Controller → Service → Repository)**
Clear separation of concerns: HTTP, business logic, and persistence. Each layer can be tested independently.

**Snapshots in LinhaFatura**
Each invoice line stores a snapshot of product data at the time of issuance. Ensures historical invoices remain unchanged when products are later edited.

**Sequential fiscal numbering**
Invoices follow the format `FT YYYY/NNNNN`, generated dynamically via SQL COUNT. Complies with Portuguese fiscal sequentiality requirements.

**Soft delete vs Hard delete**
Clients and Products use soft delete (`ativo` flag). Issued invoices are never deleted (fiscal compliance); only drafts can be removed.

**SQL transactions for compound operations**
Operations involving multiple tables (creating an invoice with lines, decrementing stock during issuance) are wrapped in `BEGIN TRANSACTION`/`COMMIT`/`ROLLBACK` to guarantee atomicity.

**Design Patterns applied**
Repository, Service, Factory Method, Builder, RAII (Database/Statement wrappers), DTO.

## Roadmap

- [x] Week 1: CRUD for Clients and Products
- [x] Week 2: Invoicing with Builder and Factory patterns
- [x] Week 3: Aggregated reports and authentication
- [ ] PDF generation (libharu)
- [ ] Java/Spring Boot version
- [ ] Docker deployment

## License

MIT