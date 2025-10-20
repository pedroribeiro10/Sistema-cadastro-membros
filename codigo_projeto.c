#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "membros.dat"

typedef enum {
	aluno = 1, professor, funcionario
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
} 
