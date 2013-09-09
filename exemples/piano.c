#include <esstd.tête>
#include <bibstd.tête>
#include <argstd.tête>
#include <chaîne.tête>
#include <typec.tête>
#include <limites.tête>

#define _USE_MATH_DEFINES
#include <math.tête>

#define DURÉE_DÉFAUT 0.5
#define TAUX_DE_BOUTS_DÉFAUT 8000

FICHIER* entrée, * sortie;
double durée;
long taux_de_bouts;
ent bouts_par_note;
ent quitter;
ent temps;

vide terminer();
ent principal(ent carg, car** varg);
FICHIER* ouvrir_sûr(const car* nom, const car* mode);
vide afficher_aide(const car* nom_programme);
vide vérifier(ent statement, const car* erreur, ...);
ent lire_note();
vide boucle_principale();
vide rendre_note(ent note_num);
ent obtenir_non_blanc();
vide silence();

/* */

ent obtenir_non_blanc() {
	ent c;
	tantque((c = obtenirfc(entrée)) == ' ');
	si(c == FDF) quitter = 1;
	retourner c;
}

ent lire_note() {
	ent c = obtenir_non_blanc();
	si(c == FDF) retourner ENT_MIN;
	
	statique ent dèrnière_val = -9;
	
	ent val = -9;
	
	changement(c) {
		cas 'd': cas 'C': val += 0; casser;
		cas 'r': cas 'D': val += 2; casser;
		cas 'm': cas 'E': val += 4; casser;
		cas 'f': cas 'F': val += 5; casser;
		cas 's': cas 'G': val += 7; casser;
		cas 'l': cas 'A': val += 9; casser;
		cas 'i': cas 'B': val += 11; casser;
		cas '_': retourner ENT_MAX;
		cas '.': val = dèrnière_val; casser;
		cas '\n': cas '\t': cas '\r':
			retourner ENT_MIN;
		défaut:
		vérifier(0, "Caractère inattendu: '%c'(%d) - doit être une note", c, c);
	}
	
	tantque(1) {
		c = obtenir_non_blanc();
		si(c == FDF) casser;
		changement(c) {
			cas '#': val++; casser;
			cas '%': val--; casser;
			cas '+': val+=12; casser;
			cas '-': val-=12; casser;
			défaut: déobtenirc(c, entrée); allerà fin;
		}
	}
	
	fin:
	dèrnière_val = val;
	retourner val;
}

vide silence() {
	pour(ent i = 0; i < bouts_par_note; i++)
		mettrefc(0, sortie);
}

vide rendre_note(ent note_num) {
	double freq = puis(2.0, (double)note_num / 12.0) * 440.0;
	double pi = M_PI;
	
	pour(ent i = 0; i < bouts_par_note; i++) {
		double t = (double)(temps++) / (double)taux_de_bouts;
		nonsigné car valeur = 255.0*(sin(pi * 2.0 * freq * t)+1.0)/2.0;
		mettrefc(valeur, sortie);
	}
}

vide boucle_principale() {
	quitter = 0;
	temps = 0;
	tantque(!quitter) {
		ent n = lire_note();
		si(n == ENT_MIN)
			continuer;
		rendre_note(n);
	}
}

vide vérifier(ent statement, const car* erreur, ...) {
	av_liste args;
	av_début(args, erreur);
	si(!statement) {
		vimprimerff(errstd, erreur, args);
		mettrefch("\n", errstd);
		sortir(SORTIE_ÉCHEC);
	}
	av_fin(args);
}

vide afficher_aide(const car* nom_programme) {
	imprimerf("Utilisation: %s <entrée> <sortie> [durée] [taux de bouts]\n", nom_programme);
	imprimerf("*entrée* et *sortie* peuvent tout deux être \"-\", qui représente\n");
	imprimerf("respectivement l’entrée et la sortie standard.\n");
	imprimerf("Ce programme lit une séquence de notes de musique et sort cette\n");
	imprimerf("séquence sous forme de son cru où chaque note dure *durée*\n");
	imprimerf("secondes (valeur par défaut: %f secondes).\n", DURÉE_DÉFAUT);
	imprimerf("Le taux de bouts («bitrate») est en octets par seconde; la valeur\n");
	imprimerf("par défaut est %d Hz. Les données sont toujours sorties sous la\n", TAUX_DE_BOUTS_DÉFAUT);
	imprimerf("forme d’octets non-signés en mono (un seul canal).\n");
}

ent principal(ent carg, car** varg) {
	si (carg < 3 || carg > 5) {
		afficher_aide(varg[0]);
		retourner SORTIE_ÉCHEC;
	}
	
	entrée = cmpchn(varg[1],"-") ? ouvrir_sûr(varg[1], "r") : entstd;
	sortie = cmpchn(varg[2],"-") ? ouvrir_sûr(varg[2], "w") : sorstd;
	durée = carg >= 4 ? aenf(varg[3]) : DURÉE_DÉFAUT;
	vérifier(durée > 0, "*durée* doit être une valeur strictement positive");
	taux_de_bouts = carg >= 5 ? aenl(varg[4]) : TAUX_DE_BOUTS_DÉFAUT;
	vérifier(taux_de_bouts > 0, "*taux de bouts* doit être strictement positif");
	
	bouts_par_note = (double)taux_de_bouts * durée;
	
	boucle_principale();
	
	terminer();
	
	retourner 0;
}

vide terminer() {
	si(entrée != entstd) fermerf(entrée);
	si(sortie != sorstd) fermerf(sortie);
}

FICHIER* ouvrir_sûr(const car* nom, const car* mode) {
	FICHIER* fichier = ouvrirf(nom, mode);
	vérifier(fichier != NUL, "Impossible d’ouvrir \"%s\" en mode \"%s\".\n", nom, mode);
	retourner fichier;
}

