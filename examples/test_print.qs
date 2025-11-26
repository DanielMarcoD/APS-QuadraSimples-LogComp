// Teste do comando print()

print("=== Bem-vindo ao QuadraSimples! ===");
print("");

{
  sessao("Treino Básico") {
    print("Iniciando aquecimento...");
    etapa("Mobilidade", dur=120);
    descanso(30);
    print("Aquecimento concluído!");
  };
};

print("");
print("Digite quantos saques você acertou:");
acertos = readline();

if (acertos > 5) {
  print("Excelente trabalho!");
}
else {
  print("Continue praticando!");
};

print("");
print("Total de acertos:");
log(acertos);

print("");
print("=== Treino finalizado! ===");
