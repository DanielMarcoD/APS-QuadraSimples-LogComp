// examples/loops.qs
// Demonstração de controle de fluxo com loops aninhados e readline

print("=== Treino de Saque com Loops Aninhados ===");
print("Para cada tentativa, digite quantos acertos você fez:");
print("");

// Configuração do treino
total_rounds = 2;
saques_por_round = 3;

round = 0;

while (round < total_rounds) {
  print("--- Round de treino:");
  log(round + 1);
  
  tentativa = 0;
  acertos_no_round = 0;
  
  while (tentativa < saques_por_round) {
    acertos = readline();
    
    if (acertos > 5) {
      print("Ótimo resultado!");
      acertos_no_round = acertos_no_round + acertos;
    }
    else {
      if (acertos > 2) {
        print("Bom trabalho.");
        acertos_no_round = acertos_no_round + acertos;
      }
      else {
        print("Continue praticando.");
        acertos_no_round = acertos_no_round + acertos;
      };
    };
    
    tentativa = tentativa + 1;
  };
  
  print("Total de acertos neste round:");
  log(acertos_no_round);
  print("");
  
  round = round + 1;
};

print("=== Treino concluído! ===");
print("");

// Pequena sessão de treino após o controle
{
  sessao("Finalização") {
    etapa("Alongamento", dur=120);
    descanso(60);
  };
};
