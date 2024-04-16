# Corrida-de-Carros

Projeto no âmbito da disciplina de Sistemas Operativos

Criadores: André Jesus / Tomás Nave

![corrida](https://github.com/TomasNave-a22208623/Corrida-de-Carros/assets/127102331/8204a93b-0ea3-44f8-a7ca-dd420b45df1e)


Objetivo:
O objetivo deste projeto é criar uma aplicação em linguagem de programação C que utilize os conceitos de sistemas operativos para criar uma simulação de uma corrida
O objetivo desta aplicação é desenvolver um servidor que realize a transmissão de telecomandose a receção de telemetria para um carro de corrida durante uma prova desportiva. 
A prova desportiva consiste num percurso de uma pista dividida em 3 setores, ao longo dos quaissão efetuadas um total de 10 voltas.

Descrição:

-Utilização de Fifos e Threads

-Utilização de Sinais 

-Criação e utilização de Scripts

Exemplo de Corrida:

```
Msg: | 3 | 10 | 3 | 45.3 | 050 | 060 | Soft |
Corrida terminou para carro numero 3
Msg: | 4 | 10 | 3 | 45.4 | 050 | 060 | Soft |
Corrida terminou para carro numero 4
Msg: | 1 | 10 | 3 | 45.1 | 050 | 060 | Soft |
Corrida terminou para carro numero 1
Msg: | 2 | 10 | 3 | 45.2 | 050 | 060 | Soft |
Corrida terminou para carro numero 2
Msg: | 9 | 10 | 3 | 45.9 | 050 | 060 | Soft |
Corrida terminou para carro numero 9
Msg: | 8 | 10 | 3 | 45.8 | 050 | 005 | Soft |
Corrida terminou para carro numero 8
Msg: | 6 | 10 | 3 | 45.6 | 005 | 060 | Soft |
Carro %d nao tem pneus disponíveis
Corrida terminou para carro numero 6
Msg: | 7 | 10 | 3 | 45.7 | 050 | 060 | Soft |
Corrida terminou para carro numero 7
Msg: | 5 | 10 | 3 | 45.5 | 050 | 060 | Soft |
Corrida terminou para carro numero 5
Posicao Id Nome Equipa Volta Tempo
1 1 Pepepe FCPorta 10 45.100
2 2 Refila FCPorta 10 45.200
3 3 Rafafa Malfica 10 45.300
4 4 Xmiiidt Malfica 10 45.400
5 5 Gyokky Sporta 10 45.500
6 6 Amoras Sporta 10 45.600
7 7 ReiArt Praga 10 45.700
8 8 Seila Praga 10 45.800
9 9 Quem?? Praga 10 45.900

```
