// Exemplo de uso de sensores read-only
// Demonstra como usar last_rally e total_points

print("=== DEMO DE SENSORES READ-ONLY ===");
print("");

// Inicializar partida
config(no_ad = 1);
start_match(3, 6);

print("--- Teste 1: Rally e sensor last_rally ---");
rally(5);
ultimo_rally = last_rally;
log(ultimo_rally);  // Deve imprimir 5

rally(12);
ultimo_rally = last_rally;
log(ultimo_rally);  // Deve imprimir 12

print("");
print("--- Teste 2: Pontos e sensor total_points ---");
point(A);
total = total_points;
log(total);  // Deve imprimir 1

ace(B);
total = total_points;
log(total);  // Deve imprimir 2

point(A);
point(B);
total = total_points;
log(total);  // Deve imprimir 4

print("");
print("--- Teste 3: Uso em condicionais ---");
// Simular um treino que monitora desempenho

sessao("Treino com Monitoramento") {
    etapa("Rally básico", reps=3);
    
    contador = 0;
    while (contador < 3) {
        rally(8);
        
        // Verificar se último rally foi bom
        if (last_rally > 5) {
            print("Bom rally!");
        } else {
            print("Rally muito curto");
        };
        
        contador = contador + 1;
    };
};

print("");
print("--- Teste 4: Estatísticas de jogo ---");
// Simular vários pontos e mostrar estatísticas

point(A);
point(B);
ace(A);
double_fault(B);
point(A);

pontos_totais = total_points;
print("Total de pontos disputados:");
log(pontos_totais);  // Deve imprimir 9 (4 anteriores + 5 novos)

score();

print("");
print("--- Teste 5: Expressões aritméticas com sensores ---");
rally(10);
dobro_rally = last_rally * 2;
log(dobro_rally);  // Deve imprimir 20

media_rallies = (last_rally + 8) / 2;
log(media_rallies);  // Deve imprimir 9

// Comparar sensores
if (total_points > last_rally) {
    print("Mais pontos que trocas no último rally");
};

print("");
print("=== FIM DO DEMO ===");
