#include "esstd.tête" // Entrées-sorties standard
#include "bibstd.tête" // Bibliothèque standard

// Fonction principale, qui affiche ses arguments puis quitte
ent principal(ent carg, car** varg) {
	ent i; // Déclaration d’entier
	pour(i = 0; i < carg; i++) // Boucle «pour»
		imprimerf("%s ", varg[i]); // Affichage formaté
	mettrech(""); // Retour à la ligne
	retourner SORTIE_SUCCÈS; // Fin
}

