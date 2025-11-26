// examples/partida_completa.qs

print("=== Partida Demonstrativa ===");
print("");

{
  sessao("Pré-partida") {
    etapa("Aquecimento leve", dur=300);
    etapa("Saque prático", reps=20);
    descanso(60);
  };
};

// Configurações da partida
config(no_ad = true);
config(shot_clock = false);

start_match(3, 6);

print("--- Início da partida oficial ---");
print("");

// Game 1 – saque do jogador A
saque(A, "deuce");
point(A);
point(B);
ace(A);
fault(A);
double_fault(A);
score();

print("");

// Game 2 – saque do jogador B
saque(B, "ad");
rally(10);
point(B);
point(B);
ace(B);
score();

print("");

// Game 3 – pequeno fluxo de decisão
pontos_A = 2;
pontos_B = 3;

if (pontos_A > pontos_B) {
  print("Jogador A está na frente neste recorte.");
}
else {
  print("Jogador B está na frente neste recorte.");
};

print("");
score();

print("");
print("=== Partida de demonstração finalizada ===");
