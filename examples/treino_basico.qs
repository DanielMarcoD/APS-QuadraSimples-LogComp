// examples/treino_basico.qs

{
  sessao("Aquecimento leve") {
    etapa("Mobilidade ombros", dur=120);
    etapa("Mobilidade quadril", dur=120);
    descanso(60);

    etapa("Shadow forehand", reps=30);
    etapa("Shadow backhand", reps=30);
    descanso(45);
  };
};

total_series = 3;
serie = 0;

while (serie < total_series) {
  etapa("Rally de base", reps=20);
  descanso(30);

  serie = serie + 1;
};

log(serie);
