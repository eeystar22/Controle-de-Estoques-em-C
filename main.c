#include <stdio.h>
#include <string.h>

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int desejaContinuar() {
    char resposta;
    printf("\nDeseja realizar outra operação? (s/n): ");
    scanf(" %c", &resposta);
    if (resposta == 's' || resposta == 'S') {
        return 1;
    } else {
        printf("Obrigada por usar o Controle de Estoques! Até a próxima.\n");
        return 0;
    }
}

int main() {
    int opcao;

    do {
        printf("\n=========================\n");
        printf("    Controle de Estoque    \n");
        printf(" ==========================\n");
        printf("Selecione uma opcao:\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Estoque\n");
        printf("4. Sair\n");
        printf("Opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Opção inválida! Tente novamente.\n");
            limparBuffer();
            continue;
        }

        if (opcao < 1 || opcao > 4) {
            printf("Opção inválida! Tente novamente.\n");
            continue;
        }

        if (opcao == 1) {
            char nome[100];
            int quantidade;

            printf("Digite o nome do item: ");
            limparBuffer();
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';

            printf("Digite a quantidade: ");
            scanf("%d", &quantidade);

            FILE *arquivo = fopen("inventory.txt", "a");
            if (arquivo != NULL) {
                fprintf(arquivo, "%s\n%d\n", nome, quantidade);
                fclose(arquivo);
                printf("Item adicionado com sucesso!\n");
            }

            if (!desejaContinuar()) break;

        } else if (opcao == 2) {
            char nomeBusca[100];
            int qtdRemover;

            printf("Digite o nome do item: ");
            limparBuffer();
            fgets(nomeBusca, sizeof(nomeBusca), stdin);
            nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

            printf("Digite a quantidade a ser removida: ");
            scanf("%d", &qtdRemover);

            FILE *arquivo = fopen("inventory.txt", "r");
            FILE *temp = fopen("temp.txt", "w");

            int encontrado = 0;
            int estoqueInsuficiente = 0;
            int qtdDisponivel = 0;
            char nomeLido[100];
            int qtdLida;

            if (arquivo != NULL && temp != NULL) {
                while (fscanf(arquivo, " %[^\n]", nomeLido) == 1 && fscanf(arquivo, "%d", &qtdLida) == 1) {
                    if (strcmp(nomeLido, nomeBusca) == 0) {
                        encontrado = 1;
                        if (qtdLida >= qtdRemover) {
                            int novaQtd = qtdLida - qtdRemover;
                            if (novaQtd > 0) {
                                fprintf(temp, "%s\n%d\n", nomeLido, novaQtd);
                                printf("Quantidade atualizada com sucesso!\n");
                            } else {
                                printf("Item removido do estoque!\n");
                            }
                        } else {
                            estoqueInsuficiente = 1;
                            qtdDisponivel = qtdLida;
                            fprintf(temp, "%s\n%d\n", nomeLido, qtdLida);
                        }
                    } else {
                        fprintf(temp, "%s\n%d\n", nomeLido, qtdLida);
                    }
                }
                fclose(arquivo);
                fclose(temp);

                remove("inventory.txt");
                rename("temp.txt", "inventory.txt");
            }

            if (!encontrado) {
                printf("Item não encontrado.\n");
            } else if (estoqueInsuficiente) {
                printf("Estoque insuficiente. Quantidade disponível: %d\n", qtdDisponivel);
            }

            if (!desejaContinuar()) break;

        } else if (opcao == 3) {
            FILE *arquivo = fopen("inventory.txt", "r");
            char nomeLido[100];
            int qtdLida;
            int temItens = 0;

            if (arquivo != NULL) {
                while (fscanf(arquivo, " %[^\n]", nomeLido) == 1 && fscanf(arquivo, "%d", &qtdLida) == 1) {
                    if (!temItens) {
                        
                        printf("\n=========================\n");
                        printf("        Estoque Atual      \n");
                        printf("  =========================\n");
                        temItens = 1;
                    }
                    printf("Nome: %s\n", nomeLido);
                    printf("Quantidade: %d\n\n", qtdLida);
                }
                fclose(arquivo);
            }

            if (!temItens) {
                printf("O estoque está vazio.\n");
            }

            if (!desejaContinuar()) break;

        } else if (opcao == 4) {
            printf("Obrigada por usar o Controle de Estoques! Até a próxima.\n");
            break;
        }

    } while (1);

    return 0;
}
