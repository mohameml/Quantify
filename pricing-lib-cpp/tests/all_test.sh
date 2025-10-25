#!/bin/bash

# PATH TO PNL : copy and paste the path for Pnl library here : 
path="/home/mohameml/ENSIMAG/3A/MEQA/05_Projet_couv_PD/Projet_CPD/pcpd/lib/pnl/build"

# Aller dans le répertoire parent
cd ../..

# Vérifier si le fichier CMakeLists.txt existe
if [ ! -f "CMakeLists.txt" ]; then
  echo "CMakeLists.txt n'existe pas dans le répertoire courant."
  exit 1
fi

# Créer le répertoire build s'il n'existe pas
if [ ! -d "build" ]; then
  echo "Le dossier build n'existe pas. Création du dossier build."
  mkdir build
fi

# Aller dans le répertoire build
cd build

# Lancer cmake et make
echo "Lancement de cmake et make..."
cmake -DCMAKE_PREFIX_PATH=${path} ..
if [ $? -ne 0 ]; then
  echo "Erreur pendant l'exécution de cmake."
  exit 1
fi

make
if [ $? -ne 0 ]; then
  echo "Erreur pendant l'exécution de make."
  exit 1
fi

# Liste des exécutables à tester
tests=("test_convert" "test_monte_carlo" "test_option" "test_black_scholes_model" "test_pricer" "test_hedge")

# Vérifier que les exécutables existent et les exécuter
for test in "${tests[@]}"; do
  if [ ! -f "$test" ]; then
    echo "L'exécutable $test est introuvable."
    exit 1
  fi

  # Afficher le titre avant chaque test
  echo "================================= $test ==============================="
  
  # Lancer le test
  ./$test
  
  # Vérifier si le test a réussi
  if [ $? -ne 0 ]; then
    echo "$test a échoué."
    exit 1
  fi
done

echo "Tous les tests ont été exécutés avec succès."
