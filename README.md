# Dernier Metro Avant Minuit
Mini-jeu narratif en C++/SFML

## Pitch
Tu es coincé sur la ligne 13.  
Tu as 5 minutes avant que le dernier train ne parte.

3 stations.  
1 seul train est le bon.  
Les autres bouclent la nuit encore et encore.

Tu dois parler aux NPC, collecter 7 indices (flags), comprendre le message crypté, et trouver le train qui mène à la sortie.

Sans tous les indices → boucle (retour station 1).  
Avec tous les indices mais mauvais train → **GAME OVER** (écran noir).  
Avec tous les indices + station gagnante → **FIN BLANCHE** (ending runner).

## Gameplay / Commandes
- `Q / D` → déplacement gauche / droite
- `E` → interagir (NPC, panneaux, train)
- `SPACE` → avancer le dialogue
- `P` → **Pause / Reprendre**
- `ESC` → quitter 

Les NPC donnent des bribes d’informations.  
Les flags se débloquent par étapes (progression multi-étapes par scène).

## Durée d’un run
**≈ 5 minutes** (timer affiché en rouge, coin haut droit)

## Structure technique / Architecture
POO respectée, encapsulation / composition / héritage :

- `Game` → **machine d’état complète**  
  `Menu / Playing / Pause / GameOver / Ending`
- `Level` → gestion scène / triggers / zones AABB
- `Player` → dérive de `Entity` (classe abstraite)
- `NPC` → dérive de `Entity`
- `DialogueSystem` → gestion des lignes + bleep audio
- `NpcInteraction` → gating flags, SETFLAG, étape par scène
- `SoundBank` → pooling SFML3 (oneshots + loops)
- `SceneNavigation` → panneaux / train
- `ScreenFader` → fade noir / fade blanc

Ending GOOD → fade blanc (runner).  
Ending BAD → fade noir + auto-exit.

## Build
**Prérequis**
- SFML 3.0.2
- Visual Studio 2022
- C++17
- CMake ≥ 3.20

**Compilation**
```bash
cmake -S . -B build
cmake --build build
Download Build.zip add menu.wav from assets into assets/sfx/
