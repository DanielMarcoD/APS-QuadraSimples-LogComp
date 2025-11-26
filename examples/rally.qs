// examples/rally.qs
// Treino de rally com entrada do usuário e feedback dinâmico

print("=== Treino de Rally Interativo ===");
print("Para cada exercício, informe quantas trocas você conseguiu.");
print("");

{
  sessao("Aquecimento para Rally") {
    etapa("Mobilidade", dur=180);
    etapa("Mini-tênis", reps=50);
    descanso(45);
  };
};

meta_rally = 10;
total_exercicios = 3;
exercicio = 0;
rallies_bem_sucedidos = 0;

while (exercicio < total_exercicios) {
  print("--- Exercício de rally:");
  log(exercicio + 1);
  
  saque(A, "deuce");
  
  trocas = readline();
  
  rally(trocas);
  
  if (trocas > meta_rally) {
    print("Excelente! Meta superada!");
    rallies_bem_sucedidos = rallies_bem_sucedidos + 1;
    
    // Aumenta a meta se está indo muito bem
    if (trocas > meta_rally + 5) {
      meta_rally = meta_rally + 2;
      print("Meta aumentada! Novo objetivo:");
      log(meta_rally);
    };
  }
  else {
    if (trocas === meta_rally) {
      print("Perfeito! Meta atingida!");
      rallies_bem_sucedidos = rallies_bem_sucedidos + 1;
    }
    else {
      print("Continue treinando. Meta atual:");
      log(meta_rally);
    };
  };
  
  print("");
  exercicio = exercicio + 1;
};

print("=== Resultado do Treino ===");
print("Rallies bem-sucedidos:");
log(rallies_bem_sucedidos);
print("");

if (rallies_bem_sucedidos === total_exercicios) {
  print("Perfeito! Você atingiu todas as metas!");
  
  {
    sessao("Prêmio - Treino Avançado") {
      etapa("Rally cruzado avançado", reps=20);
      descanso(60);
    };
  };
}
else {
  if (rallies_bem_sucedidos > 0) {
    print("Bom trabalho! Continue assim.");
  }
  else {
    print("Não desanime. A prática leva à perfeição!");
  };
};

print("");
print("=== Treino finalizado ===");
print("");

// Ponto de demonstração
print("Ponto de demonstração:");
saque(A, "deuce");
rally(15);
point(A);
