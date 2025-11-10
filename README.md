# Dernier Metro Avant Minuit
Mini-jeu narratif en C++/SFML

## Pitch
Tu es coincé sur la ligne 13.  
Tu as 5 minutes avant que le dernier train ne parte.

3 stations.  
1 seul train est le bon.  
Les autres bouclent la nuit encore et encore.

Tu dois parler aux NPC, collecter 7 indices (flags), comprendre le message crypté, et trouver le train qui mène à la sortie.

Sans tous les indices → boucle.  
Avec tous les indices mais mauvais train → GAME OVER.  
Avec tous les indices et bon train → FIN BLANCHE.

## Gameplay
- `A / D` → gauche / droite
- `E` → interagir (NPC, panneaux, train)
- `SPACE` → avancer dans un dialogue

NPC donnent des bribes d’informations, parfois mensongères.  
Les indices se débloquent dans un ordre précis (multi flags), il faut faire des allers-retours entre les stations.

## Durée d’un run
**≈ 5 minutes** (timer affiché en rouge en haut à droite)

## Structure technique
POO respectée :
- `Game` (machine d’état : Menu / Playing / GoodEnd / BadEnd)
- `Level` (scènes, triggers, navigation)
- `Player` (hérite d’IEntity abstraite → update/draw)
- `NPC` (hérite d’IEntity)
- `DialogueSystem` (texte narratif + sons bleep)
- `SoundBank` (pooling SFML3)
- `ScreenFader` (fade noir / blanc)

Fin `GOOD` → fade blanc + auto-walk du joueur (loop gauche/droite)
Fin `BAD` → fade noir et écran game_over

## Build
**Prerequis**
- SFML 3.0.2
- Visual Studio 2022
- C++17

**Compilation**
```bash
cmake .
make
