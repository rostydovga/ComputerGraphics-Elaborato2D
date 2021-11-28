#### ELABORATO 2D - Computer Graphics ####
author: Rostyslav Dovganyuk
anno accademico: 2021/2022

## FUNZIONAMENTO ##
Il programma ricrea un semplice videogioco, che vede come "protagonista" una navicella che deve schivare un cumulo di meteoriti che le vengono in contro, ma ha la possibilità di eseguire speciali manovre che le possono permettere di sfuggire più in fretta.

A ogni stormo di meteoriti che sorpassa la difficoltà aumenta, che può essere l'aumento del numero di meteoriti o l'aumento della densità di questi ultimi.
La navicella ha, come detto in precedenza, delle manovre evasive, come ad esempio la possibilità di ruotare, attorno all'asse x, e ingrandirsi o rimpicciolirsi.

## COMANDI ##
- 's' --> Smaller, quindi la navicella si rimpicciolisce;
- 'b' --> Bigger, la navicella si ingrandisce
- 'r' --> viene resettato l'angolo della navicella (torna a 0)
- '0' --> Viene chiuso il gioco

- rotella del mouse --> permette la rotazione della navicella, in base alla direzione della rotella
- mouse passivo --> per il movimento della navicella nello spazio

Ovviamente il campo da gioco è stato costruito in modo che la navicella non esca dai bordi laterali, anzì vi è anche un buon margine , e per quanto riguarda i bordi superiori e inferiori la navicella può straripare con una sua metà. Seppur questa concessione resta comunque difficile scappare dalla tempesta di meteore in quanto a ogni livello vengono rigenerate in posizioni diverse e con criteri diversi, come definito sopra.

