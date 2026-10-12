#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LETRAS 15

long termo(int tipo, int i) {
    long a, b, c;
    int k, n, d, ehPrimo;

    switch (tipo) {
    case 1:
        return 1 + (i - 1) * 2;

    case 2:
        a = 1;
        for (k = 1; k < i; k++) a *= 2;
        return a;

    case 3:
        a = 1; b = 1;
        if (i <= 2) return 1;
        for (k = 3; k <= i; k++) {
            c = a + b;
            a = b;
            b = c;
        }
        return b;

    case 4:
        n = 1;
        k = 0;
        while (k < i) {
            n++;
            ehPrimo = 1;
            for (d = 2; d * d <= n; d++) {
                if (n % d == 0) { ehPrimo = 0; break; }
            }
            if (ehPrimo) k++;
        }
        return n;
    }
    return 0;
}

const char *nomeTipo(int tipo) {
    if (tipo == 1) return "Progressao Aritmetica";
    if (tipo == 2) return "Progressao Geometrica";
    if (tipo == 3) return "Fibonacci";
    return "Numeros Primos";
}

int palavraValida(const char *p) {
    int i, tam = (int)strlen(p);
    if (tam == 0 || tam > MAX_LETRAS) return 0;
    for (i = 0; i < tam; i++) {
        if (!isalpha((unsigned char)p[i])) return 0;
    }
    return 1;
}

int main(void) {
    char palavra[64], cifrada[64];
    int shift, tipo, i, tam;
    long seq, desloc;
    FILE *saida, *log;
    time_t agora;
    char dataHora[40];

    printf("=== Criptografia em duas camadas ===\n");

    printf("Palavra secreta (ate %d letras, sem acento): ", MAX_LETRAS);
    scanf("%63s", palavra);
    if (!palavraValida(palavra)) {
        printf("Palavra invalida!\n");
        return 1;
    }

    printf("Valor do SHIFT (ex.: 3): ");
    scanf("%d", &shift);

    printf("\nEscolha a sequencia:\n");
    printf("1 - Progressao Aritmetica\n");
    printf("2 - Progressao Geometrica\n");
    printf("3 - Fibonacci\n");
    printf("4 - Numeros Primos\n");
    printf("Opcao: ");
    scanf("%d", &tipo);
    if (tipo < 1 || tipo > 4) {
        printf("Opcao invalida!\n");
        return 1;
    }

    log = fopen("log_execucao.txt", "a");
    if (log == NULL) {
        printf("Erro ao abrir o arquivo de log.\n");
        return 1;
    }
    agora = time(NULL);
    strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", localtime(&agora));
    fprintf(log, "==== Execucao em %s ====\n", dataHora);
    fprintf(log, "Palavra: %s | SHIFT: %d | Sequencia: %s\n", palavra, shift, nomeTipo(tipo));

    tam = (int)strlen(palavra);
    for (i = 0; i < tam; i++) {
        char c = (char)tolower((unsigned char)palavra[i]);
        int pos = c - 'a';

        seq = termo(tipo, i + 1);
        desloc = (shift + seq) % 26;
        if (desloc < 0) desloc += 26;

        cifrada[i] = (char)('a' + (pos + desloc) % 26);

        fprintf(log, "Letra %d: '%c' (pos %d) | seq = %ld | deslocamento = %ld | resultado = '%c'\n",
                i + 1, c, pos, seq, desloc, cifrada[i]);
    }
    cifrada[tam] = '\0';

    saida = fopen("resultado_criptografia.txt", "w");
    if (saida == NULL) {
        printf("Erro ao criar o arquivo de resultado.\n");
        fclose(log);
        return 1;
    }
    fprintf(saida, "Palavra codificada: %s | SHIFT: %d | Tipo: %d | Letras: %d\n",
            cifrada, shift, tipo, tam);
    fclose(saida);

    fprintf(log, "Palavra criptografada: %s\n\n", cifrada);
    fclose(log);

    printf("\nPalavra criptografada: %s\n", cifrada);
    printf("Arquivos gerados: resultado_criptografia.txt e log_execucao.txt\n");
    return 0;
}