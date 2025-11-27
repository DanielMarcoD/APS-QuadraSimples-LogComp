// examples/partida_completa.qs

print("=== Partida Demonstrativa ===");
print("");

// Sessão pré-partida de aquecimento
{
  sessao("Pré-partida") {
    etapa("Aquecimento leve", dur=300);
    etapa("Saque prático", reps=20);
    descanso(60);
  };
};

// Configurações da partida
config(no_ad = true);       // partida sem vantagens tradicionais
config(shot_clock = false); // sem shot clock para simplificar

// Melhor de 3 sets, 6 games por set
start_match(3, 6);

// Opcional: renomear jogadores
rename(A, "Jogador A");
rename(B, "Jogador B");

print("--- Início da partida oficial ---");
print("");

// =====================================================
// SET 1 – Jogador A vence por 6 x 3
// =====================================================

// Game 1 (set 1) – saque A, A vence
saque(A, "deuce");
ace(A);
ace(A);
ace(A);
ace(A);        // 4 pontos seguidos -> 1 game para A

// Game 2 (set 1) – saque B, A quebra o saque de B
saque(B, "deuce");
point(A);
point(A);
point(A);
point(A);      // 2 x 0 em games para A

// Game 3 (set 1) – saque A, B quebra de volta
saque(A, "ad");
rally(8);
point(B);
point(B);
point(B);
point(B);      // 2 x 1 em games (A 2, B 1)

// Game 4 (set 1) – saque B, A volta a segurar pressão
saque(B, "deuce");
rally(6);
point(A);
point(A);
point(A);
point(A);      // 3 x 1 em games

// Game 5 (set 1) – saque A, B encosta
saque(A, "deuce");
rally(10);
point(B);
point(B);
point(B);
point(B);      // 3 x 2 em games

// Game 6 (set 1) – saque B, A se mantém à frente
saque(B, "ad");
point(A);
point(A);
point(A);
point(A);      // 4 x 2 em games

// Game 7 (set 1) – saque A, A abre ainda mais
saque(A, "deuce");
ace(A);
ace(A);
point(A);
point(A);      // 5 x 2 em games

// Game 8 (set 1) – saque B, B resiste
saque(B, "deuce");
rally(12);
point(B);
point(B);
point(B);
point(B);      // 5 x 3 em games

// Game 9 (set 1) – saque A, A fecha o set
saque(A, "ad");
rally(9);
point(A);
point(A);
point(A);
point(A);      // 6 x 3 em games -> 1 set para Jogador A

print("");
print("--- Fim do 1º set (Jogador A 6 x 3) ---");
score();
print("");

// =====================================================
// SET 2 – Jogador A vence por 6 x 4
// =====================================================

// Game 1 (set 2) – saque A, A confirma
saque(A, "deuce");
point(A);
point(A);
point(A);
point(A);      // 1 x 0 em games

// Game 2 (set 2) – saque B, B responde bem
saque(B, "ad");
rally(7);
point(B);
point(B);
point(B);
point(B);      // 1 x 1

// Game 3 (set 2) – saque A, A mantém o saque
saque(A, "deuce");
ace(A);
ace(A);
point(A);
point(A);      // 2 x 1

// Game 4 (set 2) – saque B, A quebra
saque(B, "deuce");
point(A);
point(A);
point(A);
point(A);      // 3 x 1

// Game 5 (set 2) – saque A, B quebra de volta
saque(A, "ad");
rally(5);
point(B);
point(B);
point(B);
point(B);      // 3 x 2

// Game 6 (set 2) – saque B, A abre vantagem
saque(B, "deuce");
point(A);
point(A);
point(A);
point(A);      // 4 x 2

// Game 7 (set 2) – saque A, B se mantém vivo
saque(A, "deuce");
rally(9);
point(B);
point(B);
point(B);
point(B);      // 4 x 3

// Game 8 (set 2) – saque B, A encosta no set
saque(B, "ad");
point(A);
point(A);
point(A);
point(A);      // 5 x 3

// Game 9 (set 2) – saque A, B faz último game
saque(A, "deuce");
rally(6);
point(B);
point(B);
point(B);
point(B);      // 5 x 4

// Game 10 (set 2) – saque B, A fecha a partida
saque(B, "deuce");
rally(10);
point(A);
point(A);
point(A);
point(A);      // 6 x 4 no set -> 2 x 0 em sets para Jogador A

print("");
print("--- Fim do 2º set (Jogador A 6 x 4) ---");
score();
print("");

// Neste ponto, em uma partida melhor de 3 sets,
// Jogador A já venceu a partida (2 sets a 0).

print("=== Partida de demonstração finalizada ===");
