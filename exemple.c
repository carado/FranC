#include "esstd.tête"
#include "bibstd.tête"

ent principal(entier carg, car** varg) {
	ent i;
	pour(i = 0; i < carg; i++)
		imprimerf("%s ", varg[i]);
	mettrech("");
	retourner SORTIE_SUCCÈS;
}

