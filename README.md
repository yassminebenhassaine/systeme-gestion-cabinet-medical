# 🏥 Système de Gestion d'un Cabinet Médical

Application console développée en **Langage C** permettant de gérer les opérations quotidiennes d'un cabinet médical (patients, médecins, consultations et salle d'attente).

---

## 🚀 Fonctionnalités principales

- **👤 Gestion des Patients :** Ajout, affichage, modification, suppression et recherche (par ID ou CIN).
- **👨‍⚕️ Gestion des Médecins :** Suivi des médecins et de leurs spécialités.
- **📋 Gestion des Consultations :** Enregistrement des consultations avec diagnostic, traitement et tarif.
- **⏳ Salle d'attente (Structure FIFO) :** Gestion dynamique de la file d'attente des patients via une **File (Queue)** en liste chaînée.
- **💾 Persistance des données :** Sauvegarde et chargement automatiques des données dans des fichiers binaires (`.dat`).

---

## 🛠️ Notions de C utilisées

- **Structures (`struct`) et Typdef :** Modélisation des entités (Patient, Médecin, Consultation, Nœud).
- **Gestion dynamique de la mémoire :** Allocation dynamique (`malloc`, `free`) pour la file d'attente.
- **Pointeurs & Listes chaînées :** Manipulation avancée des structures de données.
- **Fichiers binaires (`fopen`, `fwrite`, `fread`) :** Stockage permanent des données.

---

## 💻 Compilation et Exécution

Pour compiler et exécuter le projet avec **GCC** :

1. **Ouvrir le terminal** dans le dossier du projet.
2. **Compiler le fichier source :**
   ```bash
   gcc "Système de Gestion d'un Cabinet Médical.c" -o "CabinetMedical.exe"