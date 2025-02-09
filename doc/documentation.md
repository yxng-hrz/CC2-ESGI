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

---

## 3. Les Threads sous Linux

### 3.1. Qu'est-ce qu'un Thread ?
Un **thread** est une unité d’exécution légère qui s’exécute dans le contexte d’un processus. Plusieurs threads peuvent partager les mêmes ressources (mémoire, descripteurs de fichiers, etc.) tout en s’exécutant de manière concurrente.
- **Différence avec un processus** : Un processus est une instance d’un programme avec son propre espace mémoire. Les threads d’un même processus partagent cet espace, ce qui permet une communication plus rapide, mais nécessite une synchronisation pour éviter les conflits.

### 3.2. La Bibliothèque POSIX Threads (pthreads)
Sur Linux, la bibliothèque **pthread** permet de créer et de gérer des threads.  
- **Création d’un thread** : La fonction `pthread_create()` est utilisée pour démarrer un nouveau thread.  
  - Exemple :  
    ```c
    pthread_t tid;
    pthread_create(&tid, NULL, fonction_thread, NULL);
    ```
- **Synchronisation** :  
  - Pour éviter les conditions de concurrence (race conditions), on utilise des mécanismes de synchronisation comme les mutex (`pthread_mutex_t`), les sémaphores, ou d’autres primitives.
- **Gestion du cycle de vie** :  
  - `pthread_join()` permet à un thread de "rejoindre" un autre, c’est-à-dire d’attendre sa terminaison.
  - `pthread_detach()` permet de détacher un thread pour qu’il libère ses ressources automatiquement lorsqu’il se termine, sans qu’un autre thread ait à l’attendre.

### 3.3. Application dans le Projet
Dans le projet, un thread est utilisé pour gérer le **port knocking**. Ce thread :
- S’exécute en parallèle du reste du programme.
- Écoute sur un port UDP une séquence spécifique.
- Une fois la séquence validée, il ouvre un socket TCP pour recevoir une commande à exécuter.
  
Ce mécanisme montre comment les threads permettent d’exécuter des tâches en arrière-plan sans bloquer le fonctionnement principal du processus.

---

## Conclusion

- **Le linker** permet de combiner des modules compilés et de gérer les références entre eux, ce qui est crucial pour la création d’exécutables ou de bibliothèques.
- **LD_PRELOAD** est un outil puissant qui exploite le fonctionnement du linker dynamique pour injecter du code dans des processus existants, et ainsi modifier leur comportement sans toucher au code source original.
- **Les threads** offrent un moyen de gérer l’exécution parallèle, essentiel pour implémenter des fonctionnalités comme le port knocking qui doivent s’exécuter en continu et en arrière-plan.

Ces concepts, bien compris et appliqués, permettent de réaliser des techniques avancées d’injection de code et d’interception de fonctions, telles que celles utilisées dans notre projet de malware (hors bonus).

---
