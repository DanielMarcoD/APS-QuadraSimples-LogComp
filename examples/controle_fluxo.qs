// examples/controle_fluxo.qs

print("Digite 3 números (um por vez):");

tentativas = 0;
max_tentativas = 3;
acertos = 0;

while (tentativas < max_tentativas) {
  x = readline();

  if (x > 10) {
    print("Maior que 10");
    acertos = acertos + 1;
  }
  else {
    print("Menor ou igual a 10");
  };

  tentativas = tentativas + 1;
};

print("");
print("Total de acertos acima de 10:");
log(acertos);
