Reseau_Project
==============

Petit jeu de Bomberman, codé en C en tant que projet pour le cours de Réseau des M1 Alma de l'Université de Nantes.

Auteurs : Coraline MARIE et Vincent RAVENEAU.

Fin de projet : Mars 2014.

Compilation:
============

Un makefile est disponible, avec 3 commandes:

	make			Compile le client et le serveur
	make client		Compile le client
	make serveur	Compile le serveur

Exécution:
==========

Le serveur s'exécute de la façon suivante:
	./serveur <nb_parties(1-100)> <nb_joueurs_par_partie(2-4)>

Le client s'exécute de la façon suivante:
	./client <pseudo_joueur> <adresse-serveur>

Problèmes éventuels:
====================

Le serveur peut déclancher une erreur de segmentation lors de la connexion du premier client. Cette erreur semble arriver moins fréquement si le serveur est lancé à l'aide d'un débuger, tel gdb, de la façon suivante:
	gdb ./serveur
	run <nb_parties(1-100)> <nb_joueurs_par_partie(2-4)>
	
Compilation du rapport :
========================
	make dans le dossier report (nécessite le pack complet TexLive)
