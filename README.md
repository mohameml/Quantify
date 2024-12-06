# Quantify

> **Quantify** est un projet innovant conçu pour offrir une **interface web intuitive et performante** dédiée au pricing de produits dérivés, y compris des produits exotiques tels que les options CALL, PUT, américaines, européennes, et bien d'autres.

-   Le projet repose sur une architecture en deux grandes parties :

    -   **Pricing-lib (C++)**

        Au coeur de Quantify se trouve une bibliothèque puissante écrite en C++ pour le pricing de produits dérivés.

        -   **Modèles pris en charge** : Black-Scholes, Heston, et Cox-Ross-Rubinstein.
        -   **Méthodes de calcul** : Monte Carlo et EDP (Équations aux Dérivées Partielles).
        -   **Fonctionnalités** : Support de plus de 40 types d'options, permettant une flexibilité et une précision inégalées pour répondre à divers besoins en finance quantitative.

    -   **Interface Web et Backend**

        Quantify propose une solution complète avec un frontend moderne et un backend robuste :

        -   **Frontend** : Développé avec React.js et stylisé avec Tailwind CSS pour une expérience utilisateur fluide et moderne.

        -   **Backend** : Construit avec Node.js et Express.js, il garantit des performances optimales. La communication entre le backend et la bibliothèque de pricing en C++ est assurée par **gRPC**, offrant une latence réduite et une interopérabilité efficace.
