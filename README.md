# Simulador de Ciclo de Vida de Processos

Projeto desenvolvido para a disciplina de **Sistemas Operacionais**, com o objetivo de simular o ciclo de vida de processos em um sistema operacional utilizando conceitos como:

* Criação de processos (`fork`)
* Espera por processos filhos (`wait`)
* Escalonamento por **Round Robin**
* Filas de **Pronto** e **Espera**
* Bloqueio e desbloqueio de processos
* Preempção por quantum
* Relatório estatístico final

---

## 📚 Conceitos Simulados

Durante a execução, cada processo pode assumir um dos seguintes estados:

* **PRONTO** → aguardando para usar a CPU.
* **EXECUTANDO** → atualmente utilizando a CPU.
* **ESPERANDO** → bloqueado por I/O ou aguardando término de um filho.
* **FINALIZADO** → terminou sua execução.

A simulação utiliza o algoritmo de escalonamento **Round Robin**, com quantum fixo de 10 unidades de tempo.

---

## 🏗️ Estrutura do Projeto

```text
Trabalho-de-SO/
├── main.cpp          # Interface e lógica principal da simulação
├── TADProcesso.h     # Estruturas e funções para processos e filas
└── README.md         # Documentação do projeto
```

---

## ⚙️ Estruturas de Dados

### `Processo`

Representa um processo do sistema.

Principais atributos:

| Campo                 | Descrição                          |
| --------------------- | ---------------------------------- |
| `pid`                 | Identificador do processo          |
| `pidPai`              | PID do processo pai                |
| `tempoTotalCPU`       | Tempo total necessário de CPU      |
| `tempoExecutadoCPU`   | Tempo já executado                 |
| `tempoTotalSistema`   | Tempo total no sistema             |
| `tempoBloqueadoTotal` | Tempo total bloqueado              |
| `filhosCriados`       | Quantidade de filhos gerados       |
| `waitsExecutados`     | Quantidade de operações `wait()`   |
| `filhosAtivos`        | Número de filhos ainda em execução |
| `estado`              | Estado atual do processo           |

### `Fila`

Fila encadeada usada para:

* Fila de Pronto
* Fila de Espera

---

## 🔁 Fluxo da Simulação

1. O usuário informa quantos processos iniciais serão criados.
2. Cada processo recebe um tempo de CPU aleatório.
3. Os processos são colocados na fila de pronto.
4. O escalonador seleciona um processo para executar.
5. Durante a execução, o processo pode:

   * Finalizar.
   * Ser bloqueado.
   * Criar um processo filho.
   * Executar `wait()`.
   * Sofrer preempção por quantum.
6. O tempo global é atualizado.
7. O processo volta para a fila apropriada.
8. Ao final, é gerado um relatório estatístico.

---

## 🧠 Funcionalidades Implementadas

### ✔ Criação de Processos

Novos processos podem ser criados dinamicamente durante a execução.

### ✔ Escalonamento Round Robin

Cada processo executa até o limite do quantum (`QUANTUM = 10`).

### ✔ Bloqueio por I/O

Processos podem ficar bloqueados por um tempo aleatório.

### ✔ `fork()` e `wait()`

O processo pai pode criar filhos e aguardar sua finalização.

### ✔ Relatório Final

Ao término da simulação, são exibidas estatísticas gerais e por processo.

---

## 📊 Relatório Final

O relatório apresenta:

* Total de processos finalizados.
* Número total de bloqueios.
* Tempo médio de bloqueio.
* Quantidade de preempções.
* Detalhamento individual por processo.

### Informações por Processo

* PID
* Tempo total no sistema
* Número de filhos criados
* Tempo total bloqueado
* Quantidade de `wait()` executados

---

## 🖥️ Interface Gráfica em Modo Texto

O projeto utiliza a biblioteca `conio2.h` para exibir:

* Molduras na tela
* Cores
* Atualização em tempo real
* Exibição das filas e do processo em execução

---

## ▶️ Como Compilar

### Requisitos

* Compilador C/C++ (Dev-C++, Code::Blocks, MinGW ou similar)
* Sistema Windows
* Biblioteca `conio2.h`

### Comando (g++)

```bash
g++ main.cpp -o simulador
```

### Executar

```bash
./simulador
```

---

## 🧪 Exemplo de Execução

```text
Informe a quantidade de processos iniciais: 3

Tempo Global: 12
EXECUÇÃO:
PID: 2
CPU: 7/20
Estado: EXECUTANDO

FILA DE PRONTO:
PID 1 CPU 4/15
PID 3 CPU 0/18

FILA DE ESPERA:
PID 4 WAIT FILHO
```

---

## 🔧 Principais Funções

### Manipulação de Filas

* `inicializarFila()`
* `filaVazia()`
* `enfileirar()`
* `desenfileirar()`
* `removerEspecifico()`

### Manipulação de Processos

* `criarProcesso()`
* `incrementarTempoFila()`
* `incrementarTempoBloqueados()`
* `desbloquearProcessos()`
* `tentarDesbloquearPai()`
* `nomeEstado()`

---

## 📐 Constantes do Sistema

```cpp
#define QUANTUM 10
#define MAX_PROCESSOS 200
```

---

## 🎯 Objetivos Acadêmicos

Este trabalho demonstra, na prática, conceitos fundamentais de Sistemas Operacionais:

* Gerenciamento de processos
* Escalonamento de CPU
* Sincronização entre processos pai e filho
* Filas de estados
* Estatísticas de execução

---

## 👨‍💻 Autor

Projeto desenvolvido por **Guilherme** para a disciplina de **Sistemas Operacionais**.

---

## 📄 Licença

Projeto acadêmico para fins educacionais.
