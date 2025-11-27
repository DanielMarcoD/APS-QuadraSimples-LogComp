// examples/match_simulation.qs
// Simulação completa de uma partida de tênis com placar real

print("=== Simulação de Partida de Tênis ===");
print("");

// Renomear jogadores
rename(A, "Rafael Nadal");
rename(B, "Roger Federer");

// Configurar regras
config(no_ad = true);
config(shot_clock = false);

// Iniciar partida: melhor de 3 sets, 6 games por set
start_match(3, 6);

print("--- Game 1 (Saque: Nadal) ---");
saque(A, "deuce");
rally(8);
point(A);
rally(12);
point(B);
rally(5);
ace(A);
rally(6);
point(A);
rally(7);
point(A);
score();

print("--- Game 2 (Saque: Federer) ---");
saque(B, "ad");
rally(15);
point(B);
fault(B);
rally(10);
point(B);
rally(7);
point(A);
rally(9);
ace(B);
rally(6);
point(B);
score();

print("--- Game 3 (Saque: Nadal) ---");
saque(A, "deuce");
rally(6);
point(A);
rally(8);
point(A);
fault(A);
double_fault(A);
rally(11);
point(A);
rally(9);
point(A);
score();

print("--- Game 4 (Saque: Federer) ---");
saque(B, "ad");
rally(20);
point(B);
rally(14);
point(B);
rally(9);
point(B);
rally(7);
point(B);
score();

print("--- Game 5 (Saque: Nadal) ---");
saque(A, "deuce");
ace(A);
rally(13);
point(A);
rally(8);
point(A);
rally(10);
point(A);
score();

print("--- Game 6 (Saque: Federer) ---");
saque(B, "ad");
rally(16);
point(B);
rally(11);
point(B);
rally(9);
point(B);
rally(12);
point(B);
score();

print("");
print("=== Partida concluída! ===");
print("Que jogo incrível!");
print("");

// Condicional baseada no resultado
games_nadal = 3;
games_federer = 3;

if (games_nadal > games_federer) {
  print("Nadal lidera em games!");
}
else {
  if (games_federer > games_nadal) {
    print("Federer lidera em games!");
  }
  else {
    print("Empate em games - tensão total!");
  };
};
