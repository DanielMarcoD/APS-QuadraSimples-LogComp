// bloco inicial
{
  sessao("Aquecimento") {
    etapa("Mobilidade", dur=180);
    etapa("Shadow", reps=40);
    descanso(30);
  };
};

series = 2;
alvo = 6;
s = 0;

while (s < series) {
  saque(A, "deuce");
  etapa("Saque no T", reps=10);
  acertos = 7;

  if (acertos > alvo) {
    alvo = alvo + 1;
  };
  else {
    alvo = alvo;
  };
  ;

  s = s + 1;
};  ;   // <- dois ; aqui: um fecha o bloco, o outro fecha o WHILE

config(no_ad = true);
start_match(3, 6);
saque(B, "deuce");
point(B);
ace(B);
score();
