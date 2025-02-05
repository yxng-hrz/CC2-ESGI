# Malware LD_PRELOAD - Extraction de Credentials et Port Knocking

Ce projet est une démonstration d'un malware basé sur la technique **LD_PRELOAD**. Il permet d'intercepter et d'extraire les credentials (username/password) envoyés lors d'une connexion SSH, d'empêcher l'accès à certains fichiers sensibles et de mettre en place un mécanisme de **port knocking**. Le projet inclut également un serveur de Command & Contrôle (C2) pour recevoir les informations remontées par le malware.

---

## Objectifs du Projet

1. **Injection via LD_PRELOAD**  
   Charger une bibliothèque partagée (`libmalware.so`) dans le démon OpenSSH pour intercepter des fonctions critiques.

2. **Extraction des Credentials**  
   Intercepter les appels à `pam_get_item` afin de récupérer (de manière simplifiée) le nom d'utilisateur et le mot de passe utilisés lors d'une connexion SSH. Les credentials sont ensuite enregistrés localement et envoyés au serveur C2.

3. **Empêcher l'ouverture d'un fichier sensible**  
   Redéfinir la fonction `open` pour bloquer l'accès au fichier `/var/log/auth.log` et ainsi empêcher la collecte des logs sensibles.

4. **Port Knocking**  
   Implémenter un mécanisme de port knocking qui, après réception de la séquence correcte ("knock1", "knock2", "knock3") via UDP, ouvre un socket TCP pour recevoir et exécuter une commande arbitraire.

5. **Serveur de Command & Contrôle (C2)**  
   Mettre en place un serveur C2 qui écoute sur un port TCP (port 9001) et affiche les messages envoyés par le malware (enregistrement de l'infection et remontée des credentials).

6. **Documentation Théorique**  
   Fournir des explications sur le fonctionnement du linker, de LD_PRELOAD et des threads sous Linux dans le dossier `doc/`.

---
