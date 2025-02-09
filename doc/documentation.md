# Documentation Théorique: Linker, LD_PRELOAD et Threads sous Linux

Ce document présente des explications détaillées sur trois concepts clés utilisés dans ce projet : le linker, la technique LD_PRELOAD et les threads sous Linux. Ces éléments sont essentiels pour comprendre comment le malware injecte son code, intercepte des fonctions et gère des tâches en parallèle.

---

## 1. Le Linker

### 1.1. Rôle et Fonctionnement
Le **linker** (ou éditeur de liens) est un outil qui combine plusieurs fichiers objets (généralement issus de la compilation de fichiers source `.c`) pour créer un exécutable ou une bibliothèque. Ses principales tâches sont :
- **Résolution des symboles** : Il associe les références (fonctions, variables) dans les différents fichiers objets aux définitions correspondantes.
- **Relocalisation** : Il ajuste les adresses des symboles dans le code afin que les appels aux fonctions ou les accès aux variables pointent aux bonnes adresses.
- **Fusion des segments** : Par exemple, tous les segments de code (texte) sont regroupés et placés dans une zone spécifique de la mémoire de l’exécutable.

### 1.2. Liage Statique vs. Liage Dynamique
- **Liage statique** :  
  - Les bibliothèques (fichiers `.a`) sont incorporées directement dans l'exécutable lors de la phase de linkage.  
  - Avantage : l'exécutable est autonome et ne dépend pas de bibliothèques externes lors de l'exécution.  
  - Inconvénient : taille plus importante et difficulté de mettre à jour les bibliothèques.
  
- **Liage dynamique** :  
  - Les bibliothèques partagées (fichiers `.so` sur Linux) sont liées à l'exécutable au moment de l'exécution par le **dynamic linker** (par exemple, `/lib/ld-linux.so.2` ou `/lib64/ld-linux-x86-64.so.2`).  
  - Avantage : partage de code entre plusieurs programmes, mise à jour simplifiée.  
  - Inconvénient : dépendance aux versions des bibliothèques installées sur le système.
