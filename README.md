# LCOM Project

LCOM Project for group T13G5.

Instruções
Menu Inicial

	Ao abrir o jogo, deparamo-nos com o menu inicial. Neste, além do logotipo do jogo, temos três opções diferentes na qual o jogador pode clicar: “Play”, “High Scores” e “Quit”.


Quit

	Ao pressionar o botão esquerdo do rato sobre o botão de “Quit”, o jogo é fechado e voltamos para o modo de escrita do Minix.


Play

	Ao pressionar com o botão esquerdo do rato sobre o botão de “Play”, somos enviados para o jogo, começando automaticamente a partida. Além do mapa, do “Time” e do “Score” como explicados na introdução, também podemos observar um botão de “Quit” no topo do ecrã à direita. Ao clicar neste botão o jogador é enviado para o menu inicial. No caso de o jogador clicar no botão de Esc, exatamente como o botão “quit” do menu inicial, o jogo é fechado e voltamos ao modo de escrita do Minix.


Game Over

	Quando o jogador entra em contacto com um monstro, o jogo acaba, parando a movimentação dos monstros e do jogador e fazendo aparecer uma mensagem de “Game Over” por cima do mapa. No entanto, o tempo e a pontuação do jogador continuam visíveis, tendo também a possibilidade de clicar no botão de “Quit”, sendo enviado para o menu inicial. Continua a ser possível clicar no botão Esc, fechando o jogo e voltando para o menu de escrita do Minix.


High Scores

	Ao pressionar com o botão esquerdo do rato sobre o botão de “High Scores”, somos enviados para o menu dos high scores. Este menu mostra a data em que a pontuação foi conseguida, tendo o dia, mês, ano, hora, minuto e segundo, o tempo de jogo para essa pontuação e a pontuação, para cada uma das 5 melhores pontuações que o jogador obteve.


Nota:

	Para o funcionamento dos high scores e do game over é necessário criar um ficheiro de texto e o path desse ficheiro tem de ser definido dentro do ficheiro “game_state/game_state.h” com o nome “HIGH_SCORES_FILE_PATH_NAME”:


Controlos

	A movimentação do personagem é controlada através das teclas W, A, S e D, fazendo o personagem ir para cima, para a esquerda, para baixo e para a direita respetivamente.

	Para atacar os monstros é necessário a utilização do rato, clicando com o botão esquerdo em cima do monstro que pretende atacar, no entanto só funciona se o monstro estiver num bloco adjacente ao bloco onde se encontra o jogador.


Group members:

1. Pedro Vieira (up202206230@up.pt)
2. Rodrigo Pombo (up202105374@up.pt)
3. Liess Aouimeur (up202206296@up.pt)
