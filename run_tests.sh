#!/bin/bash

# ----------------------------------------------------
# Configuration
# ----------------------------------------------------
PROJET_EXEC="./projet"
DOSSIER_TESTS_PUBLICS="./public"
DOSSIER_RESULTATS="./tests"
NB_TESTS=15 # Tests de 0 à 14

# Crée le dossier des résultats s'il n'existe pas
mkdir -p $DOSSIER_RESULTATS

echo "Lancement des tests de validation pour $PROJET_EXEC ..."
echo "------------------------------------------------------"

# ----------------------------------------------------
# Boucle de Test
# ----------------------------------------------------

# Boucle de 0 à (NB_TESTS - 1)
for i in $(seq 0 $((NB_TESTS - 1))); do
    # Formate le numéro de test (ex: 0 -> 00, 10 -> 10)
    # Permet de gérer t0.txt, t1.txt, ..., t14.txt si besoin, 
    # mais le script gère directement les numéros fournis.
    TEST_NUM=$(printf "%02d" $i) 

    FICHIER_ENTREE="$DOSSIER_TESTS_PUBLICS/t$TEST_NUM.txt"
    FICHIER_SORTIE_ATTENDUE="$DOSSIER_TESTS_PUBLICS/out$TEST_NUM.txt"
    FICHIER_SORTIE_PROPRE="$DOSSIER_RESULTATS/aff$TEST_NUM.txt"
    
    echo -n "Test $TEST_NUM ($FICHIER_ENTREE) : "

    # Vérifie si le fichier d'entrée existe avant de lancer le test
    if [ ! -f "$FICHIER_ENTREE" ]; then
        echo "FICHIER MANQUANT (Skipping)."
        continue
    fi
    
    # Exécution de votre programme : ./projet < txx.txt > affxx.txt
    # L'opérateur 2> /dev/null redirige les erreurs du projet (stderr) si nécessaire.
    $PROJET_EXEC < "$FICHIER_ENTREE" > "$FICHIER_SORTIE_PROPRE" 2> /dev/null

    # Lancement de la comparaison : diff -s outxx.txt affxx.txt
    DIFF_RESULTAT=$(diff -s "$FICHIER_SORTIE_ATTENDUE" "$FICHIER_SORTIE_PROPRE")

    # Vérification du résultat (Code de sortie diff: 0 = Identiques, 1 = Différences)
    if [ $? -eq 0 ]; then
        echo "✅ VALIDE (Identique)."
    else
        echo "❌ ÉCHEC (Différences trouvées) !"
        echo "   -> Commande diff: $DIFF_RESULTAT"
        # Vous pouvez décommenter la ligne suivante pour afficher les différences
        # diff "$FICHIER_SORTIE_ATTENDUE" "$FICHIER_SORTIE_PROPRE" 
    fi

done

echo "------------------------------------------------------"
echo "Tests termines. Resultats stockes dans $DOSSIER_RESULTATS."