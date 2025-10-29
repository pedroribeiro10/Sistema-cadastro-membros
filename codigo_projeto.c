#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "membros.dat"

typedef enum {
	ALUNO = 1, PROFESSOR, FUNCIONARIO
} TipoDeMembro;

typedef struct {
	char curso[50];
	int semestre;
} Aluno;

typedef struct {
	char departamento[50];
	char titulacao[50];
} Professor;

typedef struct {
	char setor[50];
	char cargo[50];
} Funcionario;

typedef union {
	Aluno aluno;
	Professor professor;
	Funcionario funcionario;
} DadosEspecificos;

typedef struct {
	int id;
	int ativo;
	TipoDeMembro tipo;
	char nome[50];
	int idade;
	char eventos[5][50];
	DadosEspecificos dados;
} Membro;

const long TamanhoRegistro = sizeof(Membro);

void LimpaEntrada() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void CadastraMembro() {
	FILE* f = fopen(ARQUIVO, "ab+");
	if(!f) {
		printf("Erro ao abrir o arquivo.\n");
		return;
	}
	Membro m;
	memset(&m, 0, sizeof(Membro));
	
	
	fseek(f, 0, SEEK_END);
	long pos = ftell(f);
	m.id = (int)(pos / TamanhoRegistro) + 1;
	m.ativo = 1;
	
	printf("\nTipo de membro (1-Aluno, 2-Professor, 3-Funcionário): ");
	scanf("%d", (int*)&m.tipo);
	LimpaEntrada();
	
	printf("Nome: ");
	fgets(m.nome, 50, stdin); m.nome[strcspn(m.nome, "\n")] = 0;
	
	printf("Idade: ");
	scanf("%d", &m.idade);
	LimpaEntrada();

	printf("Digite até 5 eventos (Pressione Enter pra parar): \n");
	for (int i = 0; i < 5; i++) {
    	printf("Evento %d: ", i + 1);
    	fgets(m.eventos[i], 50, stdin);
    	m.eventos[i][strcspn(m.eventos[i], "\n")] = 0;
    	if (strlen(m.eventos[i]) == 0) break;
    	}
	switch (m.tipo) {
    	case ALUNO:
        	printf("Curso: ");
        	fgets(m.dados.aluno.curso, 50, stdin);
        	m.dados.aluno.curso[strcspn(m.dados.aluno.curso, "\n")] = 0;
			printf("Semestre: ");
        	scanf("%d", &m.dados.aluno.semestre);
        	LimpaEntrada();
        	break;
		case PROFESSOR:
        	printf("Departamento: ");
        	fgets(m.dados.professor.departamento, 50, stdin);
        	m.dados.professor.departamento[strcspn(m.dados.professor.departamento, "\n")] = 0;
        	printf("Titulação: ");
        	fgets(m.dados.professor.titulacao, 50, stdin);
			m.dados.professor.titulacao[strcspn(m.dados.professor.titulacao, "\n")] = 0;
        	break;
		case FUNCIONARIO:
        	printf("Setor: ");
			fgets(m.dados.funcionario.setor, 50, stdin);
        	m.dados.funcionario.setor[strcspn(m.dados.funcionario.setor, "\n")] = 0;
        	printf("Cargo: ");
        	fgets(m.dados.funcionario.cargo, 50, stdin);
        	m.dados.funcionario.cargo[strcspn(m.dados.funcionario.cargo, "\n")] = 0;
			break;
	}

fseek(f, (m.id - 1) * TamanhoRegistro, SEEK_SET);
fwrite(&m, TamanhoRegistro, 1, f);

fclose(f);
printf("✅ Membro cadastrado com ID %d.\n", m.id);
}

void listarMembros() {
    FILE *f = fopen(ARQUIVO, "rb");
    if (!f) {
        printf("Nenhum registro encontrado.\n");
        return;
    }
	Membro m;
    printf("\n=== LISTA DE MEMBROS ===\n");
    while (fread(&m, TamanhoRegistro, 1, f)) {
        if (!m.ativo) continue;
        printf("\nID: %d | Nome: %s | Idade: %d\n", m.id, m.nome, m.idade);
        printf("Tipo: ");
        switch (m.tipo) {
			case ALUNO:
                printf("Aluno - Curso: %s | Semestre: %d\n", m.dados.aluno.curso, m.dados.aluno.semestre);
                break;
            case PROFESSOR:
                printf("Professor - Dep.: %s | Titulação: %s\n", m.dados.professor.departamento, m.dados.professor.titulacao);
                break;
            case FUNCIONARIO:
                printf("Funcionário - Setor: %s | Cargo: %s\n", m.dados.funcionario.setor, m.dados.funcionario.cargo);
                break;
        }
		printf("Eventos:\n");
        for (int i = 0; i < 5 && strlen(m.eventos[i]) > 0; i++) {
            printf("  - %s\n", m.eventos[i]);
    	}
	}
    fclose(f);
}
void buscarMembro() {
    FILE *f = fopen(ARQUIVO, "rb");
    if (!f) {
        printf("Arquivo não encontrado.\n");
        return;
    }

    int id;
    printf("Digite o ID a buscar: ");
    scanf("%d", &id);

	fseek(f, (id - 1) * TamanhoRegistro , SEEK_SET);
    Membro m;
    if (fread(&m, TamanhoRegistro, 1, f) && m.ativo) {
        printf("\nMembro encontrado: %s (%d anos)\n", m.nome, m.idade);
    } else {
        printf("Membro não encontrado ou removido.\n");
    }

    fclose(f);
}
void removerMembro() {
    FILE *f = fopen(ARQUIVO, "rb+");
    if (!f) {
        printf("Arquivo não encontrado.\n");
        return;
    }
	int id;
    printf("Digite o ID a remover: ");
    scanf("%d", &id);

    fseek(f, (id - 1) * TamanhoRegistro, SEEK_SET);
    Membro m;
    if (fread(&m, TamanhoRegisyro, 1, f) && m.ativo) {
        m.ativo = 0;
        fseek(f, (id - 1) * TamanhoRegistro, SEEK_SET);
        fwrite(&m, TamanhoRegistro, 1, f);
        printf(" Membro %d marcado como removido.\n", id);
    } else {
        printf(" Membro não encontrado.\n");
    }
	fclose(f);
}

int main() {
    int opcao;
    do {
        printf("\n=== SISTEMA DE CADASTRO DE MEMBROS ===\n");
        printf("1. Cadastrar membro\n");
        printf("2. Listar membros\n");
        printf("3. Buscar por ID\n");
        printf("4. Remover membro\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        LimpaEntrada();
	switch (opcao) {
            case 1: cadastrarMembro(); break;
            case 2: listarMembros(); break;
            case 3: buscarMembro(); break;
            case 4: removerMembro(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

	return 0;
}
