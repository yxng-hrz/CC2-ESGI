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

### 1.3. Processus d’Exécution
Au démarrage d’un programme lié dynamiquement, le système charge l’exécutable ainsi que toutes les bibliothèques partagées nécessaires. Le dynamic linker effectue alors la résolution finale des symboles et relocalise les adresses avant de transférer le contrôle au programme.

---

## 2. LD_PRELOAD

### 2.1. Principe et Utilisation
La variable d’environnement **LD_PRELOAD** est un mécanisme qui permet de forcer le chargeur de bibliothèques (dynamic linker) à charger, avant toute autre, une ou plusieurs bibliothèques partagées spécifiées par l’utilisateur.  
- **But** : Remplacer ou intercepter des fonctions d’une bibliothèque standard (par exemple, `open()`, `malloc()`, ou même `pam_get_item()`) sans modifier le code source de l’application cible.
- **Usage courant** :  
  - Pour le débogage ou le profilage.
  - Pour modifier le comportement d’une application (par exemple, cacher certaines informations, injecter du code malveillant, etc.).

### 2.2. Mécanisme de Redirection
Lorsqu’une fonction est appelée dans le programme, le dynamic linker cherche d’abord dans les bibliothèques chargées par LD_PRELOAD.  
- Si une fonction a été redéfinie dans une de ces bibliothèques, c’est celle-ci qui sera utilisée.
- Pour accéder à la version originale de la fonction (dans le cas d’un hook), on utilise généralement `dlsym(RTLD_NEXT, "nom_de_la_fonction")` afin de récupérer le pointeur sur la fonction d’origine.

### 2.3. Application dans le Projet
Dans notre projet, LD_PRELOAD est utilisé pour :
- Charger la bibliothèque malveillante (par exemple, `libmalware.so`) dans le processus OpenSSH.
- Intercepter des fonctions telles que `pam_get_item` et `open` pour extraire des credentials et empêcher l’accès à certains fichiers (comme `/var/log/auth.log`).
