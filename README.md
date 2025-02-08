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

## Structure du Projet



## Compilation

Pour compiler l'ensemble du projet, exécutez la commande suivante à la racine :

```bash
make

Cette commande génère :

    libmalware.so : La bibliothèque partagée à charger via LD_PRELOAD.
    c2_server : L'exécutable du serveur C2.
```

## Utilisation

1. Chargement du Malware

Sur la machine cible (après avoir arrêté le démon SSH), relancez le démon SSH en chargeant le malware avec LD_PRELOAD :

```bash
LD_PRELOAD=/chemin/vers/libmalware.so /usr/sbin/sshd

    Remarque : Remplacez /chemin/vers/ par le chemin absolu menant au fichier libmalware.so.
```


2. Lancement du Serveur C2

Sur une autre machine ou dans un terminal séparé, lancez le serveur C2 pour recevoir et afficher les messages envoyés par le malware :

```bash
./c2_server
```

Le serveur écoute sur le port 9001 et affiche les messages reçus (par exemple, l'enregistrement d'une machine infectée et les credentials interceptés).


3. Test du Port Knocking

Pour tester le mécanisme de port knocking, envoyez successivement trois paquets UDP contenant les chaînes suivantes sur le port 1337 de la machine infectée. Vous pouvez utiliser netcat (nc) :

```bash
echo "knock1" | nc -u <IP_de_la_machine_infectée> 1337
echo "knock2" | nc -u <IP_de_la_machine_infectée> 1337
echo "knock3" | nc -u <IP_de_la_machine_infectée> 1337
```

Une fois la séquence validée, un socket TCP s'ouvrira sur le port 4444 pour recevoir une commande, laquelle sera exécutée via system().

## Documentation Théorique

Pour en savoir plus sur :

    Le fonctionnement du linker et la résolution des symboles,
    La technique LD_PRELOAD et le mécanisme d'injection de code,
    La gestion des threads sous Linux,

Voir le fichier doc/documentation.md.

---
